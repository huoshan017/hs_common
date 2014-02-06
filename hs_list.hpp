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

	// 移除头结点
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
			// 新节点
			HSListNode<T>* node = new HSListNode<T>;
			node->value = value;
			node->next = NULL;
			if (!useable_count_) {
				head_ = node;
			} else {
				tail_->next = node;
			}
			tail_ = node;
			// 所有的节点数+1
			total_count_ += 1;
		} else {
			// 回收的节点
			*p = value;	
		}
		// 可用的节点数+1
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
	// 头结点移到末尾
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
		// 没有可用的节点
		if (!useable_count_) {
			head_ = NULL;
		}
	}

private:
	HSListNode<T>* head_;				// 头结点，如果为空则没有可用的值
	HSListNode<T>* tail_;				// 尾节点，如果不为空则next_成员指向的为回收的节点
	HSListNode<T>* recycled_tail_;		// 已回收的尾节点
	size_t useable_count_;			// 可用的节点数
	size_t total_count_;			// 所有的节点数，包括已回收的
};

#endif