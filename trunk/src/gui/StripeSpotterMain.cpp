/***************************************************************
 * Name:      StripeSpotterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "StripeSpotterMain.h"
#include "AddPicturesDialog.h"
#include "parameters.h"
using namespace std;

// GLOBAL database reference
PhotoDatabase *globalDB;

StripeSpotterFrame::StripeSpotterFrame(wxFrame *frame) : GUIFrame(frame) {
    globalDB = &db;
    statusBar->SetStatusText(_("StripeSpotter"), 0);
    statusBar->SetStatusText(_(""), 1);

    // Try to see if the current working directory contains a photo database.
    // If one does, then open it.
    // If not, create a new, empty database after warning the user.
    wxDir pdbDir(_("data"));
    if(!pdbDir.IsOpened()) {
        wxMessageBox(_("This is the first time you have run StripeSpotter. A new image database will now be created."), _("Setup"));
        wxMkdir(_("data"));
        if(!pdbDir.Open(_("data"))) {
            wxMessageBox(_("There was an error creating the 'data' directory for the image database."), _("Error"));
            Destroy();
            return;
        }
    }
    if(!wxSetWorkingDirectory(_("data")) || !db.open()) {
        wxMessageBox(_("Unable to open the image database."), _("Error"));
        Destroy();
        return;
    }
    if(!pdbDir.Open(_("images")) && !wxMkdir(_("images"))) {
        wxMessageBox(_("There was an error accessing the 'data/images' directory for the image database."), _("Error"));
        Destroy();
        return;
    }
    if(!wxSetWorkingDirectory(_("images"))) {
        wxMessageBox(_("Unable to open the image database."), _("Error"));
        Destroy();
        return;
    }

    // Set status infomration
    wxString status;
    status.Printf(_("%d animals, %d photographs."), db.getNumAnimals(), db.getNumPhotos());
    txtAnimalInfo->SetLabel(status);

    // Populate the list with thumbnails of the animals
    imgList = new wxImageList(THUMB_W, THUMB_H, false);
    vector<pair<wxString,wxString> > bestImgInfo = db.getBestPhotoList();
    for(unsigned i = 0; i < bestImgInfo.size(); i++) {
        pair<wxString,wxString> &animal = bestImgInfo[i];
        wxImage *img = new wxImage(animal.second);
        if(img->IsOk()) {
            animalThumbs[animal.first] = img;
            wxBitmap bmp(*img);
            imgList->Add(bmp);
        }
        animalNameOrder.push_back(animal.first);
    }
    lctrlMainDisplay->SetImageList(imgList, wxIMAGE_LIST_SMALL);
    lctrlMainDisplay->InsertColumn(0, _("Best Image"));
    lctrlMainDisplay->InsertColumn(1, _("Animal ID"));
    for(int x = 0; x < imgList->GetImageCount(); x++) {
        wxListItem li;
        li.SetColumn(0);
        li.SetImage(x);
        li.m_mask = wxLIST_MASK_IMAGE;
        lctrlMainDisplay->InsertItem(li);
        li.SetColumn(1);
        li.SetText(bestImgInfo[x].first);
        li.m_mask = wxLIST_MASK_TEXT;
        lctrlMainDisplay->SetItem(li);
    }
    lctrlMainDisplay->SetColumnWidth(0, THUMB_W+20);
    lctrlMainDisplay->SetColumnWidth(1, wxLIST_AUTOSIZE);
}

//
// User clicked a zebra in the main window
//
void StripeSpotterFrame::OnListItemSelected( wxListEvent& event ) {
    int idx = event.GetImage();
    wxString animalName = animalNameOrder[idx];
    statusBar->SetStatusText(db.getPhotoCountString(animalName.ToAscii()), 1);
    statusBar->SetStatusText(animalName, 0);
}

StripeSpotterFrame::~StripeSpotterFrame() {
    if(imgList)
        delete imgList;
    for(map<wxString,wxImage*>::iterator it = animalThumbs.begin(); it != animalThumbs.end(); it++)
        if(it->second)
            delete it->second;
}

void StripeSpotterFrame::OnClose(wxCloseEvent &event) {
    Destroy();
}

//
// Save contents of the database as a CSV file.
//
void StripeSpotterFrame::OnFileSaveCSV(wxCommandEvent& event) {
    if(!wxSetWorkingDirectory(_("..")))
        return;
    if(db.dumpDatabase(_("SightingData.csv")))
        wxMessageBox(_("Database information saved to 'SightingData.csv'\nYou can use Microsoft Excel to open the file."), _("Sighting data saved"));
    else
        wxMessageBox(_("Unable to save database information."), _("Error"));
    wxSetWorkingDirectory(_("images"));
}

//
// Refreshes the image list (duh)
//
void StripeSpotterFrame::RefreshImageList() {

}

void StripeSpotterFrame::OnAddPictures(wxCommandEvent &event) {
    DLGAddPictures *dlg = new DLGAddPictures(this);
    dlg->Show();
}

void StripeSpotterFrame::OnQuit(wxCommandEvent &event) {
    Destroy();
}

void StripeSpotterFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox(_("StripeSpotter Copyright (c) Mayank Lahiri, 2010"), _("StripeSpotter"));
}
