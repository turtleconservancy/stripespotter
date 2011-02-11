#ifndef WXIMAGEUTILS_H_INCLUDED
#define WXIMAGEUTILS_H_INCLUDED
#include <wx/image.h>



namespace Image {
	void filt_average(wxImage &img);
	void filt_threshold(wxImage &img);
	void filt_threshold_mean(wxImage &img);
	void filt_smooth(wxImage &img);

    wxImage *ExtractRect(wxImage *src, int x, int y, int w, int h);
	void DrawBox(wxImage *src, int x, int y, int w, int h, unsigned char R, unsigned char G, unsigned char B, bool filled);
    wxImage *CloneImage(wxImage *src);
    void DrawLine(wxImage *img, int x1, int y1, int x2, int y2, unsigned char R, unsigned char G, unsigned char B);


#ifdef GLOBALS
	// default values
	float filt_average_rowchunk  	= 0.1;	// percent of height
	float filt_threshold_quantile	= 0.5;	// median filter
	float filt_smooth_gauss_scale_x = 0.005;  // percent of width
	float filt_smooth_gauss_scale_y = 0.001;  // percent of height
	int multiscale_octaves          = 4;     // number of scales for multi-resolution histogram
	int multiscale_hist_bins        = 10;    // histogram bins for each feature
#else
	extern float filt_average_rowchunk;
	extern float filt_threshold_quantile;
    extern float filt_smooth_gauss_scale_x;
	extern float filt_smooth_gauss_scale_y;
    extern int multiscale_octaves;
    extern int multiscale_hist_bins;
#endif
};


#endif // WXIMAGEUTILS_H_INCLUDED
