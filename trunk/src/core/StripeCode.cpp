#include <stdio.h>
#include <float.h>
#include "StripeCode.h"

// shorthand
typedef unsigned char BYTE;
#define DATA(x) (x.GetData())
#define ROW_ITERATE(x) for(int x = imgH; --x >= 0; )
#define COL_SCAN(x) for(int x = imgW; --x >= 0; )
#define ROFFS(x) (3*imgW*(x))
#define COFFS(y) (3*(y))
#define PIX1(dat,col,val) {dat[3*col]=val;dat[3*col+1]=val;dat[3*col+2]=val;}
#define GRAYPIX(dat,col) ((0.3*(float)dat[3*col]+0.59*(float)dat[3*col+1]+0.11*(float)dat[3*col+2]))
#define PREAMBLE(x) int imgW=x.GetWidth();int imgH=x.GetHeight();unsigned char *dat = x.GetData();
#define MAX(x,y) (((x)<(y))?(y):(x))
#define MIN(x,y) (((x)<(y))?(x):(y))

#define BACKTRACE_DIAG 1
#define BACKTRACE_UP 2
#define BACKTRACE_LEFT 3

// defaults
double StripeCode::INDELCOST = 0.55;
bool StripeCode::RETMINCOST = false;
bool StripeCode::USERATIOS = true;
bool StripeCode::NED = false;

// inherited from image feature interface 'ImageFeatures'
bool StripeCode::read(const wxImage *imgPar) {
    wxImage *copy = Image::CloneImage((wxImage*)imgPar);
    wxImage &img = *copy;

    // delete old stripestrings
    stripes.clear();

    // save original image to plot difference
//    FloatImage orig(&img);
#ifdef DEBUG
   	img.SaveFile(_("processing-1.jpg"), wxBITMAP_TYPE_JPEG);
#endif

    // filter image
    Image::filt_average(img);
#ifdef DEBUG
    img.SaveFile(_("processing-2.jpg"), wxBITMAP_TYPE_JPEG);
#endif

	Image::filt_threshold(img);
#ifdef DEBUG
    img.SaveFile(_("processing-3.jpg"), wxBITMAP_TYPE_JPEG);
#endif

    // plot the difference from the original
/*    FloatImage final(&img);
    orig.subtract(&final);
    wxImage *diff = Image::CloneImage(copy);
    orig.blit(diff, 0, 0, true);
	fname.Printf(_("processing/StripeCode-%07d-4.jpg"), imgTag);
    diff->SaveFile(fname, wxBITMAP_TYPE_JPEG);
    delete diff;
*/

    // read a set of stripestrings
    int chunkHt = ceil(Image::filt_average_rowchunk * img.GetHeight());
    for(int st = ceil(1.0/Image::filt_average_rowchunk); --st >= 0; )
        stripes.push_back(read(img, st*chunkHt));

    delete copy;
    return true;
}

// inherited from image feature interface 'ImageFeatures'
double StripeCode::compare(ImageFeatures *par_img2, void *arg) const {
    StripeCode &img2 = *((StripeCode*)par_img2);
    assert(stripes.size() == img2.stripes.size());

	double mincost = DBL_MAX, avgcost = 0, maxcost = DBL_MIN;
    for(int i = (int)stripes.size(); --i >= 0; ) {
        double cost = compare(stripes[i], img2.stripes[i]);
        avgcost += cost;
        if(cost < mincost)
            mincost = cost;
		if(cost > maxcost)
			maxcost = cost;
    }
	if(stripes.size() >= 2)
		avgcost = (avgcost - maxcost) / (float) (stripes.size()-1.0);	// trimmed mean
	else
		avgcost /= (float)stripes.size();

    return RETMINCOST?mincost:avgcost;
}

