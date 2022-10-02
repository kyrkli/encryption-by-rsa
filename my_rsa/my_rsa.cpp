#include "my_rsa.h"

my_rsa::my_rsa(const XBlocks8B& modul, const XBlocks8B& public_exp, const XBlocks8B& private_exp) :
	modul(modul), public_exp(public_exp), private_exp(private_exp) {}

uint64_t my_rsa::encode(const char& ch) {
	auto result = (pow(ch, public_exp) % modul);
	std::cout << "ENCODED START = " << result << std::endl;
	//return (pow(ch, public_exp) % modul).get_uint32();
	return result.get_uint32();
}

char my_rsa::decode(const uint64_t& encode_num) {
	return (pow(encode_num, private_exp) % modul).get_uint32();
}