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
	struct tvec2 final
	{
		T x{};
		T y{};

		T magnitude() const
		{
			return sqrt<T>(x * x + y * y);
		}
		T sqr_magnitude() const
		{
			return x * x + y * y;
		}
		T normalize()
		{
			T mag{ magnitude() };
			x /= mag;
			y /= mag;

			return mag;
		}
		tvec2<T> normalized() const
		{
			T len{ magnitude() };
			return tvec2<T>{ x / len, y / len };
		}

		tvec2<T> operator+(const tvec2<T>& other) const
		{
			return { x + other.x, y + other.y };
		}
		tvec2<T> operator-(const tvec2<T>& other) const
		{
			return { x - other.x, y - other.y };
		}
		tvec2<T> operator*(T scale) const
		{
			return { x * scale, y * scale };
		}
		tvec2<T> operator/(T scale) const
		{
			return { x / scale, y / scale };
		}
		tvec2<T> operator-() const
		{
			return { -x, -y };
		}
		tvec2<T>& operator+=(const tvec2<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		tvec2<T>& operator-=(const tvec2<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		tvec2<T>& operator*=(T scale)
		{
			x *= scale;
			y *= scale;
			return *this;
		}
		tvec2<T>& operator/=(T scale)
		{
			x /= scale;
			y /= scale;
			return *this;
		}
		T& operator[](int index)
		{
			assert(index <= 1 && index >= 0);

			if (index == 0)
				return x;
			return y;
		}
		T operator[](int index) const
		{
			assert(index <= 1 && index >= 0);

			if (index == 0)
				return x;
			return y;
		}
		bool operator==(const tvec2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		static T dot(tvec2<T> lhs, tvec2<T> rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		static T cross(tvec2<T> lhs, tvec2<T> rhs)
		{
			return lhs.x * rhs.y - lhs.y * rhs.x;
		}

		static const tvec2<T> unit_x;
		static const tvec2<T> unit_y;
		static const tvec2<T> zero;
	};

	template<typename T>
		requires number<T>
	const tvec2<T> tvec2<T>::unit_x{ 1, 0 };
	template<typename T>
		requires number<T>
	const tvec2<T> tvec2<T>::unit_y{ 0, 1 };
	template<typename T>
		requires number<T>
	const tvec2<T> tvec2<T>::zero{ 0, 0 };

	using vec2 = tvec2<float>;
	using ivec2 = tvec2<int>;

	template<typename T>
		requires number<T>
	using tsize2 = tvec2<T>;

	using size2 = tsize2<float>;
	using isize2 = tsize2<int>;

	template<typename T>
		requires number<T>
	struct trect final
	{
		T x;
		T w;
		T y;
		T h;

		cat::tsize2<T> size() const
		{
			return cat::tsize2<T>{ w, h };
		}
		cat::tvec2<T> position() const
		{
			return cat::tvec2<T>{ x, y };
		}
		cat::tvec2<T> center() const
		{
			return cat::tvec2<T>{ x + w / 2, y + h / 2 };
		}

		void move(tvec2<T> vec)
		{
			x += vec.x;
			y += vec.y;
		}
		void move_to(tvec2<T> pos)
		{
			x = pos.x;
			y = pos.y;
		}
		void scale(T scale)
		{
			w *= scale;
			h *= scale;
		}
		void scale(tsize2<T> size)
		{
			w *= size.x;
			h *= size.y;
		}
		void scale_to(tsize2<T> size)
		{
			w = size.x;
			h = size.y;
		}

		static bool overlaps(tvec2<T> point, trect<T> rect)
		{
			if (point.x > rect.x && point.x < rect.x + rect.w)
				if (point.y > rect.y && point.y < rect.y + rect.h)
					return true;

			return false;
		}
		static bool overlaps(trect<T> rect1, trect<T> rect2)
		{
			if (rect1.x > rect2.x - rect1.w && rect1.x < rect2.x + rect2.w)
				if (rect1.y > rect2.y - rect1.h && rect1.y < rect2.y + rect2.h)
					return true;

			return false;
		}
	};

	using rect = trect<float>;
	using irect = trect<int>;
}

#endif
