#ifndef CAT_POINTER_H
#define CAT_POINTER_H

namespace cat
{
	/*
	Template representation of a raw pointer
	*/
	template<typename T>
	using ptr = T*;

	template<typename T>
	using cptr = ptr<const T>;

	template<typename T>
	class _basic_ptr {
	public:
		_basic_ptr(ptr<T> pData)
			: m_ptr{ pData }
		{}
		_basic_ptr(std::nullptr_t)
			: m_ptr{ nullptr }
		{}
		_basic_ptr()
			: _basic_ptr(nullptr)
		{}
		_basic_ptr(const _basic_ptr& other) = default;
		_basic_ptr(_basic_ptr&& other) noexcept = default;
		virtual ~_basic_ptr() = default;

		_basic_ptr& operator=(std::nullptr_t)
		{
			m_ptr = nullptr;
		}
		_basic_ptr& operator=(const _basic_ptr& other) = default;
		_basic_ptr& operator=(_basic_ptr&& other) = default;

		T& operator*() const { return *m_ptr; }
		ptr<T> operator->() const { return m_ptr; }
		operator bool() const { return m_ptr; }

		ptr<T>& get()
		{
			return m_ptr;
		}

	protected:
		ptr<T> m_ptr;
	};

	template<class T1, class T2>
	bool operator==(const _basic_ptr<T1>& x, const _basic_ptr<T2>& y)
	{
		return x.m_ptr == y.m_ptr;
	}
	template<class T1, class T2>
	bool operator!=(const _basic_ptr<T1>& x, const _basic_ptr<T2>& y)
	{
		return x.m_ptr != y.m_ptr;
	}
	template<class T1, class T2>
	bool operator<(const _basic_ptr<T1>& x, const _basic_ptr<T2>& y)
	{
		return x.m_ptr < y.m_ptr;
	}
	template<class T1, class T2>
	bool operator<=(const _basic_ptr<T1>& x, const _basic_ptr<T2>& y)
	{
		return x.m_ptr <= y.m_ptr;
	}
	template<class T1, class T2>
	bool operator>(const _basic_ptr<T1>&x, const _basic_ptr<T2>&y)
	{
		return x.m_ptr > y.m_ptr;
	}
	template<class T1, class T2>
	bool operator>=(const _basic_ptr<T1>& x, const _basic_ptr<T2>& y)
	{
		return x.m_ptr >= y.m_ptr;
	}
}

#endif
