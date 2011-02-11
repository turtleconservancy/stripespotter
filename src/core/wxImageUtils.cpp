#include "wxImageUtils.h"

#define MONO(R,G,B) ((0.2125*(double)R) + (0.7154*(double)G) + (0.0721*(double)B))
#define XY2IDX(x,y,w)  (((y)*(w)+(x))*3)
#define MAX(x,y) ((x)<(y)?(y):(x))
#define MIN(x,y) ((x)<(y)?(x):(y))


wxImage *Image::CloneImage(wxImage *src) {
    int w = src->GetWidth();
    int h = src->GetHeight();
    unsigned char *data = (unsigned char*) malloc(w*h*3);
    memcpy(data, src->GetData(), w*h*3);
    wxImage *ret = new wxImage(w, h, data);
    return ret;
}

wxImage *Image::ExtractRect(wxImage *src, int x, int y, int w, int h) {
    wxImage *dst = new wxImage(w, h);
    int srcw = src->GetWidth();
    unsigned char *srcdat = src->GetData();
    unsigned char *dstdat = dst->GetData();

    for(int r = 0; r < h; r++)
        memcpy(dstdat+3*r*w, srcdat+3*((r+y)*srcw+x), 3*w);
    return dst;
}

void Image::DrawBox(wxImage *src, int x, int y, int w, int h, unsigned char R, unsigned char G, unsigned char B, bool filled=false) {
    int img_w = src->GetWidth();
    unsigned char *dat = src->GetData();

    if(filled) {
        for(int r = 0; r < h; r++) {
            int idx = 3*((y+r)*img_w+x);
            for(int c = 0; c < w; c++, idx+=3)  {
                dat[idx]  = R;
                dat[idx+1]= G;
                dat[idx+2]= B;
            }
        }
        return;
    }

    int idx = 3*(y*img_w+x);
    for(int c = 0; c < w; c++, idx+=3)  {
        dat[idx]  = R;
        dat[idx+1]= G;
        dat[idx+2]= B;
    }

    idx = 3*((y+h)*img_w+x);
    for(int c = 0; c < w; c++, idx+=3)  {
        dat[idx]  = R;
        dat[idx+1]= G;
        dat[idx+2]= B;
    }
    idx = 3*(y*img_w+x);
    for(int r = 0; r < h; r++, idx+=img_w*3)    {
        dat[idx]  = R;
        dat[idx+1]= G;
        dat[idx+2]= B;
    }
    idx = 3*(y*img_w+x+w);
    for(int r = 0; r < h; r++, idx+=img_w*3)    {
        dat[idx]  = R;
        dat[idx+1]= G;
        dat[idx+2]= B;
    }
}


void Image::DrawLine(wxImage *img, int x1, int y1, int x2, int y2, unsigned char R, unsigned char G, unsigned char B) {
    unsigned char *dat = img->GetData();
    int img_w = img->GetWidth();

    // Bresenham's line algorithm
    // Implementation found on the web at
    // http://roguebasin.roguelikedevelopment.org/index.php?title=Bresenham%27s_Line_Algorithm
    char ix, iy;
    int dx = (x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1;
    int dy = (y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1;
    if (dx >= dy) {
        int error = dy - (dx >> 1);
        while (x1 != x2) {
            if (error >= 0 && (error || (ix > 0))) {
                y1 += iy;
                error -= dx;
            }
            x1 += ix;
            error += dy;

            dat[XY2IDX(x1,y1,img_w)] = R;
            dat[XY2IDX(x1,y1,img_w)+1] = G;
            dat[XY2IDX(x1,y1,img_w)+2] = B;
        }
    } else {
        int error = dx - (dy >> 1);
        while (y1 != y2) {
            if (error >= 0 && (error || (iy > 0))) {
                x1 += ix;
                error -= dy;
            }
            y1 += iy;
            error += dx;

            dat[XY2IDX(x1,y1,img_w)] = R;
            dat[XY2IDX(x1,y1,img_w)+1] = G;
            dat[XY2IDX(x1,y1,img_w)+2] = B;
        }
    }
    //
    // End of Bresenham's line algorithm
    // ==================================================================
}
