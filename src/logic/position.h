#pragma once
class position
{
private:
	size_t x_;
	size_t y_;
public:
	position();
	position(size_t x, size_t y);
	
	size_t x() const;
	size_t y() const;
	void x(size_t);
	void y(size_t);

	void SetPosition(size_t x, size_t y);

	friend bool operator == (const position& lhs, const position& rhs);

};