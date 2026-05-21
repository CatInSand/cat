#ifndef CAT_TEST_H
#define CAT_TEST_H

#include <memory>
#include <string>

namespace cat
{
	namespace test
	{
		class _Test
		{
		public:
			virtual ~_Test() = default;
			virtual void Run() const = 0;

			const std::string m_Name;
			const std::string m_SuiteName;

		protected:
			_Test(const std::string& name, const std::string& suiteName);

			static _Test* _InternalRegister(std::unique_ptr<_Test>&& pTest);

			template<typename T>
			static T* Register()
			{
				return dynamic_cast<T*>(_InternalRegister(std::make_unique<T>()));
			}
		};

		void RunAll();
	}
}

#define CATTEST(test_suite, test_name)												\
namespace cat { namespace test { namespace _prepro_names { namespace test_suite	{	\
class test_name final : public _Test												\
{																					\
public:																				\
	test_name() : _Test(#test_name, #test_suite) {}									\
	virtual ~test_name() = default;													\
	virtual void Run() const override;												\
private:																			\
	static test_name* m_pInstance;													\
};																					\
test_name* test_name::m_pInstance{ Register<test_name>() };							\
}}}}																				\
void cat::test::_prepro_names::test_suite::test_name::Run() const

#endif