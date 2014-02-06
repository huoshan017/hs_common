#include "hs_packetworker.h"
#include "hs_define.h"
#include "boost/asio.hpp"
#include <iostream>

using namespace std;

// 从长度为size的包数据packet_data中获取用户数据包，user_data_offset为相对于packet_data的偏移，user_data_size为用户数据长度
int HSPacketWorker::DecodePacket(char* packet_data, size_t size, size_t& user_data_offset, size_t& user_data_size)
{
	if (!packet_data)
		return -1;

	static size_t identify_number_size = sizeof(HS_NET_PACKET_IDENTIFY_NUMBER);

	// 不能小于包头长度+识别码长度，如果有这种情况很可能是包没接收完整，返回0等待下一次接收后处理
	if (size < HS_NET_PACKET_HEDER_LENGTH + identify_number_size) {
		cout << "数据长度" << size << "小于包头(" << (int)HS_NET_PACKET_HEDER_LENGTH << ")+识别码(" << identify_number_size << "), 返回0等待下次接收完整后处理" << endl;
		return 0;
	}

	// 用户数据不能大于HS_NET_PACKET_MAX_SIZE - (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)
	unsigned int pl = (unsigned int)boost::asio::detail::socket_ops::network_to_host_long((unsigned long)*((unsigned int*)&packet_data[0]));
	if (pl > HS_NET_PACKET_MAX_SIZE - (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)) {
		cout << "用户数据长度" << pl << "大于最大包长-包头-识别码" << endl;
		return -1;
	}

	// 识别码
	unsigned int in = (unsigned int)boost::asio::detail::socket_ops::network_to_host_long((unsigned long)*((unsigned int*)&packet_data[0+HS_NET_PACKET_HEDER_LENGTH]));
	if (in != HS_NET_PACKET_IDENTIFY_NUMBER) {
		cout << "识别码(" << in << ")不正确" << endl;
		return -1;
	}

	// 包长度不够解包，等待下一次接收完整再处理
	if (size < pl + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)) {
		cout << "数据长度" << size << "不够解包，需要长度为" << pl << "+(HS_NET_PACKET_HEDER_LENGTH + identify_number_size)，等待下次接收完整后处理" << endl;
		return 0;
	}

	user_data_offset = (HS_NET_PACKET_HEDER_LENGTH + identify_number_size);
	user_data_size = pl;

	return 1;
}

int HSPacketWorker::EncodePacket(const char* user_data, size_t size, char* out_data, size_t out_data_size, size_t& encode_size)
{
	static size_t identify_number_size = sizeof(HS_NET_PACKET_IDENTIFY_NUMBER);
	if (size + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size) > HS_NET_PACKET_MAX_SIZE) {
		cout << "用户数据包长度" << size + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size) << "过大，无法送入发送缓冲" << endl;
		return -1;
	}

	if (size + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size) > out_data_size) {
		cout << "用户数据长度(另外要加上包头长度4和识别码长度4)" << size + HS_NET_PACKET_HEDER_LENGTH + identify_number_size << "超出了发送缓冲提供的长度" << out_data_size << endl;
		return 0;
	}

	// 包长
	*((unsigned int*)out_data) = (unsigned int)boost::asio::detail::socket_ops::host_to_network_long((unsigned long)size);
	// 识别码
	*((unsigned int*)(out_data+HS_NET_PACKET_HEDER_LENGTH)) = (unsigned int)boost::asio::detail::socket_ops::host_to_network_long((unsigned long)HS_NET_PACKET_IDENTIFY_NUMBER);

	// 包数据
	memcpy(out_data + HS_NET_PACKET_HEDER_LENGTH + identify_number_size, user_data, size);

	// 包长
	encode_size = size + (identify_number_size+HS_NET_PACKET_HEDER_LENGTH);

	return 1;
}