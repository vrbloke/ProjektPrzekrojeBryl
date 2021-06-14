#include "Renderer.h"

void Renderer::Render(wxDC* parentDC, int width, int height) {
	wxBufferedDC dc(parentDC);

	// Technical constants
	int sizeX = m_cfg->getSizeX();
	int sizeY = m_cfg->getSizeY();
	int posX = m_cfg->getPosX();
	int posY = m_cfg->getPosY();

	// DC SETUP
	dc.Clear();
	dc.DestroyClippingRegion();
	dc.SetClippingRegion(posX, posY, sizeX, sizeY);
	//dc.SetDeviceOrigin(posX + sizeX / 2, posY + sizeY / 2);

	std::vector<Segment> data = m_cfg->getData();

	// Tworzenie macierzy transformacji - tylko raz
	if (!m_matrix_set)
		SetTransformationMatrix(45, 45, 45);
		SetTransformationMatrixv2(sizeX, sizeY);
	
	DrawAxes(dc, sizeX, sizeY);

	//1 - oœ_x, 2 - oœ_y, 3 - oœ_z
	int plane = 0;

	// Rysowanie osi uk³adu wspó³rzêdnych

	Vector4 start_point;
	Vector4 end_point;

	if (m_cfg->isGeoLoaded())
	{
		for (int i = 0; i < data.size(); i++)
		{
			if (plane == 1)
			{
				if (data[i].begin.y < 0)
				{
					if(data[i].begin.x > plane_pos)
						data[i].begin.y = 0;
				}
				if (data[i].end.y < 0)
				{
					if (data[i].end.x > plane_pos)
						data[i].end.y = 0;
				}

				plane_pos -= m_cfg->getVx() / 1000000;
			}

			if (plane == 2)
			{
				if (data[i].begin.x < 0)
				{
					if (data[i].begin.y > plane_pos)
						data[i].begin.x = 0;
				}
				if (data[i].end.x < 0)
				{
					if (data[i].end.y > plane_pos)
						data[i].end.x = 0;
				}

				plane_pos -= m_cfg->getVy() / 1000000;
			}

			if (plane == 3)
			{
				if (data[i].begin.z < 0)
				{
					data[i].begin.z = 0;
				}
				
				if (data[i].begin.z < 0)
				{
					data[i].end.z = 0;
				}
				
				

				plane_pos -= m_cfg->getVz() / 1000000;
			}

			start_point.Set(data[i].begin.x, data[i].begin.y, data[i].begin.z);
			end_point.Set(data[i].end.x, data[i].end.y, data[i].end.z);

			/*
			start_point = m_transformation_matrix * start_point;
			end_point = m_transformation_matrix * end_point;

			start_point.Set(start_point.GetX() / abs(start_point.GetZ()) + 0.5, start_point.GetY() / abs(start_point.GetZ()) + 0.5, start_point.GetZ());
			end_point.Set(end_point.GetX() / abs(end_point.GetZ()) + 0.5, end_point.GetY() / abs(end_point.GetZ()) + 0.5, end_point.GetZ());
			

			dc.DrawLine(start_point.GetX() * sizeX, start_point.GetY() * sizeY, end_point.GetX() * sizeX, end_point.GetY() * sizeY);
			*/

			int FIELD_OF_VIEW = 45;

			double Z0 = (sizeX / 2.0) / tan((FIELD_OF_VIEW / 2.0) * 3.14159265 / 180.0);

			start_point = 200 * start_point;
			end_point = 200 * end_point;

			start_point = m_transformation_matrix * start_point;
			end_point = m_transformation_matrix * end_point;

			start_point = ApplyPerspective(start_point, Z0);
			end_point = ApplyPerspective(end_point, Z0);

			start_point = CenterScreen(start_point, sizeX, sizeY);
			end_point = CenterScreen(end_point, sizeX, sizeY);


			dc.DrawLine(start_point.GetX(), start_point.GetY(), end_point.GetX(), end_point.GetY());
		}
	}
}

