#pragma once
#include <string>
#include <iostream>


struct Encryption
{
	struct Base64
	{
		static std::string Encode(std::string t);
		static std::string Encode(std::string t, bool url);
		static std::string Decode(std::string t);
		static std::string Decode(std::string t, bool url);
	};

	struct Xor
	{
		static std::string Run(std::string t, int key);
	};

	struct ZLib
	{
		enum class Format
		{
			DEFLATE,
			ZLIB,
			GZIP
		};

		static std::string Deflate(std::string t);
		static std::string Inflate(std::string t, ZLib::Format format);
	};
};