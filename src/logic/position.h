#pragma once
namespace ecosystem
{
	namespace logic
	{
		class position
		{
		private:
			int x_;
			int y_;
		public:
			position();
			position(int x, int y);

			int x() const;
			int y() const;
			void x(int);
			void y(int);

			void set(int x, int y);
		};
	}
}