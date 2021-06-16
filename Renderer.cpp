#include "Renderer.h"

#include <fstream>

/*
Przy reskalowaniu okna metody wxwidget często wyrzucają błędy - brak mozliwosci skalowania okna?
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

	// DC SETUP
	dc.Clear();
	dc.DestroyClippingRegion();
	dc.SetClippingRegion(pos_x, pos_y, size_x, size_y);
	dc.SetDeviceOrigin(pos_x + size_x / 2, pos_y + size_y / 2);

	// Data about figure
	std::vector<Segment> data = m_cfg->getData();

	// Axes
	DrawAxes(dc, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);

	// Choosing plane
	m_selected_plane = (PlaneID)m_cfg->getPlaneId(); // 1 - X axis plane, 2 - Y axis plane, 3 - Z axis plane

	// Drawing figure
	Vector4 start_point;
	Vector4 end_point;

	DrawPlane(dc, m_selected_plane, FIELD_OF_VIEW, X_ANGLE, Y_ANGLE, Z_ANGLE);

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

	UpdatePlanePos(dc, m_cfg->getV());
}

void Renderer::RenderCSection(wxDC* parentDC, int width, int height) {
	wxBufferedDC dc(parentDC);

	std::ofstream CSectionLog("csectionlog.txt");

	const int size_x = m_cfg->getSizeX();
	const int size_y = m_cfg->getSizeY();
	const int pos_x = m_cfg->getPosX();
	const int pos_y = m_cfg->getPosY();

	dc.Clear();
	dc.DestroyClippingRegion();
	dc.SetClippingRegion(pos_x, pos_y, size_x, size_y);
	dc.SetDeviceOrigin(pos_x + size_x / 2, pos_y + size_y / 2);
	dc.SetBrush(wxBrush(wxColor(0, 0, 255)));

	std::vector<Segment> data = m_cfg->getData();

	m_selected_plane = (PlaneID)m_cfg->getPlaneId();

	double pos = m_plane_pos;

	std::vector<Segment> cross_data;
	std::vector<wxPoint> cross_points;

	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].begin.y < pos && data[i].end.y > pos)
		{
			cross_data.push_back(data[i]);

			double tmp_x = cross_data[cross_data.size()-1].begin.x;
			double tmp_y = cross_data[cross_data.size() - 1].begin.y;
			double tmp_z = cross_data[cross_data.size() - 1].begin.z;

			cross_data[cross_data.size() - 1].begin.x = cross_data[cross_data.size() - 1].end.x;
			cross_data[cross_data.size() - 1].begin.y = cross_data[cross_data.size() - 1].end.y;
			cross_data[cross_data.size() - 1].begin.z = cross_data[cross_data.size() - 1].end.z;

			cross_data[cross_data.size() - 1].end.x = tmp_x;
			cross_data[cross_data.size() - 1].end.y = tmp_y;
			cross_data[cross_data.size() - 1].end.z = tmp_z;
		}

		if (data[i].end.y < pos && data[i].begin.y > pos)
		{
			cross_data.push_back(data[i]);
		}
	}

	dc.SetPen(wxColor(0, 0, 255));
	int text_pos = 10;
	if (m_cfg->isGeoLoaded())
	{
		if(!cross_data.empty())
		for (int i = 0; i < cross_data.size(); i++)
		{
			double y_distance_start = fabs(pos - cross_data[i].begin.y);
			double y_distance_end = fabs(pos - cross_data[i].end.y);
			double y_distance = y_distance_start + y_distance_end;

			// (dystans miedzy punktem pocz¹tkowym, a wartoci¹ y p³aszczyzny) / (dystans miêdzy dwoma punktami)
			double scale = y_distance_start / y_distance;
			// Odlegloæ miêdzy dwoma punktami (x, y, z)
			Vector4 v = Vector4(cross_data[i].end.x - cross_data[i].begin.x, cross_data[i].end.y - cross_data[i].begin.y, cross_data[i].end.z - cross_data[i].begin.z);
			// Przeskalowanie odleg³oci miêdzy dwoma punktami 
			v.Set(v.GetX() * scale, v.GetY() * scale, v.GetZ() * scale);

			// Dodanie przeskalowanej odleg³oci do punktu pocz¹tkowego
			Vector4 cross_point = Vector4(cross_data[i].begin.x + v.GetX(), cross_data[i].begin.y + v.GetY(), cross_data[i].begin.z + v.GetZ());

			cross_points.push_back(wxPoint(cross_point.GetX() * m_cfg->getSizeX(), cross_point.GetZ() * m_cfg->getSizeY()));

			//dc.DrawCircle(cross_point.GetX() * m_cfg->getSizeX(), cross_point.GetZ() * m_cfg->getSizeY(), 5);
			//text_pos += 50;
		}

		if (!cross_points.empty()) {
			for (int i = 0; i < 1; i++) {
				std::sort(cross_points.begin(), cross_points.end(), CompareOxAngle);
				dc.DrawPolygon(cross_points.size(), &cross_points[0]);
				//std::shuffle(cross_points.begin(), cross_points.end(), m_rng);
			}
		}
	}

	dc.SetPen(wxColor(0, 0, 0));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
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
	//to_return = CenterScreen(to_return);

	return to_return;
}

// Rysuje osie układu współrzędnych oraz informację w rogu panelu o ich kolorach.
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

void Renderer::DrawPlane(wxDC& dc, PlaneID id, double fov, double x_angle, double y_angle, double z_angle) {
	std::vector<Vector4> corners;
	switch (id) {
	case PlaneID::OYZ:
		corners = {
			Vector4(m_plane_pos, -1, -1),
			Vector4(m_plane_pos, 1, -1),
			Vector4(m_plane_pos, 1, 1),
			Vector4(m_plane_pos,-1, 1)
		};
		break;
	case PlaneID::OXZ:
		corners = {
			Vector4(-1, m_plane_pos, -1),
			Vector4(1, m_plane_pos, -1),
			Vector4(1, m_plane_pos, 1),
			Vector4(-1, m_plane_pos, 1)
		};
		break;
	case PlaneID::OXY:
		corners = {
			Vector4(-1, -1, m_plane_pos),
			Vector4(1, -1, m_plane_pos),
			Vector4(1, 1, m_plane_pos),
			Vector4(-1, 1, m_plane_pos)
		};
		break;
	}

	for (auto& vec : corners)
		vec = TransformVector(vec, fov, x_angle, y_angle, z_angle);

	for (int i = 0, j = 1; i < 4; ++i, j = ++j % 4) {
		dc.DrawLine(corners[i].GetX(), corners[i].GetY(), corners[j].GetX(), corners[j].GetY());
	}
}

// Przesuwa płaszczyznę wzdłuż jej wektora prędkości. Ponadto obsługuję zapisywanie animacji do plików.
void Renderer::UpdatePlanePos(wxBufferedDC& dc, float speed)
{
	m_plane_pos += (speed / 1000);

	if (m_saving)
	{		
		frame++;
		std::string path = "D:\\Test\\" + std::to_string(frame) + ".jpg";
		wxBitmap bitmap = dc.GetAsBitmap();
		wxImage image = bitmap.ConvertToImage();
		image.SaveFile(path, wxBITMAP_TYPE_JPEG);
	}

	if (m_plane_pos >= 2)
	{
		m_plane_pos = -2;

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
		m_plane_pos = -2;
		m_saving = true;
	}
}