#include "FloatImage.h"
#include <float.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

//
// CONSTRUCTORS AND DESTRUCTORS
//
FloatImage::FloatImage(wxImage *img, bool mono)   {
    width = img->GetWidth();
    height = img->GetHeight();
    bpp = mono?1:3;
    data = new float[width*height*bpp];
    unsigned char *wxdat = img->GetData();
    if(bpp==3)
        for(int x = 0; x < 3*width*height; x++)
            data[x] = (float)wxdat[x];
    else
        for(int x = 0; x < width*height; x++)
            data[x] = (float)wxdat[3*x]*0.299 + (float)wxdat[3*x+1]*0.587 + (float)wxdat[3*x+2]*0.114;
}
FloatImage::FloatImage(FloatImage *o)   {
    width = o->width;
    height = o->height;
    bpp = o->bpp;
    data = new float[width*height*bpp];
    memcpy(data, o->data, width*height*bpp*sizeof(float));
}
FloatImage::FloatImage(int w, int h, bool mono)   {
    width = w;
    height = h;
    bpp = mono?1:3;
    data = new float[w*h*bpp];
}
FloatImage::~FloatImage()   {
    delete[] data;
}


//
// Draw a FloatImage to a wxImage at a specified coordinate
//
void FloatImage::blit(wxImage *dest, int x, int y, bool normalize) {
    if(bpp==1)
        blit_BPP1(dest, x, y, normalize);
    else
        blit_BPP3(dest, x, y, normalize);
}
void FloatImage::blit_BPP3(wxImage *dest, int x, int y, bool normalize) {
    // find minimum and maximum values for each channel if we're normalizing
    float minR = FLT_MAX, maxR = -FLT_MAX;
    float minG = FLT_MAX, maxG = -FLT_MAX;
    float minB = FLT_MAX, maxB = -FLT_MAX;
    if(normalize)
        for(int i = 0; i < 3*width*height; i+=3) {
            if(data[i]<minR) minR = data[i];
            if(data[i]>maxR) maxR = data[i];
            if(data[i+1]<minG) minG = data[i+1];
            if(data[i+1]>maxG) maxG = data[i+1];
            if(data[i+2]<minB) minB = data[i+2];
            if(data[i+2]>maxB) maxB = data[i+2];
        }
    maxR -= minR;
    maxG -= minG;
    maxB -= minB;

    // blit data onto the wxImage
    unsigned char *ddata = dest->GetData();
    int dw = dest->GetWidth();
    for(int r = 0; r < height; r++)   {
        int didx = 3*((y+r)*dw+x);
        int sidx = 3*(r*width);
        for(int c = 0; c < width; c++)
            if(normalize)   {
                ddata[didx++] = (unsigned char)(255.0 * (data[sidx++]-minR) / maxR);
                ddata[didx++] = (unsigned char)(255.0 * (data[sidx++]-minG) / maxG);
                ddata[didx++] = (unsigned char)(255.0 * (data[sidx++]-minB) / maxB);
            } else {
                ddata[didx++] = trim(data[sidx++]);
                ddata[didx++] = trim(data[sidx++]);
                ddata[didx++] = trim(data[sidx++]);
            }
    }
}
void FloatImage::blit_BPP1(wxImage *dest, int x, int y, bool normalize) {
    // find minimum and maximum pixel values
    float min = FLT_MAX, max = -FLT_MAX;
    if(normalize)
        for(int i = 0; i < width*height; i++) {
            if(!isnan(data[i]) && !isinf(data[i])) {
                if(data[i]<min) min = data[i];
                if(data[i]>max) max = data[i];
            }
        }
    max -= min; // max is now range of values

    // blit data onto the wxImage
    unsigned char *ddata = dest->GetData();
    int dw = dest->GetWidth();
    for(int r = 0; r < height; r++)   {
        int didx = 3*((y+r)*dw+x);
        int sidx = (r*width);
        for(int c = 0; c < width; c++, sidx++)   {
            if(isinf(data[sidx]) || isnan(data[sidx]))
                continue;
            unsigned char pix = normalize ? (unsigned char)(255.0*(data[sidx]-min)/max) : trim(data[sidx]);
            ddata[didx++] = pix;
            ddata[didx++] = pix;
            ddata[didx++] = pix;
        }
    }

}

//
// Clips a float pixel into [0,255]
//
unsigned char FloatImage::trim(float pix)   {
    if(pix > 255)
        return 255;
    if(pix < 0)
        return 0;
    return (unsigned char)pix;
}

