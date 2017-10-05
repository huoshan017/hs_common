#include "hs_Dijkstra.h"
#include <memory.h>

int
dijkstra_list_load_data(const struct dijkstra_node* node_array_data, uint16_t len, struct dijkstra_node_list* node_list)
{
	if (!node_list || !node_array_data || !len)
		return -1;

	uint16_t i = 0;
	uint16_t ss = sizeof(node_list->nodes)/sizeof(node_list->nodes[0]);
	if (len > ss)
		len = ss;
	for (; i<len; ++i) {
		node_list->nodes[i] = node_array_data[i];
		node_list->len += 1;
	}

	return 0;
}

// ��ֵnode_idŲ������Ϊnode_serialλ����
static void
set_used_node_to_pos(uint16_t use_nodes_id[MAX_NODE_NUM], uint16_t node_num, uint16_t node_id, uint16_t node_serial)
{
	if (use_nodes_id[node_serial] == node_id)
		return;

	uint16_t i = node_serial;
	for (; i<node_num; ++i) {
		if (use_nodes_id[i] == 0)
			return;

		if (use_nodes_id[i] == node_id)
			break;
	}

	if (i == node_num)
		return;

	uint16_t temp = use_nodes_id[node_serial];
	use_nodes_id[node_serial] = node_id;
	use_nodes_id[i] = temp;
}

int32_t
dijkstra_get_path(const struct dijkstra_node_list* node_list, uint16_t begin_node, uint16_t end_node, struct path_node_cache* result_list)
{
	if (!node_list)
		return -1;

	// ���ڵ��ŵĺϷ���
	if (!begin_node || begin_node>node_list->len || !end_node || end_node>node_list->len)
		return -1;

	uint16_t list_len = sizeof(node_list->nodes)/sizeof(node_list->nodes[0]);
	if (node_list->len < list_len)
		list_len = node_list->len;

	/* ����ʼ�㿪ʼ���ҳ���ÿ���ڵ�Ȩֵ��С���¸��ڵ� */

	// ���ú�δ�ýڵ�
	uint16_t used_nodes_id[MAX_NODE_NUM];
	memset(used_nodes_id, 0, sizeof(used_nodes_id));

	// ����·��
	path_node_cache all_path_cache[MAX_NODE_NUM];
	memset(all_path_cache, 0, sizeof(all_path_cache));

	uint16_t i = 0;
	// ��ʼ��ʹ�ýڵ��б�
	for (; i<list_len; ++i) {
		used_nodes_id[i] = i + 1;
	}

	// ��ʼ��ŵ������б���
	uint16_t use_node = begin_node;

	// �ӳ�ʼ�ڵ㿪ʼ���ҳ�����С�������ʺ���Ľڵ�
	for (i=0; i<list_len; ++i) {
		// û���¸��ڵ�
		if (use_node == 0)
			break;

		set_used_node_to_pos(used_nodes_id, list_len, use_node, i);

		uint16_t j = i+1;
		for (; j<list_len; ++j) {
			uint16_t next_node = used_nodes_id[j];
			
			// use_node����j+1��Ȩֵ
			int weight = node_list->nodes[use_node-1].weight[next_node-1];
			if (weight <= 0)
				continue;

			// ��ʼ�㵽use_node�����use_node�㵽next_node�ĺ�
			int cur_weight_j = all_path_cache[next_node-1].weights_len;
			int w = all_path_cache[use_node-1].weights_len + weight;
			
			// next_node���һ�ε��Ҫ����֮ǰ��·��
			if (all_path_cache[next_node-1].path_node_num == 0 || cur_weight_j > w) {
				all_path_cache[next_node-1] = all_path_cache[use_node-1];
				uint16_t num = all_path_cache[next_node-1].path_node_num;
				all_path_cache[next_node-1].shortest_path_nodes[num] = next_node;
				all_path_cache[next_node-1].path_node_num += 1;
				all_path_cache[next_node-1].weights_len += weight;
			}
		}

		// �Ƚ�ʣ�µĽڵ�ľ��룬ȡ��С����Ϊ���������б�
		int min_len = -1; uint16_t min_len_node = 0;
		for (j=i+1; j<list_len; ++j) {
			uint16_t n = used_nodes_id[j];
			int len = all_path_cache[n-1].weights_len;
			if (len > 0) {
				if (min_len<0 || min_len>len) {
					min_len = len;
					min_len_node = n;
				}
			}
		}

		// ȷ����һ���������ýڵ㼯�ĵ�
		use_node = min_len_node;
	}

	if (all_path_cache[end_node-1].weights_len == 0)
		return -1;

	*result_list = all_path_cache[end_node-1];

	return 0;
}