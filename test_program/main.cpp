#include <iostream>
#include <vector>
#include <format>

#include "cat/test.h"
#include "cat/algorithm.h"
#include "cat/hash.h"
#include "cat/pointer.h"

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
CATTEST(Algorithms, FindNAllIfTest)
{
	TestLists testLists{};
	int index{ 0 };

	for (auto& list : testLists.lists)
	{
		auto vec{ cat::find_n_if(list.begin(), list.end(), 4, [](const float& v) { return v >= 1.f; }) };
		auto vec2{ cat::find_all_if(list.begin(), list.end(), [](const float& v) { return v >= 1.f; }) };

		if (vec != vec2)
			throw std::runtime_error("find_n_if with n = 4 different from find_all_if");

		++index;
	}
}
CATTEST(Algorithms, ContainsNIfTest)
{
	TestLists testLists{};
	
	if (!cat::contains_n_if(testLists.lists[0].begin(), testLists.lists[0].end(), 1, [](const float& v) { return v >= 1.f; }))
		throw std::runtime_error("contains_n_if bad :(");
	if (!cat::contains_n_if(testLists.lists[1].begin(), testLists.lists[1].end(), 1, [](const float& v) { return v >= 1.f; }))
		throw std::runtime_error("contains_n_if bad :(");
	if (!cat::contains_n_if(testLists.lists[2].begin(), testLists.lists[2].end(), 1, [](const float& v) { return v >= 1.f; }))
		throw std::runtime_error("contains_n_if bad :(");
	if (cat::contains_n_if(testLists.lists[3].begin(), testLists.lists[3].end(), 1, [](const float& v) { return v >= 1.f; }))
		throw std::runtime_error("contains_n_if bad :(");
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

CATTEST(Pointers, BasicPointerTest)
{
	float pi{ 3.1415f };
	cat::ptr<float> pointer{ &pi };

	cat::_basic_ptr<float> basic_pointer{ &pi };

	if(pointer != basic_pointer)
		throw std::runtime_error("Basic pointer not equal to raw pointer to same object");

	std::vector<float> testList{ 1.f, 2.f, 3.f, 4.f, 5.f, 1.f, 2.f, 6.f, 7.f, 1.f, 8.f };

	auto vecNormal{ cat::find_all_if(
		testList.begin(),
		testList.end(),
		[](float& v) { return v > 2.f; })
	};
	
	auto vecBasicPtr{ cat::find_all_if(
		cat::_basic_ptr<float>{ testList.data() },
		cat::_basic_ptr<float>{ testList.data() + testList.size() },
		[](float& v) { return v > 2.f; })
	};

	if(vecBasicPtr.size() != vecNormal.size())
		throw std::runtime_error("Algorithm not properly executed with _basic_ptr");

	for (size_t index{ 0 }; index < vecBasicPtr.size(); ++index)
	{
		if(*vecBasicPtr[index] != *vecNormal[index])
			throw std::runtime_error("Algorithm not properly executed with _basic_ptr");
	}
}
CATTEST(Pointers, RefablePointerTest)
{
	constexpr float value{ 3.1415f };

	cat::refable_ptr<float> owningPointer{ std::make_unique<float>(value) };

	if (owningPointer.get() == nullptr)
		throw std::runtime_error("refable_ptr has underlying pointer nullptr");
	if(*owningPointer != value)
		throw std::runtime_error("refable_ptr dereference yields wrong value");

	cat::ref_ptr<float> referencePointer{ owningPointer.get_reference() };

	if(!referencePointer.valid())
		throw std::runtime_error("ref_ptr invalid right after creation");
	if(owningPointer.get() != referencePointer.get())
		throw std::runtime_error("ref_ptr and refable_ptr do not point to same data");

	cat::ref_ptr<float> secondReferencePointer{ owningPointer.get_reference() };

	if (referencePointer != secondReferencePointer)
		throw std::runtime_error("ref_ptr's from same refable_ptr are not equal");
	if (referencePointer.get() != secondReferencePointer.get())
		throw std::runtime_error("ref_ptr's from same refable_ptr do not point to same data");
}

int main()
{
	cat::test::run_all();
}