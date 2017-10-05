#ifndef __HS_2D_H__
#define __HS_2D_H__

#include <vector>
#include <list>

using namespace std;

namespace hs2d {

	const float PI = 3.14159f;

	// һ������ϵ���꣨���磬���أ��������
	/*struct Point {
		float x, y;
		Point() : x(0.f), y(0.f)
		{
		}
	};*/

	// ����
	struct Vector {
		float x, y;
		Vector() : x(0.f), y(0.f)
		{
		}
		Vector(float x0, float y0) : x(x0), y(y0) {}
	};

	// ��Ļ������
	struct ScreenPoint {
		int x, y;
		ScreenPoint() : x(0), y(0)
		{
		}
	};

	// ʵ��
	struct Entity {
		struct Vector parent_pos;
		vector<Entity> children;
		vector<Vector> points;
		unsigned char type; // 0 polygon 1 polylines
	};

	/* ���������꣩�������� ƽ��������ת */
	bool Translate(struct Vector* v, float x, float y);
	bool Scale(struct Vector* v, float xp, float yp, float sx, float sy);
	bool Rotate(struct Vector* v, float xp, float yp, float radian);
	bool Rotate2(struct Vector* v, float xp, float yp, float angle);

	/* �ֲ�����任 */
	bool TranslateLocal(struct Vector* v, float x, float y);
	bool ScaleLocal(struct Vector* v, float sx, float sy);
	bool RotateLocal(struct Vector* v, float radian);
	bool RotateLocal2(struct Vector* v, float angle);

	/* �ֲ�����(������������ϵ)���������� */
	bool Local2World(struct Vector* v, float lx, float ly, float rotate_radian, float sx, float sy);
	bool Local2World2(struct Vector* v, float lx, float ly, float rotate_angle, float sx, float sy);

	/* ��������ϵ���ֲ�����ϵ */
	bool World2Local(struct Vector* v, float lx, float ly, float rotate_radian, float sx, float sy);
	bool World2Local2(struct Vector* v, float lx, float ly, float rotate_angle, float sx, float sy);

	/* �������굽��������� */
	void World2Camera(struct Vector* v, float x, float y, float rotate_radian);
	void World2Camera2(struct Vector* v, float x, float y, float rotate_angle);

	/* ��������굽ͶӰ���꣨x(-1~1) y(-1~1)�� */
	void Perspective(struct Vector* v, float fov, float aspect, float view_distance);

	/* ͶӰ���굽��Ļ����ϵ �����԰���Ļ����ϵ��������ľֲ�����ϵ��*/
	void Viewport(struct Vector* v, int x, int y, int width, int height);
};

#endif