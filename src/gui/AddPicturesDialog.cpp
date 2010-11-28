/***************************************************************
 * Name:      AddPicturesDialog.cpp
 * Purpose:   Add new images to the database
 * Author:    Mayank Lahiri (mlahiri@gmail.com)
 * Created:   2010-09-16
 * Copyright: Mayank Lahiri (http://compbio.cs.uic.edu/~mayank/)
 * License:
 **************************************************************/
#include "wx_pch.h"
#include <exif.h>
#include "AddPicturesDialog.h"
#include "SavePictureDialog.h"


DLGAddPictures::DLGAddPictures(wxWindow* parent) : AddPicturesDialog(parent) {
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
        lctrlFileList->SetItem(item);
    }

    selectedImageFilePath = filePath;
    iifImageSlicer->loadImage(filePath);
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
    wxImage *viewport = iifImageSlicer->extractViewportImage(); // viewport image will be owned by the dialog, destroyed after
    wxRect *selection = iifImageSlicer->getSelectionBox();
    if(!viewport) {
        wxMessageBox(_("Unable to save the image."), _("Error"));
        return;
    }

    DLGSavePicture *dlg = new DLGSavePicture(this, viewport, selection, selectedImageFilePath );
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
    return obj.imgNames[(unsigned)i1].compare(obj.imgNames[(unsigned)i2]);
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
    col0.SetText(_("Filename"));
    col0.SetWidth(125);
    lctrlFileList->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetId(1);
    col1.SetText(_("Time"));
    col1.SetWidth(100);
    lctrlFileList->InsertColumn(1, col1);
    wxListItem col2;
    col2.SetId(2);
    col2.SetText(wxEmptyString);
    col2.SetWidth(0);
    lctrlFileList->InsertColumn(2, col2);

    // Walk the directory
    wxArrayString allFiles;
    int filesFound = wxDir::GetAllFiles(selectedPath, &allFiles, wxEmptyString, wxDIR_FILES);
    int imagesFound = 0;
    imgNames.clear();
    for(int i = 0; i < filesFound; i++) {
        wxString &filePath = allFiles[i];
        if(filePath.EndsWith((const wxChar*)wxT("jpg")) || filePath.EndsWith((const wxChar*)wxT("JPG"))) {
            // Insert filename into list
            imagesFound++;
            wxString fileNameUnix = filePath.AfterLast((wxChar)'/');
            wxString fileNameWin = filePath.AfterLast((wxChar)'\\');
            wxString fileNameFinal = (fileNameUnix.Length()<fileNameWin.Length()) ? fileNameUnix : fileNameWin;
            long itemIndex = lctrlFileList->InsertItem(0, fileNameFinal);
            lctrlFileList->SetItem(itemIndex, 1, wxEmptyString);
            lctrlFileList->SetItem(itemIndex, 2, filePath);
            lctrlFileList->SetItemData(itemIndex, (long)imgNames.size());
            imgNames.push_back(string((const char*) fileNameFinal.ToUTF8()));

            // Update status
            wxString status;
            status.Printf(wxT("%d photos found."), imagesFound);
            txtFilesFound->SetLabel(status);
        }
    }

    // Sort the list by filename
    lctrlFileList->SortItems(fileSorter, (long)this);
    selectedImageFilePath = wxEmptyString;
}
