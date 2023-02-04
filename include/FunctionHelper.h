#pragma once

//making everything static so idont need an instance of the class lol








namespace FunctionHelper {
	
	static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";
	
	 std::string itos(int Number);
	
	 inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
	bool isNumber(const std::string &);
	std::string thousandSeparator(int n);
	std::string base64_decode(std::string const& encoded_string);
	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	std::string gjp(std::string plainText_password);
	std::string itohex(int i);
	std::string intToFormatString(int n);

};