#include <iostream>
#include <vector>
#include <format>

#include "test.h"
#include "algorithm.h"
#include "hash.h"

struct TestLists
{
	std::vector<std::vector<float>> lists{
		{ 0.f, 1.2f, 17.651f, 7.f, -19.f, -0.256f, 1.2f, 0.5f, 0.5f, -17.651f },	// duplicates
		{ 0.f, 1.2f, 17.651f, 7.f, -19.f, -0.256f, 0.5f, -17.651f },				// no duplicates
		{ 0.f, 1.2f, 17.651f, 0.f, 7.f, 0.f,-19.f, -0.256f, 0.5f, 0.f,-17.651f },	// four of a kind
		{},																			// empty
	};
};

CATTEST(Algorithms, CombinationAllAnyNoTest)
{
	TestLists testLists{};

	auto binaryPredicate{
		[](const float& v1, const float& v2) { return v1 + 1 >= v2; }
	};

	for (auto& list : testLists.lists)
	{
		bool allCombinations{ cat::all_combinations(list.begin(), list.end(), binaryPredicate) };
		bool anyCombinations{ cat::any_combinations(list.begin(), list.end(), binaryPredicate) };
		bool noCombinations{ cat::no_combinations(list.begin(), list.end(), binaryPredicate) };

		if (list.size() == 0)
		{
			if(allCombinations != true || anyCombinations != false || noCombinations != true)
				throw std::runtime_error("incorrect for empty list");

			continue;
		}

		if (allCombinations)
		{
			if(!anyCombinations)
				throw std::runtime_error("all_combinations true and any_combinations false");

			if (noCombinations)
				throw std::runtime_error("all_combinations true and no_combinations true");
		}
		else if (anyCombinations)
		{
			if (noCombinations)
				throw std::runtime_error("any_combinations true and no_combinations true");
		}
		else if(noCombinations)
		{
			// nothing left to check
		}
		else
		{
			throw std::runtime_error("all_, any_, and no_combinations all false");
		}
	}
}
CATTEST(Algorithms, FindDuplicateTest)
{
	TestLists testLists{};
	int index{ 0 };

	for (auto& list : testLists.lists)
	{
		auto it{ cat::find_duplicate(list.begin(), list.end()) };
		if (it != list.end())
		{
			if (it != std::find(list.begin(), list.end(), *it))
				throw std::runtime_error("find_duplicate did not find first element");
			if(!cat::has_duplicate(list.begin(), list.end()))
				throw std::runtime_error("find_duplicate returned iterator while has_duplicate returned false");
			if(index == 1 || index == 3)
				throw std::runtime_error("find_duplicate returned iterator while no duplcates were present");
		}

		++index;
	}
}
CATTEST(Algorithms, FindCountDuplicatesTest)
{
	TestLists testLists{};
	int index{ 0 };

	for (auto& list : testLists.lists)
	{
		auto it1{ cat::find_n_duplicates(list.begin(), list.end(), 1) };
		if (it1 == list.end() && !list.empty())
			throw std::runtime_error("find_n_duplicates couldn't find 1 element in non-empty list");

		auto it2{ cat::find_n_duplicates(list.begin(), list.end(), 2) };
		if (it2 != list.end())
		{
			if(!cat::has_duplicate(list.begin(), list.end()))
				throw std::runtime_error("find_n_duplicates found 2 elements in list without duplicates");
			if(it2 != std::find(list.begin(), list.end(), *it2))
				throw std::runtime_error("find_n_duplicates did not return first result");
		}

		auto it4{ cat::find_n_duplicates(list.begin(), list.end(), 4) };
		if (it4 != list.end())
			if (index != 2)
				throw std::runtime_error("find_n_duplicates found iterator while less than count duplicates were present");

		auto it5{ cat::find_n_duplicates(list.begin(), list.end(), 5) };
		if (it5 != list.end())
			throw std::runtime_error("find_n_duplicates found 5 elements while no list contained 5 duplicates");

		++index;
	}
}
CATTEST(Algorithms, RemoveDuplicatesTest)
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
CATTEST(Algorithms, FindNthIfTest)
{
	TestLists testLists{};
	int index{ 0 };

	for (auto& list : testLists.lists)
	{
		auto it{ cat::find_nth_if(list.begin(), list.end(), 2, [](const float& v) { return v >= 1.f; }) };
		if (it != list.end())
			if(index == 3)
				throw std::runtime_error("find_nth_if found iterator while not enough elements were present");

		if (cat::find_nth_if(list.begin(), list.end(), 1, [](const float& v) { return v >= 1.f; }) !=
			std::find_if(list.begin(), list.end(), [](const float& v) { return v >= 1.f; }))
		{
			throw std::runtime_error("find_nth_if with n = 1 does not behave like std::find_if");
		}

		++index;
	}
}
CATTEST(Algorithms, FindNIfTest)
{
	TestLists testLists{};
	int index{ 0 };

	for (auto& list : testLists.lists)
	{
		auto vec{ cat::find_n_if(list.begin(), list.end(), 4, [](const float& v) { return v >= 1.f; }) };

		++index;
	}
}

std::string GenerateString(uint32_t size)
{
	std::string result{};

	for (uint32_t index{ 0 }; index < size; ++index)
	{
		result.push_back(' ' + rand() % ('~' - ' '));
	}

	return result;
}

CATTEST(Hashing, CollisionsTest)
{
	constexpr int stringCount{ 1000 };
	constexpr int maxStringLength{ 10 };
	std::vector<std::string> stringList{ stringCount };

	for (int index{ 0 }; index < stringCount; ++index)
	{
		stringList.push_back(GenerateString(rand() % maxStringLength + 1));
	}

	//duplicate strings would create duplicate hashes
	stringList.erase(cat::remove_duplicates(stringList.begin(), stringList.end()), stringList.end());

	auto it_pair{ cat::combination_compare(stringList.begin(), stringList.end(),
		[](const std::string& v1, const std::string& v2) { return cat::make_hash(v1) == cat::make_hash(v2); })
	};

	if (it_pair.first != stringList.end())
		throw std::runtime_error(std::format("Hash collision with only {} strings", stringCount));
}
CATTEST(Hashing, EqualityTest)
{
	constexpr int stringCount{ 1000 };
	constexpr int maxStringLength{ 5 };
	std::vector<std::string> stringList{ stringCount };

	for (int index{ 0 }; index < stringCount; ++index)
	{
		stringList.push_back(GenerateString(rand() % maxStringLength + 1));
	}

	if (!cat::has_duplicate(stringList.begin(), stringList.end()))
		throw std::runtime_error("Test failed to create duplicate strings");

	auto it_pair{ cat::combination_compare(stringList.begin(), stringList.end(),
		[](const std::string& v1, const std::string& v2)
		{ 
			return v1 == v2 && cat::make_hash(v1) != cat::make_hash(v2);
		})
	};

	if (it_pair.first != stringList.end())
		throw std::runtime_error("Two equal strings did not have equal hashes");
}

using namespace cat::hash_literals;

CATTEST(Hashing, LiteralTest)
{
	std::vector<cat::hash_t> hashes{
		"cats"_h,
		"are"_h,
		"pretty"_h,
		"cool"_h
	};

	if (hashes[0] != cat::make_hash("cats")
		|| hashes[1] != cat::make_hash("are")
		|| hashes[2] != cat::make_hash("pretty")
		|| hashes[3] != cat::make_hash("cool")
		)
	{
		throw std::runtime_error("hash literal not equal to sdbm_hash");
	}
}

int main()
{
	cat::test::run_all();
}