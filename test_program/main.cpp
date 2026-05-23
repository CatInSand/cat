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
CATTEST(CombinationAllAnyNoTest)
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


std::string GenerateString(uint32_t size)
{
	std::string result{};

	for (uint32_t index{ 0 }; index < size; ++index)
	{
		result.push_back(' ' + rand() % ('~' - ' '));
	}

	return result;
}

CATTEST(HashCollisionsTest)
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
CATTEST(HashEqualityTest)
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

CATTEST(HashLiteralTest)
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
	cat::test::RunAll();
}