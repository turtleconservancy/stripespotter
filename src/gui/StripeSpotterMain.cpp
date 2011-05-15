/***************************************************************
 * Name:      StripeSpotterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "db-csv.h"
#include "StripeSpotterMain.h"
#include "AddPicturesDialog.h"
#include "parameters.h"
using namespace std;

// GLOBAL database reference
PhotoDatabase db;

StripeSpotterFrame::StripeSpotterFrame(wxFrame *frame) : GUIFrame(frame) {
	imgList = NULL;
    statusBar->SetStatusText(_("StripeSpotter"), 0);
    statusBar->SetStatusText(_(""), 1);
	wxProgressDialog wxPD(_("Loading"), _("Loading database, please wait..."), 100, this, wxPD_APP_MODAL);
	wxPD.Pulse();

    // Try to see if the current working directory contains a photo database.
    // If one does, then open it.
    // If not, create a new, empty database after warning the user.
    if(!wxSetWorkingDirectory(_("data"))) {
        wxMessageBox(_("This seems to be the first time you have run StripeSpotter. A new image database will now be created."), _("Setup"));
		if(!wxMkdir(_("data"))) {
			wxMessageBox(_("Cannot create a directory for the database. Please make sure you have write permissions on the folder."), _("Error"));
			Destroy();
			return;
		}
        if(!wxSetWorkingDirectory(_("data"))) {
            wxMessageBox(_("There was an error creating the 'data' directory for the image database."), _("Error"));
            Destroy();
            return;
        }
    }
    if(!db.open()) {
		wxMessageBox(_("Unable to open the image database: data\\SightingData.csv."), _("Error"));
        Destroy();
        return;
    }
	wxPD.Pulse();
	if(!wxSetWorkingDirectory(_("images")) && (!wxMkdir(_("images")) || !wxSetWorkingDirectory(_("images"))) ) {
        wxMessageBox(_("There was an error accessing the 'data/images' directory for the image database."), _("Error"));
        Destroy();
        return;
    }

	imgList = new wxImageList(THUMB_W, THUMB_H, false, db.getNumAnimals());
	wxPD.Pulse();
	RefreshImageList();
	wxPD.Pulse();
}

//
// User clicked a zebra in the main window
//
void StripeSpotterFrame::OnListItemSelected( wxListEvent& event ) {
/*    int idx = event.GetImage();
    wxString animalName = animalNameOrder[idx];
    statusBar->SetStatusText(db.getPhotoCountString(animalName.ToAscii()), 1);
    statusBar->SetStatusText(animalName, 0); */
}

StripeSpotterFrame::~StripeSpotterFrame() {
	delete imgList;
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
    if(db.dumpDatabase())
        wxMessageBox(_("Database information saved to 'SightingData.csv'\nYou can use Microsoft Excel to open the file."), _("Sighting data saved"));
    else
        wxMessageBox(_("Unable to save database information."), _("Error"));
    wxSetWorkingDirectory(_("images"));
}

//
// Refreshes the image list (duh)
//
void StripeSpotterFrame::RefreshImageList() {
    // Set status infomration
    wxString status;
    status.Printf(_("%d animals, %d photographs."), db.getNumAnimals(), db.getNumPhotos());
    txtAnimalInfo->SetLabel(status);

	// Clear out old list
	lctrlMainDisplay->ClearAll();
	imgList->RemoveAll();

    // Create an image list of animal thumbnails.
	vector<wxString> animal_name_order;
	for(map<string,vector<PhotoInfo*> >::iterator it=db.animal_to_photos.begin();it!=db.animal_to_photos.end();it++) {
		unsigned best = 0;
		vector<PhotoInfo*> &photos = it->second;
		for(unsigned i=1; i < photos.size(); i++)
			if(photos[i]->quality >= photos[best]->quality)
				best = i;
		wxString fname;
		fname.Printf(_("thumb-%07d.jpg"), photos[best]->photo_id);
		wxImage *img = new wxImage(fname);
        if(img->IsOk()) {
            img->Rescale(THUMB_W, THUMB_H);
            wxBitmap bmp(*img);
            imgList->Add(bmp);
			animal_name_order.push_back(wxString::FromAscii(photos[best]->animal_name.c_str()));
        }
		delete img;
	}

	// Populate the list box
    lctrlMainDisplay->SetImageList(imgList, wxIMAGE_LIST_SMALL);
    lctrlMainDisplay->InsertColumn(0, _("Best Image"));
    lctrlMainDisplay->InsertColumn(1, _("Animal ID"));
    int next_item_index=0;
    for(int x = 0; x < imgList->GetImageCount(); x++) {
        wxListItem li;
        li.SetId(next_item_index++);
        li.SetColumn(0);
        li.SetImage(x);
        li.m_mask = wxLIST_MASK_IMAGE;
        lctrlMainDisplay->InsertItem(li);
        li.SetColumn(1);
        li.SetText(animal_name_order[x]);
        li.m_mask = wxLIST_MASK_TEXT;
        lctrlMainDisplay->SetItem(li);
    }
    lctrlMainDisplay->SetColumnWidth(0, THUMB_W+20);
    lctrlMainDisplay->SetColumnWidth(1, 100);
}

void StripeSpotterFrame::OnAddPictures(wxCommandEvent &event) {
    DLGAddPictures *dlg = new DLGAddPictures(this);
    dlg->ShowModal();
	RefreshImageList();
}

void StripeSpotterFrame::OnQuit(wxCommandEvent &event) {
    Destroy();
}

void StripeSpotterFrame::OnAbout(wxCommandEvent &event) {
    char statusMsg[1024];
	sprintf(statusMsg, "StripeSpotter Copyright (c) 2010 Mayank Lahiri\nmlahiri@gmail.com\nReleased under the terms of the GNU General Public License (GPL).\n\nDeveloped at the Laboratory for Computational Population Biology\nDepartment of Computer Science\nUniversity of Illinois at Chicago\n\nThis program is free software! If you paid for this program, ask for your money back!\n\nThis version was built on %s.", __DATE__);
    wxMessageBox(wxString::FromAscii(statusMsg), _("StripeSpotter"));
}
