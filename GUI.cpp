///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	
	bSizer1->Add( m_panel1, 2, wxALL|wxEXPAND, 5 );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	
	bSizer1->Add( m_panel2, 2, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	bSizer2->SetMinSize( wxSize( 280,-1 ) ); 
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Pr\u0119dko\u015b\u0107"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_TextCtrlVelocityY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	bSizer2->Add( m_TextCtrlVelocityY, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText111 = new wxStaticText( this, wxID_ANY, wxT("P\u0142aszczyzna"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer2->Add( m_staticText111, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_choicePlaneChoices[] = { wxT("OYZ"), wxT("OXZ"), wxT("OXY") };
	int m_choicePlaneNChoices = sizeof( m_choicePlaneChoices ) / sizeof( wxString );
	m_choicePlane = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 80,-1 ), m_choicePlaneNChoices, m_choicePlaneChoices, 0 );
	m_choicePlane->SetSelection( 0 );
	bSizer2->Add( m_choicePlane, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_buttonPrismData = new wxButton( this, wxID_ANY, wxT("Wczytaj dane bry\u0142y"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonPrismData, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_buttonSaveAnimation = new wxButton( this, wxID_ANY, wxT("Zapisz animacj\u0119"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonSaveAnimation, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	
	bSizer2->Add( bSizer6, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 0, 0, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_panel1->Connect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::Redraw ), NULL, this );
	m_panel1->Connect( wxEVT_SIZE, wxSizeEventHandler( MyFrame1::PanelResized ), NULL, this );
	m_panel2->Connect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::RedrawCSection ), NULL, this );
	m_TextCtrlVelocityY->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MyFrame1::VelocityOnText ), NULL, this );
	m_choicePlane->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MyFrame1::PlaneChanged ), NULL, this );
	m_buttonPrismData->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::PrismDataClick ), NULL, this );
	m_buttonSaveAnimation->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::SaveAnimationClick ), NULL, this );
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_panel1->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::Redraw ), NULL, this );
	m_panel1->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MyFrame1::PanelResized ), NULL, this );
	m_panel2->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::RedrawCSection ), NULL, this );
	m_TextCtrlVelocityY->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MyFrame1::VelocityOnText ), NULL, this );
	m_choicePlane->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MyFrame1::PlaneChanged ), NULL, this );
	m_buttonPrismData->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::PrismDataClick ), NULL, this );
	m_buttonSaveAnimation->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::SaveAnimationClick ), NULL, this );
	
}
