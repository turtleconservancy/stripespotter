/***************************************************************
 * Name:      StripeSpotterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#ifndef STRIPESPOTTERMAIN_H
#define STRIPESPOTTERMAIN_H
#include "StripeSpotterApp.h"
#include "GUIFrame.h"
#include "db-csv.h"
#include <map>
using namespace std;

class StripeSpotterFrame: public GUIFrame {
    public:
        StripeSpotterFrame(wxFrame *frame);
        ~StripeSpotterFrame();
        void RefreshImageList();

    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnAddPictures(wxCommandEvent &event);
        virtual void OnListItemSelected( wxListEvent& event );
        virtual void OnFileSaveCSV(wxCommandEvent& event);

        wxImageList *imgList;
        map<wxString, wxImage*> animalThumbs;
        vector<wxString> animalNameOrder;
};

#endif // STRIPESPOTTERMAIN_H
