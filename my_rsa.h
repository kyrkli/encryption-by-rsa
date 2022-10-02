#pragma once
#include "XBlocks8B.h"
class my_rsa {
	XBlocks8B modul;
	XBlocks8B public_exp;
	XBlocks8B private_exp;

public:
	my_rsa() {}

	my_rsa(const XBlocks8B& modul, const XBlocks8B& public_exp, const XBlocks8B& private_exp);

	std::string encode(const char& ch);
	char decode(const uint64_t& encode_num);
};