void Renderer::SetTransformationMatrix(double angle_x, double angle_y, double angle_z)
{
	Matrix4 translation_matrix;
	translation_matrix.data[0][0] = translation_matrix.data[1][1] = translation_matrix.data[2][2] = translation_matrix.data[3][3] = 1.0;
	translation_matrix.data[0][3] = -1 / 50;
	translation_matrix.data[1][3] = 1 / 50;
	translation_matrix.data[2][3] = -1 / 50 + 3.0;

	Matrix4 rotation_x_matrix;
	rotation_x_matrix.data[0][0] = rotation_x_matrix.data[3][3] = 1.0;
	rotation_x_matrix.data[1][1] = cos(angle_x * M_PI / 180.0);
	rotation_x_matrix.data[1][2] = -sin(angle_x * M_PI / 180.0);
	rotation_x_matrix.data[2][1] = -rotation_x_matrix.data[1][2];
	rotation_x_matrix.data[2][2] = rotation_x_matrix.data[1][1];

	double y_angle = 0;
	Matrix4 rotation_y_matrix;
	rotation_y_matrix.data[1][1] = rotation_y_matrix.data[3][3] = 1.0;
	rotation_y_matrix.data[0][0] = cos(-angle_y * M_PI / 180.0);
	rotation_y_matrix.data[0][2] = sin(-angle_y * M_PI / 180.0);
	rotation_y_matrix.data[2][0] = -rotation_y_matrix.data[0][2];
	rotation_y_matrix.data[2][2] = rotation_y_matrix.data[0][0];

	double z_angle = 0;
	Matrix4 rotation_z_matrix;
	rotation_z_matrix.data[2][2] = rotation_z_matrix.data[3][3] = 1.0;
	rotation_z_matrix.data[0][0] = cos(angle_z * M_PI / 180.0);
	rotation_z_matrix.data[0][1] = -sin(angle_z * M_PI / 180.0);
	rotation_z_matrix.data[1][0] = -rotation_z_matrix.data[0][1];
	rotation_z_matrix.data[1][1] = rotation_z_matrix.data[0][0];

	Matrix4 rotation_matrix = rotation_x_matrix * rotation_y_matrix * rotation_z_matrix;
	
	m_transformation_matrix = translation_matrix * rotation_matrix;
	//m_transformation_matrix = translation_matrix;
	//m_transformation_matrix = rotation_matrix;
	m_matrix_set = true;
}