//
// Applies a Gaussian blur to the image
//
void FloatImage::blur(int gauss_x, int gauss_y)    {
    if(gauss_x%2==0) gauss_x++;
    if(gauss_y%2==0) gauss_y++;
    if(gauss_x<3) gauss_x=3;
    if(gauss_y<3) gauss_y=3;
    int fszX = gauss_x/2;
    int fszY = gauss_y/2;

    // build two convolution filters
    double *filtX = new double[gauss_x];
    double *filtY = new double [gauss_y];
    double sum = 0;
    double sigX = gauss_x/(2*2.169);
    double sigY = gauss_y/(2*2.169);
    for(int x = 0; x < gauss_x; x++)
        sum += (filtX[x] = 1.0/(sqrt(2*M_PI)*sigX)*exp(-pow(x-gauss_x/2,2)/(2*sigX*sigX))  );
    printf("x-filter: ");
    for(int x = 0; x < gauss_x; x++) {
        filtX[x] /= sum;
        printf("%f ",filtX[x]);
    }
    printf("\n");
    sum = 0;
    for(int x = 0; x < gauss_y; x++)
        sum += (filtY[x] = 1.0/(sqrt(2*M_PI)*sigY)*exp(-pow(x-gauss_y/2,2)/(2*sigY*sigY))  );
    printf("y-filter: ");
    for(int x = 0; x < gauss_y; x++) {
        filtY[x] /= sum;
        printf("%f ",filtY[x]);
    }
    printf("\n");

    // convolve image in X-direction
    FloatImage *blurred = new FloatImage(this);
    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)  {
            int didx = bpp*(r*width+c);
            blurred->data[didx] = 0;
            if(bpp==3)  {
                blurred->data[didx+1] = 0;
                blurred->data[didx+2] = 0;
            }

            for(int wx = -fszX; wx <= fszX; wx++)
                if(c+wx>=0 && c+wx<width)   {
                    int idx = bpp*(r*width+c+wx);
                    blurred->data[didx]   += filtX[wx+fszX] * data[idx];
                    if(bpp == 3)    {
                        blurred->data[didx+1] += filtX[wx+fszX] * data[idx+1];
                        blurred->data[didx+2] += filtX[wx+fszX] * data[idx+2];
                    }
                }
        }

    // convolve image in y-direction
    for(int c = 0; c < width; c++)
        for(int r = 0; r < height; r++) {
            int didx = bpp*(r*width+c);
            data[didx] = 0;
            if(bpp == 3)    {
                data[didx+1] = 0;
                data[didx+2] = 0;
            }
            for(int wy = -fszY; wy <= fszY; wy++)
                if(r+wy>=0 && r+wy<height)   {
                    int idx = bpp*((r+wy)*width+c);
                    data[didx]   += filtY[wy+fszY] * blurred->data[idx];
                    if(bpp == 3)    {
                        data[didx+1] += filtY[wy+fszY] * blurred->data[idx+1];
                        data[didx+2] += filtY[wy+fszY] * blurred->data[idx+2];
                    }
                }
        }

    delete[] filtX;
    delete[] filtY;
    delete blurred;
}


void FloatImage::blur()    {
    int fsz = 2;
    float filter[5] = {1/16.0, 4/16.0, 6/16.0, 4/16.0, 1/16.0};

    FloatImage *blurred = new FloatImage(this);

    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)  {
            int didx = bpp*(r*width+c);
            blurred->data[didx] = 0;
            if(bpp==3)  {
                blurred->data[didx+1] = 0;
                blurred->data[didx+2] = 0;
            }
            for(int wx = -fsz; wx <= fsz; wx++)
                if(c+wx>=0 && c+wx<width)   {
                    int idx = bpp*(r*width+c+wx);
                    blurred->data[didx]   += filter[wx+2] * data[idx];
                    if(bpp == 3)    {
                        blurred->data[didx+1] += filter[wx+2] * data[idx+1];
                        blurred->data[didx+2] += filter[wx+2] * data[idx+2];
                    }
                }
        }

    for(int c = 0; c < width; c++)
        for(int r = 0; r < height; r++) {
            int didx = bpp*(r*width+c);
            data[didx] = 0;
            if(bpp == 3)    {
                data[didx+1] = 0;
                data[didx+2] = 0;
            }
            for(int wy = -fsz; wy <= fsz; wy++)
                if(r+wy>=0 && r+wy<height)   {
                    int idx = bpp*((r+wy)*width+c);
                    data[didx]   += filter[wy+2] * blurred->data[idx];
                    if(bpp == 3)    {
                        data[didx+1] += filter[wy+2] * blurred->data[idx+1];
                        data[didx+2] += filter[wy+2] * blurred->data[idx+2];
                    }
                }
        }

    delete blurred;
}

