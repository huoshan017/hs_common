#ifndef __HS_BUFFER_H_20130809__
#define __HS_BUFFER_H_20130809__

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hs_define.h"

/**
 * HSBuffer为连续的内存，提供C风格的内存读写功能
 */

class HSBuffer
{
public:
	HSBuffer(size_t len = 1024) : length_(len), in_offset_(0), out_offset_(0)
	{
		if (len != 0) {
			buffer_ = new char[len];
		}
	}

	virtual ~HSBuffer()
	{
		Clear();
	}

	void Clear(bool clear_memory = true)
	{
		if (clear_memory) {
			if (buffer_) {
				delete [] buffer_;
				buffer_ = NULL;
			}
			length_ = 0;
		}
		in_offset_ = 0;
		out_offset_ = 0;
	}

	char* GetBuffer() const { return buffer_; }
	size_t GetLength() const { return length_; }

	bool DataIn(const char* data, size_t data_len)
	{
		if (in_offset_ + data_len > length_)
			return false;

		memcpy(buffer_+in_offset_, data, data_len);
		in_offset_ += data_len;

		return true;
	}

	bool DataIn(size_t data_len)
	{
		if (in_offset_ + data_len > length_)
			return false;

		in_offset_ += data_len;

		return true;
	}

	bool DataOut(char* out_data, size_t out_data_len)
	{
		if (in_offset_-out_offset_ < out_data_len)
			return false;

		memcpy(out_data, buffer_+out_offset_, out_data_len);
		out_offset_ += out_data_len;

		return true;
	}

	bool DataOut(size_t out_data_len)
	{
		if (in_offset_-out_offset_ < out_data_len)
			return false;

		out_offset_ += out_data_len;
		
		return true;
	}

	void SetInOffset(size_t offset) { in_offset_ = offset; }
	void SetOutOffset(size_t offset) { out_offset_ = offset; }

	size_t GetInOffset() const { return in_offset_; }
	size_t GetOutOffset() const { return out_offset_; }

	char* GetInOffsetData() { return buffer_+in_offset_; }
	char* GetOutOffsetData() { return buffer_+out_offset_; }

	size_t GetLeftInSize() { return length_ - in_offset_; }
	size_t GetLeftOutSize() { return length_ - out_offset_; }

protected:
	char* buffer_;
	size_t length_;
	size_t in_offset_;
	size_t out_offset_;
};

/**
 * HSRecvBuffer 接收缓冲，暂时为定长
 */

class HSRecvBuffer : public HSBuffer
{
public:
	HSRecvBuffer(size_t len = 1024) : HSBuffer(len), in_used_offset_(0), used_out_reset_(true)
	{
	}

	virtual ~HSRecvBuffer()
	{
		Clear();
	}

	void Clear(bool clear_memory = true)
	{
		HSBuffer::Clear(clear_memory);
		in_used_offset_ = 0;
		used_out_reset_ = true;
	}

	size_t GetUsedSize()
	{
		return in_used_offset_;
	}

	// 使用完即重置
	void SetUsedOutReset(bool reset)
	{
		used_out_reset_ = reset;
	}

	bool CanUse(size_t len)
	{
		if (in_offset_ - in_used_offset_ < len)
			return false;

		return true;
	}

	bool DataUse(size_t len, bool check = false)
	{
		if (check && !CanUse(len))
			return false;

		in_used_offset_ += len;

		if (in_used_offset_ == in_offset_ && used_out_reset_) {
			in_offset_ = 0;
			in_used_offset_ = 0;
		}

		return true;
	}

	void MoveUnuseData2Begin()
	{
		if (in_used_offset_ < in_offset_) {
			memmove(buffer_, buffer_+in_used_offset_, in_offset_-in_used_offset_);
			size_t new_offset = in_offset_ - in_used_offset_;
			in_used_offset_ = 0;
			in_offset_ = new_offset;
		}
	}

	char* GetCanUseBuffer()
	{
		return buffer_ + in_used_offset_;
	}

	size_t GetCanUseSize()
	{
		return in_offset_ - in_used_offset_;
	}

private:
	size_t in_used_offset_;			// 已处理的偏移量，总是小于接收的偏移量，相等时表示处理完所有的数据
	bool used_out_reset_;
};


/**
 * HSSendBuffer 发送缓冲，与接收缓冲类似
 */

class HSSendBuffer : public HSBuffer
{
public:
	HSSendBuffer(size_t buffer_size = SESSION_DATA_BUFFER_DEFAULT_SIZE)
		: HSBuffer(buffer_size), sent_offset_(0), sent_out_reset_(true)
	{
	}

	virtual ~HSSendBuffer()
	{
		Clear();
	}

	void Clear(bool clear_memory = true)
	{
		HSBuffer::Clear(clear_memory);
		sent_offset_ = 0;
		sent_out_reset_ = true;
	}

	void SetSentOutReset(bool reset)
	{
		sent_out_reset_ = reset;
	}

	size_t GetSentSize() { return sent_offset_; }

	char* GetCanSendBuffer()
	{
		return buffer_ + sent_offset_;
	}

	size_t GetCanSendSize()
	{
		assert(in_offset_ >= sent_offset_);
		return in_offset_ - sent_offset_;
	}

	bool CanSend(size_t length)
	{
		if (length > GetCanSendSize())
			return false;

		return true;
	}

	bool Sent(size_t length, bool check = false)
	{
		if (check) {
			if (!CanSend(length))
				return false;
		}

		sent_offset_ += length;
		if (sent_out_reset_ && sent_offset_ == in_offset_) {
			sent_offset_ = 0;
			in_offset_ = 0;
		}

		return true;
	}

private:
	size_t sent_offset_;					// 已发送偏移
	bool sent_out_reset_;					// 全部发送完是否重置
};

#endif