#pragma once
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164

////////////////////
//  CLASS PLAYER  //
////////////////////
class Player
{
public:
	Player()
	{
		posx = 0;
		posy = 0;
		angle = 0;
		fov = 60. * PI / 180.;
		vfov = 60. * PI / 180.;
	}
	~Player()
	{
	}

private:
	float posx, posy, angle, fov, vfov;  // Positions, rotation angle and FOV

public:  // SET
	void set_position(float x, float y)
	{
		posx = x;
		posy = y;
	}
	void set_camera_angle(float theta)
	{
		angle = theta;
	}
	void set_FOV(float theta)
	{
		fov = theta;
	}

public:  // GET
	float get_x()
	{
		return posx;
	}
	float get_y()
	{
		return posy;
	}
	float get_camera_angle()
	{
		return angle;
	}
	float get_FOV()
	{
		return fov;
	}
	float get_vFOV()
	{
		return vfov;
	}
};