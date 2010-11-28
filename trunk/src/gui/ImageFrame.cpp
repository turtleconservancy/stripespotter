/***************************************************************
 * Name:      ImageFrame.cpp
 * Purpose:   Interactive and non-interactive image frames.
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "ImageFrame.h"
#include <string.h>

//
// Constructor and destructor
//
ImageFrame::ImageFrame(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL ) {
    img = NULL;
    ar = 1;
    imgW = imgH = 1;
}

ImageFrame::~ImageFrame() {
    if(img)
        delete img;
}

//
// Loads and displays the specified image
//
bool ImageFrame::loadImage(const wxString &path) {
    // Load the image
    if(!img)
        delete img;
    img = new wxImage(path);
    if(!img->IsOk()) {
        delete img;
        img = NULL;
        return false;
    }
    return loadImage(img);
}

bool ImageFrame::loadImage(wxImage *p_img) {
    if(!p_img)
        return false;

    // Display the goddamn image
    img     = p_img;
    imgW    = img->GetWidth();
    imgH    = img->GetHeight();
    boxX = boxY = boxW = boxH = 0;
    ar      = imgW*1.0 / imgH;
    Refresh(false);

    return true;
}

//
// resets
//
void InteractiveImageFrame::resetView() {
    viewX   = viewY = drawX = drawY = 0;
    viewW   = drawW = imgW;
    viewH   = drawY = imgH;
    draggingMouse = false;
    boxX = boxY = boxW = boxH = 0;
}

//
// handles a GUI redraw event
//
void ImageFrame::OnPaintEvent(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    render(dc);
}
void InteractiveImageFrame::OnPaintEvent(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    render(dc);
}

//
// actually draws the contents of the frame onto a device context
//
void ImageFrame::render(wxDC &dc) {
    wxCoord width, height;
    dc.GetSize(&width, &height);

    // draw border
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetBackgroundMode(wxSOLID);
    dc.SetPen(wxPen(wxColor(0,0,0), 1));
    dc.Clear();
    dc.DrawLine(0, 0, width-1, 0);
    dc.DrawLine(width-1, 0, width-1, height-1);
    dc.DrawLine(width-1, height-1, 0, height-1);
    dc.DrawLine(0, height-1, 0, 0);

    // draw an image if we have the data
    width  -= 2;
    height -= 2;
    if(img) {
        // First determine our drawing size and coordinates
        if(width/ar > height) {
            // scale image so that it's height fits
            drawH = height;
            drawW = height*ar;
            drawY = 0;
            drawX = (width - drawW) / 2;
        } else {
            // scale image so that it's width fits
            drawW = width;
            drawH = width/ar;
            drawX = 0;
            drawY = (height - drawH) / 2;
        }

        // Grab the image viewport, scale it, and shove it into a bitmap
        wxImage *viewport = CloneImage(img);
        viewport->Rescale(drawW, drawH);
        wxBitmap bmp(*viewport);
        dc.DrawBitmap(bmp, 1+drawX, 1+drawY, false);
        delete viewport;
    }

    // draw box markers if we need to
    if(boxW && boxH) {
        // draw full box
        int bx, by, bw, bh;
        img2scr(boxX, boxY, bx, by);
        img2scr(boxX+boxW-1, boxY+boxH-1, bw, bh);
        bw -= bx - 1;
        bh -= by - 1;
        dc.SetPen(wxPen(wxColor(255,0,0), 2));
        dc.DrawLine(bx, by, bx+bw-1, by);
        dc.DrawLine(bx+bw-1, by, bx+bw-1, by+bh-1);
        dc.DrawLine(bx+bw-1, by+bh-1, bx, by+bh-1);
        dc.DrawLine(bx, by+bh-1, bx, by);
    }
}

void ImageFrame::setSelectionBox(const wxRect *rct) {
    boxX = rct->GetX();
    boxY = rct->GetY();
    boxW = rct->GetWidth();
    boxH = rct->GetHeight();

    Refresh(true);
}

//
// Returns the rectangle of the highlight box relative to the current viewport
//
wxRect *ImageFrame::getSelectionBox() {
    if(!(boxW && boxH))
        return NULL;
    wxRect *rect = new wxRect(boxX, boxY, boxW, boxH);
    return rect;
}

wxRect *InteractiveImageFrame::getSelectionBox() {
    if(!(boxW && boxH))
        return NULL;
    wxRect *rect = new wxRect(boxX-viewX, boxY-viewY, boxW, boxH);
    return rect;
}

void ImageFrame::reset() {
}

wxImage *ImageFrame::CloneImage(wxImage *src)   {
    int w = src->GetWidth();
    int h = src->GetHeight();
    unsigned char *data = (unsigned char*) malloc(w*h*3);
    memcpy(data, src->GetData(), w*h*3);
    wxImage *ret = new wxImage(w, h, data);
    return ret;
}


void InteractiveImageFrame::render(wxDC &dc) {
    wxCoord width, height;
    dc.GetSize(&width, &height);

    // draw border
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetBackgroundMode(wxSOLID);
    dc.SetPen(wxPen(wxColor(0,0,0), 1));
    dc.Clear();
    dc.DrawLine(0, 0, width-1, 0);
    dc.DrawLine(width-1, 0, width-1, height-1);
    dc.DrawLine(width-1, height-1, 0, height-1);
    dc.DrawLine(0, height-1, 0, 0);

    // draw an image viewport if we have the data
    width  -= 2;        // remove border area from consideration
    height -= 2;        // remove border area from consideration
    if(img) {
        // First determine our drawing size and coordinates
        double viewportAR = viewW*1.0 / viewH;
        if(width/viewportAR > height) {
            // scale image so that it's height fits
            drawH = height;
            drawW = height*viewportAR;
            drawY = 0;
            drawX = (width - drawW) / 2;
        } else {
            // scale image so that it's width fits
            drawW = width;
            drawH = width/viewportAR;
            drawX = 0;
            drawY = (height - drawH) / 2;
        }

        // Grab the image viewport, scale it, and shove it into a bitmap
        wxImage *viewport = ExtractRectangle(img, viewX, viewY, viewW, viewH);
        viewport->Rescale(drawW, drawH);
        wxBitmap bmp(*viewport);
        dc.DrawBitmap(bmp, 1+drawX, 1+drawY, false);
        delete viewport;
    }

    // draw box markers if we need to
    if(boxW && boxH) {
        // draw full box
        dc.SetPen(wxPen(wxColor(255,0,0), 2));
        int bx, by, bw, bh;
        img2scr(boxX, boxY, bx, by);
        img2scr(boxX+boxW-1, boxY+boxH-1, bw, bh);
        bw -= bx - 1;
        bh -= by - 1;
        dc.DrawLine(bx, by, bx+bw-1, by);
        dc.DrawLine(bx+bw-1, by, bx+bw-1, by+bh-1);
        dc.DrawLine(bx+bw-1, by+bh-1, bx, by+bh-1);
        dc.DrawLine(bx, by+bh-1, bx, by);
    } else
        if(draggingMouse) {
            // draw the initial marker only
            dc.SetBrush(*wxRED_BRUSH);
            dc.SetPen(wxPen(wxColor(255,0,0), 2));
            int bx, by;
            img2scr(boxX, boxY, bx, by);
            dc.DrawCircle(bx, by, 3);
        }
}

//
// Zoom in and out of the selected box
//
void InteractiveImageFrame::zoomIn() {
    if(!(boxW && boxH)) {
        wxMessageBox(_("Please select a box in the top right image by clicking on opposite box corners."), _("Error"));
        return;
    }
    viewX = boxX;
    viewY = boxY;
    viewW = boxW;
    viewH = boxH;

    Refresh(true);
}
void InteractiveImageFrame::zoomOut() {
    resetView();
    Refresh(true);
}

wxImage* ImageFrame::ExtractRectangle(wxImage *srcimg, int x, int y, int w, int h) {
    wxImage *retimg = new wxImage(w, h);
    unsigned char *dstDat = retimg->GetData();
    unsigned char *srcDat = srcimg->GetData();
    int srcW = srcimg->GetWidth();

    for(int r = 0; r < h; r++)
        memcpy(dstDat+(r*w*3), srcDat+3*((r+y)*srcW+x), w*3);

    return retimg;
}

//
// Returns the current viewport as an independent, deep-copied image
//
wxImage *InteractiveImageFrame::extractViewportImage() {
    if(!img)
        return NULL;
    return ExtractRectangle(img, viewX, viewY, viewW, viewH);
}

InteractiveImageFrame::InteractiveImageFrame(wxWindow *parent) : ImageFrame(parent) {
    resetView();
}

//
// Convert between screen and image coordinates
//
void InteractiveImageFrame::scr2img(int x, int y, int& rx, int& ry) {
    rx = viewX + (x-drawX)*((double)viewW/(double)drawW);
    ry = viewY + (y-drawY)*((double)viewH/(double)drawH);
}
void InteractiveImageFrame::img2scr(int x, int y, int& rx, int& ry) {
    rx = drawX + (x-viewX)*((double)drawW/(double)viewW);
    ry = drawY + (y-viewY)*((double)drawH/(double)viewH);
}
void ImageFrame::img2scr(int x, int y, int& rx, int& ry) {
    rx = drawX + x*((double)drawW/(double)imgW);
    ry = drawY + y*((double)drawH/(double)imgH);
}


void InteractiveImageFrame::OnMouseUp(wxMouseEvent &evt) {
    wxPoint pos = evt.GetPosition();

    // if there's no image, or the click is outside the image, ignore it.
    if(!img || pos.x < 1+drawX || pos.x > drawX+drawW || pos.y < 1+drawY || pos.y > drawY+drawH)
        return;

    if(!draggingMouse) {
        // start drawing a box
        draggingMouse = true;
        scr2img(pos.x, pos.y, boxX, boxY);
        boxW = 0;
        boxH = 0;
    } else {
        // stop drawing a box
        draggingMouse = false;
        scr2img(pos.x, pos.y, pos.x, pos.y);
        if(pos.x < boxX) {
            boxW = boxX - pos.x;
            boxX = pos.x;
        } else
            boxW = pos.x - boxX;

        if(pos.y < boxY) {
            boxH = boxY - pos.y;
            boxY = pos.y;
        } else
            boxH = pos.y - boxY;
    }
    Refresh(true);
}




BEGIN_EVENT_TABLE(ImageFrame, wxPanel)
EVT_PAINT(ImageFrame::OnPaintEvent)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(InteractiveImageFrame, wxPanel)
EVT_LEFT_UP(InteractiveImageFrame::OnMouseUp)
EVT_PAINT(InteractiveImageFrame::OnPaintEvent)
/* EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_MOTION(InteractiveImageFrame::OnMouseMove)
 EVT_LEFT_DOWN(InteractiveImageFrame::OnMouseDown)
 EVT_LEAVE_WINDOW(InteractiveImageFrame::OnMouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */
END_EVENT_TABLE()
