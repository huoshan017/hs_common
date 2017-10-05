#ifndef __HS_DIJKSTRA_H__
#define __HS_DIJKSTRA_H__

#include <stdint.h>

#define MAX_NODE_NUM 5

/* 节点编号从1开始 */
struct dijkstra_node {
	uint16_t id;
	int32_t weight[MAX_NODE_NUM];		// 节点编号做索引   正数表示权值，0表示没有，负数表示无穷大
};

// 节点列表
struct dijkstra_node_list {
	struct dijkstra_node nodes[MAX_NODE_NUM];
	uint16_t len;
};

struct path_node_cache {
	uint16_t shortest_path_nodes[MAX_NODE_NUM];
	uint16_t path_node_num;
	int32_t weights_len;
};

// 载入节点数据
int
dijkstra_list_load_data(const struct dijkstra_node* node_array_data, uint16_t len, struct dijkstra_node_list* node_list);

// 计算两点之间的路径
int32_t
dijkstra_get_path(const struct dijkstra_node_list* node_list, uint16_t begin_node, uint16_t end_node, struct path_node_cache* cache);

#endif