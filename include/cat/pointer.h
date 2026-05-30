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

	/*
	Reference-like pointer that allows reassignment using operator=
	*/
	/*template<typename T>
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
		ref_ptr& operator=(ptr<T> p_data)
		{
			_basic_ptr<T>::m_ptr = p_data;
		}
		ref_ptr& operator=(std::nullptr_t)
		{
			_basic_ptr<T>::m_ptr = nullptr;
		}
		ref_ptr& operator=(const ref_ptr& other) = default;
		ref_ptr& operator=(ref_ptr&& other) noexcept = default;

		operator const ptr<T>& () const { return _basic_ptr<T>::m_ptr; }
	};*/

	template<typename T>
	class _owning_block final
	{
	public:
		_owning_block(std::unique_ptr<T>&& uptr)
			: m_uptr{ std::move(uptr) }
		{
		}

		void delete_resource()
		{
			m_uptr.release();
		}

		std::unique_ptr<T> m_uptr;
	};

	template<typename T>
	class ref_ptr final
	{
	public:
		ref_ptr(std::shared_ptr<_owning_block<T>>& block)
			: m_block{ block }
		{
		}

		T& operator*() const { return *(m_block->m_uptr); }
		ptr<T> operator->() const { return m_block->m_uptr; }

		explicit operator bool() const { return m_block->m_uptr; }

		bool operator==(const ref_ptr<T>& other) { return m_block->m_uptr == other.m_block->m_uptr; }
		bool operator!=(const ref_ptr<T>& other) { return m_block->m_uptr != other.m_block->m_uptr; }
		bool operator<(const ref_ptr<T>& other) { return m_block->m_uptr < other.m_block->m_uptr; }
		bool operator<=(const ref_ptr<T>& other) { return m_block->m_uptr <= other.m_block->m_uptr; }
		bool operator>(const ref_ptr<T>&other) { return m_block->m_uptr > other.m_block->m_uptr; }
		bool operator>=(const ref_ptr<T>&other) { return m_block->m_uptr >= other.m_block->m_uptr; }

		ptr<T> get()
		{
			return m_block->m_uptr.get();
		}
		bool valid()
		{
			return m_block->m_uptr != nullptr;
		}

	private:
		std::shared_ptr<_owning_block<T>> m_block;
	};

	template<typename T>
	class refable_ptr final
	{
	public:
		refable_ptr(std::unique_ptr<T>&& uptr)
			: m_block{ std::make_shared<_owning_block<T>>(std::move(uptr)) }
		{}
		refable_ptr()
			: refable_ptr(nullptr)
		{}
		~refable_ptr()
		{
			m_block->delete_resource();
		}
		refable_ptr(const refable_ptr& other) = delete;
		refable_ptr(refable_ptr&& other) noexcept = default;
		refable_ptr& operator=(const refable_ptr& other) = delete;
		refable_ptr& operator=(refable_ptr&& other) noexcept = default;

		T& operator*() const { return *(m_block->m_uptr); }
		ptr<T> operator->() const { return m_block->m_uptr; }

		explicit operator bool() const { return m_block->m_uptr; }

		bool operator==(const refable_ptr<T>& other) { return m_block->m_uptr == other.m_block->m_uptr; }
		bool operator!=(const refable_ptr<T>& other) { return m_block->m_uptr != other.m_block->m_uptr; }
		bool operator<(const refable_ptr<T>& other) { return m_block->m_uptr < other.m_block->m_uptr; }
		bool operator<=(const refable_ptr<T>& other) { return m_block->m_uptr <= other.m_block->m_uptr; }
		bool operator>(const refable_ptr<T>& other) { return m_block->m_uptr > other.m_block->m_uptr; }
		bool operator>=(const refable_ptr<T>& other) { return m_block->m_uptr >= other.m_block->m_uptr; }

		ref_ptr<T> get_reference()
		{
			return ref_ptr<T>{ m_block };
		}

		ptr<T> get()
		{
			return m_block->m_uptr.get();
		}
		ptr<T> release()
		{
			return m_block->m_uptr.release();
		}
		void reset(ptr<T> pointer)
		{
			m_block->m_uptr.reset(pointer);
		}
		void swap(refable_ptr& other) noexcept
		{
			m_block->m_uptr.swap(other->m_block->m_uptr);
		}
		
	private:
		std::shared_ptr<_owning_block<T>> m_block;
	};
}

#endif
