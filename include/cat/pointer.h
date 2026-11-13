#ifndef CAT_POINTER_H
#define CAT_POINTER_H

#include <memory>

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

	template<typename T>
	class ref_ptr final
	{
	public:
		ref_ptr(std::shared_ptr<std::unique_ptr<T>>& block)
			: m_pblock{ block }
		{
		}

		T& operator*() const { return *(*m_pblock); }
		ptr<T> operator->() const { return *m_pblock; }

		explicit operator bool() const { return *m_pblock; }

		bool operator==(const ref_ptr<T>& other) { return *m_pblock == *other.m_pblock; }
		bool operator!=(const ref_ptr<T>& other) { return *m_pblock != *other.m_pblock; }
		bool operator<(const ref_ptr<T>& other) { return *m_pblock < *other.m_pblock; }
		bool operator<=(const ref_ptr<T>& other) { return *m_pblock <= *other.m_pblock; }
		bool operator>(const ref_ptr<T>&other) { return *m_pblock > *other.m_pblock; }
		bool operator>=(const ref_ptr<T>&other) { return *m_pblock >= *other.m_pblock; }

		ptr<T> get()
		{
			return m_pblock->get();
		}
		bool valid()
		{
			return *m_pblock != nullptr;
		}

	private:
		std::shared_ptr<std::unique_ptr<T>> m_pblock;
	};

	template<typename T>
	class sticky_ptr final
	{
	public:
		sticky_ptr(std::unique_ptr<T>&& uptr)
			: m_pblock{ std::make_shared<std::unique_ptr<T>>(std::move(uptr)) }
		{}
		sticky_ptr()
			: sticky_ptr(nullptr)
		{}
		~sticky_ptr()
		{
			m_pblock->release();
		}
		sticky_ptr(const sticky_ptr& other) = delete;
		sticky_ptr(sticky_ptr&& other) noexcept = default;
		sticky_ptr& operator=(const sticky_ptr& other) = delete;
		sticky_ptr& operator=(sticky_ptr&& other) noexcept = default;

		T& operator*() const { return *(*m_pblock); }
		ptr<T> operator->() const { return *m_pblock; }

		explicit operator bool() const { return *m_pblock; }

		bool operator==(const sticky_ptr<T>& other) { return *m_pblock == *other.m_pblock; }
		bool operator!=(const sticky_ptr<T>& other) { return *m_pblock != *other.m_pblock; }
		bool operator<(const sticky_ptr<T>& other) { return *m_pblock < *other.m_pblock; }
		bool operator<=(const sticky_ptr<T>& other) { return *m_pblock <= *other.m_pblock; }
		bool operator>(const sticky_ptr<T>& other) { return *m_pblock > *other.m_pblock; }
		bool operator>=(const sticky_ptr<T>& other) { return *m_pblock >= *other.m_pblock; }

		ref_ptr<T> get_reference()
		{
			return ref_ptr<T>{ m_pblock };
		}

		ptr<T> get()
		{
			return m_pblock->get();
		}
		ptr<T> release()
		{
			return m_pblock->release();
		}
		void reset(ptr<T> pointer)
		{
			m_pblock->reset(pointer);
		}
		void swap(sticky_ptr& other) noexcept
		{
			m_pblock->swap(*other->m_pblock);
		}
		
	private:
		std::shared_ptr<std::unique_ptr<T>> m_pblock;
	};
}

#endif
