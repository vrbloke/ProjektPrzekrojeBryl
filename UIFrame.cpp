#include "UIFrame.h"
#include "FrameStepper.h"

#include <string>
#include <stdexcept>
#include <fstream>

UIFrame::UIFrame(wxWindow* parent) : MyFrame1(parent), 
m_cfg(new Configurer(this)), 
m_rnd(new Renderer(m_cfg)),
m_stp(new FrameStepper(this))
{
}

UIFrame::~UIFrame() {
  m_stp->Kill();
}

void UIFrame::PrismDataClick(wxCommandEvent& event) {
  wxFileDialog WxOpenFileDialog(this, _("Choose a file"), _(""), _(""), _("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (WxOpenFileDialog.ShowModal() == wxID_OK)
  {
    m_cfg->loadPrism(WxOpenFileDialog.GetPath().ToAscii());
  }
}

void UIFrame::SaveAnimationClick(wxCommandEvent& event) {
  wxDirDialog WxOpenDirDialog(this, _("Choose a folder to save animation"), _(""));

  if (WxOpenDirDialog.ShowModal() == wxID_OK)
  {
    m_cfg->saveAnimation(WxOpenDirDialog.GetPath().ToAscii());
  }
}

void UIFrame::PanelResized(wxSizeEvent& event) {
  m_cfg->setSizeX(m_panel1->GetSize().x);
  m_cfg->setSizeY(m_panel1->GetSize().y);
  m_cfg->setPosX(m_panel1->GetPosition().x);
  m_cfg->setPosY(m_panel1->GetPosition().y);
}

void UIFrame::Redraw(wxPaintEvent& event) {
  Redraw();
}

void UIFrame::Redraw() {
  if (!(m_stp->IsAlive()))
    m_stp->Run();
  wxClientDC dc(m_panel1);
  m_rnd->Render(&dc, m_panel1->GetSize().x, m_panel1->GetSize().y);
}

void UIFrame::RedrawCSection(wxPaintEvent& event) {
  RedrawCSection();
}

void UIFrame::RedrawCSection() {
  if (!(m_stp->IsAlive()))
    m_stp->Run();
  wxClientDC dc(m_panel2);
  m_rnd->RenderCSection(&dc, m_panel2->GetSize().x, m_panel2->GetSize().y);
}

void UIFrame::PlaneChanged(wxCommandEvent& event) {
  m_cfg->setPlaneId(event.GetSelection() + 1);
}

void UIFrame::VelocityOnText(wxCommandEvent& event) {
  try {
    m_cfg->setV(std::stof(static_cast<std::string>(event.GetString())));
  }
  catch (std::out_of_range& ex) {}
  catch (std::invalid_argument& ex) {}
}