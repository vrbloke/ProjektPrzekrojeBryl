#include "Renderer.h"

/*
Przy reskalowaniu okna metody wxwidget czêsto wyrzucaj¹ b³êdy - brak mozliwosci skalowania okna?
*/

void Renderer::Render(wxDC* parentDC, int width, int height) {
	wxBufferedDC dc(parentDC);

	// Technical constants
	const int size_x = m_cfg->getSizeX();
	const int size_y = m_cfg->getSizeY();
	const int pos_x = m_cfg->getPosX();
	const int pos_y = m_cfg->getPosY();

	// Perspective constants
	const double FIELD_OF_VIEW = 45;
	const double X_ANGLE = 10;
	const double Y_ANGLE = 10;
	const double Z_ANGLE = 10;

	// Model constants
	const double vx = m_cfg->getVx();
	const double vy = m_cfg->getVy();
	const double vz = m_cfg->getVz();

	// DC SETUP
	dc.Clear();
	dc.DestroyClippingRegion();
	dc.SetClippingRegion(pos_x, pos_y, size_x, size_y);

	// Data about figure
	std::vector<Segment> data = m_cfg->getData();

	// Axes
	DrawAxes(dc, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);

	// Temporary - chosing plane
	int axis_plane = 2; // 1 - X axis plane, 2 - Y axis plane, 3 - Z axis plane

	// Testowanie algorytmów
	//Test(data);
	//Test2(data, axis_plane);
	//CogenSutherland(data);

	// Drawing figure
	Vector4 start_point;
	Vector4 end_point;

	switch (axis_plane)
	{
	case 1:
		DrawXPlane(dc, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);
		break;
	case 2:
		DrawYPlane(dc, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);
		break;
	case 3:
		DrawZPlane(dc, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);
		break;
	default:
		break;
	}

	if (m_cfg->isGeoLoaded())
	{
		for (int i = 0; i < data.size(); i++)
		{	
			start_point.Set(data[i].begin.x, data[i].begin.y, data[i].begin.z);
			end_point.Set(data[i].end.x, data[i].end.y, data[i].end.z);

			start_point = TransformVector(start_point, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);
			end_point = TransformVector(end_point, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);
			
			dc.DrawLine(start_point.GetX(), start_point.GetY(), end_point.GetX(), end_point.GetY());
		}
	}

	UpdatePlanePos(dc, sqrt(vx*vx + vy*vy + vz*vz));
}

Vector4 Renderer::Scale(Vector4& original)
{
	Vector4 to_return;

	double x = original.GetX() * (m_cfg->getSizeX() / 2.0);
	double y = original.GetY() * (m_cfg->getSizeY() / 2.0);
	double z = original.GetZ() * (m_cfg->getSizeX() / 2.0);

	to_return.Set(x, y, z);

	return to_return;
}

Vector4 Renderer::Rotate(Vector4& original, double x_angle, double y_angle, double z_angle)
{
	Vector4 to_return;

	if(!m_matrix_set)
	{
		Matrix4 rot_x_matrix;
		x_angle = x_angle + 180; // Normalize to clockwise coordinate system
		rot_x_matrix.data[0][0] = rot_x_matrix.data[3][3] = 1.0;
		rot_x_matrix.data[1][1] = cos(x_angle * M_PI / 180);
		rot_x_matrix.data[1][2] = -sin(x_angle * M_PI / 180);
		rot_x_matrix.data[2][1] = sin(x_angle * M_PI / 180);
		rot_x_matrix.data[2][2] = cos(x_angle * M_PI / 180);

		Matrix4 rot_y_matrix;
		rot_y_matrix.data[1][1] = rot_y_matrix.data[3][3] = 1.0;
		rot_y_matrix.data[0][0] = cos(y_angle * M_PI / 180.0);
		rot_y_matrix.data[0][2] = sin(y_angle * M_PI / 180.0);
		rot_y_matrix.data[2][0] = -sin(y_angle * M_PI / 180.0);
		rot_y_matrix.data[2][2] = cos(y_angle * M_PI / 180.0);

		Matrix4 rot_z_matrix;
		rot_z_matrix.data[2][2] = rot_z_matrix.data[3][3] = 1.0;
		rot_z_matrix.data[0][0] = cos(z_angle * M_PI / 180.0);
		rot_z_matrix.data[0][1] = -sin(z_angle * M_PI / 180.0);
		rot_z_matrix.data[1][0] = sin(z_angle * M_PI / 180.0);
		rot_z_matrix.data[1][1] = cos(z_angle * M_PI / 180.0);

		
		m_rotation_matrix = rot_x_matrix * rot_y_matrix * rot_z_matrix;
		m_matrix_set = true;
	}
	
	to_return.Set(original.GetX(), original.GetY(), original.GetZ());
	to_return = m_rotation_matrix * to_return;

	return to_return;
}

