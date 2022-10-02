#include "XBlocks8B.h"
#include <string>
#include <sstream>
//private
const std::pair<uint32_t, uint32_t> XBlocks8B::mult4B(const uint32_t& factor1, const uint32_t& factor2) const {
	return cutting_overflow_and_rest(uint64_t(factor1) * factor2);
}

const std::pair<uint32_t, uint32_t> XBlocks8B::cutting_overflow_and_rest(const uint64_t& general8B) const {
	return std::make_pair(general8B / uint64_t(MAX_VALUE_OF_BLOCK + 1), general8B % uint64_t(MAX_VALUE_OF_BLOCK + 1));
}

int XBlocks8B::actual_size(const XBlocks8B& number) const {
	int counter = 0;
	for (int i = 0; i < number.data.size(); i++) {
		if (number.data[i] == 0)
			counter++;
		else break;
	}

	if (number.data.size() == counter)
		return 1;

	return number.data.size() - counter;
}

int XBlocks8B::number_of_blocks(const std::string& str) {
	if (str.length() % 9 == 0)
		return str.length() / 9;
	else return str.length() / 9 + 1;
}

void XBlocks8B::convert_string_into_the_blocks(const std::string& str) {
	data.assign(number_of_blocks(str), 0);
	int start_ch = str.length();
	std::string number_in_str;

	//write 9 digits or less in string and save in data[i] as a number
	for (int i = data.size() - 1; i >= 0; i--) {
		start_ch -= 9;

		if (start_ch < 0) // if first block isn’t full
			number_in_str.assign(str, 0, 9 + start_ch);
		else number_in_str.assign(str, start_ch, 9); //write 9 digits in string

		this->data[i] = std::atoi(number_in_str.c_str()); //save string as a number in a block
	}
}

void XBlocks8B::cutting_sums16B_into8B_range(std::vector<uint64_t>& sums16B) const {
	for (int i = sums16B.size() - 1; i >= 1; i--) { //from a last block to a first
		auto over_and_rest = cutting_overflow_and_rest(sums16B[i]);
		sums16B[i - 1] += std::get<0>(over_and_rest);
		sums16B[i] = std::get<1>(over_and_rest);
	}
}

std::vector<uint64_t> XBlocks8B::sums16B_of_Blocks(const XBlocks8B& left, const XBlocks8B& right) const {
	std::vector<uint64_t> sums16B(std::max(actual_size(left), actual_size(right)) + 1);

	for (int index_left = left.size() - 1, //The start of each index is at the end of the number 
		index_right = right.size() - 1,
		index_sums = sums16B.size() - 1;
		(index_left >= 0 || index_right >= 0);
		index_left--, index_right--, index_sums--)
	{
		if (index_left < 0)
			sums16B[index_sums] = right.data[index_right];
		else if (index_right < 0)
			sums16B[index_sums] = left.data[index_left];
		else sums16B[index_sums] = left.data[index_left] + right.data[index_right];
	}

	return sums16B;
}

XBlocks8B XBlocks8B::operator-(const XBlocks8B& right) const {
	//if the first number is less than the right number
	assert(*this >= right);

	XBlocks8B result(this->size(), 0);
	XBlocks8B left = *this;

	for (int index_left = left.size() - 1, index_right = right.size() - 1;
		index_left >= 0;
		index_left--, index_right--)
	{
		if (index_right < 0)	//in that case we have no digits to substract
			result[index_left] = left.data[index_left];
		else if (left.data[index_left] >= right[index_right])	//here there is a common substract
			result[index_left] = left.data[index_left] - right[index_right];
		else {  //here we need to take 1 from the left block, 
			//because the block of the left number is less than the block of the right number
			int shift = 1;
			while (left.data[index_left - shift] == 0) {	//if the left block consists from nulls
				left.data[index_left - shift] = MAX_VALUE_OF_BLOCK;
				shift++;
			}
			left.data[index_left - shift] -= 1; //the first block NOT null
			uint64_t block_plus_prev1 = left.data[index_left] + MAX_VALUE_OF_BLOCK + 1;

			result[index_left] = block_plus_prev1 - right[index_right];
		}

	}
	return result;
}