void Renderer::DrawAxes(wxBufferedDC& dc, int size_x, int size_y)
{
	//oœ x
	dc.SetPen(wxPen(wxColor(255, 0, 0)));
	dc.SetTextForeground(wxColor(255, 0, 0));

	Vector4 start_x, end_x;
	start_x.Set(-0.5, 0, 0);
	end_x.Set(0.5, 0, 0);
	start_x = m_transformation_matrix * start_x;
	end_x = m_transformation_matrix * end_x;
	start_x.Set(start_x.GetX() / abs(start_x.GetZ()) + 0.5, start_x.GetY() / abs(start_x.GetZ()) + 0.5, start_x.GetZ());
	end_x.Set(end_x.GetX() / abs(end_x.GetZ()) + 0.5, end_x.GetY() / abs(end_x.GetZ()) + 0.5, end_x.GetZ());
	dc.DrawLine(start_x.GetX() * size_x, start_x.GetY() * size_y, end_x.GetX() * size_x, end_x.GetY() * size_y);

	//strza³ka x
	start_x.Set(-0.4, 0.05, 0);
	end_x.Set(-0.5, 0, 0);
	start_x = m_transformation_matrix * start_x;
	end_x = m_transformation_matrix * end_x;
	start_x.Set(start_x.GetX() / abs(start_x.GetZ()) + 0.5, start_x.GetY() / abs(start_x.GetZ()) + 0.5, start_x.GetZ());
	end_x.Set(end_x.GetX() / abs(end_x.GetZ()) + 0.5, end_x.GetY() / abs(end_x.GetZ()) + 0.5, end_x.GetZ());
	dc.DrawLine(start_x.GetX() * size_x, start_x.GetY() * size_y, end_x.GetX() * size_x, end_x.GetY() * size_y);

	start_x.Set(-0.4, -0.05, 0);
	end_x.Set(-0.5, 0, 0);
	start_x = m_transformation_matrix * start_x;
	end_x = m_transformation_matrix * end_x;
	start_x.Set(start_x.GetX() / abs(start_x.GetZ()) + 0.5, start_x.GetY() / abs(start_x.GetZ()) + 0.5, start_x.GetZ());
	end_x.Set(end_x.GetX() / abs(end_x.GetZ()) + 0.5, end_x.GetY() / abs(end_x.GetZ()) + 0.5, end_x.GetZ());
	dc.DrawLine(start_x.GetX() * size_x, start_x.GetY() * size_y, end_x.GetX() * size_x, end_x.GetY() * size_y);
	
	Vector4 axe_x_name_pos;
	axe_x_name_pos.Set(-0.45, -0.15, 0);
	axe_x_name_pos = m_transformation_matrix * axe_x_name_pos;
	axe_x_name_pos.Set(axe_x_name_pos.GetX() / abs(axe_x_name_pos.GetZ()) + 0.5, axe_x_name_pos.GetY() / abs(axe_x_name_pos.GetZ()) + 0.5, axe_x_name_pos.GetZ());
	dc.DrawText("X", axe_x_name_pos.GetX() * size_x, axe_x_name_pos.GetY() * size_y);

	//oœ y
	dc.SetPen(wxPen(wxColor(0, 255, 0)));
	dc.SetTextForeground(wxColor(0, 255, 0));

	Vector4 start_y, end_y;
	start_y.Set(0, -0.5, 0);
	end_y.Set(0, 0.5, 0);
	start_y = m_transformation_matrix * start_y;
	end_y = m_transformation_matrix * end_y;
	start_y.Set(start_y.GetX() / abs(start_y.GetZ()) + 0.5, start_y.GetY() / abs(start_y.GetZ()) + 0.5, start_y.GetZ());
	end_y.Set(end_y.GetX() / abs(end_y.GetZ()) + 0.5, end_y.GetY() / abs(end_y.GetZ()) + 0.5, end_y.GetZ());
	dc.DrawLine(start_y.GetX() * size_x, start_y.GetY() * size_y, end_y.GetX() * size_x, end_y.GetY() * size_y);

	//strza³ka y
	start_y.Set(0.05, 0.4, 0);
	end_y.Set(0, 0.5, 0);
	start_y = m_transformation_matrix * start_y;
	end_y = m_transformation_matrix * end_y;
	start_y.Set(start_y.GetX() / abs(start_y.GetZ()) + 0.5, start_y.GetY() / abs(start_y.GetZ()) + 0.5, start_y.GetZ());
	end_y.Set(end_y.GetX() / abs(end_y.GetZ()) + 0.5, end_y.GetY() / abs(end_y.GetZ()) + 0.5, end_y.GetZ());
	dc.DrawLine(start_y.GetX() * size_x, start_y.GetY() * size_y, end_y.GetX() * size_x, end_y.GetY() * size_y);

	start_y.Set(-0.05, 0.4, 0);
	end_y.Set(0, 0.5, 0);
	start_y = m_transformation_matrix * start_y;
	end_y = m_transformation_matrix * end_y;
	start_y.Set(start_y.GetX() / abs(start_y.GetZ()) + 0.5, start_y.GetY() / abs(start_y.GetZ()) + 0.5, start_y.GetZ());
	end_y.Set(end_y.GetX() / abs(end_y.GetZ()) + 0.5, end_y.GetY() / abs(end_y.GetZ()) + 0.5, end_y.GetZ());
	dc.DrawLine(start_y.GetX() * size_x, start_y.GetY() * size_y, end_y.GetX() * size_x, end_y.GetY() * size_y);

	Vector4 axe_y_name_pos;
	axe_y_name_pos.Set(0.1, 0.45, 0);
	axe_y_name_pos = m_transformation_matrix * axe_y_name_pos;
	axe_y_name_pos.Set(axe_y_name_pos.GetX() / abs(axe_y_name_pos.GetZ()) + 0.5, axe_y_name_pos.GetY() / abs(axe_y_name_pos.GetZ()) + 0.5, axe_y_name_pos.GetZ());
	dc.DrawText("Y", axe_y_name_pos.GetX() * size_x, axe_y_name_pos.GetY() * size_y);

	//oœ z
	dc.SetPen(wxPen(wxColor(0, 0, 255)));
	dc.SetTextForeground(wxColor(0, 0, 255));

	Vector4 start_z, end_z;
	start_z.Set(0, 0, -0.5);
	end_z.Set(0, 0, 0.5);
	start_z = m_transformation_matrix * start_z;
	end_z = m_transformation_matrix * end_z;
	start_z.Set(start_z.GetX() / abs(start_z.GetZ()) + 0.5, start_z.GetY() / abs(start_z.GetZ()) + 0.5, start_z.GetZ());
	end_z.Set(end_z.GetX() / abs(end_z.GetZ()) + 0.5, end_z.GetY() / abs(end_z.GetZ()) + 0.5, end_z.GetZ());
	dc.DrawLine(start_z.GetX() * size_x, start_z.GetY() * size_y, end_z.GetX() * size_x, end_z.GetY() * size_y);

	//strza³ka z
	start_z.Set(0.05, 0, -0.45);
	end_z.Set(0, 0, -0.5);
	start_z = m_transformation_matrix * start_z;
	end_z = m_transformation_matrix * end_z;
	start_z.Set(start_z.GetX() / abs(start_z.GetZ()) + 0.5, start_z.GetY() / abs(start_z.GetZ()) + 0.5, start_z.GetZ());
	end_z.Set(end_z.GetX() / abs(end_z.GetZ()) + 0.5, end_z.GetY() / abs(end_z.GetZ()) + 0.5, end_z.GetZ());
	dc.DrawLine(start_z.GetX() * size_x, start_z.GetY() * size_y, end_z.GetX() * size_x, end_z.GetY() * size_y);

	start_z.Set(-0.05, 0, -0.45);
	end_z.Set(0, 0, -0.5);
	start_z = m_transformation_matrix * start_z;
	end_z = m_transformation_matrix * end_z;
	start_z.Set(start_z.GetX() / abs(start_z.GetZ()) + 0.5, start_z.GetY() / abs(start_z.GetZ()) + 0.5, start_z.GetZ());
	end_z.Set(end_z.GetX() / abs(end_z.GetZ()) + 0.5, end_z.GetY() / abs(end_z.GetZ()) + 0.5, end_z.GetZ());
	dc.DrawLine(start_z.GetX() * size_x, start_z.GetY() * size_y, end_z.GetX() * size_x, end_z.GetY() * size_y);

	Vector4 axe_z_name_pos;
	axe_z_name_pos.Set(0.1, 0, -0.45);
	axe_z_name_pos = m_transformation_matrix * axe_z_name_pos;
	axe_z_name_pos.Set(axe_z_name_pos.GetX() / abs(axe_z_name_pos.GetZ()) + 0.5, axe_z_name_pos.GetY() / abs(axe_z_name_pos.GetZ()) + 0.5, axe_z_name_pos.GetZ());
	dc.DrawText("Z", axe_z_name_pos.GetX() * size_x, axe_z_name_pos.GetY() * size_y);

	dc.SetPen(wxPen(wxColor(0, 0, 0)));
}

