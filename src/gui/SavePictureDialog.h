#ifndef SAVEPICTUREDIALOG_H_INCLUDED
#define SAVEPICTUREDIALOG_H_INCLUDED
#include "GUIFrame.h"
#include <exif.h>

class DLGSavePicture : public SaveImageDialog {
	protected:
        virtual void OnBtnSave( wxCommandEvent& event );
        virtual void OnBtnCancel( wxCommandEvent& event );
        virtual void OnAnimalIDFocused( wxFocusEvent& event );
        virtual void OnAnimalIDChanged( wxCommandEvent& event );

        EXIFInfo exif;
        char roi[256];
        wxString originalPath;
        char sightdate[11];
        char sighttime[6];
        char exposure[16];
        char aperture[8];
        char focallen[8];
        char makemodel[256];

    public:
        DLGSavePicture(wxWindow* parent, wxImage *img, const wxRect *, const wxString &filePath);
        static wxString lastAnimalID;
        static wxString lastSighting;
        static int lastFlank;
        static int lastPhotoQuality;
};



#endif // SAVEPICTUREDIALOG_H_INCLUDED
