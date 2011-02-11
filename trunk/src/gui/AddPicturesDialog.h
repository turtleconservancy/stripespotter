#ifndef ADDPICTURESDIALOG_H_INCLUDED
#define ADDPICTURESDIALOG_H_INCLUDED
#include "GUIFrame.h"
using namespace std;

class DLGAddPictures : public AddPicturesDialog {
    private:
        wxString selectedImageFilePath;
		wxImageList *imgList;
		vector<pair<double,int> > searchResults;
		vector<wxString> sortKeys;
		int searchResultPhoto;

	protected:
        static int wxCALLBACK fileSorter(long i1, long i2, long sortData);
        virtual void OnScan( wxCommandEvent& event );
        virtual void OnListItemSelected( wxListEvent& event );
        virtual void OnZoomIn( wxCommandEvent& event );
        virtual void OnZoomOut( wxCommandEvent& event );
        virtual void OnSaveImage( wxCommandEvent& event );
        virtual void OnDirChanged( wxFileDirPickerEvent& event );
        virtual void OnIdentifyAnimal( wxCommandEvent& event );
		virtual void OnSearchResultSelected( wxListEvent &event );
		virtual void OnAcceptMatch(wxCommandEvent &event);

    public:
        DLGAddPictures(wxWindow* parent);
        ~DLGAddPictures();

};


#endif // ADDPICTURESDIALOG_H_INCLUDED
