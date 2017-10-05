#include "hs_astar.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

struct _astar_node {
	int index;					// 索引值
	int state;					// 0未考察 1开启(open) 2已用(close)
	int f, g, h;				// g：已用的实际距离，h：剩下的估计距离，f：g+h
	struct _astar_node* parent; // 父节点
};

static inline int calc_gvalue(struct _astar_node* parent) {
	if (!parent)
		return 0;
	return parent->g + 1;
}

static inline int calc_hvalue(uint16_t cx, uint16_t cy, uint16_t ex, uint16_t ey) {
	return abs(cx-ex) + abs(cy-ey);
}

static inline int calc_hvalue_byindex(int index, uint16_t width, uint16_t height, uint16_t end_x, uint16_t end_y) {
	uint16_t bx, by;
	if (hs_index2coord(index, width, height, &bx, &by) < 0)
		return -1;
	return calc_hvalue(bx, by, end_x, end_y);
}

static int
get_min_f_index(struct _astar_node** open_list, uint16_t open_list_len) {
	uint16_t i = 0; int f = 0; uint16_t min_f_i = 0;
	for (; i<open_list_len; ++i) {
		if (open_list[i] && (!f || f>=open_list[i]->f)) {
			f = open_list[i]->f;
			min_f_i = i;
		}
	}
	return min_f_i;
}

static int
is_in_closed(struct _astar_node** closed_list, int closed_list_len, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
	int index = hs_coord2index(x, y, width, height);
	int i = 0;
	for (; i<closed_list_len; ++i) {
		if (closed_list[i] && closed_list[i]->index==index)
			return 0;
	}
	return -1;
}

static int
is_blocked(const t_astar_array* aa, int16_t cx, int16_t cy) {
	if (cy<0 || cy>=aa->height)
		return 0;

	if (cx<0 || cx>=aa->width)
		return 0;

	int index = hs_coord2index(cx, cy, aa->width, aa->height);
	if (aa->arr[index] == 1)
		return 0;

	return -1;
}

static int
is_in_open_byindex(struct _astar_node** open_list, int open_list_len, int index, int* pos) {
	int i = 0;
	for (; i<open_list_len; ++i)
		if (open_list[i] && open_list[i]->index==index) {
			if (pos) *pos = i;
			return 0;
		}
	return -1;
}

static int
is_in_open(struct _astar_node** open_list, int open_list_len, uint16_t width, uint16_t height, uint16_t x, uint16_t y, int* pos) {
	int index = hs_coord2index(x, y, width, height);
	if (index < 0)
		return -1;
	return is_in_open_byindex(open_list, open_list_len, index, pos);
}

static const uint16_t _alloc_list_array_size = 32;
static const uint16_t _alloc_one_list_size = 128;

struct _node_alloc_data {
	uint8_t inited;
	uint16_t cur_list;
	uint16_t cur_index;
	struct _astar_node* list_array[_alloc_list_array_size];
};

static void atar_node_allocator_init(struct _node_alloc_data* alloc_data)
{
	memset(alloc_data, 0, sizeof(struct _node_alloc_data));
	alloc_data->list_array[0] = (struct _astar_node*)malloc(sizeof(struct _astar_node)*_alloc_one_list_size);
	memset(alloc_data->list_array[0], 0, sizeof(struct _astar_node)*_alloc_one_list_size);
	alloc_data->inited = 1;
}

static struct _astar_node* astar_node_alloc(struct _node_alloc_data* alloc_data)
{
	if (alloc_data->cur_list>=_alloc_list_array_size-1 && alloc_data->cur_index>=_alloc_one_list_size-1)
		return NULL;

	if (alloc_data->cur_index >= _alloc_one_list_size) {
		if (!alloc_data->list_array[alloc_data->cur_list+1]) {
			alloc_data->list_array[alloc_data->cur_list+1] = (struct _astar_node*)malloc(sizeof(struct _astar_node)*_alloc_one_list_size);
			memset(alloc_data->list_array[alloc_data->cur_list+1], 0, sizeof(struct _astar_node)*_alloc_one_list_size);
		}
		alloc_data->cur_list += 1;
		alloc_data->cur_index = 0;
	}

	return &(alloc_data->list_array[alloc_data->cur_list][alloc_data->cur_index++]);
}

static void astar_node_allocator_recycle(struct _node_alloc_data* alloc_data)
{
	if (!alloc_data->inited) {
		atar_node_allocator_init(alloc_data);
	} else {
		// 回收不释放内存
		alloc_data->cur_index = 0;
		alloc_data->cur_list = 0;
	}
}

static void astar_node_free(struct _node_alloc_data* alloc_data)
{
	uint16_t i = 0;
	for (; i<alloc_data->cur_list; ++i) {
		if (alloc_data->list_array[i]) {
			free(alloc_data->list_array[i]);
		}
	}
	memset(alloc_data, 0, sizeof(*alloc_data));
}

// 分配器
struct _node_alloc_data alloc_data;

// 获得寻路结果
static void
get_result_list(struct _astar_node** closed_list, struct t_astar_res* result_list) {
	struct _astar_node* node = closed_list[0];
	int i = 0;
	for ( ; ; ) {
		if (!node) break;
		result_list->index_list[i++] = node->index;
		node = node->parent;
	}
	result_list->count = i;
}

