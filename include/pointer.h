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
		_basic_ptr& operator=(ptr<T> p_data)
		{
			m_ptr = p_data;
		}
		_basic_ptr& operator=(std::nullptr_t)
		{
			m_ptr = nullptr;
		}
		_basic_ptr& operator=(const _basic_ptr& other) = default;
		_basic_ptr& operator=(_basic_ptr&& other) noexcept = default;

		T& operator*() const { return *m_ptr; }
		ptr<T> operator->() const { return m_ptr; }

		operator const ptr<T>&() const { return m_ptr; }
		operator ptr<T>&() { return m_ptr; }

	protected:
		ptr<T> m_ptr;
	};

	/*
	A pointer wrapper that is constant except for assignment operations
	*/
	template<typename T>
	class ref_ptr : public _basic_ptr<T>
	{
	public:
		ref_ptr(ptr<T> p_data)
			: _basic_ptr(p_data)
		{}
		ref_ptr(std::nullptr_t = nullptr)
			: _basic_ptr(nullptr)
		{}

		ref_ptr(const ref_ptr& other) = default;
		ref_ptr(ref_ptr&& other) noexcept = default;
		virtual ~ref_ptr() = default;
		_basic_ptr& operator=(ptr<T> p_data)
		{
			m_ptr = p_data;
		}
		ref_ptr& operator=(std::nullptr_t)
		{
			m_ptr = nullptr;
		}
		ref_ptr& operator=(const ref_ptr& other) = default;
		ref_ptr& operator=(ref_ptr&& other) noexcept = default;

		operator const ptr<T>& () const { return m_ptr; }
	};
}

#endif