const bool XBlocks8B::less_or_less_equal(const XBlocks8B& right, const bool& equal) const {
	if (this->number_of_digits() < right.number_of_digits())
		return true;
	if (this->number_of_digits() > right.number_of_digits())
		return false;

	int count_equals = 0;

	//if ==
	for (int start_index_left = this->size() - actual_size(*this),
		start_index_right = right.size() - actual_size(right);
		start_index_left < this->size() || start_index_right < right.size();
		start_index_left++, start_index_right++)
	{
		if (this->data[start_index_left] < right.data[start_index_right])
			return true;
		else if (equal == 1 && this->data[start_index_left] == right.data[start_index_right])
			count_equals++;
		else 	if (this->data[start_index_left] == right.data[start_index_right])
			continue;
		else return false;
	}

	if (count_equals == actual_size(*this) && count_equals == actual_size(right))
		return true;

	return false;
}

//public
XBlocks8B::XBlocks8B() {
	//cout << __PRETTY_FUNCTION__ << endl;
}

XBlocks8B::XBlocks8B(const int& N, const int& default_number) {
	assert(N > 0);
	data.assign(N, default_number);
	//cout << __PRETTY_FUNCTION__ << endl;
}

//copy constructor
XBlocks8B::XBlocks8B(const XBlocks8B& other) {
	data = other.data;
	//cout << __PRETTY_FUNCTION__ << endl;
}

//move constructor
XBlocks8B::XBlocks8B(XBlocks8B&& other) {
	data = std::move(other.data);
}

XBlocks8B::XBlocks8B(std::string str) {

	if (str.find_first_not_of("0123456789") == std::string::npos) //check if str consists not only of the digits 
		convert_string_into_the_blocks(str);
	else {
		str.erase(remove(str.begin(), str.end(), ':'), str.end()); //removing :
		if (str.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos) { //if str is a hexadecimal representation

			XBlocks8B base = 16;
			XBlocks8B exponent = 0;
			std::vector<XBlocks8B> powers_of_sixteen; //16 to the power index * 4

			for (int i = 0; i < str.length() / 8; i++, exponent += 8)
				powers_of_sixteen.push_back(pow(base, exponent));

			std::vector<std::string> four_bytes;

			for (int start = str.length() - 8; start >= 0; start -= 8)
				four_bytes.push_back(str.substr(start, 8)); // taking 4 byte of the hexadecimal number from the str

			XBlocks8B result = 0;
			for (int i = 0; i < four_bytes.size(); i++) {
				std::size_t pos;
				const long long decimal_number = std::stoll(four_bytes[i], &pos, 16); // converting the hexadecimal number into decimal
				XBlocks8B part_result(decimal_number);

				part_result *= powers_of_sixteen[i]; //getting the desired number including the order in the hexadecimal system 
				result += part_result;
			}
			*this = result;
		}
		else assert(1 < 2 && "wrong decimal or hexadecimal representation of number");
	}
}

XBlocks8B::XBlocks8B(const uint64_t& number) {
	assert(number >= 0);
	auto over_and_rest = cutting_overflow_and_rest(number);

	data.push_back(over_and_rest.first);
	data.push_back(over_and_rest.second);
	//cout << __PRETTY_FUNCTION__ << endl;
}

std::ostream& operator<<(std::ostream& os, const XBlocks8B& number) {
	return (os << number.get_string());
}
// ???
uint32_t& XBlocks8B::operator[](const int& i) {
	return data[i];
}

// ???
const uint32_t& XBlocks8B::operator[](const int& i) const {
	return data[i];
}

const uint32_t XBlocks8B::size() const {
	return data.size();
}

void XBlocks8B::size(const uint32_t& size) {
	data.assign(size, 0);
}

