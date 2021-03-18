#pragma once
#include <queue>
#include <list>
#include <stdexcept>

template <class T>
class pool
{
public:

	pool(size_t size)
	{
		data_ = new T[size];
		for (size_t i = 0; i < size; i++)
		{
			free_.push(&data_[i]);
		}
	}
	~pool()
	{
		delete[] data_;
	}

	struct iterator : public std::list<T*>::iterator
	{
		iterator()
			: std::list<T*>::iterator() {};
		iterator(typename std::list<T*>::iterator it) 
			: std::list<T*>::iterator(it) {};

		T* operator-> ()
		{
			return *(*this);
		}
	};
	struct const_iterator : public std::list<T*>::const_iterator
	{
		const_iterator()
			: std::list<T*>::const_iterator() {};
		const_iterator(typename std::list<T*>::iterator it)
			: std::list<T*>::const_iterator(it) {};
		const_iterator(typename std::list<T*>::const_iterator it)
			: std::list<T*>::const_iterator(it) {};

		const T* const operator-> ()
		{
			return *(*this);
		}
	};

	iterator begin()
	{
		return busy_.begin();
	}

	iterator end()
	{
		return busy_.end();
	}

	const_iterator begin() const
	{
		return busy_.begin();
	}

	const_iterator end() const
	{
		return busy_.end();
	}

	size_t size() const
	{
		return busy_.size();
	}

	iterator get(const T& value)
	{
		if (!free_.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free_.front();
		*obj = value;
		free_.pop();
		busy_.emplace_front(obj);
		return busy_.begin();
	}
	iterator get(T&& value)
	{
		if (!free_.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free_.front();
		*obj = std::move(value);
		free_.pop();
		busy_.emplace_front(obj);
		return busy_.begin();
	}
	iterator get()
	{
		if (!free_.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free_.front();
		free_.pop();
		busy_.emplace_front(obj);
		return busy_.begin();
	}

	iterator free(iterator it)
	{
		free_.push(*it);
		it = busy_.erase(it);
		return it;
	}

private:
	std::list<T*> busy_;
	std::queue<T*> free_;
	T* data_;
};