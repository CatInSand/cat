#include <iostream>

#include "test.h"

CATTEST(AlgorithmTest)
{
	throw std::runtime_error("something went wrong");
	return;
}

int main()
{
	cat::test::RunAll();
}