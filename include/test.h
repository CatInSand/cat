#ifndef CAT_TEST_H
#define CAT_TEST_H

#include <memory>
#include <string>

namespace cat
{
	namespace test
	{
		class Test
		{
		public:
			virtual ~Test() = default;
			virtual void Run() = 0;

			const std::string m_Name;

		protected:
			Test(const std::string& name);
		};

		Test* _InternalRegister(std::unique_ptr<Test>&& pTest);

		template<typename T>
		T* Register()
		{
			return dynamic_cast<T*>(_InternalRegister(std::make_unique<T>()));
		}

		void RunAll();
	}
}

#define CATTEST(test_name)													\
namespace cat { namespace test { namespace _prepro_names {					\
class test_name final : public Test											\
{																			\
public:																		\
	test_name() : Test(#test_name) {}										\
	virtual ~test_name() = default;											\
	virtual void Run() override;											\
	static test_name* m_pInstance;											\
};																			\
test_name* test_name::m_pInstance{ Register<test_name>() };					\
}}}																			\
void cat::test::_prepro_names::test_name::Run()


#endif