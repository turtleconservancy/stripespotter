///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  4 2010)
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
#ifdef __WXMAC__
	menuFileQuit = new wxMenuItem( fileMenu, wxID_EXIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl+Q"), wxT("Quit the application"), wxITEM_NORMAL );
#else
	menuFileQuit = new wxMenuItem( fileMenu, wxID_EXIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
#endif
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") ); 
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, wxID_ABOUT, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
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

#ifdef __WXMAC__
    wxString bmp_file = wxGetCwd() + wxT("/StripeSpotter.app/Contents/Resources/icon-zebra.bmp");
    if (!wxFileExists(bmp_file)) {
        bmp_file = wxGetCwd() + wxT("/icon-zebra.bmp");
    }
#else
    wxString bmp_file = wxT("icon-zebra.bmp");
#endif
	
	btnAddNewPictures = new wxBitmapButton( this, wxID_ANY, wxBitmap(bmp_file, wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bToolAreaSizer->Add( btnAddNewPictures, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	bSizer12->SetMinSize( wxSize( 100,-1 ) ); 
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Animal database information"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer12->Add( m_staticText2, 0, wxALL, 5 );
	
	txtAnimalInfo = new wxStaticText( this, wxID_ANY, wxT("StripeSpotter"), wxDefaultPosition, wxDefaultSize, 0 );
	txtAnimalInfo->Wrap( -1 );
	bSizer12->Add( txtAnimalInfo, 0, wxALL, 5 );
	
	bToolAreaSizer->Add( bSizer12, 1, wxEXPAND, 5 );
	
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
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
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
	
	bSizer20->Add( bSizer19, 1, wxEXPAND|wxALIGN_RIGHT, 5 );
	
	leftPane->Add( bSizer20, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2083;
	bSizer2083 = new wxBoxSizer( wxVERTICAL );
	
	lctrlFileList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES|wxWANTS_CHARS );
	bSizer2083->Add( lctrlFileList, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Instructions:\n1. Choose a directory by clicking \"Browse\"\n2. Zoom the image in to the ENTIRE zebra body\n3. Select the region to be used for identification\n4. Click \"Identify\" or \"Save\"\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( 250 );
	bSizer2083->Add( m_staticText24, 0, wxALL, 5 );
	
	bSizer23->Add( bSizer2083, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer1912;
	bSizer1912 = new wxBoxSizer( wxHORIZONTAL );
	
	rbAlgStripeCode = new wxRadioButton( this, wxID_ANY, wxT("StripeCode Algorithm"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer1912->Add( rbAlgStripeCode, 1, wxALL|wxEXPAND, 5 );
	
	rbAlgCO1 = new wxRadioButton( this, wxID_ANY, wxT("CO-1 Algorithm"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1912->Add( rbAlgCO1, 1, wxALL|wxEXPAND, 5 );
	
	bSizer24->Add( bSizer1912, 0, 0, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("When using the CO-1 algorithm, ensure that only the animal's body is selected (i.e., no grass, etc.)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( 250 );
	bSizer24->Add( m_staticText25, 0, wxALL, 5 );
	
	btnIdentifyAnimal = new wxButton( this, wxID_ANY, wxT("IDENTIFY &ANIMAL"), wxDefaultPosition, wxDefaultSize, 0 );
	btnIdentifyAnimal->SetDefault(); 
	btnIdentifyAnimal->SetFont( wxFont( 10, 70, 90, 92, false, wxEmptyString ) );
	btnIdentifyAnimal->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	bSizer24->Add( btnIdentifyAnimal, 0, wxALL|wxEXPAND, 5 );
	
	lctrlIDResults = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_NO_SORT_HEADER|wxLC_REPORT );
	bSizer24->Add( lctrlIDResults, 1, wxALL|wxEXPAND, 5 );
	
	bSizer23->Add( bSizer24, 1, wxEXPAND, 5 );
	
	leftPane->Add( bSizer23, 1, wxEXPAND, 5 );
	
	bSizer16->Add( leftPane, 1, wxEXPAND, 5 );
	
	wxBoxSizer* rightPane;
	rightPane = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer202;
	bSizer202 = new wxBoxSizer( wxHORIZONTAL );
	
	btnZoomIn = new wxButton( this, wxID_ANY, wxT("Zoom &In"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer202->Add( btnZoomIn, 1, wxALL|wxEXPAND, 5 );
	
	btnZoomOut = new wxButton( this, wxID_ANY, wxT("Zoom &Out"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer202->Add( btnZoomOut, 1, wxALL|wxEXPAND, 5 );
	
	rightPane->Add( bSizer202, 0, wxEXPAND, 5 );
	
	iifImageSlicer = new InteractiveImageFrame(this);
	rightPane->Add( iifImageSlicer, 1, wxALL|wxEXPAND, 5 );
	
	txtMatchInfo = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	txtMatchInfo->Wrap( -1 );
	txtMatchInfo->SetFont( wxFont( 14, 70, 90, 92, false, wxEmptyString ) );
	
	rightPane->Add( txtMatchInfo, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxHORIZONTAL );
	
	btnSaveAsNewAnimal = new wxButton( this, wxID_ANY, wxT("&Save as new animal"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSaveAsNewAnimal->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer201->Add( btnSaveAsNewAnimal, 1, wxALL|wxEXPAND, 5 );
	
	btnSaveAsOldAnimal = new wxButton( this, wxID_ANY, wxT("Accept match"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( btnSaveAsOldAnimal, 1, wxALL|wxEXPAND, 5 );
	
	rightPane->Add( bSizer201, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	ifMatchDisplay = new ImageFrame(this);
	rightPane->Add( ifMatchDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer16->Add( rightPane, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	// Connect Events
	dirctrlImageDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( AddPicturesDialog::OnDirChanged ), NULL, this );
	lctrlFileList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnListItemSelected ), NULL, this );
	btnIdentifyAnimal->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnIdentifyAnimal ), NULL, this );
	lctrlIDResults->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnSearchResultSelected ), NULL, this );
	btnZoomIn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomIn ), NULL, this );
	btnZoomOut->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomOut ), NULL, this );
	btnSaveAsNewAnimal->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnSaveImage ), NULL, this );
	btnSaveAsOldAnimal->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnAcceptMatch ), NULL, this );
}

AddPicturesDialog::~AddPicturesDialog()
{
	// Disconnect Events
	dirctrlImageDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( AddPicturesDialog::OnDirChanged ), NULL, this );
	lctrlFileList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnListItemSelected ), NULL, this );
	btnIdentifyAnimal->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnIdentifyAnimal ), NULL, this );
	lctrlIDResults->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AddPicturesDialog::OnSearchResultSelected ), NULL, this );
	btnZoomIn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomIn ), NULL, this );
	btnZoomOut->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnZoomOut ), NULL, this );
	btnSaveAsNewAnimal->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnSaveImage ), NULL, this );
	btnSaveAsOldAnimal->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnAcceptMatch ), NULL, this );
	btnEditDistanceVisualizer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AddPicturesDialog::OnAcceptMatch ), NULL, this );
	
}

