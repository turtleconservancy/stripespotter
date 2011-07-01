#ifndef SAVEPICTUREDIALOG_H_INCLUDED
#define SAVEPICTUREDIALOG_H_INCLUDED
#include "StripeCode.h"
#include "GUIFrame.h"
#include "db-csv.h"
#include <exif.h>

class DLGSavePicture : public SaveImageDialog {
	protected:
        virtual void OnBtnSave( wxCommandEvent& event );
        virtual void OnBtnCancel( wxCommandEvent& event );
        virtual void OnAnimalIDFocused( wxFocusEvent& event );
        virtual void OnAnimalIDChanged( wxCommandEvent& event );
		virtual void OnSightingIDChanged( wxCommandEvent& event );

        EXIFInfo exif;
        char roi[256];
        wxString originalPath;
        char sightdate[11];
        char sighttime[6];
        char exposure[16];
        char aperture[8];
        char focallen[8];
        char makemodel[256];
		StripeCode stripeCode;
		MultiScaleHistogram MRHisto;

    public:
		DLGSavePicture(wxWindow* parent, wxImage *img, const wxRect *, const wxString &filePath, StripeCode &sc, MultiScaleHistogram &mrh, bool newAnimal=true);
        static wxString lastAnimalID;
        static wxString lastSighting;
		static wxString lastSightingLocation;
		static wxString lastGPSLat, lastGPSLon;
		static int lastGroupSize;
		static wxString lastNotes;
        static FLANK lastFlank;
        static QUALITY lastPhotoQuality;
		static wxString lastAge;
		static SEX lastSex; 
		static wxString lastReproductive;
};



#endif // SAVEPICTUREDIALOG_H_INCLUDED