double StripeCode::compare(const StripeString &s1, const StripeString &s2, DPMatrix *dpmat) {
    // create DP matrix or set up the one user provided
    DPMatrix *loc;
    if(dpmat) {
        loc = dpmat;
        loc->clear();
        loc->init(s1.size(), s2.size());
    } else
        loc = new DPMatrix(s1.size(), s2.size());
    DPMatrix &dpmatrix = *loc;

    // outer iteration for normalized edit distance computation
    double cost         = 0;
    double oldcost      = 0;
    double ned_constant = 0;
    for(int ned_iter = 0; ned_iter < 4; ned_iter++) {
        // fill in row and column 0
        for(int r = 0; r < dpmatrix.nrows; r++) {
            cost = INDELCOST-ned_constant  ;
            dpmatrix(r,0) = cost*r;
            dpmatrix.costAt(r,0) = cost;
            dpmatrix.back(r,0) = BACKTRACE_UP;
        }
        for(int c = 0; c < dpmatrix.ncols; c++) {
            cost = INDELCOST-ned_constant  ;
            //assert(cost >= 0);
            dpmatrix(0,c) = cost*c;
            dpmatrix.costAt(0,c) = cost;
            dpmatrix.back(0,c) = BACKTRACE_LEFT;
        }

        // set up for DP
        double fac = (double)s1[0].abslen / (double)s2[0].abslen;

        // start filling in the DP matrix
        for(int d = 1; d <= dpmatrix.nrows+dpmatrix.ncols; d++) {
        	const int max_i = (d<=dpmatrix.nrows?d:dpmatrix.ncol+dpmatrix.nrows-d);
#			pragma omp parallel for shared(dpmatrix) schedule(guided)
            for(int i = 1; i <= max_i; i++) {

            	const int r, c;
                double a1,a2;
                if(USERATIOS) {
                    a1 = s1[r].ratio;
                    a2 = s2[c].ratio;
                } else {
                    a1 = s1[r].abslen;
                    a2 = s2[c].abslen * fac;
                }

                double matchcost = (1.0 - (a1<a2 ? a1/a2 : a2/a1)) + ((s1[r].col==s2[c].col)?0:1000);

                double leftcost = dpmatrix(r,c-1) + INDELCOST       -ned_constant ;
                double upcost   = dpmatrix(r-1,c) + INDELCOST       -ned_constant ;
                double diagcost = dpmatrix(r-1,c-1) + matchcost     -ned_constant ;
                //assert(leftcost >= 0);
                //assert(upcost >= 0);
                //assert(diagcost >= 0);

                // find optimal edit path
                if(leftcost < upcost && leftcost < diagcost) {
                    dpmatrix.back(r,c)  = BACKTRACE_LEFT;
                    dpmatrix(r,c)       = leftcost;
                    dpmatrix.costAt(r,c)= INDELCOST;
                } else {
                    if(diagcost < leftcost && diagcost < upcost) {
                        dpmatrix.back(r,c)  = BACKTRACE_DIAG;
                        dpmatrix(r,c)       = diagcost;
                        dpmatrix.costAt(r,c)= matchcost;
                    } else {
                        dpmatrix.back(r,c)  = BACKTRACE_UP;
                        dpmatrix(r,c)       = upcost;
                        dpmatrix.costAt(r,c)= INDELCOST;
                    }
                }
            }
        }


        // backtrace to compute edit path length and non-NED cost
        int r = dpmatrix.nrows-1; int c = dpmatrix.ncols-1;
        int editlen = 0; double true_cost = 0;
        while(r>=0 && c>=0) {
            editlen++;
            if(dpmatrix.back(r,c)==BACKTRACE_UP) {
                r--;
                if(r >= 0)
                    true_cost += INDELCOST;
            } else {
                if(dpmatrix.back(r,c)==BACKTRACE_LEFT) {
                    c--;
                    if(c >= 0)
                        true_cost += INDELCOST;
                } else {
                    double a1 = s1[r].ratio;
                    double a2 = s2[c].ratio;
                    true_cost += (1.0 - (a1<a2 ? a1/a2 : a2/a1)) + ((s1[r].col==s2[c].col)?0:1000);
                    r--;
                    c--;
                }
            }
        }

        // if we're not using normalized edit distance, this is it--just quit
        // but first, dump the edit matrix
//        wxImage *img = plotEditPath(s1, s2, dpmatrix);
        wxString fname;
        fname.Printf(_("EDITPATH-%d.png"), ned_iter);
//        img->SaveFile(fname, wxBITMAP_TYPE_PNG);
//        delete img;

        // compute NED ratios
        cost = dpmatrix(dpmatrix.nrows-1, dpmatrix.ncols-1);
        double lam_star = true_cost / (double)editlen;
       // printf("NED_ITER %d: cost %f (true %f) oldcost %f edit_len %d ned_constant %f lam_star %f diff %f\n", ned_iter, cost, true_cost, oldcost, editlen, ned_constant, lam_star, fabs(lam_star-oldcost));
        if(!NED || fabs(oldcost-lam_star)<1e-5) {
            // fractional programming has converged to the solution
            cost = lam_star;
            break;
        }
        oldcost = lam_star;
        ned_constant = lam_star;

    }


    if(dpmat==NULL)
        delete &dpmatrix;

	return cost;
}

