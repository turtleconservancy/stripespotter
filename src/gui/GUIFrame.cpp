///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 640,480 ), wxDefaultSize );
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileCSV;
	menuFileCSV = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("&Save data spreadsheet...") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( menuFileCSV );
	
	wxMenuItem* m_separator1;
	m_separator1 = fileMenu->AppendSeparator();
	
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") );
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") );
	
	this->SetMenuBar( mbar );
	
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer2->AddGrowableRow( 1 );
	fgSizer2->SetFlexibleDirection( wxVERTICAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxBoxSizer* bToolAreaSizer;
	bToolAreaSizer = new wxBoxSizer( wxHORIZONTAL );
	
	btnAddNewPictures = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("icon-zebra.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bToolAreaSizer->Add( btnAddNewPictures, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	bSizer12->SetMinSize( wxSize( 100,-1 ) ); 
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Animal database information"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer12->Add( m_staticText2, 0, wxALL, 5 );
	
	txtAnimalInfo = new wxStaticText( this, wxID_ANY, wxT("12 sightings.\n480 pictures."), wxDefaultPosition, wxDefaultSize, 0 );
	txtAnimalInfo->Wrap( -1 );
	bSizer12->Add( txtAnimalInfo, 0, wxALL, 5 );
	
	bToolAreaSizer->Add( bSizer12, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer14->Add( m_staticText4, 0, wxALL, 7 );
	
	rbtnAnimalNames = new wxRadioButton( this, wxID_ANY, wxT("Animal names"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer14->Add( rbtnAnimalNames, 0, wxALL, 5 );
	
	rbtnMetadata = new wxRadioButton( this, wxID_ANY, wxT("Metadata"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( rbtnMetadata, 0, wxALL, 5 );
	
	bSizer13->Add( bSizer14, 1, wxALIGN_RIGHT, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	editSearchTerm = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	editSearchTerm->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer15->Add( editSearchTerm, 0, wxALL, 5 );
	
	btnSearch = new wxButton( this, wxID_ANY, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( btnSearch, 0, wxALL, 5 );
	
	bSizer13->Add( bSizer15, 1, wxALIGN_RIGHT, 5 );
	
	bToolAreaSizer->Add( bSizer13, 1, wxALIGN_RIGHT, 5 );
	
	fgSizer2->Add( bToolAreaSizer, 1, wxALL|wxEXPAND, 2 );
	
	lctrlMainDisplay = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES|wxRAISED_BORDER|wxVSCROLL );
	fgSizer2->Add( lctrlMainDisplay, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menuFileCSV->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnFileSaveCSV ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	btnAddNewPictures->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnAddPictures ), NULL, this );
	lctrlMainDisplay->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GUIFrame::OnListItemSelected ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnFileSaveCSV ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	btnAddNewPictures->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnAddPictures ), NULL, this );
	lctrlMainDisplay->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GUIFrame::OnListItemSelected ), NULL, this );
}

AddPicturesDialog::AddPicturesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 700,480 ), wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* leftPane;
	leftPane = new wxFlexGridSizer( 2, 1, 0, 0 );
	leftPane->AddGrowableRow( 1 );
	leftPane->SetFlexibleDirection( wxVERTICAL );
	leftPane->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Directory with new images"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer19->Add( m_staticText5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 12 );
	
	dirctrlImageDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST );
	bSizer19->Add( dirctrlImageDirectory, 1, wxALL|wxEXPAND, 5 );
	
	btnScanDirectory = new wxButton( this, wxID_ANY, wxT("S&can"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( btnScanDirectory, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer20->Add( bSizer19, 1, wxEXPAND|wxALIGN_RIGHT, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	progDirScan = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizer22->Add( progDirScan, 1, wxALL|wxEXPAND, 5 );
	
	txtFilesFound = new wxStaticText( this, wxID_ANY, wxT("No photographs found."), wxDefaultPosition, wxDefaultSize, 0 );
	txtFilesFound->Wrap( -1 );
	bSizer22->Add( txtFilesFound, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer20->Add( bSizer22, 1, wxEXPAND|wxALIGN_RIGHT, 5 );
	
	leftPane->Add( bSizer20, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	lctrlFileList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer23->Add( lctrlFileList, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	btnZoomIn = new wxButton( this, wxID_ANY, wxT("Zoom &In"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( btnZoomIn, 1, wxALL, 5 );
	
	btnZoomOut = new wxButton( this, wxID_ANY, wxT("Zoom &Out"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( btnZoomOut, 1, wxALL, 5 );
	
	bSizer24->Add( bSizer25, 0, wxEXPAND, 5 );
	
	btnIdentifyAnimal = new wxButton( this, wxID_ANY, wxT("IDENTIFY &ANIMAL"), wxDefaultPosition, wxDefaultSize, 0 );
	btnIdentifyAnimal->SetDefault(); 
	btnIdentifyAnimal->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	btnIdentifyAnimal->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	bSizer24->Add( btnIdentifyAnimal, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Automatic identification results:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer24->Add( m_staticText7, 0, wxALL, 5 );
	
	lctrlIDResults = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_NO_HEADER|wxLC_NO_SORT_HEADER|wxLC_REPORT );
	bSizer24->Add( lctrlIDResults, 1, wxALL|wxEXPAND, 5 );
	
	btnSaveAsNewAnimal = new wxButton( this, wxID_ANY, wxT("&Save image to database"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSaveAsNewAnimal->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer24->Add( btnSaveAsNewAnimal, 0, wxALL|wxEXPAND, 5 );
	
	bSizer23->Add( bSizer24, 1, wxEXPAND, 5 );
	
	leftPane->Add( bSizer23, 1, wxEXPAND, 5 );
	
	bSizer16->Add( leftPane, 1, wxEXPAND, 5 );
	
	wxBoxSizer* rightPane;
	rightPane = new wxBoxSizer( wxVERTICAL );
	
	iifImageSlicer = new InteractiveImageFrame(this);
	rightPane->Add( iifImageSlicer, 1, wxALL|wxEXPAND, 5 );
	
	txtMatchInfo = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	txtMatchInfo->Wrap( -1 );
	rightPane->Add( txtMatchInfo, 0, wxALL, 5 );
	
	ifMatchDisplay = new ImageFrame(this);
	rightPane->Add( ifMatchDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer16->Add( rightPane, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	// Connect Events
	dirctrlImageDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( AddPicturesDialog::OnDirChanged ), NULL, this );
	btnScanDirectory->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnScan ), NULL, this );
	lctrlFileList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnListItemSelected ), NULL, this );
	btnZoomIn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomIn ), NULL, this );
	btnZoomOut->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomOut ), NULL, this );
	btnSaveAsNewAnimal->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnSaveImage ), NULL, this );
}

AddPicturesDialog::~AddPicturesDialog()
{
	// Disconnect Events
	dirctrlImageDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( AddPicturesDialog::OnDirChanged ), NULL, this );
	btnScanDirectory->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnScan ), NULL, this );
	lctrlFileList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnListItemSelected ), NULL, this );
	btnZoomIn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomIn ), NULL, this );
	btnZoomOut->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomOut ), NULL, this );
	btnSaveAsNewAnimal->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnSaveImage ), NULL, this );
}

