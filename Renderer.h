#pragma once
#include "Configurer.h"

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include <memory>
#include <algorithm>
#include <numeric>
#include <random>

class Renderer {
private:
  enum PlaneID {
    OYZ = 1,
    OXZ = 2,
    OXY = 3
  };

  std::shared_ptr<Configurer> m_cfg;
  Matrix4 m_rotation_matrix;
  bool m_matrix_set;
  double m_plane_pos;
  PlaneID m_selected_plane;
  bool m_saving;
  unsigned short int frame;
  std::default_random_engine m_rng;

  Vector4 Scale(Vector4& original);
  Vector4 Rotate(Vector4& original, double x_angle, double y_angle, double z_angle);
  Vector4 ApplyPerspective(Vector4& original, double fov);
  Vector4 CenterScreen(Vector4& original);

  Vector4 TransformVector(Vector4& original, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);

  void DrawAxes(wxBufferedDC& dc, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);

  void DrawPlane(wxDC& dc, PlaneID id, double fov, double x_angle, double y_angle, double z_angle);

  void UpdatePlanePos(wxBufferedDC& dc, float speed);

public:
  Renderer(std::shared_ptr<Configurer> cfg) : m_cfg(cfg), m_rotation_matrix(Matrix4()), 
    m_matrix_set(false), m_plane_pos(-2), m_saving(false), frame(0), m_selected_plane((PlaneID)1),
    m_rng(std::default_random_engine())
  {
    wxImage::AddHandler(new wxJPEGHandler);
  }
  void Render(wxDC* parentDC, int width, int height);
  void RenderCSection(wxDC* parentDC, int width, int height);
};