string StripeCode::toString() {
    string result;
    char buf[128];

    sprintf(buf, "STRIPECODE %d\n", (int)stripes.size());
    result = buf;

    int n = 0;
	for(vector<StripeString>::iterator itr = stripes.begin(); itr!= stripes.end(); itr++) {
	    StripeString &stripe = *itr;
	    sprintf(buf, "stripestring %d %d\n", n++, (int)stripe.size());
	    result += buf;
	    for(StripeString::iterator it2 = stripe.begin(); it2 != stripe.end(); it2++) {
            sprintf(buf, "# %d %f %f\n", it2->col, it2->abslen, it2->ratio);
            result += buf;
	    }
	}
	result += "END STRIPECODE\n";
	return result;
}

StripeString StripeCode::read(wxImage &img, int row) {
	return read(img.GetData()+(3*img.GetWidth()*row), img.GetWidth());
}

StripeString StripeCode::read(unsigned char *row, int len) {
	StripeString result;
	Stripe s;
	s.col = row[0];
	s.abslen = 1;
	s.ratio = 1;
	for(int x = 3; x < 3*len; x+=3)
		if(row[x] != s.col) {
			// color change
			s.abslen = x/3 - s.abslen + 1;
			s.ratio  = s.abslen;
			if(result.size())
				s.ratio = s.abslen / result[result.size()-1].abslen;
			result.push_back(s);

			// new stripe
			s.abslen = x/3;
			s.col    = row[x];
		}
	// push last stripe
	s.abslen = len - s.abslen;
	if(result.size())
		s.ratio = s.abslen / result[result.size()-1].abslen;
	result.push_back(s);

	return result;
}



void StripeCode::plotStripeString(wxImage &img, StripeString &code, int x, int y, int w, int h, bool grad) {
	unsigned char *data = img.GetData();
	int imgW = img.GetWidth();

    // get scaling factor
	double sc_w = code[0].abslen; double lastabs = sc_w;
	for(unsigned j = 1; j < code.size(); j++)
		sc_w += (lastabs *= code[j].ratio);
	double scale = w / sc_w;
//	printf("w %f sc_w %f scale %f\n", (double)w, sc_w, scale);

	// plot
	int c = 0;
	for(unsigned s = 0; s < code.size(); s++) {
		const Stripe &stripe = code[s];
//		int len = scale* ((s==0)?stripe.abslen: stripe.ratio*code[s-1].abslen);
		int len = scale * stripe.abslen;
//		printf("replot strip %u abslen %f ratio %f --> len %d\n", s, stripe.abslen, stripe.ratio, len);
		for(int i = 0; i < len; i++) {
			for(int r = 0; r < h; r++) {
                if(!grad)
                    data[3*((y+r)*imgW + c+i+x)]
                    = data[3*((y+r)*imgW + c+i+x) + 1]
                    = data[3*((y+r)*imgW + c+i+x) + 2]
                    = stripe.col;
                else {
                    data[3*((y+r)*imgW + c+i+x)] = stripe.col;
                    data[3*((y+r)*imgW + c+i+x) + 1] = (stripe.col==0||stripe.col==0xFF) ? stripe.col : 0;
                    data[3*((y+r)*imgW + c+i+x) + 2] = (stripe.col==0||stripe.col==0xFF) ? stripe.col : 0;
                }
			}
			if(i == 0) {
			    // draw border
			    for(int r = 0; r < h; r++)
                    data[3*((y+r)*imgW + c+i+x)]
                    = data[3*((y+r)*imgW + c+i+x) + 1]
                    = data[3*((y+r)*imgW + c+i+x) + 2]
                    = 0;
			}
		}

		c += len;
	}
}

int StripeCode::totalWidth() {
//    if(stripes.size() < 1)
//        return 0;
//	int wid = stripes[0].abslen; double sum = wid;
//	for(int w = 1; w < (int)stripes.size(); w++)
//		wid += (sum *= stripes[w].ratio);
//	return wid;
    return 0;
}

#define INDELCOST 0

//double StripeCode::compare(StripeCode &s2, void* dpmat) {


