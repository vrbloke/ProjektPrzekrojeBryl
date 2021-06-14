#pragma once

#include "UIFrame.h"

#include <wx/thread.h>

class FrameStepper : public wxThread {
public:
  FrameStepper(UIFrame* frame) : wxThread(wxTHREAD_JOINABLE), m_frame(frame) {}

  virtual ExitCode Entry() override {
    while (m_frame) {
      Sleep(50);
      if (m_frame) m_frame->Redraw();
    }
    return (ExitCode)0;
  }
private:
  UIFrame* m_frame;
};