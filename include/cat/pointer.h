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
	};

	template<typename T>
	class _owning_block
	{
	public:
		_owning_block(std::unique_ptr<T>&& uptr)
			: m_uptr{ std::move(uptr) }
		{
		}

		void delete_resource()
		{
			m_uptr.reset(nullptr);
		}

		std::unique_ptr<T> m_uptr;
	};

	template<typename T>
	class non_owning_ptr
	{
	public:
		non_owning_ptr(std::shared_ptr<_owning_block<T>>& block)
			: m_block{ block }
		{
		}

		bool valid()
		{
			return m_block->m_uptr;
		}

	private:
		std::shared_ptr<_owning_block<T>> m_block;
	};

	template<typename T>
	class owning_ptr
	{
	public:
		owning_ptr(std::unique_ptr<T>&& uptr)
			: m_block{ std::make_shared<_owning_block<T>>(std::move(uptr)) }
		{
		}
		~owning_ptr()
		{
			m_block->delete_resource();
		}

		non_owning_ptr<T> get_reference()
		{
			return non_owning_ptr<T>{ m_block };
		}
		
	private:
		std::shared_ptr<_owning_block<T>> m_block;
	};
}

#endif