bool StripeCode::read(FILE *fp) {
    char buf[4096];

    // get the number of stripestrings
    fgets(buf, 4096, fp);
    if(strncmp(buf, "STRIPECODE ", 11)!=0)
        return false;
    int nStripeStrings;
    if(sscanf(buf, "STRIPECODE %d", &nStripeStrings)==0)
        return false;
    stripes.clear();

    // read the stripestrings
    for(int i = 0; i < nStripeStrings; i++) {
        StripeString stripestr;

        fgets(buf, 4096, fp);
        if(strncmp(buf, "stripestring ", 12)!=0)
            return false;
        int ssnum, nstripes;
        if(sscanf(buf, "stripestring %d %d", &ssnum, &nstripes)!=2)
            return false;
        for(int j = 0; j < nstripes; j++) {
            fgets(buf, 4096, fp);
            if(buf[0] != '#')
                return false;
            Stripe str; int col;
            if(sscanf(buf, "# %d %f %f", &col, &str.abslen, &str.ratio)!=3)
                return false;
            str.col = col;
            stripestr.push_back(str);
        }
        stripes.push_back(stripestr);
    }

    return true;
}

wxImage* StripeCode::plotComparison(StripeCode &sc2) {
    DPMatrix dpmat(1,1);
    int nStripes = (int) stripes.size();
    assert(nStripes == (int)sc2.stripes.size());
    wxImage *ret = new wxImage(640, 20*nStripes);
    printf("plotComp\n");
    memset(ret->GetData(), 0x40, 3*640*20*nStripes);

    for(int s = 0; s < nStripes; s++) {
        //double cost = compare(stripes[s], sc2.stripes[s], &dpmat);
        //assert(cost == dpmat(dpmat.nrows-1, dpmat.ncols-1));
        StripeString recolored = stripes[s];

        int r = dpmat.nrows-1;
        int c = dpmat.ncols-1;
        while(r>0 && c>0) {
            if(dpmat.back(r,c)==BACKTRACE_DIAG) {
                assert(dpmat.costAt(r,c)<=1.01 && dpmat.costAt(r,c)>=0);

                // assign colors by k-means centers, k = 3
                int col;
                double d1 = fabs(dpmat.costAt(r,c)-0.14418159);
                double d2 = fabs(dpmat.costAt(r,c)-0.28142097);
                double d3 = fabs(dpmat.costAt(r,c)-0.04022515);
                if(d1<d2 && d1<d3)
                    col = 201;  // lowest cost = brightest red
                else {
                    if(d2<d3 && d2<d1)
                        col = 134;
                    else
                        col = 67;
                }

                // assign colors by interquartile range
                double cost = dpmat.costAt(r,c);
                if(cost < 0.037957)
                    col = 0xFE;
                else {
                    if(cost < 0.0909)
                        col = 201;
                    else {
                        if(cost < 0.162321)
                            col = 134;
                        else
                            col = 67;
                    }
                }

                recolored[r].col = col;//+bwmod;
                //printf("--- cost %f col %d\n", dpmat.costAt(r,c), recolored[r].col);
                if(recolored[r].col==0) recolored[r].col++;
                if(recolored[r].col==0xFF) recolored[r].col--;
                r--;
                c--;
            } else {
                if(dpmat.back(r,c)==BACKTRACE_LEFT) {
                    c--;
                } else {
                    //recolored[r].col=0xEE;
                    r--;
                }
            }
        }

        plotStripeString(*ret, recolored, 0, s*20, 310, 20, true);
        plotStripeString(*ret, sc2.stripes[s], 320, s*20, 310, 20);


    }

    return ret;
}

DPMatrix::DPMatrix() {
    dat = NULL;
    backtrace = NULL;
    cost = NULL;
}

void DPMatrix::dump() {
	for(int r = 0; r < nrows; r++, printf("\n"))
		for(int c = 0; c < ncols; c++)
			printf("%.1f(%d) ", dat[r][c], backtrace[r][c]);
}

DPMatrix::DPMatrix(int nr, int nc) {
    dat = NULL;
    backtrace = NULL;
    cost = NULL;
    init(nr, nc);
}

void DPMatrix::clear() {
    if(dat) {
        for(int r = 0; r < nrows; r++) {
            delete[] dat[r];
            delete[] backtrace[r];
            delete[] cost[r];
        }
        delete[] cost;
        delete[] dat;
        delete[] backtrace;
        dat = NULL;
    }
}