//
// Adds or subtracts another image from the current one
//
void FloatImage::subtract(FloatImage *o)    {
    assert(bpp==o->bpp && width==o->width && height==o->height);
    for(int i = bpp*width*height-1; i>=0; i--)
        data[i] -= o->data[i];
}
void FloatImage::absdifference(FloatImage *o)  {
    assert(bpp==o->bpp && width==o->width && height==o->height);
    for(int i = bpp*width*height-1; i>=0; i--)
        data[i] = fabs(data[i]-o->data[i]);
}
void FloatImage::add(FloatImage *o)    {
    assert(bpp==o->bpp && width==o->width && height==o->height);
    for(int i = bpp*width*height-1; i>=0; i--)
        data[i] += o->data[i];
}

//
// Subsamples current image into a smaller image
//
void FloatImage::subsample(FloatImage *dest)   {
    int dw = dest->GetWidth();
    int dh = dest->GetHeight();
    assert(dh<=ceil(height/2.0) && dw<=ceil(width/2.0));

    for(int r = 0; r < dh; r++) {
        int didx = bpp*r*dw;
        int sidx = bpp*(2*r)*width;
        for(int c = 0; c < dw; c++,sidx+=bpp) {
            dest->data[didx++] = data[sidx++];
            if(bpp == 3)    {
                dest->data[didx++] = data[sidx++];
                dest->data[didx++] = data[sidx++];
            }
        }
    }
}

//
// 1-D convolution in either X or Y direction
//
void FloatImage::convolveX(double *filter, int halfwin) {
    FloatImage *convolved = new FloatImage(this);
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++)  {
            int didx = bpp*(r*width+c);
            convolved->data[didx] = 0;
            if(bpp==3)  {
                convolved->data[didx+1] = 0;
                convolved->data[didx+2] = 0;
            }
            for(int wx = -halfwin; wx <= halfwin; wx++)
                if(c+wx>=0 && c+wx<width)   {
                    int idx = bpp*(r*width+c+wx);
                    convolved->data[didx]   += filter[wx+halfwin] * data[idx];
                    if(bpp == 3)    {
                        convolved->data[didx+1] += filter[wx+halfwin] * data[idx+1];
                        convolved->data[didx+2] += filter[wx+halfwin] * data[idx+2];
                    }
                }
        }
    }

    memcpy(data, convolved->data, bpp*width*height*sizeof(float));
    delete convolved;
}

void FloatImage::convolveY(double *filter, int halfwin) {
    FloatImage *convolved = new FloatImage(this);

    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)  {
            int didx = bpp*(r*width+c);
            convolved->data[didx] = 0;
            if(bpp==3)  {
                convolved->data[didx+1] = 0;
                convolved->data[didx+2] = 0;
            }
            for(int wy = -halfwin; wy <= halfwin; wy++)
                if(r+wy>=0 && r+wy<height)   {
                    int idx = bpp*((r+wy)*width+c);
                    convolved->data[didx]   += filter[wy+halfwin] * data[idx];
                    if(bpp == 3)    {
                        convolved->data[didx+1] += filter[wy+halfwin] * data[idx+1];
                        convolved->data[didx+2] += filter[wy+halfwin] * data[idx+2];
                    }
                }
        }

    memcpy(data, convolved->data, bpp*width*height*sizeof(float));
    delete convolved;
}


