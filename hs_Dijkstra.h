#ifndef __HS_DIJKSTRA_H__
#define __HS_DIJKSTRA_H__

#include <stdint.h>

#define MAX_NODE_NUM 5

/* �ڵ��Ŵ�1��ʼ */
struct dijkstra_node {
	uint16_t id;
	int32_t weight[MAX_NODE_NUM];		// �ڵ���������   ������ʾȨֵ��0��ʾû�У�������ʾ�����
};

// �ڵ��б�
struct dijkstra_node_list {
	struct dijkstra_node nodes[MAX_NODE_NUM];
	uint16_t len;
};

struct path_node_cache {
	uint16_t shortest_path_nodes[MAX_NODE_NUM];
	uint16_t path_node_num;
	int32_t weights_len;
};

// ����ڵ�����
int
dijkstra_list_load_data(const struct dijkstra_node* node_array_data, uint16_t len, struct dijkstra_node_list* node_list);

// ��������֮���·��
int32_t
dijkstra_get_path(const struct dijkstra_node_list* node_list, uint16_t begin_node, uint16_t end_node, struct path_node_cache* cache);

#endif