void DPMatrix::init(int nr, int nc) {
    clear();
	nrows = nr;
	ncols = nc;
	dat = new double* [nr];
	backtrace = new int* [nr];
	cost = new double* [nr];
	for(int r = 0; r < nrows; r++) {
		dat[r] = new double [nc];
		backtrace[r] = new int [nc];
		cost[r] = new double[nc];
		memset(cost[r], 0, sizeof(double)*nc);
		memset(dat[r], 0, sizeof(double)*nc);
		memset(backtrace[r], 0, sizeof(int)*nc);
	}
}

DPMatrix::~DPMatrix() {
    clear();
}

double& DPMatrix::operator()(const int r, const int c) {
	return dat[r][c];
}
int& DPMatrix::back(const int r, const int c) {
	return backtrace[r][c];
}
double& DPMatrix::costAt(const int row, const int col) {
    return cost[row][col];
}


//
// Image utility functions
//

// filters: replace every N rows with the median value of the rows
//          i.e., img[x,0]   = median( (x,0), (x,1), ..., (x,N-1) )
//                img[x,1]   = median( (x,0), (x,1), ..., (x,N-1) )
//                ...
//                img[x+N,0] = median( (x,N), (x,N+1), ..., (x,2N-1) )
//                ...
void Image::filt_smooth(wxImage &img) {
	FloatImage *fimg = new FloatImage(&img);
	fimg->blur(img.GetWidth()*filt_smooth_gauss_scale_x, img.GetHeight()*filt_smooth_gauss_scale_y);
	fimg->blit(&img, 0, 0, true);
	delete fimg;
}




// filters: replace every N rows with the average row
void Image::filt_average(wxImage &img) {
	PREAMBLE(img);
	int *avgrow = new int[imgW*3];
	int chunk = ceil(Image::filt_average_rowchunk * (float)imgH);
	if(chunk < 1) {
		printf("die %d %d rc %f\n", imgH, chunk, filt_average_rowchunk);
		abort();
	}

	for(int y = 0; y < imgH; y += chunk) {
		memset(avgrow, 0, sizeof(int)*imgW*3);
		int rows = 0;
		// compute average pixel for each column
		for(int y2 = y; y2 < imgH && rows < chunk; y2++, rows++) {
			BYTE *row = dat+ROFFS(y2);
			for(int x = 0; x < 3*imgW; x++)
				avgrow[x] += row[x];
		}
		for(int x = 0; x < 3*imgW; x++)
			avgrow[x] /= rows;

		// replace image data
		for(int y2 = y; y2 < y+rows; y2++) {
			BYTE *row = dat+ROFFS(y2);
			for(int x = 0; x < 3*imgW; x++)
				row[x] = (unsigned char) avgrow[x];
		}
	}

	delete[] avgrow;
}

void Image::filt_threshold_mean(wxImage &img) {
	PREAMBLE(img);

	ROW_ITERATE(y) {
		BYTE *row = dat + ROFFS(y);

		// compute mean pixel value for this row
		double pix_sum = 0, pix_num = 0;
		COL_SCAN(x) {
			BYTE pix = GRAYPIX(row, x);
			pix_sum += pix;
			pix_num++;
		}

		// find threshold pixel value
		BYTE thresh = pix_sum/pix_num;

		// threshold filter this row
		COL_SCAN(x)
			PIX1(row, x, (GRAYPIX(row, x)<thresh) ? 0 : 255);
	}
}

// filters: threshold each row independently based on a quantile
void Image::filt_threshold(wxImage &img) {
	PREAMBLE(img);
	int hist[256], hist_sum;

	ROW_ITERATE(y) {
		// build grayscale histogram for this row
		BYTE *row = dat + ROFFS(y);
		memset(hist, 0, sizeof(int)*256);
		hist_sum = 0;
		COL_SCAN(x) {
			BYTE pix = GRAYPIX(row, x);
			hist[pix]++;
			hist_sum ++;
		}

		// find threshold pixel value as the requested quantile of pixel values (grayscale)
		int thresh = 0;
		for(int m = 0, sum = 0, lim = hist_sum*filt_threshold_quantile; m < 256; m++, thresh=m)
			if((sum+=hist[m]) >= lim)
				break;

		// threshold filter this row
		COL_SCAN(x)
			PIX1(row, x, (GRAYPIX(row, x)<thresh) ? 0 : 255);
	}
}




