#pragma once

#include "patch.h"
#include <fstream>
//#include <gd.h>
//#include <CCFileUtils.h>
#include "FunctionHelper.h"
#include <sstream>
#include <locale>



std::string FunctionHelper::intToFormatString(int n) {

    if(n < 1000 || n > 100000000)
    return FunctionHelper::itos(n);

    std::string str = FunctionHelper::thousandSeparator(n);
    str = str.substr(0, str.find(".") + 2);

    char sufix;

    if(n < 1000000) { sufix = 'K'; return str + sufix; }
    if(n < 100000000) { sufix = 'M'; return str + sufix;  }

     return FunctionHelper::itos(n);
}

std::string FunctionHelper::thousandSeparator(int n)
{
	using namespace std;
    string ans = "";
  
    // Convert the given integer
    // to equivalent string
    string num = FunctionHelper::itos(n);
  
    // Initialise count
    int count = 0;
  
    // Traverse the string in reverse
    for (int i = num.size() - 1;
         i >= 0; i--) {
        count++;
        ans.push_back(num[i]);
  
        // If three characters
        // are traversed
        if (count == 3) {
            ans.push_back('.');
            count = 0;
        }
    }
  
    // Reverse the string to get
    // the desired output
    reverse(ans.begin(), ans.end());
  
    // If the given string is
    // less than 1000
    if (ans.size() % 4 == 0) {
  
        // Remove ','
        ans.erase(ans.begin());
    }
  
    return ans;
}


std::string FunctionHelper::itohex(int i)
{
  std::stringstream stream;
  stream << std::hex << i;
  return stream.str();
}

bool FunctionHelper::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

std::string FunctionHelper::itos(int Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}




std::string FunctionHelper::base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && FunctionHelper::is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
			char_array_4[i] = FunctionHelper::base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
		char_array_4[j] = 0;

		for (j = 0; j <4; j++)
		char_array_4[j] = FunctionHelper::base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}


std::string FunctionHelper::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
			ret += FunctionHelper::base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
		char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		ret += FunctionHelper::base64_chars[char_array_4[j]];

		while((i++ < 3))
		ret += '=';

	}

	return ret;

}



std::string FunctionHelper::gjp(std::string password) {
	
	const char* key = "37526";
	const size_t keyLen = 5;
	size_t gjpCurrent = 0;
	for(auto& character : password) {
		character ^= key[gjpCurrent];
		gjpCurrent = (gjpCurrent + 1) % keyLen;
	}
	return FunctionHelper::base64_encode((const unsigned char *)password.c_str(), password.length());
}