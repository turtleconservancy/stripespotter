/***************************************************************
 * Name:      StripeSpotterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "StripeSpotterApp.h"
#include "StripeSpotterMain.h"
#include "EditDistanceVisualizer.h"
#include <wx/wx.h>
#include <wx/sizer.h>
// Uncomment the following line to run the app from Terminal
#define __MAC_TERMINAL_APP__

#if defined(__WXMAC__) && defined(__MAC_TERMINAL_APP__)
#include <ApplicationServices/ApplicationServices.h>
#endif

IMPLEMENT_APP(StripeSpotterApp);

bool StripeSpotterApp::OnInit() {

#if defined(__WXMAC__) && defined(__MAC_TERMINAL_APP__)
    ProcessSerialNumber PSN;
    GetCurrentProcess(&PSN);
    TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif

    wxInitAllImageHandlers();
    StripeSpotterFrame* frame = new StripeSpotterFrame(0L);
    frame->Show();
/*    wxInitAllImageHandlers();
    wxFrame *frame2;
    wxImagePanel * drawPane;
        
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame2 = new wxFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,600));
            
    // then simply create like this
    drawPane = new wxImagePanel( frame, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
    sizer->Add(drawPane, 1, wxEXPAND);
            
    frame2->SetSizer(sizer);
            
    frame2->Show();*/
}
