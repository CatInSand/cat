#ifndef CAT_POINTER_H
#define CAT_POINTER_H

namespace cat
{
	/*
	Template representation of a raw pointer
	*/
	template<typename T>
	using ptr = T*;

	/*
	Template representation of a raw pointer to const
	*/
	template<typename T>
	using cptr = ptr<const T>;

	template<typename T>
	class _basic_ptr
	{
	public:
		_basic_ptr(ptr<T> p_data)
			: m_ptr{ p_data }
		{}
		_basic_ptr(std::nullptr_t = nullptr)
			: m_ptr{ nullptr }
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

		operator const ptr<T>&() const { return m_ptr; }
		operator ptr<T>&() { return m_ptr; }

	protected:
		ptr<T> m_ptr;
	};
}

#endif