Vector4 Renderer::ApplyPerspective(Vector4& original, double fov)
{
	Vector4 to_return;

	double Z0 = (m_cfg->getSizeX() / 2.0) / tan((fov / 2.0) * 3.14159265 / 180.0);

	double x = original.GetX() * Z0 / (original.GetZ() + Z0);
	double y = original.GetY() * Z0 / (original.GetZ() + Z0);
	double z = original.GetZ();

	to_return.Set(x, y, z);

	return to_return;
}

Vector4 Renderer::CenterScreen(Vector4& original)
{
	Vector4 to_return;
	double x = original.GetX() + (m_cfg->getSizeX() / 2.0);
	double y = original.GetY() + (m_cfg->getSizeY() / 2.0);
	double z = original.GetZ();

	to_return.Set(x, y, z);

	return to_return;
}

Vector4 Renderer::TransformVector(Vector4& original, double fov, double x_angle, double y_angle, double z_angle)
{
	Vector4 to_return(original.GetX(), original.GetY(), original.GetZ());

	to_return = Scale(to_return);
	to_return = Rotate(to_return, x_angle, y_angle, z_angle);
	to_return = ApplyPerspective(to_return, fov);
	to_return = CenterScreen(to_return);

	return to_return;
}

void Renderer::DrawAxes(wxBufferedDC& dc, double fov, double x_angle, double y_angle, double z_angle)
{
	// Drawing X axis
	dc.SetPen(wxColor(255, 0, 0));
	Vector4 start_x(-1, 0, 0);
	Vector4 end_x(1, 0, 0);

	start_x = TransformVector(start_x, fov, x_angle, y_angle, z_angle);
	end_x = TransformVector(end_x, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(start_x.GetX(), start_x.GetY(), end_x.GetX(), end_x.GetY());

	// Drawing arrow for x axis
	start_x.Set(1, 0, 0);
	end_x.Set(0.9, 0.1, 0);
	start_x = TransformVector(start_x, fov, x_angle, y_angle, z_angle);
	end_x = TransformVector(end_x, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_x.GetX(), start_x.GetY(), end_x.GetX(), end_x.GetY());

	start_x.Set(1, 0, 0);
	end_x.Set(0.9, -0.1, 0);
	start_x = TransformVector(start_x, fov, x_angle, y_angle, z_angle);
	end_x = TransformVector(end_x, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_x.GetX(), start_x.GetY(), end_x.GetX(), end_x.GetY());

	start_x.Set(1, 0, 0);
	end_x.Set(0.9, 0, 0.1);
	start_x = TransformVector(start_x, fov, x_angle, y_angle, z_angle);
	end_x = TransformVector(end_x, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_x.GetX(), start_x.GetY(), end_x.GetX(), end_x.GetY());

	start_x.Set(1, 0, 0);
	end_x.Set(0.9, 0, -0.1);
	start_x = TransformVector(start_x, fov, x_angle, y_angle, z_angle);
	end_x = TransformVector(end_x, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_x.GetX(), start_x.GetY(), end_x.GetX(), end_x.GetY());

	// Drawing Y axe
	dc.SetPen(wxColor(0, 255, 0));
	Vector4 start_y(0, -1, 0);
	Vector4 end_y(0, 1, 0);

	start_y = TransformVector(start_y, fov, x_angle, y_angle, z_angle);
	end_y = TransformVector(end_y, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(start_y.GetX(), start_y.GetY(), end_y.GetX(), end_y.GetY());

	// Drawing arrow for y axis
	start_y.Set(0, 1, 0);
	end_y.Set(0.1, 0.9, 0);
	start_y = TransformVector(start_y, fov, x_angle, y_angle, z_angle);
	end_y = TransformVector(end_y, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_y.GetX(), start_y.GetY(), end_y.GetX(), end_y.GetY());

	start_y.Set(0, 1, 0);
	end_y.Set(-0.1, 0.9, 0);
	start_y = TransformVector(start_y, fov, x_angle, y_angle, z_angle);
	end_y = TransformVector(end_y, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_y.GetX(), start_y.GetY(), end_y.GetX(), end_y.GetY());

	start_y.Set(0, 1, 0);
	end_y.Set(0, 0.9, 0.1);
	start_y = TransformVector(start_y, fov, x_angle, y_angle, z_angle);
	end_y = TransformVector(end_y, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_y.GetX(), start_y.GetY(), end_y.GetX(), end_y.GetY());

	start_y.Set(0, 1, 0);
	end_y.Set(0, 0.9, -0.1);
	start_y = TransformVector(start_y, fov, x_angle, y_angle, z_angle);
	end_y = TransformVector(end_y, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_y.GetX(), start_y.GetY(), end_y.GetX(), end_y.GetY());

	// Drawing Z axe
	dc.SetPen(wxColor(0, 0, 255));
	Vector4 start_z(0, 0, -1);
	Vector4 end_z(0, 0, 1);

	start_z = TransformVector(start_z, fov, x_angle, y_angle, z_angle);
	end_z = TransformVector(end_z, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(start_z.GetX(), start_z.GetY(), end_z.GetX(), end_z.GetY());

	// Drawing arrow for Z axis
	start_z.Set(0, 0, 1);
	end_z.Set(0.1, 0, 0.9);
	start_z = TransformVector(start_z, fov, x_angle, y_angle, z_angle);
	end_z = TransformVector(end_z, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_z.GetX(), start_z.GetY(), end_z.GetX(), end_z.GetY());

	start_z.Set(0, 0, 1);
	end_z.Set(-0.1, 0, 0.9);
	start_z = TransformVector(start_z, fov, x_angle, y_angle, z_angle);
	end_z = TransformVector(end_z, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_z.GetX(), start_z.GetY(), end_z.GetX(), end_z.GetY());

	start_z.Set(0, 0, 1);
	end_z.Set(0, 0.1, 0.9);
	start_z = TransformVector(start_z, fov, x_angle, y_angle, z_angle);
	end_z = TransformVector(end_z, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_z.GetX(), start_z.GetY(), end_z.GetX(), end_z.GetY());

	start_z.Set(0, 0, 1);
	end_z .Set(0, -0.1, 0.9);
	start_z = TransformVector(start_z, fov, x_angle, y_angle, z_angle);
	end_z = TransformVector(end_z, fov, x_angle, y_angle, z_angle);
	dc.DrawLine(start_z.GetX(), start_z.GetY(), end_z.GetX(), end_z.GetY());

	// Axes Description
	dc.SetTextForeground(wxColor(255, 0, 0));
	dc.DrawText("X", wxPoint(10, m_cfg->getSizeY() - 20));

	dc.SetTextForeground(wxColor(0, 255, 0));
	dc.DrawText("Y", wxPoint(20, m_cfg->getSizeY() - 20));

	dc.SetTextForeground(wxColor(0, 0, 255));
	dc.DrawText("Z", wxPoint(30, m_cfg->getSizeY() - 20));

	// Default text and pen color
	dc.SetPen(wxColor(0, 0, 0));
	dc.SetTextForeground(wxColor(0, 0, 0));
}

void Renderer::DrawXPlane(wxBufferedDC& dc, double fov, double x_angle, double y_angle, double z_angle)
{
	double pos = -1.0 + (2 * m_cfg->getPos() / 100);
	Vector4 top_left(-1 + m_plane_start_pos, pos, -1);
	Vector4 top_right(1 + m_plane_start_pos, pos, -1);
	Vector4 bottom_right(1 + m_plane_start_pos, pos, 1);
	Vector4 bottom_left(-1 + m_plane_start_pos, pos, 1);

	top_left = TransformVector(top_left, fov, x_angle, y_angle, z_angle);
	top_right = TransformVector(top_right, fov, x_angle, y_angle, z_angle);
	bottom_right = TransformVector(bottom_right, fov, x_angle, y_angle, z_angle);
	bottom_left = TransformVector(bottom_left, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(top_left.GetX(), top_left.GetY(), top_right.GetX(), top_right.GetY());
	dc.DrawLine(top_right.GetX(), top_right.GetY(), bottom_right.GetX(), bottom_right.GetY());
	dc.DrawLine(bottom_right.GetX(), bottom_right.GetY(), bottom_left.GetX(), bottom_left.GetY());
	dc.DrawLine(bottom_left.GetX(), bottom_left.GetY(), top_left.GetX(), top_left.GetY());

	//UpdatePlanePos(dc, m_cfg->getVx());
}

void Renderer::DrawYPlane(wxBufferedDC& dc, double fov, double x_angle, double y_angle, double z_angle)
{
	double pos = -1.0 + (2 * m_cfg->getPos() / 100);
	Vector4 top_left(-1, 1 + m_plane_start_pos, pos);
	Vector4 top_right(1, 1 + m_plane_start_pos, pos);
	Vector4 bottom_right(1, -1 + m_plane_start_pos, pos);
	Vector4 bottom_left(-1, -1 + m_plane_start_pos, pos);

	top_left = TransformVector(top_left, fov, x_angle, y_angle, z_angle);
	top_right = TransformVector(top_right, fov, x_angle, y_angle, z_angle);
	bottom_right = TransformVector(bottom_right, fov, x_angle, y_angle, z_angle);
	bottom_left = TransformVector(bottom_left, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(top_left.GetX(), top_left.GetY(), top_right.GetX(), top_right.GetY());
	dc.DrawLine(top_right.GetX(), top_right.GetY(), bottom_right.GetX(), bottom_right.GetY());
	dc.DrawLine(bottom_right.GetX(), bottom_right.GetY(), bottom_left.GetX(), bottom_left.GetY());
	dc.DrawLine(bottom_left.GetX(), bottom_left.GetY(), top_left.GetX(), top_left.GetY());

	//UpdatePlanePos(dc, m_cfg->getVy());
}

void Renderer::DrawZPlane(wxBufferedDC& dc, double fov, double x_angle, double y_angle, double z_angle)
{
	double pos = -1.0 + (2 * m_cfg->getPos() / 100);
	Vector4 top_left(-1, pos, -1 + m_plane_start_pos);
	Vector4 top_right(1, pos, -1 + m_plane_start_pos);
	Vector4 bottom_right(1, pos, 1 + m_plane_start_pos);
	Vector4 bottom_left(-1, pos, 1 + m_plane_start_pos);

	top_left = TransformVector(top_left, fov, x_angle, y_angle, z_angle);
	top_right = TransformVector(top_right, fov, x_angle, y_angle, z_angle);
	bottom_right = TransformVector(bottom_right, fov, x_angle, y_angle, z_angle);
	bottom_left = TransformVector(bottom_left, fov, x_angle, y_angle, z_angle);

	dc.DrawLine(top_left.GetX(), top_left.GetY(), top_right.GetX(), top_right.GetY());
	dc.DrawLine(top_right.GetX(), top_right.GetY(), bottom_right.GetX(), bottom_right.GetY());
	dc.DrawLine(bottom_right.GetX(), bottom_right.GetY(), bottom_left.GetX(), bottom_left.GetY());
	dc.DrawLine(bottom_left.GetX(), bottom_left.GetY(), top_left.GetX(), top_left.GetY());

	//UpdatePlanePos(dc, m_cfg->getVz());
}

void Renderer::UpdatePlanePos(wxBufferedDC& dc, float speed)
{
	m_plane_start_pos += (speed / 1000);

	if (m_saving)
	{		
		frame++;
		std::string path = "D:\\Test\\" + std::to_string(frame) + ".jpg";
		wxBitmap bitmap = dc.GetAsBitmap();
		wxImage image = bitmap.ConvertToImage();
		image.SaveFile(path, wxBITMAP_TYPE_JPEG);
	}

	if (m_plane_start_pos >= 2)
	{
		m_plane_start_pos = -2;

		if (m_cfg->isSaveToFile() && m_saving)
		{
			m_cfg->setSaveToFile(false);
			m_saving = false;
			frame = 0;
		}
	}
	else if (m_cfg->isSaveToFile() && m_saving && frame > 300) {
		m_cfg->setSaveToFile(false);
		m_saving = false;
		frame = 0;
	}
		
	
	if (m_cfg->isSaveToFile() && !m_saving)
	{
		m_plane_start_pos = -2;
		m_saving = true;
	}
}

void Renderer::Test(std::vector<Segment>& data)
{
	int plane_pos_x = 0;
	int plane_pos_y = 0;
	int plane_pos_z = 0;

	for (int i = 0; i < data.size(); i++)
	{
		Vector4 v;
		v.Set(data[i].begin.x - data[i].end.x, data[i].begin.y - data[i].end.y, data[i].begin.z - data[i].end.z);

		double x = data[i].begin.x;
		double x_lambda = v.GetX();

		double y = data[i].begin.y;
		double y_lambda = v.GetY();

		double z = data[i].begin.z;
		double z_lambda = v.GetZ();

		double fp = (plane_pos_x - x) / 2 + (plane_pos_y -y) / 2 + (plane_pos_x  - z) / 2;

		if (fp == 0)
		{
			if (data[i].begin.x > 0)
				data[i].begin.x = plane_pos_x;

			if (data[i].end.x < 0)
				data[i].end.x = plane_pos_x;
		}
	}
}

void Renderer::Test2(std::vector<Segment>& data, int axis_plane)
{
	for (int i = 0; i < data.size(); i++)
	{
		switch (axis_plane)
		{
		case 1:
			if (data[i].begin.y > 0 || data[i].end.y > 0)
			{
				if (data[i].begin.x < m_plane_start_pos + 1.0)
					data[i].begin.y = 0;
				if (data[i].end.x < m_plane_start_pos + 1.0)
					data[i].end.y = 0;
			}
			break;
		case 2:
			if (data[i].begin.z > 0 || data[i].end.z > 0)
			{
				if (data[i].begin.y < m_plane_start_pos + 1.0)
					data[i].begin.z = 0;
				if (data[i].end.y < m_plane_start_pos + 1.0)
					data[i].end.z = 0;
			}
			break;
		case 3:
			if (data[i].begin.y > 0 || data[i].end.y > 0)
			{
				if (data[i].begin.z < m_plane_start_pos + 1.0)
					data[i].begin.y = 0;
				if (data[i].end.z < m_plane_start_pos + 1.0)
					data[i].end.y = 0;
			}
			break;
		default:
			break;
		}
	}
}

void Renderer::Test3(std::vector<Segment>& data)
{
	/*
	Steps:
		calculate the centroid Z = (A + B + C + ...) / numPoints
		calculate the normal n = AB cross BC
		get the Vector from the centroid to the first point : ZA
		order all points P by the signed angle ZA to ZP with normal n
		(signed angle == angle(ZA, ZP) * sign(n dot(ZA cross ZP
	*/
	double centroid_x = 0;
	double centroid_y = 0;
	double centroid_z = 0;

	for (int i = 0; i < data.size(); i++)
	{
		centroid_x += data[i].begin.x + data[i].end.x;
		centroid_y += data[i].begin.y + data[i].end.y;
		centroid_z += data[i].begin.z + data[i].end.z;

		double dx = data[i].begin.x - data[i].end.x;
		double dy = data[i].begin.y - data[i].end.y;
		double dz = data[i].begin.z - data[i].end.z;
	}
	centroid_x = centroid_x / (data.size() * 2);
	centroid_y = centroid_y / (data.size() * 2);
	centroid_y = centroid_z / (data.size() * 2);
}

void Renderer::Test4(std::vector<Segment>& data)
{
	/*
	* Active C as a fron clipping plane
	* For all pixels do Z=0, I =-, Mp = 0
	* For each solid S do:
	*	Render all the daces of S togglin Mp
	*	Deactivate C
	*	Shade C and reset Mp where Mp == 1
	* https://www.cc.gatech.edu/~jarek/papers/Capping.pdf
	*/
}

void Renderer::CogenSutherland(std::vector<Segment>& data)
{
	//https://www.cs.drexel.edu/~david/Classes/CS430/Lectures/L-14_Color.6.pdf
}