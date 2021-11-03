#pragma once
#include <string>
#include <cmath>
#include "player.h"

//////////////////
//  CLASS MAZE  //
//////////////////
// # = wall
// . = nothing
// P = player initial location
class Maze
{
public:
	Maze()
	{
		str = L"";
		height = 0;
		width = 0;
		player.set_position(0, 0);
		wall_height = 2.f;
		
		camera_width = 0;
		camera_height = 0;
	}
	~Maze()
	{
	}

private:  // VARIABLES
	int height, width;
	std::wstring str;	// The maze itself
	Player player;
	int camera_width;
	int camera_height;
	float* wall_distance;
	float wall_height;
	int* wall_view_height;
	//wchar_t** screen;
	float max_dist = 10.f;
	bool* wall_corner;

public:  // SET
	void set(std::wstring maze_str, int w, int h, float px, float py, int cam_w, int cam_h)
	{
		str = maze_str;
		width = w;
		height = h;
		player.set_position(px, py);
		str[ind_str(int(px), int(py))] = 'P';

		camera_width = cam_w;
		camera_height = cam_h;
		wall_distance = new float[camera_width];
		wall_view_height = new int[camera_width];
		wall_corner = new bool[camera_width];

		//screen = &scr;
	}

public:  // GET
	std::wstring get() { return str; }
	int get_height() { return height; }
	int get_width() { return width; }

private:  // INDEX
	int ind_i(int I)
	{
		return I % width;
	}
	int ind_j(int I)
	{
		return I / width;
	}
	int ind_str(int i, int j)
	{
		return i + j * width;
	}
	int ind_cam(int i, int j)
	{
		return i + j * camera_width;
	}

public:  // Aquí comienza la fiesta
	void player_forward(float dist)
	{
		float a = player.get_camera_angle();
		float px = player.get_x();
		float py = player.get_y();

		float dx = dist * cos(a);
		float dy = dist * sin(a);

		str[player_ind()] = '.';

		player.set_position(px+dx, py+dy);
		if (player_out_of_bounds()) {
			player.set_position(px, py);
		}
		else if(player_collision())
		{
			player.set_position(px, py);
		}

		str[player_ind()] = 'P';
	}
	void player_turn(float theta)
	{
		float a = player.get_camera_angle();

		player.set_camera_angle(a + theta);
	}

	int player_ind()
	{
		float px = player.get_x();
		float py = player.get_y();

		return ind_str(int(px), int(py));
	}

	bool player_collision()
	{
		if (str[player_ind()] == '#')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool player_out_of_bounds()
	{
		float x = player.get_x();
		float y = player.get_y();
		if (x <= 0.f || x >= float(width) || y <= 0.f || y >= float(height))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// RAY CASTING
	float ray_cast(float x, float y, float theta, int& indx, int& indy)
	{
		float d = 0.01f;
		float dx = d*cos(theta);
		float dy = d*sin(theta);

		float total_dist = 0.f;
		while (true)
		{
			x += dx;
			y += dy;
			total_dist += d;
			if (int(x) > width-1 || int(x) < 0 || int(y) > height-1 || int(y) < 0)
			{
				indx = int(x);
				indy = int(y);
				return 10000;
			}
			else if (str[ind_str(int(x), int(y))] == '#')
			{
				indx = int(x);
				indy = int(y);
				return total_dist;
			}
		}
	}

	// FINAL RENDER
	wchar_t* get_player_view()
	{
		float a = player.get_camera_angle();
		float fov = player.get_FOV();
		float theta_0 = a - 0.5f * fov;
		float theta_f = a + 0.5f * fov;
		float Dtheta = fov / (camera_width - 1);
		float px = player.get_x();
		float py = player.get_y();

		float theta = theta_0;
		int former_indx, former_indy;
		for (int i=0; i<camera_width; i++)
		{
			int indx, indy;
			wall_distance[i] = ray_cast(px, py, theta, indx, indy) * cos(a-theta);
			wall_view_height[i] = int( (wall_height*camera_height) / (4.*wall_distance[i] * sin(0.5*player.get_vFOV())) );
			
			wall_corner[i] = false;
			if (i == 0)
			{
				former_indx = indx;
				former_indy = indy;
			}
			else if (former_indx != indx || former_indy != indy)
			{
				former_indx = indx;
				former_indy = indy;
				wall_corner[i] = true;
			}
			
			theta += Dtheta;
		}
		
		wchar_t* player_view = new wchar_t[camera_width * camera_height];
		for (int j = 0; j < camera_height; j++)
		{
			for (int i = 0; i < camera_width; i++)
			{
				if (j < 0.5 * camera_height - wall_view_height[i])
				{
					player_view[ind_cam(i, j)]= L' ';
				}
				else if (j < 0.5 * camera_height + wall_view_height[i])
				{
					if (wall_corner[i])								player_view[ind_cam(i, j)] = L' ';
					else if (wall_distance[i] < 0.25f * max_dist)	player_view[ind_cam(i, j)] = 0x2588;
					else if (wall_distance[i] < 0.5f * max_dist)	player_view[ind_cam(i, j)] = 0x2593;
					else if (wall_distance[i] < 0.75f * max_dist)	player_view[ind_cam(i, j)] = 0x2592;
					else if (wall_distance[i] < 1.f * max_dist)		player_view[ind_cam(i, j)] = 0x2591;
					else											player_view[ind_cam(i, j)] = L' ';
				}
				else
				{
					if (camera_height - j < 0.17 * camera_height)		player_view[ind_cam(i, j)] = L'x';
					else if (camera_height - j < 0.33 * camera_height)	player_view[ind_cam(i, j)] = L'.';
					else												player_view[ind_cam(i, j)] = L'-';
				}
			}
		}

		return player_view;
	}
};


std::wstring empty_maze(int width, int height)
{
	std::wstring str = L"";
	std::wstring str1 = L"";
	std::wstring str2 = L"#";

	str1.append(width, '#');

	str2.append(width - 2, '.');
	str2.append(L"#\n");

	str.append(str1);
	str.append(L"\n");
	for (int i = 1; i < height - 1; i++)
	{
		str.append(str2);
	}
	str.append(str1);

	return str;
}