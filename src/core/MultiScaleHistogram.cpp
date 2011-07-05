#include <stdio.h>
#include <float.h>
#include "StripeCode.h"

bool MultiScaleHistogram::read(const wxImage *imgPar) {
	// Rescale image to a smaller size
	wxImage *imgCopy = Image::CloneImage((wxImage*)imgPar);
	if(imgCopy->GetWidth() > 640) {
		int nh = ((float)imgCopy->GetHeight() / (float)imgCopy->GetWidth()) * 640;
		// only rescale if the height decreases too
		// TODO: need special code for portrait photos (e.g., for giraffe)
		// as of now, rescaling doesn't kick in if the height of the photo is
		// greater than its width.
		// also enforcing minimum dimensions of the rescaled image to be 640x100,
		// which is reasonable.
		if(nh < imgCopy->GetHeight() && nh > 100)
			imgCopy->Rescale(640, nh);
	}

#ifdef DEBUG
	int imgTag = 1;
#endif
    FloatImage *img = new FloatImage(imgCopy, true);
	delete imgCopy;
    wxString fname;

    // delete old histogram
    histogram.clear();

    // save original image
#ifdef DEBUG
    fname.Printf(_("..\\MultiScaleHistogram-%07d-1.jpg"), imgTag);
	imgPar->SaveFile(fname, wxBITMAP_TYPE_JPEG);
#endif
	int imgW = img->GetWidth(), imgH = img->GetHeight();

    // filter image at different scales
    for(int n = 0; n < Image::multiscale_octaves; n++) {
        wxImage *scaled = new wxImage(imgW, imgH);

        // build filters
        double scale = (n+1) * sqrt((double)2);
        int halfwin = ceil(scale * 3);
#ifdef DEBUG
        printf("halfwin %d scale %f\n", halfwin, scale);
#endif
        double *G1dx = new double[2*halfwin+1];
        double *G2dx = new double[2*halfwin+1];

        // first partial derivative filter of 2-D Gaussian function
        for(int x = -halfwin; x <= halfwin; x++) {
            G1dx[x+halfwin] = -exp(-pow((float)x,(float)2.0)/(2*pow((float)scale,(float)2.0))) * (double)x / (2*M_PI*pow((float)scale,(float)4.0));
            G2dx[x+halfwin] = -2*M_PI*pow((float)scale,(float)4.0)/(float)x;
			if(x==0)
				G2dx[x+halfwin] = 0.99;
        }

        // generate Ix and Iy
        // convolve with gaussian derivative in x direction
        FloatImage *Ix = new FloatImage(img);
        Ix->convolveX(G1dx, halfwin);
        Ix->convolveY(G1dx, halfwin);
        FloatImage *Iy = new FloatImage(Ix);
        Ix->convolveY(G2dx, halfwin);
        Iy->convolveX(G2dx, halfwin);


#ifdef DEBUG
        // save intermediate image at size of original image
        Ix->blit(scaled, 0, 0, true);
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+2);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
        Iy->blit(scaled, 0, 0, true);
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+3);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
#endif

        // generate second xx and yy partial derivative filters based on Ix and Iy
        double *G2dxx = new double[2*halfwin+1];
        double *G2dxy = new double[2*halfwin+1];
        for(int x = -halfwin; x <= halfwin; x++) {
            G2dxx[x+halfwin] = ( (1.0/x) - ((double)x/scale) );
			if(x==0)
				G2dxx[x+halfwin]=1;
            //if(isinf(G2dxx[x+halfwin])) G2dxx[x+halfwin]=1;
            G2dxy[x+halfwin] = (-x/pow(scale,2));
        }

        // generate Ixx, Ixy, Iyy
        FloatImage *Ixx = new FloatImage(Ix);
        FloatImage *Iyy = new FloatImage(Iy);
        FloatImage *Ixy = new FloatImage(Ix);
        Ixx->convolveX(G2dxx, halfwin);
        Iyy->convolveY(G2dxx, halfwin);
        Ixy->convolveX(G2dxy, halfwin);

