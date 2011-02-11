#ifndef FLOATIMAGE_H_INCLUDED
#define FLOATIMAGE_H_INCLUDED

#include <wx/wxprec.h>
#include <wx/image.h>
#include "wxImageUtils.h"


//
// FloatImage
// -- represents an image in floating point
// -- conversion routines to/from wxImage
//
class FloatImage {
    public:
        FloatImage(wxImage *, bool mono=false);
        FloatImage(int w, int h, bool mono=false);
        FloatImage(FloatImage *o);
        ~FloatImage();
        float *GetData()        { return data; };
        int GetWidth()          { return width; };
        int GetHeight()         { return height; };
        void clear();

        // draw a FloatImage to a wxImage at coords (x,y)
        void blit(wxImage *dest, int x, int y, bool normalize=true);

        // filtering
        void convolveX(double *filter, int halfwin);
        void convolveY(double *filter, int halfwin);

        // Gaussian blur
        void blur();
        void blur(int gauss_x, int gauss_y);

        // threshold
        void median_threshold();

        // Image arithmetic
        void absdifference(FloatImage *op);
        void subtract(FloatImage *op);
        void add(FloatImage *op);
        void multiply(float op);
        void multiply(FloatImage *op);
        void divide(FloatImage *op);

        // Subsample this image into another one
        void subsample(FloatImage *dest);

        // Upsamples this image into another one
        void upsample(FloatImage *upsample);

        // Normalizes or clips image into [0, N] range
        void normalize(float max = 255.0);
        void clip(float max = 255.0);

    private:
        float *data;
        int width;
        int height;
        int bpp;

        void blit_BPP3(wxImage *, int, int, bool);
        void blit_BPP1(wxImage *, int, int, bool);
        void upsample_BPP3(FloatImage *);
        unsigned char trim(float);
};


#endif // FLOATIMAGE_H_INCLUDED
