# cat
A quality of life library

This library is a collection of code I personally find useful and plan to use again, though it is programmed in such a way that it is usable in many contexts. It will grow as I find new pieces of code that are generally useful.

All code is contained in namespace "cat".


# headers

## algorithm.h

A collection of STL style algorithms.


### Combination algorithms

combination_compare: Takes a binary predicate and returns the first pair of elements that satisfies that predicate.

for_each_combination: Takes a binary function and applies it to each pair of elements.

all_combinations: Takes a binary predicate and returns true if all pairs of elements satisfy the predicate.

any_combinations: Takes a binary predicate and returns true if any pair of elements satisfies the predicate.

no_combinations: Takes a binary predicate and returns true if no pair of elements satisfies the predicate.



### Duplicates algorithms

All duplicates algorithms have an overload that takes a value and only tests for duplicates equal to that value.

find_duplicate: Returns the first element that appears more than once.

has_duplicate: Returns true if there are duplicate elements.

find_late_duplicate: Return the latter instance of the first element that appears more than once.

find_n_duplicates: Takes a count and returns the first element that appears count or more times.

remove_duplicates: Removes duplicate elements.



### Find algorithms

find_nth_if: Takes a count n and a predicate and returns the nth element that satisfies the predicate.

find_nth: Takes a count n and a value and returns the nth element equal to the value.

find_n_if: Takes a count n and a predicate and returns the first n elements that satisfy the predicate as a std::vector.

find_all_if: Takes a predicate and returns the all elements that satisfy the predicate as a std::vector.

contains_n_if: Takes a count n and a predicate and returns true if it contains n or more elements that satisfy the predicate.

contains_n: Takes a count n and a value and returns true if it contains n or more elements equal to the value.


## hash.h

String hashing functions and literal overload. Uses sdbm hash.

Call cat::make_hash(const std::string& string) or cat::make_hash(const char* string, size_t length) to create a hash of type cat::hash_t (uint32_t by default).
You can also add "using namespace cat::hash_literals;" to use the literal overload "_h" to convert strings to hash_t at compile time:

cat::hash_t nameHash{ "CatInSand"_h };

The namespace "cat::hash_literals" contains no other code so it is safe to use a using directive.

Specify the size of cat::hash_t by using #define with CAT_HASH_TYPE_UINT8, CAT_HASH_TYPE_UINT16, CAT_HASH_TYPE_UINT32 or CAT_HASH_TYPE_UINT64 before including cat/hash.h


## test.h

Helps with creating unit tests.

Call preprocessor define CATTEST to register a test:

CATTEST(suite_name, test_name)
{
	//test code
}

Run all tests using cat::test::run_all(). Any errors thrown inside the test body will be caught and the test will be set to have failed.


## pointer.h

Pointer wrappers and smart pointers.

cat::ptr\<T\>: Template representation of raw pointer.

cat::sticky_ptr\<T\>: Owning pointer that allows safe referencing with ref_ptr. All ref_ptr's return nullptr after sticky_ptr goes out of scope.

cat::ref_ptr\<T\>: Non-owning pointer that references a sticky_ptr.


## timer.h

Class cat::timer for tracking elapsed time.

cat::timer::timer(float duration): Constructs a timer with a duration.

cat::timer::update(float delta_time): Increments elapsed time by delta_time.

cat::timer::reset(): Sets elapsed time to 0.

cat::timer::reset(float duration): Sets elapsed time to 0 and sets duration.

cat::timer::set(float elapsed_time): Sets elapsed time.

cat::timer::set(float elapsed_time, float duration): Sets elapsed time and duration.

cat::timer::modulo_get(): Returns the number of times the duration has elapsed and sets elapsed time to the remainder.

cat::timer::decrement_get(): If duration has elapsed, returns true and decrements elapsed time by duration. Otherwise returns false.

cat::timer::elapsed(): Returns elapsed time.

cat::timer::remaining(): Returns remaining time.

cat::timer::duration(): Returns duration.

cat::timer::done(): Returns true if duration has elapsed.
