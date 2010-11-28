#ifndef ADDPICTURESDIALOG_H_INCLUDED
#define ADDPICTURESDIALOG_H_INCLUDED
#include "GUIFrame.h"
using namespace std;

class DLGAddPictures : public AddPicturesDialog {
    private:
        vector<string> imgNames;
        wxString selectedImageFilePath;

	protected:
        static int wxCALLBACK fileSorter(long i1, long i2, long sortData);
        virtual void OnScan( wxCommandEvent& event );
        virtual void OnListItemSelected( wxListEvent& event );
        virtual void OnZoomIn( wxCommandEvent& event );
        virtual void OnZoomOut( wxCommandEvent& event );
        virtual void OnSaveImage( wxCommandEvent& event );
        virtual void OnDirChanged( wxFileDirPickerEvent& event );

    public:
        DLGAddPictures(wxWindow* parent);

};


#endif // ADDPICTURESDIALOG_H_INCLUDED