//
// Upsamples current image into a larger one
//
void FloatImage::upsample(FloatImage *dest)    {
    assert(bpp == dest->bpp);
    if(bpp == 3)
        upsample_BPP3(dest);
}
void FloatImage::upsample_BPP3(FloatImage *dest)   {
    int dw = dest->GetWidth();
    int dh = dest->GetHeight();
    assert(dh<=2*height && dw<=2*width);
    float filter[5] = {1/16.0, 4/16.0, 6/16.0, 4/16.0, 1/16.0};
    memset(dest->data, 0, dw*dh*3*sizeof(float));

    for(int r = 0; r < height; r++) {
        int didx = 3*2*r*dw;
        int sidx = 3*r*width;
        for(int c = 0; c < width; c++)   {
            for(int b = 0; b < 3; b++)
                dest->data[didx++] = data[sidx++];
            didx += 3;
        }
    }
    return;


    for(int r = 0; r < dh; r++) {
        int didx = 3*r*dw;
        for(int c = 0; c < dw; c++) {
            int sidx = 3*((r/2)*width+(c/2));
            dest->data[didx++] = data[sidx++];
            dest->data[didx++] = data[sidx++];
            dest->data[didx++] = data[sidx++];
        }
    }



    for(int r = 0; r < dh; r++)
        for(int c = 0; c < dw; c++) {
            double R = 0, G = 0, B = 0;
            for(int m = -2; m <= 2; m++)
                for(int n = -2; n <= 2; n++)
                    if( (r+m)%2==0 && ((r+m)/2>=0) && ((r+m)/2<height) &&
                        (c+n)%2==0 && ((c+n)/2>=0) && ((c+n)/2<width) )  {
                            int sidx = 3*(((r+m)/2)*width + (c+n)/2);
                            R += filter[m+2]*filter[n+2]*data[sidx];
                            G += filter[m+2]*filter[n+2]*data[sidx+1];
                            B += filter[m+2]*filter[n+2]*data[sidx+2];
                    }

            int didx = (r*dw+c)*3;
            dest->data[didx] = 4*R;
            dest->data[didx+1] = 4*G;
            dest->data[didx+2] = 4*B;
        }

}


//
// Multiplies or divides an image by a scalar or by another image pixel-wise
//
void FloatImage::multiply(float op) {
    for(int i = 0; i < bpp*width*height; i++)
        data[i] *= op;
}
void FloatImage::multiply(FloatImage *op)   {
    const int c = op->bpp==3 ? 1 : 3;
    for(int i = 0; i < bpp*width*height; i++)
        data[i] *= op->data[i/c];
}
void FloatImage::divide(FloatImage *op) {
    const int c = op->bpp==3 ? 1 : 3;
    for(int i = 0; i < bpp*width*height; i++)
        data[i] /= op->data[i/c];
}


//
// Normalizes image data into [0,N] range
//
void FloatImage::normalize(float N) {
    float minR = FLT_MAX, maxR = -FLT_MAX;
    float minG = FLT_MAX, maxG = -FLT_MAX;
    float minB = FLT_MAX, maxB = -FLT_MAX;

    for(int i = 0; i < width*height; i++) {
        if(data[bpp*i]<minR) minR = data[bpp*i];
        if(data[bpp*i]>maxR) maxR = data[bpp*i];
        if(bpp == 3) {
            if(data[bpp*i+1]<minG) minG = data[bpp*i+1];
            if(data[bpp*i+1]>maxG) maxG = data[bpp*i+1];
            if(data[bpp*i+2]<minB) minB = data[bpp*i+2];
            if(data[bpp*i+2]>maxB) maxB = data[bpp*i+2];
        }
    }
    maxR -= minR;
    maxG -= minG;
    maxB -= minB;

    for(int i = 0; i < width*height; i++) {
        data[bpp*i] = (N * (data[bpp*i]-minR) / maxR);
        if(bpp == 3) {
            data[bpp*i+1] = (N * (data[bpp*i+1]-minG) / maxG);
            data[bpp*i+2] = (N * (data[bpp*i+2]-minB) / maxB);
        }
    }
}
void FloatImage::clip(float N)  {
    for(int i = 0; i < bpp*width*height; i++)   {
        if(data[i] < 0)
            data[i] = 0;
        if(data[i] > N)
            data[i] = N;
    }
}

void FloatImage::clear()    {
    memset(data, 0, sizeof(float)*bpp*width*height);
}

//
// threshold the image based on the median pixel value in
// each channel
//
void FloatImage::median_threshold() {
    vector<unsigned char> pixel_values;

    for(int i = bpp*width*height-3; i >= 0; i-=bpp)
        pixel_values.push_back(data[i]);
    sort(pixel_values.begin(), pixel_values.end());
    unsigned s = pixel_values.size();
    unsigned char median = (s%2==0) ? (pixel_values[s/2]+pixel_values[s/2+1])/2 : pixel_values[s/2];

    for(int i = bpp*width*height-3; i >= 0; i-=bpp)   {
        data[i] = (data[i]<median) ? 0 : 0xFF;
        if(bpp == 3)
            data[i+1] = data[i+2] = data[i];
    }
}


