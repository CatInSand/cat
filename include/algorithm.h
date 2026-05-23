#ifndef CAT_ALGORITHM_H
#define CAT_ALGORITHM_H

#include <algorithm>
#include <vector>

namespace cat
{
	//
	// - - - - Combinations - - - -
	//
	
	/*
	Find the first pair of elements that satisfy the predicate
	*/
	template<class ForwardIt, class BinaryPred>
	constexpr std::pair<ForwardIt, ForwardIt> combination_compare(ForwardIt first, ForwardIt last, BinaryPred p)
	{
		if (first == last)
			return { last, last };

		for (ForwardIt next{}; first != last - 1; ++first)
			for (next = first + 1; next != last; ++next)
				if (p(*first, *next))
					return { first, next };

		return { last, last };
	}

	/*
	Test if all pairs of elements satisfy the predicate
	*/
	template<class ForwardIt, class BinaryPred>
	constexpr bool all_combinations(ForwardIt first, ForwardIt last, BinaryPred p)
	{
		if (first == last)
			return true;

		for (ForwardIt next{}; first != last - 1; ++first)
		{
			next = first + 1;
			for (; next != last; ++next)
				if (!p(*first, *next))
					return false;
		}

		return true;
	}
	/*
	Test if any pair of elements satisfies the predicate
	*/
	template<class ForwardIt, class BinaryPred>
	constexpr bool any_combinations(ForwardIt first, ForwardIt last, BinaryPred p)
	{
		return combination_compare(first, last, p).first != last;
	}
	/*
	Test if no pairs of elements satisfy the predicate
	*/
	template<class ForwardIt, class BinaryPred>
	constexpr bool no_combinations(ForwardIt first, ForwardIt last, BinaryPred p)
	{
		return !any_combinations(first, last, p);
	}

	//
	// !Combinations - - - -
	//


	//
	// - - - - Duplicates - - - - 
	//

	/*
	Find the first element that appears more than once
	*/
	template<class ForwardIt>
	constexpr ForwardIt find_duplicate(ForwardIt first, ForwardIt last)
	{
		return combination_compare(first, last, [](auto v1, auto v2) { return v1 == v2; }).first;
	}
	/*
	Find the first element equal to val that appears more than once
	*/
	template<class ForwardIt, class T>
	constexpr ForwardIt find_duplicate(ForwardIt first, ForwardIt last, const T& val)
	{
		if (first == last)
			return last;

		auto it{ std::find(first, last, val) };
		if (it != last)
			if (std::find(it + 1, last, val) != last)
				return it;

		return last;
	}

	/*
	test if any elements appear more than once
	*/
	template<class ForwardIt>
	constexpr bool has_duplicate(ForwardIt first, ForwardIt last)
	{
		return find_duplicate(first, last) != last;
	}
	/*
	test if any elements equal to val appear more than once
	*/
	template<class ForwardIt, class T>
	constexpr bool has_duplicate(ForwardIt first, ForwardIt last, const T& val)
	{
		return find_duplicate(first, last, val) != last;
	}

	/*
	Find the latter instance of the first element that appears more than once
	*/
	template<class ForwardIt>
	constexpr ForwardIt find_late_duplicate(ForwardIt first, ForwardIt last)
	{
		return combination_compare(first, last, [](auto v1, auto v2) { return v1 == v2; }).second;
	}
	/*
	Find the latter instance of the first element equal to val that appears more than once
	*/
	template<class ForwardIt, class T>
	constexpr ForwardIt find_late_duplicate(ForwardIt first, ForwardIt last, const T& val)
	{
		if (first == last)
			return last;

		auto it{ std::find(first, last, val) };
		if (it != last)
			return std::find(it + 1, last, val);

		return last;
	}

	template<class ForwardIt, class Size>
	constexpr ForwardIt _find_n_duplicates_recursive(ForwardIt first, ForwardIt last, Size count)
	{
		if (count == 2)
			return find_duplicate(first, last);
		else
			for (ForwardIt next{}; first != last - count + 1; ++first)
			{
				next = _find_n_duplicates_recursive(first + 1, last, count - 1);
				if (next == last)
					return last;
				if (*first == *next)
					return first;
			}

		return last;
	}
	template<class ForwardIt, class Size, class T>
	constexpr ForwardIt _find_n_duplicates_recursive(ForwardIt first, ForwardIt last, Size count, const T& val)
	{
		if (count == 2)
			return find_duplicate(first, last, val);

		auto it{ std::find(first, last, val) };
		if (_find_n_duplicates_recursive(it + 1, last, count - 1, val) != last)
			return it;

		return last;
	}

