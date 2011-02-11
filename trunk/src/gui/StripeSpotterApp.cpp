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

IMPLEMENT_APP(StripeSpotterApp);

bool StripeSpotterApp::OnInit() {
    wxInitAllImageHandlers();
    StripeSpotterFrame* frame = new StripeSpotterFrame(0L);
    frame->Show();
    return true;
}