#define EP_IMGW 1000
#define EP_SCHT 20
wxImage* StripeCode::plotEditPath(StripeString &s1, StripeString &s2, DPMatrix &mat) {
    wxImage *ret = NULL;
    double *len1 = new double[s1.size()];
    double *len2 = new double[s2.size()];
    double totalWidth1 = 0, totalWidth2 = 0;

    // compute stripecode widths
    double max1 = 0;
    for(unsigned x = 0; x < s1.size(); x++)
        totalWidth1 += (len1[x] = s1[x].abslen);
    for(unsigned x = 0; x < s2.size(); x++)
        totalWidth2 += (len2[x] = s2[x].abslen);
    for(unsigned x = 0; x < s1.size(); x++) {
        len1[x] /= totalWidth1;
        if(len1[x] > max1)
            max1 = len1[x];
    }
    for(unsigned x = 0; x < s2.size(); x++)
        len2[x] /= totalWidth2;
    int scW = ceil((double)EP_IMGW / (1.0+max1));
    int imgH = (s1.size()+1)*EP_SCHT;

    // rescale widths to our image size
    max1 = 0;
    for(unsigned x = 0; x < s1.size(); x++) {
        len1[x] = ceil(len1[x]* scW);
        if(len1[x] > max1)
            max1 = len1[x];
    }
    int imgW = max1;
    for(unsigned x = 0; x < s2.size(); x++)
        imgW += (len2[x] = ceil(len2[x]*scW));
    imgW++;
    imgH++;

    // create image and clear it to a grayish color
    ret = new wxImage(imgW, imgH);
    memset(ret->GetData(), 0xDD, ret->GetWidth()*ret->GetHeight()*3);
//    unsigned char *dat = ret->GetData();

    // draw stripestring along left
    for(unsigned i = 0; i < s1.size(); i++) {
        Image::DrawBox(ret, max1-len1[i], EP_SCHT*(i+1), len1[i], EP_SCHT, s1[i].col, s1[i].col, s1[i].col, true);
        Image::DrawBox(ret, max1-len1[i], EP_SCHT*(i+1), len1[i], EP_SCHT, 0,0,0, false);
    }

    // draw stripestriping along top
    for(unsigned i = 0, s = max1; i < s2.size(); s+=len2[i], i++) {
        Image::DrawBox(ret, s, 0, len2[i], EP_SCHT, s2[i].col, s2[i].col, s2[i].col, true);
        Image::DrawBox(ret, s, 0, len2[i], EP_SCHT, 0, 0, 0, false);
    }

    // draw grid
    Image::DrawBox(ret, max1, EP_SCHT, imgW-max1-1, imgH-EP_SCHT-1, 255, 255, 255, true);

    for(int r = 0; r <= (int)s1.size(); r++)
        Image::DrawLine(ret, max1, EP_SCHT*(r+1), imgW-1, EP_SCHT*(r+1), 80, 80, 80);
    for(unsigned c=0, x=max1; c < s2.size(); x+=len2[c], c++)
        Image::DrawLine(ret, x, EP_SCHT, x, imgH-1, 80, 80, 80);

    // draw border around edit matrix
    Image::DrawBox(ret, max1, EP_SCHT, imgW-max1-1, imgH-EP_SCHT-1, 0, 0, 0, false);

    // draw edit path
    int r = mat.nrows-1, c = mat.ncols-1;
    assert(c == (int)s2.size()-1 && r == (int)s1.size()-1);

    int y = imgH-1;
    int x = imgW-1;
    while(r >= 0 && c >= 0) {
        int b = mat.back(r,c);
        int nx = 0, ny = 0;
        if(b == BACKTRACE_LEFT) {
            nx = x - len2[c];
            ny = y;
            if(c == mat.ncols-1)
                nx++;   // hack to make pretty
            c--;
        } else {
            if(b == BACKTRACE_UP) {
                r--;
                nx = x;
                ny = y - EP_SCHT;
            } else {
                nx = x - len2[c];
                ny = y - EP_SCHT;
                if(c == mat.ncols-1)
                    nx++;   // pretty hack
                r--;
                c--;
            }
        }

        Image::DrawLine(ret, x, y, nx, ny, 0xFF, 0, 0);

        x = nx;
        y = ny;
    }

    delete[] len1;
    delete[] len2;

    return ret;
}