void Renderer::SetTransformationMatrixv2(int size_x, int size_y)
{
	

	Matrix4 tmp;
}

Vector4 Renderer::Translate(Vector4& original, Vector4& translation)
{
	Vector4 to_return;

	double r_x = original.GetX() + translation.GetX();
	double r_y = original.GetY() + translation.GetY();
	double r_z = original.GetZ() + translation.GetZ();

	to_return.Set(r_x, r_y, r_z);

	return to_return;
}

Vector4 Renderer::Rotate(Vector4& original, Vector4& rotation)
{
	Vector4 to_return;

	double r_x = original.GetX() * (cos(rotation.GetZ()) * cos(rotation.GetY()))+ 
		original.GetY() * (cos(rotation.GetZ()) * sin(rotation.GetY()) * sin(rotation.GetX()) - sin(1)) +
		original.GetZ() * (cos(rotation.GetZ()) * sin(rotation.GetY()) * cos(rotation.GetX()) + sin(1));

	double r_y = original.GetX() * (sin(rotation.GetZ()) * cos(rotation.GetY()))+ 
		original.GetY() * (sin(rotation.GetZ()) * sin(rotation.GetY()) * sin(rotation.GetX()) + cos(0)) +
		original.GetZ() * (sin(rotation.GetZ()) * sin(rotation.GetY()) * cos(rotation.GetX()) - cos(0));

	double r_z = original.GetX() * (-sin(rotation.GetY())) + 
		original.GetY() * (cos(rotation.GetY()) * sin(rotation.GetX())) + 
		original.GetZ() * (cos(rotation.GetY()) * cos(rotation.GetY()));

	to_return.Set(r_x, r_y, r_z);

	return to_return;
}

Vector4 Renderer::ApplyPerspective(Vector4& original, double z0)
{
	Vector4 to_return;

	double r_x = original.GetX() * z0 / (z0 + original.GetZ());
	double r_y = original.GetY() * z0 / (z0 + original.GetZ());;
	double r_z = original.GetZ();

	to_return.Set(r_x, r_y, r_z);

	return to_return;
}

Vector4 Renderer::CenterScreen(Vector4& original, int size_x, int size_y)
{
	Vector4 to_return;

	double r_x = original.GetX() + size_x / 2;
	double r_y = original.GetY() + size_y / 2;
	double r_z = original.GetZ();

	to_return.Set(r_x, r_y, r_z);

	return to_return;
}