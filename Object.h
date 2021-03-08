#pragma once
class Object
{
private:
	size_t x;
	size_t y;
public:
	Object();
	Object(size_t x, size_t y);

	size_t GetX() const;
	size_t GetY() const;
	void SetX(size_t);
	void SetY(size_t);

	void SetPosition(size_t nx, size_t ny);
};