#ifndef CAT_HASH_H
#define CAT_HASH_H

#include <string>

namespace cat
{
	/*
	Type used to store string hashes
	*/
#ifdef CAT_HASH_TYPE_UINT8
	using hash_t = uint8_t;
#else
#ifdef CAT_HASH_TYPE_UINT16
	using hash_t = uint16_t;
#else
#ifdef CAT_HASH_TYPE_UINT32
	using hash_t = uint32_t;
#else
#ifdef CAT_HASH_TYPE_UINT64
	using hash_t = uint64_t;
#else
	using hash_t = uint32_t;
#endif
#endif
#endif
#endif

	/*
	Creates a hash from a c_string using the sdbm hash
	*/
	constexpr hash_t make_hash(const char* string, size_t length)
	{
		hash_t result{ 0 };

		for (size_t charIndex{ 0 }; charIndex < length; ++charIndex)
		{
			result = string[charIndex] + (result << 6) + (result << 16) - result;
		}

		return result;
	}

	/*
	Creates a hash from a string using the sdbm hash
	*/
	constexpr hash_t make_hash(const std::string& string)
	{
		return make_hash(string.c_str(), string.size());
	}

	namespace hash_literals
	{
		consteval cat::hash_t operator""_h(const char* string, size_t length)
		{
			return cat::make_hash(string, length);
		}
	}
}

#endif