XBlocks8B XBlocks8B::divide(const XBlocks8B& right, const bool& rest) const {
	//if the first number is less than the right number
	assert(right != 0);

	if (*this < right) {
		if (rest)
			return *this;
		else return 0;
	}

	if (*this == right) {
		if (rest)
			return 0;
		else return 1;
	}

	std::string str_left;
	//left units write into string
	for (int i = 0; i < this->size(); i++)
		str_left += std::to_string(this->data[i]);

	int subpos = 0;//start of a substruct

	//detecting a number for the first substract
	XBlocks8B substr_cmp(str_left.substr(0, right.number_of_digits()));

	XBlocks8B substruct_of_left_block =
		substr_cmp >= right ?
		std::move(substr_cmp) :
		XBlocks8B(str_left.substr(0, right.number_of_digits() + 1)); //if the number isn´t enough
	subpos += substruct_of_left_block.number_of_digits();

	int digit_of_result = 0;
	std::string str_result;
	bool exit = 0;
	while (subpos <= str_left.length()) {
		while (substruct_of_left_block >= right) {
			substruct_of_left_block -= right;
			digit_of_result++;
		}
		if (digit_of_result)
			str_result += std::to_string(digit_of_result);

		//finding a new number for substruct, that >= right, for subtracting
		if (subpos < str_left.length()) {
			substruct_of_left_block.push_back(str_left[subpos]);
			subpos++;

			if (substruct_of_left_block < right && subpos == str_left.length())
				str_result += "0";

			//if the new number < right, add at the end of the number the next digit and write into result 0
			while (substruct_of_left_block < right && subpos < str_left.length()) {
				substruct_of_left_block.push_back(str_left[subpos]);
				subpos++;
				str_result += "0";
			}
		}

		digit_of_result = 0;

		if (exit)
			break;

		if (subpos == str_left.length())
			exit = 1;
	}

	if (rest)
		return substruct_of_left_block;
	else return XBlocks8B(str_result);
}

XBlocks8B XBlocks8B::operator*(const XBlocks8B& right) const {
	if (*this == 0 || right == 0) {
		return 0;
	}
	else if (*this == 1 || right == 1) {
		return std::max(*this, right);
	}
	std::vector<uint64_t> sums16B(actual_size(*this) + actual_size(right));

	//we need to make every iteration a shift for correctly saving sums
	//it goes from a last block to first in the right factor
	for (int shift = 0, index_right = right.size() - 1;
		shift < right.size() && index_right >= 0;
		shift++, index_right--)
	{
		//sums from overflows and rests
		//it multiplies one block from the right number and the whole left number
		if (right.data[index_right] == 0)
			continue;
		else {
			for (int index_sums = sums16B.size() - 1 - shift, index_left = this->size() - 1;
				index_sums >= 1 && index_left >= 0;
				index_sums--, index_left--)
			{
				if (this->data[index_left] == 0)
					continue;
				else {
					auto over_and_rest = mult4B(this->data[index_left], right.data[index_right]);
					sums16B[index_sums - 1] += std::get<0>(over_and_rest);
					sums16B[index_sums] += std::get<1>(over_and_rest);
				}
			}
		}
	}
	//cutting sums, if there is an overflow after adding
	cutting_sums16B_into8B_range(sums16B);

	int start_index_sums = 0; //first NOT null index

	//search if there is nulls at the start
	for (int i = 0; i < sums16B.size(); i++) {
		if (sums16B[i] == 0)
			start_index_sums++;
		else break;
	}

	XBlocks8B result(sums16B.size() - start_index_sums, 0);
	//save in the result
	for (int i = 0; i < result.size(); i++)
		result.data[i] = sums16B[i + start_index_sums];

	return result;
}

