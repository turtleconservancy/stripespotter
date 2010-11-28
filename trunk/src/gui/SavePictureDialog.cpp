/***************************************************************
 * Name:      SavePictureDialog.cpp
 * Purpose:   After the animal ID, save a picture to the database
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include "SavePictureDialog.h"
#include "db.h"
#include "parameters.h"

// GLOBAL database reference
extern PhotoDatabase *globalDB;
wxString DLGSavePicture::lastAnimalID = wxEmptyString;
wxString DLGSavePicture::lastSighting = wxEmptyString;
int DLGSavePicture::lastFlank = 0;
int DLGSavePicture::lastPhotoQuality = 0;

DLGSavePicture::DLGSavePicture(wxWindow* parent, wxImage *img, const wxRect *select, const wxString &filePath) : SaveImageDialog(parent) {
    ifFinalPreview->loadImage(img);

    originalPath = filePath;
    if (select) {
        ifFinalPreview->setSelectionBox(select);
        sprintf(roi, "%d,%d,%d,%d", select->GetLeft(), select->GetTop(), select->GetWidth(), select->GetHeight());
    } else
        roi[0] = 0;
    txtAnimalID->SetFocus();
    txtAnimalID->SetValue(DLGSavePicture::lastAnimalID);
    txtSightingLocation->SetValue(DLGSavePicture::lastSighting);
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
        wxDateTime tstamp;
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
        spinHour->SetValue(exif.hour > 12 ? exif.hour-12 : exif.hour);
        spinMin->SetValue(exif.min);
        choiceAMPM->SetSelection(exif.hour > 12 ? 1 : 0);

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
// User doesn't want to save the image after all
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
    if (globalDB->queryAnimalName(query.ToAscii())) {
        label = query + _(": FOUND IN DATABASE!");
        txtAnimalSearchResult->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    } else {
        label = query + _(": New animal.");
        txtAnimalSearchResult->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    }
    txtAnimalSearchResult->SetLabel(label);
}

//
// Saves an image to the database
// i18n NOTE: UTF-8 is used, but only ascii characters are pulled.
//
void DLGSavePicture::OnBtnSave(wxCommandEvent& event) {
    // Grab information from dialog box
    lastAnimalID = txtAnimalID->GetValue();
    lastSighting = txtSightingLocation->GetValue();
    lastPhotoQuality = 0;
    if (rbPicOK->GetValue())
        lastPhotoQuality = 1;
    if (rbPicGood->GetValue())
        lastPhotoQuality  = 2;
    if (rbPicBest->GetValue())
        lastPhotoQuality = 3;
    lastFlank = 0;          // 0 = left, 1 = right, 2 = front, 3 = back
    if (rbFlankRight->GetValue())
        lastFlank = 1;
    if (rbFlankFront->GetValue())
        lastFlank = 2;
    if (rbFlankRear->GetValue())
        lastFlank = 3;
    wxString notes = txtSightingNotes->GetValue();

    // Insert image into database, obtain image ID
    //const char *animalName, int imgQuality, int flank, const char *notes, const char *sighting
    int animalID = globalDB->addPicture(lastAnimalID.ToAscii(),
                                        lastPhotoQuality,
                                        lastFlank,
                                        notes.ToAscii(),
                                        lastSighting.ToAscii(),
                                        roi,
                                        originalPath.ToAscii(),
                                        sightdate,
                                        sighttime,
                                        exposure,
                                        focallen,
                                        aperture,
                                        makemodel
                                       );
    if (animalID < 0) {
        wxMessageBox(_("Database error, sorry."), _("Error"));
        return;
    }

    // Save the image to the data directory
    wxString fname;
    fname.Printf(_("img-%07d.jpg"), animalID);
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
    fname.Printf(_("thumb-%07d.jpg"), animalID);
    thumb->SaveFile(fname, wxBITMAP_TYPE_JPEG);
    delete thumb;

    // Save the ROI if one has been selected
    // TODO: remove from production
    wxRect *sel = NULL;
    if ( (sel = ifFinalPreview->getSelectionBox()) ) {
        wxImage *roi = ifFinalPreview->ExtractRectangle(img, sel->GetLeft(), sel->GetTop(), sel->GetWidth(), sel->GetHeight());
        fname.Printf(_("roi-%07d.jpg"), animalID);
        roi->SaveFile(fname, wxBITMAP_TYPE_JPEG);
        delete roi;
    }

    Close();
}
