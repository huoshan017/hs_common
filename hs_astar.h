#ifndef __HS_ASTAR_H__
#define __HS_ASTAR_H__

#include <stdint.h>

#define MAX_ARRAY_WIDTH 128			// �����
#define MAX_ARRAY_HEIGHT 128		// ���߶�

// Ѱ·��������
struct t_astar_array {
	int arr[MAX_ARRAY_WIDTH*MAX_ARRAY_HEIGHT];
	uint16_t width, height;
};

// �����������б�
typedef int t_index_list[MAX_ARRAY_WIDTH*MAX_ARRAY_HEIGHT/4];

struct t_astar_res {
	t_index_list index_list;
	int count;
};

// ���굽�������������������0��ʼ�㣩
inline int hs_coord2index(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	if (x>=width || y>=height)
		return -1;
	return y * width + x;
}

// ����������
inline int hs_index2coord(int index, uint16_t width, uint16_t height, uint16_t* to_x, uint16_t* to_y) {
	if (index>=width*height || !to_x || !to_y)
		return -1;
	*to_y = index / width;
	*to_x = index - (*to_y)*width;
	return 0;
}

// ��������
int hs_astar_load_data(const int* data, uint16_t width, uint16_t height, struct t_astar_array* ta);

// A*Ѱ·
int hs_astar_find_path(struct t_astar_array* ta, int begin_index, int end_index, struct t_astar_res* res);

// �����ڴ�
void hs_astar_clear(struct t_astar_array* ta);

// ����
void hs_astar_recycle(struct t_astar_array* ta);

#endif