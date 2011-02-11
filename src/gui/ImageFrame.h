#ifndef IMAGEFRAME_H_INCLUDED
#define IMAGEFRAME_H_INCLUDED

#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/event.h>

class ImageFrame : public wxPanel  {
    public:
        ImageFrame(wxWindow *parent);
        ~ImageFrame();

        bool loadImage(const wxString &path);
        bool loadImage(wxImage *img);
        void reset();
        void render(wxDC &dc);
        void OnPaintEvent(wxPaintEvent &evt);
        void img2scr(int x, int y, int&, int&);
        wxImage *getImage() { return img; };

        void setSelectionBox(const wxRect *);
        wxRect *getSelectionBox();


        // STATIC HELPER FUNCTIONS
        static wxImage* CloneImage(wxImage *); // deep-copies another wxImage
        static wxImage* ExtractRectangle(wxImage *, int x, int y, int w, int h); // cuts out and deep-copies a rectangle from an image

    DECLARE_EVENT_TABLE()

    protected:
        wxImage *img;           // actual image data
        double ar;              // aspect ratio (width/height)
        int imgW;
        int imgH;
        int boxX, boxY, boxW, boxH;     // selection-box drawing coordinates
        int drawW, drawH, drawX, drawY; // current image drawing coordinates and size on screen
};

class InteractiveImageFrame : public ImageFrame {
    public:
        InteractiveImageFrame(wxWindow *parent);
        ~InteractiveImageFrame() { if(img) delete img; img = NULL; }
        bool loadImage(const wxString &path) { if(ImageFrame::loadImage(path)) {resetView(); return true; } else return false; };
        void resetView();
        void render(wxDC &dc);
        void OnPaintEvent(wxPaintEvent &evt);
        bool loaded() { return img!=NULL; };

        void scr2img(int x, int y, int&, int&);
        void img2scr(int x, int y, int&, int&);

        wxImage *extractViewportImage();
		wxImage *extractSelectionImage();	// cuts out and deep-copies the selection box
        wxRect *getSelectionBox();

        void zoomIn();
        void zoomOut();

        void OnMouseUp(wxMouseEvent &evt);

    DECLARE_EVENT_TABLE()

    protected:
        bool draggingMouse;
        int viewX, viewY, viewW, viewH; // zoom window on the image
};



#endif // IMAGEFRAME_H_INCLUDED
