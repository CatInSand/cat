#include "test.h"

#include <iostream>
#include <vector>

#include <unordered_map>

namespace cat
{
	namespace test
	{
		using tests_type = std::unordered_map<std::string, std::vector<std::unique_ptr<_Test>>>;

		_Test::_Test(const std::string& name, const std::string& suiteName)
			: m_Name{ name }
			, m_SuiteName{ suiteName }
		{
		}

		tests_type& TestsInstance()
		{
			static tests_type gTests{};
			return gTests;
		}

		_Test* _Test::_InternalRegister(std::unique_ptr<_Test>&& pTest)
		{
			TestsInstance()[pTest->m_SuiteName].push_back(std::move(pTest));
			return nullptr;
		}

		void RunTest(const std::unique_ptr<_Test>& pTest, int currentTest, int totalTests, std::vector<_Test*>& testsFailed)
		{
			std::cout << "\"" << pTest->m_Name << "\" (" << currentTest << "/" << totalTests << "): ";
			try
			{
				pTest->Run();
				std::cout << "[SUCCESS]\n";
			}
			catch (const std::exception& e)
			{
				testsFailed.push_back(pTest.get());
				std::cout << "[FAILED]\n";
				std::cout << "\t\EXCEPTION THROWN!: " << e.what() << "\n";
			}
		}
		void RunTestSuite(const std::string& suiteName, std::vector<_Test*>& testsFailed)
		{
			const int totalTests{ static_cast<int>(TestsInstance().at(suiteName).size()) };
			int currentTest{ 0 };

			std::cout << "\n";
			std::cout << "Running suite: \"" << suiteName << "\" (" << currentTest << "/" << totalTests << ")\n\n";
			for (const auto& pTest : TestsInstance().at(suiteName))
			{
				++currentTest;
				RunTest(pTest, currentTest, totalTests, testsFailed);
			}
			std::cout << "\n";
		}
		void RunAll()
		{
			std::vector<_Test*> testsFailed{};
			int totalTests{ 0 };
			for (const auto& [suiteName, suite] : TestsInstance())
			{
				totalTests += suite.size();
			}

			if (TestsInstance().size() < 1)
			{
				std::cout << "\n- - - - - - - - - - - - - - - - - -\n";
				std::cout << "CAT - - - - No tests to run - - - -\n";
				std::cout << "- - - - - - - - - - - - - - - - - -\n\n";
				return;
			}

			std::cout << "\n- - - - - - - - - - - - - - - - - - -\n";
			std::cout << "CAT - - - - Running all tests - - - -\n";
			std::cout << "- - - - - - - - - - - - - - - - - - -\n\n";
			for (const auto& [suiteName, suite] : TestsInstance())
			{
				RunTestSuite(suiteName, testsFailed);
			}

			std::cout << "- - - - - - - - - - - - - - - - - - -\n\n";
			if (testsFailed.empty())
			{
				std::cout << "All tests succeeded\n";
			}
			else
			{
				std::cout << "Tests failed (" << testsFailed.size() << "/" << totalTests << ")\n";
				for (_Test* pTest : testsFailed)
				{
					std::cout << "Suite: \"" << pTest->m_SuiteName << "\", test: \"" << pTest->m_Name << "\"\n";
				}
			}
			std::cout << "\n\n- - - - - - - - - - - - - - - - - -\n";
			std::cout << "CAT - - - - End of testing  - - - -\n";
			std::cout << "- - - - - - - - - - - - - - - - - -\n\n";
		}
	}
}