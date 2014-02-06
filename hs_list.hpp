#ifndef __HS_LIST_H_20130816__
#define __HS_LIST_H_20130816__

template <class T>
struct HSListNode
{
	T value;
	HSListNode* next;
};

template <class T>
class HSList
{
public:
	HSList() : head_(NULL), tail_(NULL), recycled_tail_(NULL), useable_count_(0), total_count_(0)
	{
	}

	~HSList()
	{
	}

	size_t GetCount() const { return useable_count_; }

	bool GetHeadValue(T*& value_ptr)
	{
		if (head_ == NULL)
			return false;

		value_ptr = &head_->value;

		return true;
	}

	// �Ƴ�ͷ���
	bool RemoveHead()
	{
		if (!useable_count_)
			return false;

		MoveHead2Tail();

		return true;
	}

	void PushValue(const T& value)
	{
		T* p = NULL;
		if (!GetTailNextValue(p)) {
			// �½ڵ�
			HSListNode<T>* node = new HSListNode<T>;
			node->value = value;
			node->next = NULL;
			if (!useable_count_) {
				head_ = node;
			} else {
				tail_->next = node;
			}
			tail_ = node;
			// ���еĽڵ���+1
			total_count_ += 1;
		} else {
			// ���յĽڵ�
			*p = value;	
		}
		// ���õĽڵ���+1
		useable_count_ += 1;
	}

	bool GetTailNextValue(T*& value_ptr)
	{
		if (tail_ == NULL)
			return false;

		if (tail_->next == NULL)
			return false;

		value_ptr = &tail_->next->value;

		return true;
	}

private:
	// ͷ����Ƶ�ĩβ
	void MoveHead2Tail()
	{
		if (!useable_count_)
			return;

		HSListNode<T>* new_head = head_->next;
		head_->next = NULL;
		if (!recycled_tail_)
			recycled_tail_ = head_;
		else
			recycled_tail_->next = head_;
		--useable_count_;
		// û�п��õĽڵ�
		if (!useable_count_) {
			head_ = NULL;
		}
	}

private:
	HSListNode<T>* head_;				// ͷ��㣬���Ϊ����û�п��õ�ֵ
	HSListNode<T>* tail_;				// β�ڵ㣬�����Ϊ����next_��Աָ���Ϊ���յĽڵ�
	HSListNode<T>* recycled_tail_;		// �ѻ��յ�β�ڵ�
	size_t useable_count_;			// ���õĽڵ���
	size_t total_count_;			// ���еĽڵ����������ѻ��յ�
};

#endif