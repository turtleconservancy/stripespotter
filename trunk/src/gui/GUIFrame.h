///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFrame__
#define __GUIFrame__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/frame.h>
#include <wx/filepicker.h>
#include <wx/gauge.h>
#include "ImageFrame.h"
#include <wx/dialog.h>
#include <wx/calctrl.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxBitmapButton* btnAddNewPictures;
		wxStaticText* m_staticText2;
		wxStaticText* txtAnimalInfo;
		wxStaticText* m_staticText4;
		wxRadioButton* rbtnAnimalNames;
		wxRadioButton* rbtnMetadata;
		wxTextCtrl* editSearchTerm;
		wxButton* btnSearch;
		wxListCtrl* lctrlMainDisplay;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnFileSaveCSV( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPictures( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("StripeSpotter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 840,593 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUIFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AddPicturesDialog
///////////////////////////////////////////////////////////////////////////////
class AddPicturesDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText5;
		wxDirPickerCtrl* dirctrlImageDirectory;
		wxButton* btnScanDirectory;
		wxGauge* progDirScan;
		wxStaticText* txtFilesFound;
		wxListCtrl* lctrlFileList;
		wxButton* btnZoomIn;
		wxButton* btnZoomOut;
		wxButton* btnIdentifyAnimal;
		wxStaticText* m_staticText7;
		wxListCtrl* lctrlIDResults;
		wxButton* btnSaveAsNewAnimal;
		InteractiveImageFrame *iifImageSlicer;
		wxStaticText* txtMatchInfo;
		ImageFrame *ifMatchDisplay;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDirChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnScan( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnZoomIn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZoomOut( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveImage( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AddPicturesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Add pictures to database"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1020,572 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER );
		~AddPicturesDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SaveImageDialog
///////////////////////////////////////////////////////////////////////////////
class SaveImageDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText8;
		wxTextCtrl* txtAnimalID;
		
		wxStaticText* txtAnimalSearchResult;
		wxStaticText* m_staticText10;
		wxTextCtrl* txtSightingLocation;
		wxStaticText* m_staticText11;
		wxRadioButton* rbFlankLeft;
		wxRadioButton* rbFlankRight;
		wxRadioButton* rbFlankFront;
		wxRadioButton* rbFlankRear;
		wxStaticText* m_staticText13;
		wxTextCtrl* txtSightingNotes;
		wxStaticText* m_staticText15;
		wxRadioButton* rbPicBad;
		wxRadioButton* rbPicOK;
		wxRadioButton* rbPicGood;
		wxRadioButton* rbPicBest;
		wxButton* btnCancel;
		wxButton* btnSave;
		ImageFrame *ifFinalPreview;
		wxStaticText* m_staticText9;
		wxCalendarCtrl* calSightDate;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText16;
		wxSpinCtrl* spinHour;
		wxStaticText* m_staticText17;
		wxSpinCtrl* spinMin;
		wxChoice* choiceAMPM;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAnimalIDChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnSave( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SaveImageDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Save image to database"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 798,552 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~SaveImageDialog();
	
};

#endif //__GUIFrame__
