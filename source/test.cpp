#include "test.h"

#include <iostream>
#include <vector>

namespace cat
{
	namespace test
	{
		Test::Test(const std::string& name)
			: m_Name{ name }
		{
		}

		std::vector<std::unique_ptr<Test>>& GTest()
		{
			static std::vector<std::unique_ptr<Test>> gTests{};
			return gTests;
		}

		Test* _InternalRegister(std::unique_ptr<Test>&& pTest)
		{
			GTest().push_back(std::move(pTest));
			return GTest().back().get();
		}
		void RunAll()
		{
			std::vector<std::string> testsFailed{};
			const int totalTests{ static_cast<int>(GTest().size()) };
			int currentTest{ 0 };

			if (totalTests < 1)
			{
				std::cout << "\nCAT - - - - No tests to run - - - - \n\n";
				return;
			}

			std::cout << "\nCAT - - - - Running all tests - - - - \n\n";
			for (auto& pTest : GTest())
			{
				++currentTest;
				std::cout << "Running test: \"" << pTest->m_Name << "\" (" << currentTest << "/" << totalTests << ")\n";
				try
				{
					pTest->Run();
					std::cout << "[SUCCESS]\n";
				}
				catch (const std::exception& e)
				{
					testsFailed.push_back(pTest->m_Name);
					std::cout << "Exception thrown: " << e.what() << "\n";
					std::cout << "[FAILED]\n";
				}
			}
			std::cout << "\n";
			if (testsFailed.empty())
			{
				std::cout << "All tests succeeded\n";
			}
			else
			{
				std::cout << "(" << testsFailed.size() << "/" << totalTests << ") tests failed\n";
				for (std::string& name : testsFailed)
				{
					std::cout << "\"" << name << "\"\n";
				}
			}
			std::cout << "\nCAT - - - - End of testing - - - - \n\n";
		}
	}
}