SaveImageDialog::SaveImageDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 640,338 ), wxDefaultSize );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 13, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->AddGrowableRow( 10 );
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
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Sighting ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer3->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtSightingID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( txtSightingID, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, wxT("Location"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer3->Add( m_staticText23, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtSightingLocation = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( txtSightingLocation, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("GPS"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer3->Add( m_staticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer181->Add( m_staticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtGPSLat = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( txtGPSLat, 0, wxALL, 5 );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	bSizer181->Add( m_staticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtGPSLon = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( txtGPSLon, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer181, 1, wxEXPAND, 5 );
	
	m_staticText31 = new wxStaticText( this, wxID_ANY, wxT("Group size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer3->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtGroupSize = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	txtGroupSize->SetMaxLength( 3 ); 
	fgSizer3->Add( txtGroupSize, 0, wxALL, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Reproductive status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer3->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer199;
	bSizer199 = new wxBoxSizer( wxHORIZONTAL );
	
	txtReproductive = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	txtReproductive->SetMaxLength( 32 ); 
	bSizer199->Add( txtReproductive, 1, wxALL, 5 );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("Age"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	bSizer199->Add( m_staticText20, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtAge = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	txtAge->SetMaxLength( 32 ); 
	bSizer199->Add( txtAge, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer199, 1, wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Flank"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	rbFlankLeft = new wxRadioButton( this, wxID_ANY, wxT("&Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer16->Add( rbFlankLeft, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rbFlankRight = new wxRadioButton( this, wxID_ANY, wxT("&Right"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankRight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rbFlankFront = new wxRadioButton( this, wxID_ANY, wxT("Fro&nt"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankFront, 0, wxALL, 5 );
	
	rbFlankRear = new wxRadioButton( this, wxID_ANY, wxT("&Back"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( rbFlankRear, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer16, 1, wxEXPAND, 5 );
	
	m_staticText171 = new wxStaticText( this, wxID_ANY, wxT("Sex"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	fgSizer3->Add( m_staticText171, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	rbSexMale = new wxRadioButton( this, wxID_ANY, wxT("&Male"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer161->Add( rbSexMale, 0, wxALL, 5 );
	
	rbSexFemale = new wxRadioButton( this, wxID_ANY, wxT("&Female"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( rbSexFemale, 0, wxALL, 5 );
	
	rbSexUnknown = new wxRadioButton( this, wxID_ANY, wxT("&Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( rbSexUnknown, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer161, 1, wxEXPAND, 5 );
	
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
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Notes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	txtSightingNotes = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	fgSizer3->Add( txtSightingNotes, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText231 = new wxStaticText( this, wxID_ANY, wxT("Tip: press the Alt key to see keyboard accelerators. Then, press Alt+the underlined letter to quickly make choices."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText231->Wrap( 250 );
	fgSizer3->Add( m_staticText231, 0, wxALL, 5 );
	
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
	fgSizer4->Add( calSightDate, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Sighting time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer4->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Hour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer19->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	spinHour = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS|wxSP_WRAP, 0, 23, 0 );
	bSizer19->Add( spinHour, 0, wxALL, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer19->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	spinMin = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS|wxSP_WRAP, 0, 59, 0 );
	bSizer19->Add( spinMin, 0, wxALL, 5 );
	
	fgSizer4->Add( bSizer19, 1, wxEXPAND, 5 );
	
	bSizer17->Add( fgSizer4, 0, wxALL|wxEXPAND, 5 );
	
	bSizer14->Add( bSizer17, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer14 );
	this->Layout();
	
	// Connect Events
	txtAnimalID->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnAnimalIDChanged ), NULL, this );
	txtSightingID->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnSightingIDChanged ), NULL, this );
	btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnCancel ), NULL, this );
	btnSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnSave ), NULL, this );
}

SaveImageDialog::~SaveImageDialog()
{
	// Disconnect Events
	txtAnimalID->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnAnimalIDChanged ), NULL, this );
	txtSightingID->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SaveImageDialog::OnSightingIDChanged ), NULL, this );
	btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnCancel ), NULL, this );
	btnSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageDialog::OnBtnSave ), NULL, this );
	
}