// 载入数据
int hs_astar_load_data(const int* data, uint16_t width, uint16_t height, struct t_astar_array* ta)
{
	if (!data || !width || !height || !ta)
		return -1;

	if (width>MAX_ARRAY_WIDTH || height>MAX_ARRAY_HEIGHT)
		return -1;

	uint16_t i = 0;
	for (; i<width; ++i) {
		uint16_t j = 0;
		for (; j<height; ++j) {
			int index = hs_coord2index(i, j, width, height);
			ta->arr[index] = *((const int*)data+width*j+i);
		}
	}
	ta->width = width;
	ta->height = height;

	astar_node_allocator_recycle(&alloc_data);

	return 0;
}

// 寻路
int hs_astar_find_path(struct t_astar_array* aa, int begin_index, int end_index, struct t_astar_res* res)
{
	printf("hs_astar_find_path begin ... \n");
	long begin_tick = clock();

	if (!aa || begin_index<0 || end_index<0)
		return -1;

	uint16_t  cx, cy, ex, ey;
	if (hs_index2coord(begin_index, aa->width, aa->height, &cx, &cy) < 0)
		return -1;
	if (hs_index2coord(end_index, aa->width, aa->height, &ex, &ey) < 0)
		return -1;

	// open表从0索引开始向后，close表从最后一个索引开始向前
	struct _astar_node* open_close_nodes[MAX_ARRAY_WIDTH*MAX_ARRAY_HEIGHT];
	memset(open_close_nodes, 0, sizeof(open_close_nodes));
	const static uint16_t tt_len = sizeof(open_close_nodes)/sizeof(open_close_nodes[0]);
	//const static uint16_t closed_start_index = tt_len - 1;
	uint16_t open_num = 0; uint16_t closed_num = 0;
	uint16_t closed_curr_index = tt_len - 1 - closed_num;

	// 把起始点加入开启列表
	struct _astar_node* new_node = astar_node_alloc(&alloc_data);
	new_node->state = 1;
	new_node->parent = NULL;
	new_node->index = begin_index;
	new_node->g = 0;
	new_node->h = calc_hvalue(cx, cy, ex, ey);
	new_node->f = new_node->g + new_node->h;
	open_close_nodes[open_num] = new_node;
	open_num += 1;

	struct _astar_node* cur_open_node = NULL;
	while (1) {
		// open表为空，路径不存在
		if (open_num == 0)
			return -1;

		// open表中找出最小的f索引(open表中的索引)
		int f_index = get_min_f_index(open_close_nodes, open_num);

		cur_open_node = open_close_nodes[f_index];
		if (!cur_open_node)
			continue;

		// 把此节点从open表挪到closed表
		open_close_nodes[closed_curr_index] = open_close_nodes[f_index];
		if (f_index < open_num-1)
			memmove(&open_close_nodes[f_index], &open_close_nodes[f_index+1], sizeof(open_close_nodes[0])*(open_num-f_index-1));
		memset(&open_close_nodes[open_num-1], 0, sizeof(open_close_nodes[open_num-1]));
		open_num -= 1;
		closed_num += 1;
		closed_curr_index -= 1;
		
		// 地图中的索引
		int grid_index = cur_open_node->index;

		if (grid_index == end_index) {
			get_result_list(open_close_nodes+closed_curr_index+1, res);
			long cur_tick = clock();
			printf("hs_astar_find_path end, cost %d ms\n", cur_tick-begin_tick);
			return 0;
		}

		// 访问其邻居节点
		hs_index2coord(grid_index, aa->width, aa->height, &cx, &cy);

		int16_t nx_arr[] = { cx, cx, cx-1, cx+1 };
		int16_t ny_arr[] = { cy-1, cy+1, cy, cy };

		uint8_t i = 0;
		uint8_t s = sizeof(nx_arr)/sizeof(nx_arr[0]);
		uint8_t s2 = sizeof(ny_arr)/sizeof(ny_arr[0]);
		if (s > s2) s = s2;
		for (; i<s; ++i) {
			// 不可达
			if (is_blocked(aa, nx_arr[i], ny_arr[i]) == 0)
				continue;

			// 在closed表中
			if (is_in_closed(open_close_nodes+closed_curr_index, closed_num, aa->width, aa->height, nx_arr[i], ny_arr[i]) == 0)
				continue;

			int pos = -1;
			// 不在open表中，加入open表
			if (is_in_open(open_close_nodes, open_num, aa->width, aa->height, nx_arr[i], ny_arr[i], &pos) < 0) {
				new_node = astar_node_alloc(&alloc_data);
				new_node->index = hs_coord2index(nx_arr[i], ny_arr[i], aa->width, aa->height);
				new_node->state = 1;
				// 父节点为当前节点
				new_node->parent = cur_open_node;
				new_node->g = calc_gvalue(cur_open_node);
				new_node->h = calc_hvalue(nx_arr[i], ny_arr[i], ex, ey);
				new_node->f = new_node->g + new_node->h;
				open_close_nodes[open_num] = new_node;
				open_num += 1;
			} else {
				struct _astar_node* pos_node = open_close_nodes[pos];
				if (!pos_node)
					continue;
				// 比较f值
				int old_g = pos_node->g;
				int new_g = cur_open_node->g + 1;
				if (old_g > new_g) {
					pos_node->parent = cur_open_node;
					pos_node->g = calc_gvalue(cur_open_node);
					pos_node->h = calc_hvalue(nx_arr[i], ny_arr[i], ex, ey);
					pos_node->f = pos_node->g + pos_node->h;
				}
			}
		}
	}
	return -1;
}

// 清理内存
void hs_astar_clear(struct t_astar_array* ta)
{
	astar_node_free(&alloc_data);
}

// 回收
void hs_astar_recycle(struct t_astar_array* ta)
{
	astar_node_allocator_recycle(&alloc_data);
}