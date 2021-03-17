#pragma once
#include <queue>
#include <list>
#include <stdexcept>

template <class T>
class pool
{
public:
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

	iterator begin()
	{
		return delta.begin();
	}

	iterator end()
	{
		return delta.end();
	}

	size_t size()
	{
		return delta.size();
	}

	iterator Add(const T& value)
	{
		if (!free.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free.front();
		*obj = value;
		free.pop();
		delta.emplace_front(obj);
		return delta.begin();
	}
	iterator Add(T&& value)
	{
		if (!free.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free.front();
		*obj = std::move(value);
		free.pop();
		delta.emplace_front(obj);
		return delta.begin();
	}
	iterator Add()
	{
		if (!free.size())
		{
			throw std::out_of_range("Pool out of size");
		}

		T* obj = free.front();
		free.pop();
		delta.emplace_front(obj);
		return delta.begin();
	}


	iterator Del(iterator it)
	{
		free.push(*it);
		it = delta.erase(it);
		return it;
	}

	pool(size_t size)
	{
		data = new T[size];
		for (size_t i = 0; i < size; i++)
		{
			free.push(&data[i]);
		}
	}
	~pool()
	{
		delete[] data;
	}

private:
	std::list<T*> delta;
	std::queue<T*> free;
	T* data;
};