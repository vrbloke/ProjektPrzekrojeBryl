#pragma once
#include "Configurer.h"

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include <memory>
#include <tuple>

class Renderer {
private:
  std::shared_ptr<Configurer> m_cfg;
  Matrix4 m_rotation_matrix;
  bool m_matrix_set;
  double m_plane_start_pos;
  bool m_saving;
  unsigned short int frame;

  Vector4 Scale(Vector4& original);
  Vector4 Rotate(Vector4& original, double x_angle, double y_angle, double z_angle);
  Vector4 ApplyPerspective(Vector4& original, double fov);
  Vector4 CenterScreen(Vector4& original);

  double Dot(const Vector4& lhs, const Vector4& rhs) const;
  Vector4 Cross(const Vector4& lhs, const Vector4& rhs) const;
  Vector4 Normalize(const Vector4& org) const;
  double Magnitude(const Vector4& org) const;
  // p is plane normal, x and y are arbitrarily chosen coordinates of the perpendicular vector. Result is normalized.
  Vector4 FindPerpendicular(const Vector4& p, const double x, const double y) const;
  // Given plane normal vector p, find two perpendicular vectors perpendicular to p.
  std::vector<Vector4> FindBasis(const Vector4& p) const;
  // Given plane normal vector p and its root dist, find affine matrix to transform the XY plane into the plane defined by p and dist.
  Matrix4 TransformPlane(const Vector4& p, const double dist) const;

  Vector4 TransformVector(Vector4& original, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);

  void DrawAxes(wxBufferedDC& dc, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);

  void DrawPlane(wxBufferedDC& dc, double fov, double x_angle, double y_angle, double z_angle);
  void DrawXPlane(wxBufferedDC& dc, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);
  void DrawYPlane(wxBufferedDC& dc, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);
  void DrawZPlane(wxBufferedDC& dc, double fov = 45, double x_angle = 0, double y_angle = 0, double z_angle = 0);

  void UpdatePlanePos(wxBufferedDC& dc, float speed);

  //Testowanie algorytmów do obcinania bry³
  void Test(std::vector<Segment>& data); 
  void Test2(std::vector<Segment>& data, int axis_plane);
  void Test3(std::vector<Segment>& data);
  void Test4(std::vector<Segment>& data);
  void CogenSutherland(std::vector<Segment>& data);

public:
  Renderer(std::shared_ptr<Configurer> cfg) : m_cfg(cfg), m_rotation_matrix(Matrix4()), m_matrix_set(false), m_plane_start_pos(-2), m_saving(false), frame(0) {
    wxImage::AddHandler(new wxJPEGHandler);
  }
  void Render(wxDC* parentDC, int width, int height);
};
