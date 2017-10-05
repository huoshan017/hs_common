#include "hs_2d.h"
#include <math.h>

namespace hs2d {
	
	bool Translate(struct Vector* v, float x, float y)
	{
		if (!v)
			return false;

		v->x += x;
		v->y += y;

		return true;
	}

	bool Scale(struct Vector* v, float xp, float yp, float sx, float sy)
	{
		if (!v)
			return false;

		v->x -= xp;
		v->y -= yp;

		v->x *= sx;
		v->y *= sy;

		v->x += xp;
		v->y += yp;

		return true;
	}

	// seta为角度
	bool Rotate(struct Vector* v, float xp, float yp, float radian)
	{
		if (!v)
			return false;

		v->x -= xp;
		v->y -= yp;

		v->x = v->x*cos(radian) - v->y*sin(radian);
		v->y = v->y*cos(radian) + v->x*sin(radian);

		v->x += xp;
		v->y += yp;

		return true;
	}

	bool Rotate2(struct Vector* v, float xp, float yp, float angle)
	{
		float radian = PI * angle / 180;
		return Rotate(v, xp, yp, radian);
	}

	/* 局部坐标变换 */
	bool TranslateLocal(struct Vector* v, float x, float y)
	{
		return Translate(v, x, y);
	}

	bool ScaleLocal(struct Vector* v, float sx, float sy)
	{
		return Scale(v, 0.f, 0.f, sx, sy);
	}

	bool RotateLocal(struct Vector* v, float radian)
	{
		return Rotate(v, 0.f, 0.f, radian);
	}

	bool RotateLocal2(struct Vector* v, float angle)
	{
		return Rotate2(v, 0.f, 0.f, angle);
	}

	/* 局部坐标(包含惯性坐标系)到世界坐标 */
	bool Local2World(struct Vector* v, float x, float y, float radian, float sx, float sy)
	{
		bool res = RotateLocal(v, radian);
		if (!res)
			return false;
		
		res = ScaleLocal(v, sx, sy);
		if (!res)
			return false;

		res = Translate(v, x, y);
		return res;
	}
	bool Local2World2(struct Vector* v, float x, float y, float angle, float sx, float sy)
	{
		bool res = RotateLocal2(v, angle);
		if (!res)
			return false;

		res = ScaleLocal(v, sx, sy);
		if (!res)
			return false;

		res = Translate(v, x, y);
		return res;
	}

	/* 世界坐标系到局部坐标系 */
	bool World2Local(struct Vector* v, float x, float y, float radian, float sx, float sy)
	{
		bool res = Translate(v, -x, -y);
		if (!res)
			return false;

		res = RotateLocal(v, -radian);
		if (!res)
			return false;

		res = ScaleLocal(v, 1/sx, 1/sy);
		return res;
	}
	bool World2Local2(struct Vector* v, float x, float y, float angle, float sx, float sy)
	{
		bool res = Translate(v, -x, -y);
		if (!res)
			return false;

		res = RotateLocal(v, -angle);
		if (!res)
			return false;

		res = ScaleLocal(v, 1/sx, 1/sy);
		return res;
	}

	/* 世界坐标到眼睛坐标 */
	void World2Camera(struct Vector* v, float x, float y, float rotate_radian)
	{
		Translate(v, -x, -y);
		RotateLocal(v, -rotate_radian);
	}

	void World2Camera2(struct Vector* v, float x, float y, float rotate_angle)
	{
		Translate(v, -x, -y);
		RotateLocal2(v, -rotate_angle);
	}

	/* 摄像机坐标到投影坐标（x(-1~1) y(-1~1)） */
	void Perspective(struct Vector* v, float fov, float aspect, float view_distance)
	{
		// 焦距
		float r = 1.0f / tan(fov/2);
		v->x = v->x * r / view_distance;
		v->y = v->y * r / view_distance / aspect;
	}

	/* 投影坐标到屏幕坐标 （可以把屏幕坐标系看成特殊的局部坐标系）*/
	void Viewport(struct Vector* v, int x, int y, int width, int height)
	{
		v->x = (v->x-(-1.0f)) * (width/(1.0f-(-1.0f)));

		float aspect = (float)height / (float)width;
		v->y = (v->y*aspect -(-aspect)) * (height/(aspect-(-aspect)));

		v->x += x;
		v->y += y;
	}
}