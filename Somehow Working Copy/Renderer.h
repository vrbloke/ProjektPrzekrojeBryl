#pragma once
#include "Configurer.h"

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include <memory>

class Renderer {
private:
  std::shared_ptr<Configurer> m_cfg;
  Matrix4 m_transformation_matrix;
  bool m_matrix_set;

  double plane_pos;

  void SetTransformationMatrix(double angle_x, double angle_y, double angle_z);
  void DrawAxes(wxBufferedDC& dc, int size_x, int size_y);

  void SetTransformationMatrixv2(int size_x, int size_y);
  Vector4 Translate(Vector4& original, Vector4& translation);
  Vector4 Rotate(Vector4& original, Vector4& rotation);
  Vector4 ApplyPerspective(Vector4& original, double z0);
  Vector4 CenterScreen(Vector4& original, int size_x, int size_y);

public:
  Renderer(std::shared_ptr<Configurer> cfg) : m_cfg(cfg), m_transformation_matrix(Matrix4()), m_matrix_set(false), plane_pos(1.0){}
  void Render(wxDC* parentDC, int width, int height);
};
