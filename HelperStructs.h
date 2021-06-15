#pragma once

#include <iostream>
#include <array>

struct Vector4 {
  std::array<double,4> data;
  Vector4() : data({ 0,0,0,1 }) {}
  Vector4(double d1, double d2, double d3) : data({ d1, d2, d3, 1 }) {}
  Vector4(double d1, double d2, double d3, double d4) : data({ d1, d2, d3, d4 }) {}
  inline void Print(std::ostream& str) { str << data[0] << '\t' << data[1] << '\t' << data[2] << '\t' << data[3]; }
  inline void Set(double d1, double d2, double d3) { data[0] = d1; data[1] = d2; data[2] = d3; data[3] = 1; }
  inline double GetX() const { return data[0]; }
  inline double GetY() const { return data[1]; }
  inline double GetZ() const { return data[2]; }
  inline Vector4 operator-(const Vector4& rhs) const { return Vector4(data[0] - rhs.data[0], data[1] - rhs.data[1], data[2] - rhs.data[2]); }
  inline Vector4 operator*(double rhs) const { return Vector4(data[0]*rhs, data[1]*rhs, data[2]*rhs, data[3]*rhs);  }
  inline friend Vector4 operator*(double lhs, const Vector4& rhs);
};

Vector4 operator*(double lhs, const Vector4& rhs) {
  return Vector4(rhs.data[0] * lhs, rhs.data[1] * lhs, rhs.data[2] * lhs, rhs.data[3] * lhs);
}

struct Matrix4 {
  std::array<std::array<double, 4>, 4> data;
  //double d[4][4];
  Matrix4() : data({ std::array<double,4>({0., 0., 0., 0.}), 
    std::array<double,4>({0., 0. ,0. ,0. }), 
    std::array<double,4>({0., 0., 0., 0.}), 
    std::array<double,4>({0., 0., 0., 1.}) }) {}
  void Print(std::ostream& str) { for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) str << data[i][j]; }
  Matrix4 operator*(const Matrix4&);
  Vector4 operator*(const Vector4&);
  friend Vector4 operator*(const Vector4&, const Matrix4&);
};

struct Point {
  double x, y, z;
  Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
  Point() : x(0), y(0), z(0) {}
};

struct Color {
  int r, g, b;
  Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
#ifdef DEFAULT_BLUE
  Color() : r(0), g(0), b(255) {}
#endif //default blue
#ifndef DEFAULT_BLUE
  Color() : r(0), g(0), b(0) {}
#endif
};

typedef Color Colour;

struct Segment {
  Point begin, end;
  Color color;
  Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
  Segment(Point _begin, Point _end) : begin(_begin), end(_end), color(Color(0, 0, 255)) {}
  Segment() : begin(Point()), end(Point()), color(Color(0, 0, 255)) {}
};
