#pragma once
class Object
{
private:
	size_t x_;
	size_t y_;
public:
	Object();
	Object(size_t x, size_t y);

	size_t x() const;
	size_t y() const;
	void x(size_t);
	void y(size_t);

	void SetPosition(size_t nx, size_t ny);
};