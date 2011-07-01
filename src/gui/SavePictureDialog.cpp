/***************************************************************
 * Name:      SavePictureDialog.cpp
 * Purpose:   After the animal ID, save a picture to the database
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "StripeCode.h"
#include "SavePictureDialog.h"
#include "db-csv.h"
#include "parameters.h"

// GLOBAL database reference
extern PhotoDatabase db;
wxString DLGSavePicture::lastAnimalID	= wxEmptyString;
wxString DLGSavePicture::lastSighting	= wxEmptyString;
wxString DLGSavePicture::lastNotes		= wxEmptyString;
wxString DLGSavePicture::lastGPSLat		= wxEmptyString;
wxString DLGSavePicture::lastGPSLon		= wxEmptyString;
int DLGSavePicture::lastGroupSize	= 0;
wxString DLGSavePicture::lastSightingLocation = wxEmptyString;
FLANK DLGSavePicture::lastFlank = FL_LEFT;
QUALITY DLGSavePicture::lastPhotoQuality = PQ_OK;
SEX DLGSavePicture::lastSex = UNKNOWN;
wxString DLGSavePicture::lastReproductive = wxEmptyString;
wxString DLGSavePicture::lastAge = wxEmptyString;

DLGSavePicture::DLGSavePicture(wxWindow* parent, wxImage *img, const wxRect *select, const wxString &filePath, StripeCode &sc, MultiScaleHistogram &mrh, bool newAnimal) : SaveImageDialog(parent) {
	// NOTE: the image frame owns the image, and will destroy it when done
    ifFinalPreview->reset();
	ifFinalPreview->loadImage(img);

    originalPath = filePath;
	stripeCode = sc;
	MRHisto = mrh;
    if (select) {
        ifFinalPreview->setSelectionBox(select);
        sprintf(roi, "%d %d %d %d", select->GetLeft(), select->GetTop(), select->GetWidth(), select->GetHeight());
    } else
        roi[0] = 0;
    txtAnimalID->SetFocus();
	if(!newAnimal) {
		txtAnimalID->SetValue(DLGSavePicture::lastAnimalID);
		txtAge->SetValue(DLGSavePicture::lastAge);
		txtReproductive->SetValue(DLGSavePicture::lastReproductive);
		switch(DLGSavePicture::lastSex) {
			case MALE:
				rbSexMale->SetValue(true);
				break;
			case FEMALE:
				rbSexFemale->SetValue(true);
				break;
			default:
				rbSexUnknown->SetValue(true);
				break;
		}
	}
    txtSightingID->SetValue(DLGSavePicture::lastSighting);
	txtSightingLocation->SetValue(DLGSavePicture::lastSightingLocation);
	txtGPSLat->SetValue(DLGSavePicture::lastGPSLat);
	txtGPSLon->SetValue(DLGSavePicture::lastGPSLon);
	wxString tmp;
	tmp.Printf(_("%d"), DLGSavePicture::lastGroupSize);
	txtGroupSize->SetValue(tmp);
	txtSightingNotes->SetValue(DLGSavePicture::lastNotes);
    switch (lastFlank) {
    case 0:
        rbFlankLeft->SetValue(true);
        break;
    case 1:
        rbFlankRight->SetValue(true);
        break;
    case 2:
        rbFlankFront->SetValue(true);
        break;
    case 3:
        rbFlankRear->SetValue(true);
        break;
    }
    switch (lastPhotoQuality) {
    case 0:
        rbPicBad->SetValue(true);
        break;
    case 1:
        rbPicOK->SetValue(true);
        break;
    case 2:
        rbPicGood->SetValue(true);
        break;
    case 3:
        rbPicBest->SetValue(true);
        break;
    }

    // Get EXIF information from the file
    wxFile file((const wxChar*)filePath, wxFile::read);
    if (file.IsOpened()) {
        wxFileOffset fsize = file.SeekEnd();
        file.Seek(0);
        unsigned char *buf = new unsigned char[fsize];
        if (buf) {
            file.Read(buf, fsize);
            ParseEXIF(buf, fsize, exif);
            exif.breakDownTimestamp();
        }
        delete[] buf;
        file.Close();
    }

    // Set the date/time information for this image
    if (exif.year) {
		wxDateTime tstamp = wxDateTime::Now();
        switch (exif.month) {
        case 1:
            tstamp.SetMonth(wxDateTime::Jan);
            break;
        case 2:
            tstamp.SetMonth(wxDateTime::Feb);
            break;
        case 3:
            tstamp.SetMonth(wxDateTime::Mar);
            break;
        case 4:
            tstamp.SetMonth(wxDateTime::Apr);
            break;
        case 5:
            tstamp.SetMonth(wxDateTime::May);
            break;
        case 6:
            tstamp.SetMonth(wxDateTime::Jun);
            break;
        case 7:
            tstamp.SetMonth(wxDateTime::Jul);
            break;
        case 8:
            tstamp.SetMonth(wxDateTime::Aug);
            break;
        case 9:
            tstamp.SetMonth(wxDateTime::Sep);
            break;
        case 10:
            tstamp.SetMonth(wxDateTime::Oct);
            break;
        case 11:
            tstamp.SetMonth(wxDateTime::Nov);
            break;
        case 12:
            tstamp.SetMonth(wxDateTime::Dec);
            break;
        }
        tstamp.SetDay(exif.day);
        tstamp.SetYear(exif.year);
        calSightDate->SetDate(tstamp);

        // set the time
        spinHour->SetValue(exif.hour);
        spinMin->SetValue(exif.min);

        // save values
        sprintf(sightdate, "%4d-%02d-%02d", exif.year, exif.month, exif.day);
        sprintf(sighttime, "%02d:%02d", exif.hour, exif.min);
        exposure[0] = aperture[0] = focallen[0] = makemodel[0] = 0;
        if(exif.exposureTime)
            sprintf(exposure, "1/%d", (int) (1.0/exif.exposureTime));
        if(exif.FStop)
            sprintf(aperture, "f/%g", exif.FStop);
        if(exif.focalLength)
            sprintf(focallen, "%d", exif.focalLength);
        sprintf(makemodel, "%s %s", exif.cameraMake, exif.cameraModel);
    } else {
        sightdate[0] = sighttime[0] = focallen[0] = aperture[0] = exposure[0] = makemodel[0] = 0;
    }
}

//
// Notified when the animal ID box is focused
//
void DLGSavePicture::OnAnimalIDFocused(wxFocusEvent& event) {
    txtAnimalID->SetSelection(0, txtAnimalID->GetValue().Length()-1);
    event.Skip();
}

//
// Fickle user doesn't want to save the image after all
//
void DLGSavePicture::OnBtnCancel(wxCommandEvent& event) {
    Close();
}

//
// Notified when the animal ID has been edited
//
void DLGSavePicture::OnAnimalIDChanged( wxCommandEvent& event ) {
    wxString query = txtAnimalID->GetValue();
    wxString label;
    if (db.queryAnimalName(query.ToAscii())) {
        label = query + _(": FOUND IN DATABASE!");
        txtAnimalSearchResult->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    } else {
        label = query + _(": New animal.");
        txtAnimalSearchResult->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    }
    txtAnimalSearchResult->SetLabel(label);
}

//
//
//
void DLGSavePicture::OnSightingIDChanged( wxCommandEvent& event ) {
	string query = (const char*) (txtSightingID->GetValue().mb_str(wxConvUTF8));
	if(db.sighting_to_photo.find(query)!=db.sighting_to_photo.end()) {
		PhotoInfo *pi = db.sighting_to_photo[query];
		lastGPSLat = wxString::FromAscii(pi->gps_lat.c_str());
		lastGPSLon = wxString::FromAscii(pi->gps_lon.c_str());
		lastSightingLocation = wxString::FromAscii(pi->sighting_location.c_str());
		lastGroupSize = pi->group_size;
	} else {
		lastGPSLat = wxEmptyString;
		lastGPSLon = wxEmptyString;
		lastSightingLocation = wxEmptyString;
		lastGroupSize = 0;
	}
	txtGPSLat->SetValue(lastGPSLat);
	txtGPSLon->SetValue(lastGPSLon);
	wxString tmp;
	tmp.Printf(_("%d"), lastGroupSize);
	txtGroupSize->SetValue(tmp);
	txtSightingLocation->SetValue(lastSightingLocation);
}

//
// Saves an image to the database
// i18n NOTE: UTF-8 is used, but only ascii characters are pulled.
//
void DLGSavePicture::OnBtnSave(wxCommandEvent& event) {
    // Grab information from dialog box
    lastAnimalID = txtAnimalID->GetValue();
	lastSighting = txtSightingID->GetValue();
	lastPhotoQuality = PQ_BAD;
    if (rbPicOK->GetValue())
        lastPhotoQuality = PQ_OK;
    if (rbPicGood->GetValue())
        lastPhotoQuality  = PQ_GOOD;
    if (rbPicBest->GetValue())
		lastPhotoQuality = PQ_BEST;
    lastFlank = FL_LEFT;
    if (rbFlankRight->GetValue())
		lastFlank = FL_RIGHT;
    if (rbFlankFront->GetValue())
        lastFlank = FL_FRONT;
    if (rbFlankRear->GetValue())
        lastFlank = FL_REAR;
    lastNotes = txtSightingNotes->GetValue();
	lastSightingLocation = txtSightingLocation->GetValue();
	lastGPSLat = txtGPSLat->GetValue();
	lastGPSLon = txtGPSLon->GetValue();
	lastGroupSize = atoi(txtGroupSize->GetValue().ToAscii());
	if(lastGroupSize < 1)
		lastGroupSize = 0;
	wxString age = txtAge->GetValue();
	SEX sex = MALE;
	if(rbSexFemale->GetValue())
		sex = FEMALE;
	if(rbSexUnknown->GetValue())
		sex = UNKNOWN;

	// Create a photo structure
	// Build a PhotoInfo structure
	PhotoInfo *pi = new PhotoInfo;
	pi->animal_name = lastAnimalID.ToAscii();
	pi->aperture	= aperture;
	pi->camera_info = makemodel;
	pi->date		= sightdate;
	pi->flank		= lastFlank;
	pi->focal_length= focallen;
	pi->notes		= lastNotes.ToAscii();
	pi->original_filename = originalPath.ToAscii();
	pi->photo_exposure = exposure;
	pi->quality		= lastPhotoQuality;
	pi->roi			= roi;
	pi->sighting_id	= lastSighting.ToAscii();
	pi->time		= sighttime;
	pi->age			= age.ToAscii();
	pi->gps_lat		= lastGPSLat.ToAscii();
	pi->gps_lon		= lastGPSLon.ToAscii();
	pi->group_size	= lastGroupSize;
	pi->reproductive= txtReproductive->GetValue().ToAscii();
	pi->sighting_location = lastSightingLocation.ToAscii();
	pi->sex			= sex;

    // Insert image into database, obtain image ID
    //const char *animalName, int imgQuality, int flank, const char *notes, const char *sighting
    int photoID = db.addPicture(pi);
    if (photoID < 1) {
		delete pi;
        wxMessageBox(_("Database error, sorry."), _("Error"));
        return;
    }

    // Save the image to the data directory
    wxString fname;
    fname.Printf(_("img-%07d.jpg"), photoID );
    wxImage *img = ifFinalPreview->getImage();
    if (!img) {
        wxMessageBox(_("Unable to save the image to disk!"), _("Error"));
        return;
    }
    img->SaveFile(fname, wxBITMAP_TYPE_JPEG);

    // Create a thumbnail and save the image to the data directory
    wxImage *thumb = ImageFrame::CloneImage(img);
    int w = thumb->GetWidth();
    int h = thumb->GetHeight();
    double ar = w*1.0 / h;
    if (h > w || (THUMB_W/ar > THUMB_H)) {
        h = THUMB_H;
        w = ar * h;
    } else {
        w = THUMB_W;
        h = w / ar;
    }
    thumb->Rescale(w, h);
    fname.Printf(_("thumb-%07d.jpg"), photoID );
    thumb->SaveFile(fname, wxBITMAP_TYPE_JPEG);
    delete thumb;

	// Save the StripeCode and multi-resolution histogram
	db.stripeCodes[photoID] = stripeCode;
	db.MRHistograms[photoID] = MRHisto;

	// Write the database back
	if(wxSetWorkingDirectory(_(".."))) {
		if(!db.dumpDatabase())
			wxMessageBox(_("The database file could not be opened for writing -- is it open in another program?"), _("WARNING"));
		wxSetWorkingDirectory(_("images"));
	}

    Close();
}
