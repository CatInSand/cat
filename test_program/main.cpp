#include <iostream>

#include "test.h"
#include "algorithm.h"

#include <vector>

CATTEST(AlgorithmRemoveDuplicatesTest)
{
	std::vector<float> numbers{ 0.f, 1.2f, 17.651f, 7.f, -19.f, -0.256f, 1.2f, 0.5f, 0.5f, -17.651f };

	numbers.erase(cat::remove_duplicates(numbers.begin(), numbers.end()), numbers.end());
	if (cat::any_combinations(numbers.begin(), numbers.end(), [](const float& v1, const float& v2) { return v1 == v2; }))
	{
		throw std::runtime_error("Vector still contains duplicates");
	}
}

CATTEST(BaseTest)
{
}

int main()
{
	cat::test::RunAll();
}