SaveImageDialog::SaveImageDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 640,338 ), wxDefaultSize );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableRow( 4 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Animal &name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtAnimalID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( txtAnimalID, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	txtAnimalSearchResult = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	txtAnimalSearchResult->Wrap( -1 );
	txtAnimalSearchResult->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	txtAnimalSearchResult->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	
	fgSizer3->Add( txtAnimalSearchResult, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Sighting"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer3->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtSightingLocation = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( txtSightingLocation, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Flank"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	rbFlankLeft = new wxRadioButton( this, wxID_ANY, wxT("&Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer16->Add( rbFlankLeft, 0, wxALL, 5 );
	
	rbFlankRight = new wxRadioButton( this, wxID_ANY, wxT("&Right"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankRight, 0, wxALL, 5 );
	
	rbFlankFront = new wxRadioButton( this, wxID_ANY, wxT("&Front"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankFront, 0, wxALL, 5 );
	
	rbFlankRear = new wxRadioButton( this, wxID_ANY, wxT("&Back"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankRear, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer16, 1, wxEXPAND, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Notes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALL, 5 );
	
	txtSightingNotes = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	fgSizer3->Add( txtSightingNotes, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Photo quality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer3->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	rbPicBad = new wxRadioButton( this, wxID_ANY, wxT("Bad"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer18->Add( rbPicBad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rbPicOK = new wxRadioButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( rbPicOK, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rbPicGood = new wxRadioButton( this, wxID_ANY, wxT("&Good"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( rbPicGood, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rbPicBest = new wxRadioButton( this, wxID_ANY, wxT("BEST!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( rbPicBest, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer3->Add( bSizer18, 1, wxEXPAND, 5 );
	
	btnCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( btnCancel, 1, wxALL|wxEXPAND, 5 );
	
	btnSave = new wxButton( this, wxID_ANY, wxT("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSave->SetDefault(); 
	btnSave->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	btnSave->SetForegroundColour( wxColour( 255, 27, 0 ) );
	
	fgSizer3->Add( btnSave, 1, wxALL|wxEXPAND, 5 );
	
	bSizer14->Add( fgSizer3, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	ifFinalPreview = new ImageFrame(this);
	bSizer17->Add( ifFinalPreview, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Sighting &date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer4->Add( m_staticText9, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	calSightDate = new wxCalendarCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SHOW_HOLIDAYS|wxCAL_SHOW_SURROUNDING_WEEKS );
	fgSizer4->Add( calSightDate, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Sighting time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer4->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Hour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer19->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	spinHour = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS|wxSP_WRAP, 1, 12, 1 );
	bSizer19->Add( spinHour, 0, wxALL, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer19->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	spinMin = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS|wxSP_WRAP, 0, 59, 0 );
	bSizer19->Add( spinMin, 0, wxALL, 5 );
	
	wxString choiceAMPMChoices[] = { wxT("AM"), wxT("PM") };
	int choiceAMPMNChoices = sizeof( choiceAMPMChoices ) / sizeof( wxString );
	choiceAMPM = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceAMPMNChoices, choiceAMPMChoices, 0 );
	choiceAMPM->SetSelection( 0 );
	bSizer19->Add( choiceAMPM, 0, wxALL, 5 );
	
	fgSizer4->Add( bSizer19, 1, wxEXPAND, 5 );
	
	bSizer17->Add( fgSizer4, 1, wxALL|wxEXPAND, 5 );
	
	bSizer14->Add( bSizer17, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer14 );
	this->Layout();
	
	// Connect Events
	txtAnimalID->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnAnimalIDChanged ), NULL, this );
	btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnCancel ), NULL, this );
	btnSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnSave ), NULL, this );
}

SaveImageDialog::~SaveImageDialog()
{
	// Disconnect Events
	txtAnimalID->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnAnimalIDChanged ), NULL, this );
	btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnCancel ), NULL, this );
	btnSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnSave ), NULL, this );
}
