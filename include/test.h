#ifndef CAT_TEST_H
#define CAT_TEST_H

#include <memory>
#include <string>

namespace cat
{
	namespace test
	{
		using _troll_t = uint8_t;

		class _test_t
		{
		public:
			virtual ~_test_t() = default;
			virtual void run() const = 0;

			const std::string m_name;
			bool m_succeeded{ false };

		protected:
			_test_t(const std::string& name);

			static void InternalRegister(std::unique_ptr<_test_t>&& pTest, const std::string& suiteName);

			template<typename T>
			static _troll_t Register(const std::string& suiteName)
			{
				InternalRegister(std::make_unique<T>(), suiteName);
				return _troll_t{};
			}
		};

		void run_all();
	}
}

#define CATTEST(suite_name, test_name)												\
namespace cat { namespace test { namespace _prepro_names { namespace suite_name	{	\
class test_name final : public _test_t												\
{																					\
public:																				\
	test_name() : _test_t(#test_name) {}											\
	virtual ~test_name() = default;													\
	virtual void run() const override;												\
private:																			\
	static _troll_t m_troll;														\
};																					\
_troll_t test_name::m_troll{ Register<test_name>(#suite_name) };					\
}}}}																				\
void cat::test::_prepro_names::suite_name::test_name::run() const

#endif