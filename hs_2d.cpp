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

	// setaΪ�Ƕ�
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

	/* �ֲ�����任 */
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

	/* �ֲ�����(������������ϵ)���������� */
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

	/* ��������ϵ���ֲ�����ϵ */
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

	/* �������굽�۾����� */
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

	/* ��������굽ͶӰ���꣨x(-1~1) y(-1~1)�� */
	void Perspective(struct Vector* v, float fov, float aspect, float view_distance)
	{
		// ����
		float r = 1.0f / tan(fov/2);
		v->x = v->x * r / view_distance;
		v->y = v->y * r / view_distance / aspect;
	}

	/* ͶӰ���굽��Ļ���� �����԰���Ļ����ϵ��������ľֲ�����ϵ��*/
	void Viewport(struct Vector* v, int x, int y, int width, int height)
	{
		v->x = (v->x-(-1.0f)) * (width/(1.0f-(-1.0f)));

		float aspect = (float)height / (float)width;
		v->y = (v->y*aspect -(-aspect)) * (height/(aspect-(-aspect)));

		v->x += x;
		v->y += y;
	}
}