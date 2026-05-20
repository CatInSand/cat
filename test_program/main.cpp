#include <iostream>

#include "test.h"
#include "algorithm.h"

#include <vector>

struct TestLists
{
	std::vector<std::vector<float>> lists{
		{ 0.f, 1.2f, 17.651f, 7.f, -19.f, -0.256f, 1.2f, 0.5f, 0.5f, -17.651f },	// duplicates
		{ 0.f, 1.2f, 17.651f, 7.f, -19.f, -0.256f, 0.5f, -17.651f },				// no duplicates
		{ 0.f, 1.2f, 17.651f, 0.f, 7.f, 0.f,-19.f, -0.256f, 0.5f, 0.f,-17.651f },	// four of a kind
		{},																			// empty
	};
};

CATTEST(AlgorithmRemoveDuplicatesTest)
{
	TestLists testLists{};

	for (auto& list : testLists.lists)
	{
		list.erase(cat::remove_duplicates(list.begin(), list.end()), list.end());

		for (auto it1{ list.cbegin() }; it1 != list.cend(); ++it1)
			for (auto it2{ it1 + 1 }; it2 != list.cend(); ++it2)
				if (*it1 == *it2)
					throw std::runtime_error("list contains duplicates after calling remove_duplicates");
	}

	testLists = {};

	constexpr float testValue{ 0.f };

	for (auto& list : testLists.lists)
	{
		list.erase(cat::remove_duplicates(list.begin(), list.end(), testValue), list.end());

		for (auto it1{ list.cbegin() }; it1 != list.cend(); ++it1)
			for (auto it2{ it1 + 1 }; it2 != list.cend(); ++it2)
				if (*it1 == *it2 && *it1 == testValue)
					throw std::runtime_error("list contains duplicates after calling remove_duplicates for specific value");
	}
}

CATTEST(BaseTest)
{
}

int main()
{
	cat::test::RunAll();
}