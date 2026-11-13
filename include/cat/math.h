#ifndef CAT_MATH_H
#define CAT_MATH_H

#include <concepts>
#include <cassert>

namespace cat
{
	template<typename T>
	concept number = std::is_integral<T>::value || std::is_floating_point<T>::value;

	template<typename T>
		requires number<T>
	struct vec2 final
	{
		T x;
		T y;

		T length() const
		{
			return sqrt<T>(x * x + y * y);
		}
		void normalize()
		{
			T len{ length() };
			x /= len;
			y /= len;
		}
		vec2<T> normalized()
		{
			T len{ length() };
			return vec2<T>{ x / len, y / len };
		}
	};

	template<typename T>
	struct size2 final
	{
		T x;
		T y;
	};

	template<typename T>
		requires number<T>
	struct rect final
	{
		T x;
		T w;
		T y;
		T h;

		cat::size2<T> size() const
		{
			return cat::size2<T>{ w, h };
		}
		cat::vec2<T> position() const
		{
			return cat::vec2<T>{ x, y };
		}
		cat::vec2<T> center() const
		{
			return cat::vec2<T>{ x + w / 2, y + h / 2 };
		}

		static bool overlap(rect<T> rect1, rect<T> rect2)
		{
			if (rect1.x > rect2.x - rect1.w && rect1.x < rect2.x + rect2.w)
				if (rect1.y > rect2.y - rect1.h && rect1.y < rect2.y + rect2.h)
					return true;

			return false;
		}
	};
}

#endif
