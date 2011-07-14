/***************************************************************
 * Name:      AddPicturesDialog.cpp
 * Purpose:   Add new images to the database
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "wx/msgdlg.h"
#include <wx/progdlg.h>
#include "parameters.h"
#include "StripeCode.h"
#include "db-csv.h"
#include <algorithm>
#include <exif.h>
#include "AddPicturesDialog.h"
#include "SavePictureDialog.h"
extern PhotoDatabase db;
wxString selectedResult;

DLGAddPictures::DLGAddPictures(wxWindow* parent) : AddPicturesDialog(parent) {
	imgList = new wxImageList(THUMB_W, THUMB_H, false);
	lctrlIDResults->SetImageList(imgList, wxIMAGE_LIST_NORMAL);
	btnSaveAsOldAnimal->Disable();
	searchResultPhoto = -1;
}

DLGAddPictures::~DLGAddPictures() {
    delete imgList;
}

//
// User selected a directory
//
void DLGAddPictures::OnDirChanged( wxFileDirPickerEvent& event ) {
    wxCommandEvent ev;
    OnScan(ev);
}

//
// User clicked a filename in the file list
//
void DLGAddPictures::OnListItemSelected(wxListEvent& event) {
    // Get the freshly selected file
    wxListItem item;
	item.SetMask(wxLIST_MASK_TEXT);
    item.SetId(event.GetIndex());
    item.SetColumn(2);
    if(!lctrlFileList->GetItem(item)) {
        wxMessageBox(_("Cannot get information about that item."), _("Internal error."));
        return;
    }
    wxString filePath = item.GetText();
    
    
    
    // See if the thing has a damn timestamp
    item.SetColumn(1);
    if(!lctrlFileList->GetItem(item)) {
        wxMessageBox(_("Cannot get information about that item."), _("Internal error."));
        return;
    }
    if(item.GetText() == wxEmptyString) {
        // Grab EXIF date+time from file
        wxFile file((const wxChar*)filePath, wxFile::read);
        wxString timestamp = wxEmptyString;
        if(file.IsOpened()) {
            wxFileOffset fsize = file.SeekEnd();
            file.Seek(0);
            unsigned char *buf = new unsigned char[fsize];
            if(buf) {
                file.Read(buf, fsize);
                EXIFInfo exif;
                ParseEXIF(buf, fsize, exif);
                if(exif.breakDownTimestamp())
                    timestamp.Printf(_("%02d:%02d"), exif.hour, exif.min);
            }
            delete[] buf;
            file.Close();
        }

        // Set the timestamp for the image
        item.SetColumn(1);
        item.SetText(timestamp);
		item.SetMask(wxLIST_MASK_TEXT);
        lctrlFileList->SetItem(item);
    }

    selectedImageFilePath = filePath;
    iifImageSlicer->reset();
    iifImageSlicer->loadImage(filePath);
	ifMatchDisplay->reset();
	txtMatchInfo->SetLabel(_(" "));
	lctrlIDResults->ClearAll();
	imgList->RemoveAll();
	btnSaveAsOldAnimal->Disable();
	event.Skip();
}



//
// The main animal ID routine
//
enum ALGORITHM { STRIPECODE, CO1 };
void DLGAddPictures::OnIdentifyAnimal( wxCommandEvent& event ) {
	// disable the button
	btnIdentifyAnimal->Disable();

	// which algorithm does the user want?
	ALGORITHM alg = STRIPECODE;
	if(rbAlgCO1->GetValue())
		alg = CO1;

    //
	// extract Stripecode from ROI
	//
	wxImage *img = iifImageSlicer->extractSelectionImage();
	if(!img) {
        wxMessageBox(_("Please select the animal's body first."), _("Error"));
		btnIdentifyAnimal->Enable();
        return;
    }
	wxProgressDialog wxPD(_("Searching..."), _("Analyzing stripes, please wait..."), 100, this, wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_SMOOTH|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME|wxPD_AUTO_HIDE );
	ImageFeatures *imgFeat;
	if(alg == STRIPECODE)
		imgFeat = new StripeCode;
	else
		imgFeat = new MultiScaleHistogram;
	imgFeat->read(img);
	delete img;
	wxPD.Update(0, _("Looking for a match."));

	//
	// Search the database
	//
	map<string,pair<int,double> > costs;
	int photos_scanned = 0; int total_photos = db.getNumPhotos();
	for(map<int,PhotoInfo*>::iterator it=db.photo_to_info.begin();it!=db.photo_to_info.end();it++) {
		double cost;
		if(alg == STRIPECODE)
			cost = db.stripeCodes[it->first].compare(imgFeat, NULL);
		else
			cost = db.MRHistograms[it->first].compare(imgFeat, NULL);

		string &aname = it->second->animal_name;
		assert(it->first >= 1);
		if(costs.find(aname)==costs.end() || costs[aname].second > cost)
			costs[aname] = make_pair<int,double>(it->first,cost);
		if(!wxPD.Update(++photos_scanned*100/total_photos))
			break;
	}
	wxPD.Update(100);

	//
	// Sort results by cost
	//
	multimap<double,int> results;
	for(map<string,pair<int,double> >::iterator it=costs.begin();it!=costs.end(); it++)
		results.insert(make_pair<double,int>(it->second.second,it->second.first));
	searchResults.clear();
	int NR = 0;
	for(multimap<double,int>::iterator it=results.begin(); it!=results.end() && NR < 100; it++, NR++)
		searchResults.push_back(make_pair<double,int>(it->first,it->second));

	//
	// Clear out old results
	//
	lctrlIDResults->ClearAll();
	imgList->RemoveAll();

	//
	// Show new results
	//
	lctrlIDResults->InsertColumn(0, _("Match"));

	for(vector<pair<double,int> >::iterator it=searchResults.begin();it!=searchResults.end();it++) {
		wxString fname;
		fname.Printf(_("thumb-%07d.jpg"), it->second);

		wxImage *img = new wxImage(fname);
		if(!img || !img->IsOk())
			wxMessageBox(fname, _("Cannot load result image."));
		else {
	        img->Rescale(THUMB_W, THUMB_H);
			wxBitmap bmp(*img);
			imgList->Add(bmp);
		}
		if(img)
			delete img;
        
	}

	lctrlIDResults->SetImageList(imgList, wxIMAGE_LIST_SMALL);
	NR=0;
	for(vector<pair<double,int> >::iterator it=searchResults.begin();it!=searchResults.end();it++,NR++)
		lctrlIDResults->InsertItem(NR, NR);

	lctrlIDResults->SetColumnWidth(0, THUMB_W);
	btnIdentifyAnimal->Enable();
	delete imgFeat;
}

void DLGAddPictures::OnSearchResultSelected( wxListEvent &event ) {
	int idx = event.GetIndex();
	pair<double,int> &result = searchResults[idx];
	searchResultPhoto = idx;

	assert(db.photo_to_info.find(result.second)!=db.photo_to_info.end());
    PhotoInfo *ptmp = db.photo_to_info[result.second];
    assert(ptmp && ptmp->photo_id >= 1);

    // Tell the user what's going on
    char status[1024];
    sprintf(status, "Rank %d: '%s' photo %d cost %f", idx+1, ptmp->animal_name.c_str(), result.second, result.first);
	txtMatchInfo->SetLabel(wxString::FromAscii(status));
    selectedResult = wxString::FromAscii(ptmp->original_filename.c_str());
    // Load the thumbnail
    wxString fname;
	fname.Printf(_("img-%07d.jpg"), result.second);
    
	ifMatchDisplay->reset();
	ifMatchDisplay->loadImage(fname);
	btnSaveAsOldAnimal->Enable();
	event.Skip();
}
wxString DLGAddPictures::TrimFileName(wxString& path){
          wxString fileNameUnix = path.AfterLast((wxChar)'/');
          wxString fileNameWin = path.AfterLast((wxChar)'\\');
#ifdef WIN32
            return fileNameWin;
#else
			return fileNameUnix;
#endif
}
wxString DLGAddPictures::TrimFileNameW(wxString& path){
          wxString fileNameUnix = path.AfterLast((wxChar)'/');
          wxString fileNameWin = path.AfterLast((wxChar)'\\');
          return fileNameWin;
}
void DLGAddPictures::onEditDistanceCostVisualizer(wxCommandEvent& event) {
    wxMessageDialog *dialogs = new wxMessageDialog( NULL, selectedResult,selectedResult, wxOK| wxICON_ERROR); 
    dialogs->ShowModal();
    selectedResult = TrimFileName(selectedResult);
    selectedResult = TrimFileNameW(selectedResult);
	for(map<int,PhotoInfo*>::iterator it=db.photo_to_info.begin();it!=db.photo_to_info.end();it++) {
         wxString path = wxString::FromAscii(it->second->original_filename.c_str());
         path = TrimFileNameW(path);
         path = TrimFileName(path);
         //wxMessageDialog *dialogs = new wxMessageDialog( NULL, path,path, wxOK| wxICON_ERROR); 
         //dialogs->ShowModal();
         if(path.Cmp(selectedResult)==0){
           wxMessageDialog *dialog2s = new wxMessageDialog( NULL, path,path, wxOK| wxICON_ERROR); 
           dialog2s->ShowModal();
           wxMessageDialog *dialog3s = new wxMessageDialog( NULL, selectedResult,selectedResult, wxOK| wxICON_ERROR); 
           dialog3s->ShowModal();
            int x,y,w,h;
            x=y=w=h=0;
            wxString roi = wxString::FromAscii(it->second->roi.c_str());
            wxMessageDialog *dialogs = new wxMessageDialog( NULL, roi,roi, wxOK| wxICON_ERROR); 
            dialogs->ShowModal();
            ifMatchDisplay->setSelectionBox(new wxRect(0,0,100,1000)); 
         }
    }
}


void DLGAddPictures::OnZoomIn(wxCommandEvent& event) {
    iifImageSlicer->zoomIn();
}
void DLGAddPictures::OnZoomOut(wxCommandEvent& event) {
    iifImageSlicer->zoomOut();
}

//
// User pressed the "Save image to database" button
//
void DLGAddPictures::OnSaveImage(wxCommandEvent &event) {
    if(!iifImageSlicer->loaded()) {
        wxMessageBox(_("Please select an image first."), _("Error"));
        return;
    }

	// get the ROI image first
	wxImage *roi = iifImageSlicer->extractSelectionImage();
	if(!roi) {
        wxMessageBox(_("Please select the animal's body first."), _("Error"));
        return;
    }

    wxImage *viewport = iifImageSlicer->extractViewportImage(); // viewport image will be owned by the dialog, destroyed after
    wxRect *selection = iifImageSlicer->getSelectionBox();
    if(!viewport) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
        return;
    }

	// Extract StripeCode and MRHistogram from the ROI
	wxProgressDialog wxPD(_("Preparing image"), _("Analyzing stripes, please wait..."), 100, this, wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_SMOOTH|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME|wxPD_AUTO_HIDE );
	StripeCode sc;
	if(!sc.read(roi)) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
		delete roi;
        return;
    }
	wxPD.Update(50);
	MultiScaleHistogram mrh;
	if(!mrh.read(roi)) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
		delete roi;
        return;
	}
	delete roi;
	wxPD.Update(100);

    DLGSavePicture *dlg = new DLGSavePicture(this, viewport, selection, selectedImageFilePath, sc, mrh);
	// viewport image will be destroyed by dialog box
    dlg->ShowModal();
    if(selection)
        delete selection;

    // Focus the file picker for speed
    lctrlFileList->SetFocus();
}

//
// Sort elements in a list by their filename
//
int wxCALLBACK DLGAddPictures::fileSorter(long i1, long i2, long sortData) {
    DLGAddPictures &obj = *((DLGAddPictures*) sortData);

	return obj.sortKeys[i1].compare(obj.sortKeys[i2]);
}

//
// User pressed the "Scan directory" button
//
void DLGAddPictures::OnScan(wxCommandEvent &event) {
    wxString selectedPath = dirctrlImageDirectory->GetPath();
    if(!wxDir::Exists(selectedPath)) {
        wxMessageBox(_("Invalid directory, please try choosing another one."), _("Error"));
        return;
    }

    // Clear the file list and re-populate column headers
    lctrlFileList->ClearAll();
    wxListItem col0;
    col0.SetId(0);
    col0.SetText(_("File"));
    col0.SetWidth(125);
    col0.SetMask(wxLIST_MASK_TEXT|wxLIST_MASK_WIDTH);
    lctrlFileList->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetId(1);
    col1.SetText(_("Time"));
    col1.SetWidth(100);
    col1.SetMask(wxLIST_MASK_TEXT|wxLIST_MASK_WIDTH);
    lctrlFileList->InsertColumn(1, col1);
    wxListItem col2;
    col2.SetId(2);
    col2.SetText(_("Full path"));
    col2.SetWidth(1);
    col2.SetMask(wxLIST_MASK_TEXT|wxLIST_MASK_WIDTH);
    lctrlFileList->InsertColumn(2, col2);

    // Walk the directory
    wxArrayString allFiles;
    int filesFound = wxDir::GetAllFiles(selectedPath, &allFiles, wxEmptyString, wxDIR_FILES);
    int imagesFound = 0;
    long itemIndex = -1;
    for(int i = 0; i < filesFound; i++) {
        wxString &filePath = allFiles[i];
        if(filePath.EndsWith((const wxChar*)wxT("jpg")) || filePath.EndsWith((const wxChar*)wxT("JPG"))) {
            // Insert filename into list
            imagesFound++;
            wxString fileNameUnix = filePath.AfterLast((wxChar)'/');
            wxString fileNameWin = filePath.AfterLast((wxChar)'\\');
#ifdef WIN32
            wxString fileNameFinal = fileNameWin;
#else
			wxString fileNameFinal = fileNameUnix;
#endif
            lctrlFileList->InsertItem(++itemIndex, fileNameFinal);
            lctrlFileList->SetItem(itemIndex, 1, wxEmptyString);
            lctrlFileList->SetItem(itemIndex, 2, filePath);
			lctrlFileList->SetItemData(itemIndex, (long) sortKeys.size());
			sortKeys.push_back(fileNameFinal);
        }
    }

    // Sort the list by filename
    lctrlFileList->SortItems(fileSorter, (long)this);

	// Clear out all the other controls
	lctrlIDResults->ClearAll();
	imgList->RemoveAll();
	iifImageSlicer->reset();
	ifMatchDisplay->reset();
	txtMatchInfo->SetLabel(wxEmptyString);
    selectedImageFilePath = wxEmptyString;
	searchResultPhoto = -1;
	btnSaveAsOldAnimal->Disable();
}


void DLGAddPictures::OnAcceptMatch(wxCommandEvent &event) {
    if(!iifImageSlicer->loaded()) {
        wxMessageBox(_("Please select an image first."), _("Error"));
        return;
    }
	if(searchResultPhoto < 0 || searchResultPhoto >= (int)searchResults.size()) {
        wxMessageBox(_("Please select a search result first."), _("Error"));
        return;
	}

	// get the ROI image first
	wxImage *roi = iifImageSlicer->extractSelectionImage();
	if(!roi) {
        wxMessageBox(_("Please select the animal's body first."), _("Error"));
        return;
    }

    wxImage *viewport = iifImageSlicer->extractViewportImage(); // viewport image will be owned by the dialog, destroyed after
    wxRect *selection = iifImageSlicer->getSelectionBox();
    if(!viewport) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
        return;
    }

	// Extract StripeCode and MRHistogram from the ROI
	wxProgressDialog wxPD(_("Preparing image"), _("Analyzing stripes, please wait..."), 100, this, wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_SMOOTH|wxPD_ESTIMATED_TIME|wxPD_REMAINING_TIME|wxPD_AUTO_HIDE );
	StripeCode sc;
	if(!sc.read(roi)) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
		delete roi;
        return;
    }
	wxPD.Update(50);
	MultiScaleHistogram mrh;
	if(!mrh.read(roi)) {
        char error[1024];
        sprintf(error, "Unable to save the image.\nLocation:\n%s\n%d", __FILE__, __LINE__);
        wxMessageBox(wxString::FromAscii(error), _("Internal Error"));
		delete roi;
        return;
	}
	delete roi;
	wxPD.Update(100);

	// get matched animal info
	pair<double,int> &result = searchResults[searchResultPhoto];
	PhotoInfo &photo = *(db.photo_to_info[result.second]);
	DLGSavePicture::lastAnimalID = wxString::FromAscii(photo.animal_name.c_str());
	DLGSavePicture::lastAge = wxString::FromAscii(photo.age.c_str());
	DLGSavePicture::lastNotes = wxString::FromAscii(photo.notes.c_str());
	DLGSavePicture::lastSex = photo.sex;
	DLGSavePicture::lastReproductive = wxString::FromAscii(photo.reproductive.c_str());


    DLGSavePicture *dlg = new DLGSavePicture(this, viewport, selection, selectedImageFilePath, sc, mrh, false);
	// viewport image will be destroyed by dialog box
    dlg->ShowModal();
    if(selection)
        delete selection;

    // Focus the file picker for speed
    lctrlFileList->SetFocus();
}