	/*
	Find the first element that appears count times or more
	*/
	template<class ForwardIt, class Size>
	constexpr ForwardIt find_n_duplicates(ForwardIt first, ForwardIt last, Size count)
	{
		if (first == last || last - first < count)
			return last;

		if (count == 1)
			return first;

		return _find_n_duplicates_recursive(first, last, count);
	}
	/*
	Find the first element equal to val that appears count times or more
	*/
	template<class ForwardIt, class Size, class T>
	constexpr ForwardIt find_n_duplicates(ForwardIt first, ForwardIt last, Size count, const T& val)
	{
		if (first == last || last - first < count)
			return last;

		if (count == 1)
			return std::find(first, last, val);

		return _find_n_duplicates_recursive(first, last, count, val);
	}

	/*
	Remove duplicates of elements already found in the list
	*/
	template<class ForwardIt>
	constexpr ForwardIt remove_duplicates(ForwardIt first, ForwardIt last)
	{
		auto it{ find_duplicate(first, last) };
		if (it != last)
		{
			for (ForwardIt i{ it + 1 }; i != last; ++i)
				if (std::find(first, i, *i) == i)
					*(++it) = std::move(*i);

			*(++it) = std::move(*(last - 1));
		}

		return it;
	}
	/*
	Remove duplicates of elements equal to val already found in the list
	*/
	template<class ForwardIt, class T>
	constexpr ForwardIt remove_duplicates(ForwardIt first, ForwardIt last, const T& val)
	{
		first = find_duplicate(first, last, val);
		if (first != last)
		{
			for (ForwardIt i{ first + 1 }; i != last; ++i)
				if (*i != val)
					*(++first) = std::move(*i);

			*(++first) = std::move(*(last - 1));
		}

		return first;
	}

	//
	// !Duplicates - - - -
	//


	//
	// - - - - Find - - - -
	//

	/*
	Find the nth instance of an element that satisfies the predicate (n starts at 1)
	*/
	template<class ForwardIt, class Size, class UnaryPred>
	constexpr ForwardIt find_nth_if(ForwardIt first, ForwardIt last, Size count, const UnaryPred& pred)
	{
		if (first == last || count <= 0)
			return last;

		first = std::find_if(first, last, pred);
		for (Size index{ 0 }; first != last; first = std::find_if(first, last, pred))
			if (++index == count)
				return first;

		return last;
	}

	/*
	Find the nth instance of an element equal to val (n starts at 1)
	*/
	template<class ForwardIt, class Size, class T>
	constexpr ForwardIt find_nth(ForwardIt first, ForwardIt last, Size count, const T& val)
	{
		if (first == last || count <= 0)
			return last;

		first = std::find(first, last, val);
		for (Size index{ 0 }; first != last; first = std::find(first, last, val))
			if (++index == count)
				return first;

		return last;
	}

	/*
	Find the first n elements that satisfy the predicate
	*/
	template<class ForwardIt, class Size, class UnaryPred>
	constexpr std::vector<ForwardIt> find_n_if(ForwardIt first, ForwardIt last, Size count, const UnaryPred& pred)
	{
		std::vector<ForwardIt> result{};

		if (first == last || count <= 0)
			return result;

		for (Size index{ 0 }; first != last; ++first)
		{
			if (pred(*first))
			{
				if (++index <= count)
					result.push_back(first);
				else
					return result;
			}
		}

		return result;
	}

	/*
	Find the first all elements that satisfy the predicate
	*/
	template<class ForwardIt, class UnaryPred>
	constexpr std::vector<ForwardIt> find_all_if(ForwardIt first, ForwardIt last, const UnaryPred& pred)
	{
		std::vector<ForwardIt> result{};

		if (first == last)
			return result;

		for (; first != last; ++first)
			if (pred(*first))
				result.push_back(first);

		return result;
	}

	/*
	Test if at least n elements satisfy the predicate
	*/
	template<class ForwardIt, class Size, class UnaryPred>
	constexpr bool contains_n_if(ForwardIt first, ForwardIt last, Size count, const UnaryPred& pred)
	{
		return find_nth_if(first, last, count, pred) != last;
	}

	/*
	Test if at least n elements eqaul to val
	*/
	template<class ForwardIt, class Size, class T>
	constexpr bool contains_n(ForwardIt first, ForwardIt last, Size count, const T& val)
	{
		return contains_n_if(first, last, count, [&](const T& other) { return val == other; });
	}

	//
	// !Find - - - -
	//
}

#endif