XBlocks8B XBlocks8B::operator+(const XBlocks8B& right) const {
	if (*this == 0 || right == 0)
		return std::max(*this, right);

	auto sums16B = sums16B_of_Blocks(*this, right);
	//cutting sums, if there is an overflow after summing
	cutting_sums16B_into8B_range(sums16B);

	//a finding of a suitable size for the result 
	auto act_size = sums16B[0] == 0 ? sums16B.size() - 1 : sums16B.size();
	XBlocks8B result(act_size, 0);

	//if there is not an overflow
	if (act_size == sums16B.size() - 1)
		sums16B.erase(sums16B.begin());

	//save in the result
	for (int i = 0; i < result.size(); i++)
		result[i] = sums16B[i];

	return result;
}

XBlocks8B& XBlocks8B::operator++(int) {
	*this += std::to_string(1);
	return *this;
}

XBlocks8B& XBlocks8B::operator+=(const XBlocks8B& right) {
	*this = *this + right;
	return *this;
}

XBlocks8B& XBlocks8B::operator=(const XBlocks8B& right) {
	this->data = right.data;
	return *this;
}

const bool XBlocks8B::operator<=(const XBlocks8B& right) const {
	return less_or_less_equal(right, 1);
}

const bool XBlocks8B::operator<(const XBlocks8B& right) const {
	return less_or_less_equal(right, 0);
}

const bool XBlocks8B::operator>=(const XBlocks8B& right) const {
	return !less_or_less_equal(right, 0);
}

const bool XBlocks8B::operator>(const XBlocks8B& right) const {
	return !less_or_less_equal(right, 1);
}

const bool XBlocks8B::operator==(const XBlocks8B& right) const {
	if (this->number_of_digits() != right.number_of_digits())
		return false;

	//if ==
	for (int start_index_left = this->size() - actual_size(*this),
		start_index_right = right.size() - actual_size(right);
		start_index_left < this->size() || start_index_right < right.size();
		start_index_left++, start_index_right++)
	{
		if (this->data[start_index_left] != right.data[start_index_right])
			return false;
	}

	return true;
}

const int XBlocks8B::number_of_digits() const {
	int index_first_not_null = this->size() - actual_size(*this);
	int number_of_nulls = 0;
	//counts amount of nulls in the first NOT null block
	while (this->data[index_first_not_null] < std::pow(10, 8 - number_of_nulls)) {
		number_of_nulls++;
		if (number_of_nulls == 9)
			return 0;
	}

	return actual_size(*this) * 9 - number_of_nulls;
}

XBlocks8B& XBlocks8B::operator*=(const XBlocks8B& other) {
	*this = *this * other;
	return *this;
}

XBlocks8B& XBlocks8B::operator-=(const XBlocks8B& other) {
	*this = *this - other;
	return *this;
}

XBlocks8B XBlocks8B::operator%(const XBlocks8B& right) const {
	return divide(right, 1);
}

void XBlocks8B::push_back(const char ch) {
	assert(isdigit(ch));
	*this *= XBlocks8B(10);
	*this += XBlocks8B(std::atoi(&ch));
}

const bool XBlocks8B::operator!=(const XBlocks8B& right) const {
	return !(*this == right);
}

XBlocks8B XBlocks8B::operator/(const XBlocks8B& right) const {
	return divide(right, 0);
}

const uint32_t& XBlocks8B::get_uint32() const {
	return data[this->size() - 1];
}

XBlocks8B pow(const XBlocks8B& base, const XBlocks8B& exp) {
	if (exp == 0)
		return 1;
	if (exp == 1)
		return base;

	XBlocks8B cur_exp = 2;
	XBlocks8B next_exp = 4;
	XBlocks8B result = base * base;

	while (next_exp < exp) {
		std::cout << "cur exp = " << cur_exp << std::endl << "wanted exp = " << exp << std::endl;
		result *= result;
		cur_exp = next_exp;
		next_exp *= 2;
	}

	return (result * pow(base, exp - cur_exp));
}

const std::string XBlocks8B::get_string() const
{
	std::ostringstream os;
	for (int i = 0; i < data.size(); i++) {
		if (i == 0)
			os << data[i];
		else os << std::setw(9) << std::setfill('0') << data[i];
	}

	return os.str();
}