#pragma once

//making everything static so i dont need an instance of the class lol

class FunctionHelper {
	
	
public:
	
	static const std::string base64_chars;	
	
	
public:

	

	static std::string itos(int Number);
	
	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	static std::string base64_decode(std::string const& encoded_string);
	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	static std::string gjp(std::string plainText_password);
	



};