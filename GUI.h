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
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/sizer.h>
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
		wxPanel* m_panel2;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_TextCtrlVelocityY;
		wxStaticText* m_staticText111;
		wxChoice* m_choicePlane;
		wxButton* m_buttonPrismData;
		wxButton* m_buttonSaveAnimation;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText11;
		wxSlider* m_sliderPlaneLocation;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Redraw( wxPaintEvent& event ) { event.Skip(); }
		virtual void PanelResized( wxSizeEvent& event ) { event.Skip(); }
		virtual void RedrawCSection( wxPaintEvent& event ) { event.Skip(); }
		virtual void VelocityOnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void PrismDataClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaneLocationOnScrollChanged( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Przekroje brył 3D"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1200,487 ), long style = wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		
		~MyFrame1();
	
};

#endif //__GUI_H__
