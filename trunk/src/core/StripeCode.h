#ifndef __STRIPECODE_H
#define __STRIPECODE_H
#include <stdio.h>
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/image.h>
#include <vector>
#include <string>
#include <map>
#include "FloatImage.h"
using namespace std;


// a single color segment with an absolute length,
// color code (for zebras, just 0 and 1), and ratio
// from previous stripe if in a collection.
typedef struct {
	unsigned char col;
	float abslen;
	float ratio;
} Stripe;
typedef vector<Stripe> StripeString;

// dynamic programming matrix
class DPMatrix {
	public:
		DPMatrix(int nr, int nc);
		DPMatrix();
		~DPMatrix();
		double& operator()(const int row, const int col);
		int& back(const int row, const int col);
		double& costAt(const int row, const int col);
		void dump();
		void clear();
		void init(int nr, int nc);

		double **dat;
		int **backtrace;
		double **cost;
		int nrows, ncols;
};

// a high-level image feature set (can plug various algorithms in here)
class ImageFeatures {
    public:
		virtual bool read(const wxImage *img) = 0;              // read from image
		virtual bool read(FILE *fp) = 0;                                    // read serialized version from file
        virtual double compare(ImageFeatures *img2, void *arg) const = 0;
        virtual string toString() = 0;
        virtual ImageFeatures *clone() = 0;
};

// Multi-scale histogram of image features as described in [Ravela and Gamble, 2004]
class MultiScaleHistogram : public ImageFeatures {
    public:
		// inherited from image feature interface 'ImageFeatures'
		bool read(const wxImage *img);              // read from image
		bool read(FILE *fp);                                    // read serialized version from file
		string toString();                                      // serialize this object
		double compare(ImageFeatures *img2, void *arg) const;   // distance function
        MultiScaleHistogram *clone() { return new MultiScaleHistogram; }

    protected:
        vector<double> histogram;
        double l2norm, mean;
};

// StripeCode functionality
class StripeCode : public ImageFeatures {
	public:
		// inherited from image feature interface 'ImageFeatures'
		bool read(const wxImage *img);              // read from image
		bool read(FILE *fp);                                    // read serialized version from file
		string toString();                                      // serialize this object
		double compare(ImageFeatures *img2, void *arg) const;   // distance function
        StripeCode *clone() { return new StripeCode; }

        // public but specific to StripeCodes (for now)
        wxImage* plotComparison(StripeCode &sc2);
        static wxImage* plotEditPath(StripeString &s1, StripeString &s2, DPMatrix &mat);

        // args
        static double INDELCOST;
        static bool RETMINCOST;
        static bool USERATIOS;
        static bool NED;

    protected:
        vector<StripeString> stripes;

        static double compare(const StripeString &s1, const StripeString &s2, DPMatrix *dpmat=NULL);

        // feature extraction from various sources
		StripeString read(unsigned char *row, int len);     // reads Stripestring from a single line of 'len' pixels
		StripeString read(wxImage &img, int row);           // same as above, but first arg is wxImage (convenience only)

        // utility
		void plotStripeString(wxImage &img, StripeString &code, int x, int y, int w, int h, bool gradient=false);
		int totalWidth();

};

#endif
