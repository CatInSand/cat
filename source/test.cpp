#include "cat/test.h"

#include <iostream>
#include <vector>

#include <unordered_map>

namespace cat
{
	namespace test
	{
		using suite_t = std::vector<std::unique_ptr<_test_t>>;

		std::unordered_map<std::string, suite_t>& SuiteMap()
		{
			static std::unordered_map<std::string, suite_t> suiteList{};
			return suiteList;
		}

		_test_t::_test_t(const std::string& name)
			: m_name{ name }
		{
		}
		void _test_t::InternalRegister(std::unique_ptr<_test_t>&& pTest, const std::string& suiteName)
		{
			SuiteMap()[suiteName].push_back(std::move(pTest));
		}

		std::vector<std::pair<std::string, std::string>> FailedTests()
		{
			std::vector<std::pair<std::string, std::string>> failedTests{};

			for (const auto& [suiteName, suite] : SuiteMap())
			{
				for (const auto& pTest : suite)
				{
					if (!pTest->m_succeeded)
					{
						failedTests.push_back({ suiteName, pTest->m_name });
					}
				}
			}

			return failedTests;
		}
		void RunTest(const std::unique_ptr<_test_t>& pTest, uint32_t currentTest, uint32_t totalTests)
		{
			std::cout << "\"" << pTest->m_name << "\" (" << currentTest << "/" << totalTests << "): ";
			try
			{
				pTest->run();
				pTest->m_succeeded = true;
				std::cout << "[SUCCESS]\n";
			}
			catch (const std::exception& e)
			{
				std::cout << "[FAILED]\n";
				std::cout << "\tEXCEPTION THROWN!: " << e.what() << "\n";
			}
		}
		void RunTestSuite(const suite_t& suite, const std::string& suiteName)
		{
			const uint32_t totalTests{ static_cast<uint32_t>(suite.size()) };
			uint32_t currentTest{ 0 };

			std::cout << "\n";
			std::cout << "Running suite: \"" << suiteName << "\" (" << currentTest << "/" << totalTests << ")\n\n";
			for (const std::unique_ptr<_test_t>& pTest : suite)
			{
				++currentTest;
				RunTest(pTest, currentTest, totalTests);
			}
			std::cout << "\n";
		}
		void run_all()
		{
			if (SuiteMap().size() < 1)
			{
				std::cout << "\n- - - - - - - - - - - - - - - - - -\n";
				std::cout << "CAT - - - - No tests to run - - - -\n";
				std::cout << "- - - - - - - - - - - - - - - - - -\n\n";
				return;
			}

			uint32_t totalTestCount{ 0 };
			for (const auto& [suiteName, suite] : SuiteMap())
			{
				totalTestCount += suite.size();
			}

			std::cout << "\n- - - - - - - - - - - - - - - - - - -\n";
			std::cout << "CAT - - - - Running all tests - - - -\n";
			std::cout << "- - - - - - - - - - - - - - - - - - -\n\n";
			for (const auto& [suiteName, suite] : SuiteMap())
			{
				RunTestSuite(suite, suiteName);
			}

			std::cout << "- - - - - - - - - - - - - - - - - - -\n\n";

			auto failedTests{ FailedTests() };
			if (failedTests.empty())
			{
				std::cout << "All tests succeeded\n";
			}
			else
			{
				std::cout << "Tests failed (" << failedTests.size() << "/" << totalTestCount << ")\n";

				for (const auto& [suiteName, name] : failedTests)
				{
					std::cout << "Suite: \"" << suiteName << "\", test: \"" << name << "\"\n";
				}
			}
			std::cout << "\n\n- - - - - - - - - - - - - - - - - -\n";
			std::cout << "CAT - - - - End of testing  - - - -\n";
			std::cout << "- - - - - - - - - - - - - - - - - -\n\n";
		}
	}
}