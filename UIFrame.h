#pragma once

#include "GUI.h"
#include "Renderer.h"

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>

#include <memory>

class FrameStepper;

class UIFrame : public MyFrame1 {
public:
	UIFrame(wxWindow* parent);
	~UIFrame();

	virtual void PrismDataClick(wxCommandEvent& event) override;
	virtual void SaveAnimationClick(wxCommandEvent& event) override;
	virtual void PlaneLocationOnScrollChanged(wxScrollEvent& event) override;
	virtual void PanelResized(wxSizeEvent& event) override;

	void Redraw(wxPaintEvent& event) override;
	void Redraw();

	virtual void RedrawCSection(wxPaintEvent& event) override;
	void RedrawCSection();

	virtual void PlaneChanged(wxCommandEvent& event) override;
	virtual void VelocityOnText(wxCommandEvent& event) override;

	void GetPanelSize(int* width, int* height) { m_panel1->GetSize(width, height); }
	void GetPanelPosition(int* width, int* height) { m_panel1->GetPosition(width, height); }
	void RefreshPanel() { m_panel1->Refresh(); }
private:
	std::shared_ptr<Configurer> m_cfg;
	std::shared_ptr<Renderer> m_rnd;
	std::unique_ptr<FrameStepper> m_stp;
};