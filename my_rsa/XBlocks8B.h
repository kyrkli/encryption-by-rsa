#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>
#include <utility>
#include <iomanip>
#include <cassert>


class XBlocks8B {
	std::vector<uint32_t> data; //9 digits in a uint32_t is maximum
	const uint32_t MAX_VALUE_OF_BLOCK = 999999999;

	const std::pair<uint32_t, uint32_t> mult4B(const uint32_t& factor1, const uint32_t& factor2) const;
	const std::pair<uint32_t, uint32_t> cutting_overflow_and_rest(const uint64_t& general8B) const;
	int actual_size(const XBlocks8B& type) const;
	int number_of_blocks(const std::string& str);
	void convert_string_into_the_blocks(const std::string& str);
	void cutting_sums16B_into8B_range(std::vector<uint64_t>& sums64) const;
	std::vector<uint64_t> sums16B_of_Blocks(const XBlocks8B& left, const XBlocks8B& right) const;
	const bool less_or_less_equal(const XBlocks8B& right, const bool& equal) const;
	void push_back(const char ch);
	XBlocks8B divide(const XBlocks8B& right, const bool& rest) const;

public:
	XBlocks8B();
	XBlocks8B(const int& N, const int& default_number);
	XBlocks8B(const XBlocks8B& other);//copy constructor
	XBlocks8B(XBlocks8B&& other);//move constructor
	XBlocks8B(const uint64_t& number);
	XBlocks8B(std::string str);

	friend std::ostream& operator<<(std::ostream& os, const XBlocks8B& type);

	uint32_t& operator[](const int& i);
	const uint32_t& operator[](const int& i) const;

	XBlocks8B operator*(const XBlocks8B& other) const;
	XBlocks8B& operator*=(const XBlocks8B& other);

	XBlocks8B operator+(const XBlocks8B& right) const;
	XBlocks8B& operator++(int);
	XBlocks8B& operator+=(const XBlocks8B& right);

	XBlocks8B& operator=(const XBlocks8B& right);

	XBlocks8B operator-(const XBlocks8B& right) const;
	XBlocks8B& operator-=(const XBlocks8B& other);
	XBlocks8B& operator--(int);

	XBlocks8B operator/(const XBlocks8B& right) const;
	XBlocks8B& operator/=(const XBlocks8B& right);
	XBlocks8B operator%(const XBlocks8B& right) const;

	friend XBlocks8B pow(const XBlocks8B& base, const XBlocks8B& exp);

	const uint32_t size() const;
	void size(const uint32_t& size);

	const bool operator<=(const XBlocks8B& right) const;
	const bool operator<(const XBlocks8B& right) const;
	const bool operator>=(const XBlocks8B& right) const;
	const bool operator>(const XBlocks8B& right) const;
	const bool operator==(const XBlocks8B& right) const;
	const bool operator!=(const XBlocks8B& right) const;

	const int number_of_digits() const;

	const uint32_t& get_uint32() const;
};