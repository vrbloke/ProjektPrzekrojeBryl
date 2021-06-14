///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_TextCtrlVelocityX;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_TextCtrlVelocityY;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrlVelocityZ;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_textCtrlPlaneX;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textCtrlPlaneY;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_textCtrlPlaneZ;
		wxButton* m_buttonPrismData;
		wxButton* m_buttonSaveAnimation;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText11;
		wxSlider* m_sliderPlaneLocation;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Redraw( wxPaintEvent& event ) { event.Skip(); }
		virtual void PanelResized( wxSizeEvent& event ) { event.Skip(); }
		virtual void VelocityXOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void VelocityYOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void VelocityZOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneXOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneYOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneZOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void PrismDataClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneLocationOnScrollChanged( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Przekroje brył 3D"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 772,487 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MyFrame1();
	
};

#endif //__GUI_H__