#ifdef DEBUG
        // save intermediate images at size of original image
        Ixx->blit(scaled, 0, 0, true);
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+4);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
        Iyy->blit(scaled, 0, 0, true);
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+5);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
        Ixy->blit(scaled, 0, 0, true);
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+6);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
#endif

        // compute orientation P and shape index C images
        FloatImage *C = new FloatImage(imgW, imgH, true);
        FloatImage *P = new FloatImage(imgW, imgH, true);
        float *dIx = Ix->GetData();
        float *dIy = Iy->GetData();
        float *dIxx = Ixx->GetData();
        float *dIxy = Ixy->GetData();
        float *dIyy = Iyy->GetData();
        float *dC   = C->GetData();
        float *dP   = P->GetData();

        for(int r = 0; r < imgH; r++)
            for(int c = 0; c < imgW; c++) {
                int i = r*imgW+c;
                double A = (float) (pow((float) pow((float)dIx[i],(float)2) + (float)pow((float)dIy[i],(float)2), (float)(-3.0/2.0)));
                double N = A * (2*dIx[i]*dIy[i]*dIxy[i] - (float)pow((float)dIx[i],2)*dIyy[i] - (float)pow((float)dIy[i],(float)2)*dIxx[i]);
                double T = A * (dIxy[i]*((float)pow((float)dIx[i],(float)2)-(float)pow((float)dIy[i],(float)2)) + dIx[i]*dIy[i]*(dIyy[i]-dIxx[i]));
                dC[i] = 0.5 - (1.0/M_PI)*atan((N+T)/(N-T));
                dP[i] = atan2(dIy[i],dIx[i]);
            }
        C->normalize(1.0);
        P->normalize(1.0);

        // compute histograms for C image at this scale
        C->blit(scaled, 0, 0, true);
        int fb = (int) histogram.size();
        for(int b = 0; b < Image::multiscale_hist_bins; b++)
            histogram.push_back(0);
        double sum = 0;
        for(int i = 0; i < imgW*imgH; i++) {
#ifdef WIN32
			if(_isnan(dC[i]) || !_finite(dC[i]))
				continue;
#else
            if(isinf(dC[i])||isnan(dC[i]))
                continue;
#endif
            int bin = dC[i]*Image::multiscale_hist_bins;
            if(bin < 0) bin = 0; if(bin >= Image::multiscale_hist_bins) bin = Image::multiscale_hist_bins-1;
            sum ++;
            histogram[fb+bin]++;
        }
        for(int i = 0; i < Image::multiscale_hist_bins; i++) {
#ifdef DEBUG
            printf("histo %i -> %f  (sum %f) == %f \n", i, histogram[fb+i], sum, histogram[fb+i]/sum);
#endif
            histogram[fb+i] /= sum;
//            Image::DrawBox(scaled, i*imgW/Image::multiscale_hist_bins, imgH-imgH*histogram[fb+i]-1, imgW/Image::multiscale_hist_bins-1, imgH*histogram[fb+i], 80, 0, 0, true);
        }
#ifdef DEBUG
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+4);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
#endif


        // compute histograms for P image at this scale
        P->blit(scaled, 0, 0, true);
        fb = (int) histogram.size();
        for(int b = 0; b < Image::multiscale_hist_bins; b++)
            histogram.push_back(0);
        sum = 0;
        for(int i = 0; i < imgW*imgH; i++) {
#ifdef WIN32
			if(_isnan(dP[i]) || !_finite(dP[i]))
				continue;
#else
            if(isinf(dP[i])||isnan(dP[i]))
                continue;
#endif
            int bin = dP[i]*Image::multiscale_hist_bins;

			// shouldn't ever hit these cases, but just in case -- we don't want a production system crashing
			if(bin < 0) bin = 0; if(bin >= Image::multiscale_hist_bins) bin = Image::multiscale_hist_bins-1;

            sum ++;
            histogram[fb+bin]++;
        }
        for(int i = 0; i < Image::multiscale_hist_bins; i++) {
#ifdef DEBUG
            printf("histo %i -> %f  (sum %f) == %f \n", i, histogram[fb+i], sum, histogram[fb+i]/sum);
#endif
            histogram[fb+i] /= sum;
//            Image::DrawBox(scaled, i*imgW/Image::multiscale_hist_bins, imgH-imgH*histogram[fb+i]-1, imgW/Image::multiscale_hist_bins-1, imgH*histogram[fb+i], 80, 0, 0, true);
        }
