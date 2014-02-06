#include "hs_packetworker.h"
#include "hs_define.h"
#include "boost/asio.hpp"
#include <iostream>

using namespace std;

// �ӳ���Ϊsize�İ�����packet_data�л�ȡ�û����ݰ���user_data_offsetΪ�����packet_data��ƫ�ƣ�user_data_sizeΪ�û����ݳ���
int HSPacketWorker::DecodePacket(char* packet_data, size_t size, size_t& user_data_offset, size_t& user_data_size)
{
	if (!packet_data)
		return -1;

	static size_t identify_number_size = sizeof(HS_NET_PACKET_IDENTIFY_NUMBER);

	// ����С�ڰ�ͷ����+ʶ���볤�ȣ��������������ܿ����ǰ�û��������������0�ȴ���һ�ν��պ���
	if (size < HS_NET_PACKET_HEDER_LENGTH + identify_number_size) {
		cout << "���ݳ���" << size << "С�ڰ�ͷ(" << (int)HS_NET_PACKET_HEDER_LENGTH << ")+ʶ����(" << identify_number_size << "), ����0�ȴ��´ν�����������" << endl;
		return 0;
	}

	// �û����ݲ��ܴ���HS_NET_PACKET_MAX_SIZE - (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)
	unsigned int pl = (unsigned int)boost::asio::detail::socket_ops::network_to_host_long((unsigned long)*((unsigned int*)&packet_data[0]));
	if (pl > HS_NET_PACKET_MAX_SIZE - (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)) {
		cout << "�û����ݳ���" << pl << "����������-��ͷ-ʶ����" << endl;
		return -1;
	}

	// ʶ����
	unsigned int in = (unsigned int)boost::asio::detail::socket_ops::network_to_host_long((unsigned long)*((unsigned int*)&packet_data[0+HS_NET_PACKET_HEDER_LENGTH]));
	if (in != HS_NET_PACKET_IDENTIFY_NUMBER) {
		cout << "ʶ����(" << in << ")����ȷ" << endl;
		return -1;
	}

	// �����Ȳ���������ȴ���һ�ν��������ٴ���
	if (size < pl + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size)) {
		cout << "���ݳ���" << size << "�����������Ҫ����Ϊ" << pl << "+(HS_NET_PACKET_HEDER_LENGTH + identify_number_size)���ȴ��´ν�����������" << endl;
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
		cout << "�û����ݰ�����" << size + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size) << "�����޷����뷢�ͻ���" << endl;
		return -1;
	}

	if (size + (HS_NET_PACKET_HEDER_LENGTH + identify_number_size) > out_data_size) {
		cout << "�û����ݳ���(����Ҫ���ϰ�ͷ����4��ʶ���볤��4)" << size + HS_NET_PACKET_HEDER_LENGTH + identify_number_size << "�����˷��ͻ����ṩ�ĳ���" << out_data_size << endl;
		return 0;
	}

	// ����
	*((unsigned int*)out_data) = (unsigned int)boost::asio::detail::socket_ops::host_to_network_long((unsigned long)size);
	// ʶ����
	*((unsigned int*)(out_data+HS_NET_PACKET_HEDER_LENGTH)) = (unsigned int)boost::asio::detail::socket_ops::host_to_network_long((unsigned long)HS_NET_PACKET_IDENTIFY_NUMBER);

	// ������
	memcpy(out_data + HS_NET_PACKET_HEDER_LENGTH + identify_number_size, user_data, size);

	// ����
	encode_size = size + (identify_number_size+HS_NET_PACKET_HEDER_LENGTH);

	return 1;
}