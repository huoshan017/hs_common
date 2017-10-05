#ifndef __HS_2D_H__
#define __HS_2D_H__

#include <vector>
#include <list>

using namespace std;

namespace hs2d {

	const float PI = 3.14159f;

	// 一般坐标系坐标（世界，本地，摄像机）
	/*struct Point {
		float x, y;
		Point() : x(0.f), y(0.f)
		{
		}
	};*/

	// 向量
	struct Vector {
		float x, y;
		Vector() : x(0.f), y(0.f)
		{
		}
		Vector(float x0, float y0) : x(x0), y(y0) {}
	};

	// 屏幕点坐标
	struct ScreenPoint {
		int x, y;
		ScreenPoint() : x(0), y(0)
		{
		}
	};

	// 实体
	struct Entity {
		struct Vector parent_pos;
		vector<Entity> children;
		vector<Vector> points;
		unsigned char type; // 0 polygon 1 polylines
	};

	/* （绝对坐标）世界坐标 平移缩放旋转 */
	bool Translate(struct Vector* v, float x, float y);
	bool Scale(struct Vector* v, float xp, float yp, float sx, float sy);
	bool Rotate(struct Vector* v, float xp, float yp, float radian);
	bool Rotate2(struct Vector* v, float xp, float yp, float angle);

	/* 局部坐标变换 */
	bool TranslateLocal(struct Vector* v, float x, float y);
	bool ScaleLocal(struct Vector* v, float sx, float sy);
	bool RotateLocal(struct Vector* v, float radian);
	bool RotateLocal2(struct Vector* v, float angle);

	/* 局部坐标(包含惯性坐标系)到世界坐标 */
	bool Local2World(struct Vector* v, float lx, float ly, float rotate_radian, float sx, float sy);
	bool Local2World2(struct Vector* v, float lx, float ly, float rotate_angle, float sx, float sy);

	/* 世界坐标系到局部坐标系 */
	bool World2Local(struct Vector* v, float lx, float ly, float rotate_radian, float sx, float sy);
	bool World2Local2(struct Vector* v, float lx, float ly, float rotate_angle, float sx, float sy);

	/* 世界坐标到摄像机坐标 */
	void World2Camera(struct Vector* v, float x, float y, float rotate_radian);
	void World2Camera2(struct Vector* v, float x, float y, float rotate_angle);

	/* 摄像机坐标到投影坐标（x(-1~1) y(-1~1)） */
	void Perspective(struct Vector* v, float fov, float aspect, float view_distance);

	/* 投影坐标到屏幕坐标系 （可以把屏幕坐标系看成特殊的局部坐标系）*/
	void Viewport(struct Vector* v, int x, int y, int width, int height);
};

#endif