#ifdef DEBUG
        fname.Printf(_("..\\MultiScaleHistogram-%07d-%d.jpg"), imgTag, 5*n+5);
        scaled->SaveFile(fname, wxBITMAP_TYPE_JPEG);
#endif

        // free mem
        delete scaled;
        delete C;
        delete P;
        delete[] G2dxx;
        delete[] G2dxy;
        delete[] G1dx;
        delete[] G2dx;
        delete Ix;
        delete Iy;
        delete Ixx;
        delete Iyy;
        delete Ixy;
    }
    delete img;

	assert((int)histogram.size() == Image::multiscale_hist_bins*Image::multiscale_octaves*2);      // factor 2 because we measure two image features (C and P)

    // compute mean value
    mean = 0;
    for(int i = (int)histogram.size(); --i >= 0; )
        mean += histogram[i];
    mean /= (float) histogram.size();

    // compute L2 norm of vector-mean
    l2norm = 0;
    for(int i = (int)histogram.size(); --i >= 0; )
        l2norm += pow(histogram[i]-mean, 2);
    l2norm = sqrt(l2norm);

    return true;
}

double MultiScaleHistogram::compare(ImageFeatures *par_img2, void *arg) const {
    MultiScaleHistogram &img = *((MultiScaleHistogram*)par_img2);
    assert(histogram.size() == img.histogram.size());

    double innerprod  = 0;
    #pragma omp parallel for reduction(+:innerprod) schedule(guided)
    for(int b = (int)histogram.size(); --b >= 0; ) {
        double vi = histogram[b] - mean;
        double vj = img.histogram[b] - img.mean;
        innerprod += vi * vj;
    }
    innerprod /= (l2norm * img.l2norm);

    return -innerprod;
}

string MultiScaleHistogram::toString() {
    string result;
    char buf[128];
    sprintf(buf, "HISTOGRAM bins %d scales %d\n", Image::multiscale_hist_bins, Image::multiscale_octaves);
    result += buf;
    for(int i = 0; i < (int)histogram.size(); i++) {
        sprintf(buf, "# %f\n", histogram[i]);
        result += buf;
    }
    result += "\n";
    return result;
}

bool MultiScaleHistogram::read(FILE *fp) {
    char buf[4096];

    if(!fgets(buf, 4096, fp))
        return false;
    if(strncmp("HISTOGRAM", buf, 9)!=0)
        return false;
    int bins, scales;
    if(sscanf(buf, "HISTOGRAM bins %d scales %d", &bins, &scales)!=2)
        return false;

    // read histogram bins
    histogram.clear();
    while(fgets(buf, 4096, fp))
        if(buf[0] != '#')
            break;
        else {
            float val;
            if(sscanf(buf, "# %f", &val)!=1)
                return false;
            histogram.push_back(val);
        }

    // This gives error on databases from kenya
    //assert((int)histogram.size() == bins*scales*2);      // factor 2 because we measure two image features (C and P)

    // compute mean value
    mean = 0;
    for(int i = (int)histogram.size(); --i >= 0; )
        mean += histogram[i];
    mean /= (float) histogram.size();

    // compute L2 norm of vector-mean
    l2norm = 0;
    for(int i = (int)histogram.size(); --i >= 0; )
        l2norm += pow(histogram[i]-mean, 2);
    l2norm = sqrt(l2norm);

    return true;
}
