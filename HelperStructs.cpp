#include "HelperStructs.h"

Matrix4 Matrix4::operator* (const Matrix4& rhs)
{
  int i, j, k;
  Matrix4 tmp;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    {
      tmp.data[i][j] = 0.0;
      for (k = 0; k < 4; k++)
        tmp.data[i][j] = tmp.data[i][j] + (data[i][k] * rhs.data[k][j]);
    }
  return tmp;
}

Vector4 operator* (const Vector4& gVector, const Matrix4& gMatrix)
{
  unsigned int i, j;
  Vector4 tmp;

  for (i = 0; i < 4; i++)
  {
    tmp.data[i] = 0.0;
    for (j = 0; j < 4; j++) tmp.data[i] = tmp.data[i] + (gMatrix.data[i][j] * gVector.data[j]);
  }
  return tmp;
}

Vector4 Matrix4::operator* (const Vector4& gVector)
{
  unsigned int i, j;
  Vector4 tmp;

  for (i = 0; i < 4; i++)
  {
    tmp.data[i] = 0.0;
    for (j = 0; j < 4; j++) tmp.data[i] = tmp.data[i] + (data[i][j] * gVector.data[j]);
  }
  return tmp;
}

double Dot(const Vector4& lhs, const Vector4& rhs) {
  return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY() + lhs.GetZ() * rhs.GetZ();
}

Vector4 Cross(const Vector4& lhs, const Vector4& rhs) {
  return Vector4(
    lhs.GetY() * rhs.GetZ() - lhs.GetZ() * rhs.GetY(),
    lhs.GetZ() * rhs.GetX() - lhs.GetX() * rhs.GetZ(),
    lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX()
  );
}

double Magnitude(const Vector4& org) {
  return sqrt(org.GetX() * org.GetX() + org.GetY() * org.GetY() + org.GetZ() * org.GetZ());
}

Vector4 Normalize(const Vector4& org) {
  double len = Magnitude(org);
  if (len == 0) return Vector4();
  return Vector4(org.GetX() / len, org.GetY() / len, org.GetZ() / len);
}

Vector4 FindPerpendicular(const Vector4& p, const double a, const double b, const char nonzero) {
  switch (nonzero) {
  case 'x':
    return Normalize(Vector4((-a * p.GetY() - b * p.GetZ()) / p.GetX(), a, b));
    break;
  case 'y':
    return Normalize(Vector4(a, (-a * p.GetX() - b * p.GetZ()) / p.GetY(), b));
    break;
  case 'z':
    return Normalize(Vector4(a, b, (-a * p.GetX() - b * p.GetY()) / p.GetZ()));
    break;
  default:
    throw std::runtime_error("Invalid nonzero vector identifier");
  }
}

std::vector<Vector4> FindBasis(const Vector4& p) {
  Vector4 v1 = FindPerpendicular(p, 1.0, 0.0, p.GetX() != 0 ? 'x' : p.GetY() != 0 ? 'y' : 'z');
  return std::vector<Vector4>({ v1, Normalize(Cross(p, v1)) });
}

Matrix4 TransformPlane(const Vector4& p, const Vector4& v, const double dist) {
  auto basis = FindBasis(p);
  Vector4 vnorm = Normalize(v);
  vnorm = Vector4(0, 1, 0);
  Vector4 v1 = basis[0];
  Vector4 v2 = basis[1];
  Matrix4 T;
  T.data[0][0] = v1.GetX(); T.data[0][1] = v2.GetX(); T.data[0][2] = 0; T.data[0][3] = vnorm.GetX()*dist; //p.GetX() + (v * (-1 + dist)).GetX();
  T.data[1][0] = v1.GetY(); T.data[1][1] = v2.GetY(); T.data[1][2] = 0; T.data[1][3] = vnorm.GetY()*dist;
  T.data[2][0] = v1.GetZ(); T.data[2][1] = v2.GetZ(); T.data[2][2] = 1; T.data[2][3] = vnorm.GetZ()*dist;
  T.data[3][0] = 0;					T.data[3][1] = 0;					T.data[3][2] = 0; T.data[3][3] = 1;
  return T;
}

Matrix4 Project(double n, double f, double t, double b, double l, double r) {
  Matrix4 T;
  T.data[0][0] = 2 * n / (r - l); T.data[0][1] = 0; T.data[0][2] = (r + l) / (r - l); T.data[0][3] = 0;
  T.data[1][0] = 0; T.data[1][1] = 2 * n / (t - b); T.data[1][2] = (t + b) / (t - b); T.data[1][3] = 0;
  T.data[2][0] = 0; T.data[2][1] = 0; T.data[2][2] = -(f + n) / (f - n); T.data[2][3] = -2 * f * n / (f - n);
  T.data[3][0] = 0; T.data[3][1] = 0; T.data[3][2] = -1; T.data[3][3] = 0;
  return T;
}