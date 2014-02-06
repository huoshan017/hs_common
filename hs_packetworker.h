#ifndef __HS_PACKET_WORKER_H_20130806__
#define __HS_PACKET_WORKER_H_20130806__

class HSPacketWorker
{
public:
	// ½â°ü
	static int DecodePacket(char* packet_data, size_t size, size_t& user_data_offset, size_t& user_data_size);
	// ·â°ü
	static int EncodePacket(const char* user_data, size_t size, char* out_data, size_t out_data_size, size_t& encode_size);
};

#endif