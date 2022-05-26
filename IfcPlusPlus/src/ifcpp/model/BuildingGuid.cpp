/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <array>
#include <string>
#include <random>
#include <sstream>
#include "ifcpp/reader/ReaderUtil.h"
#include "BuildingGuid.h"

///@brief Creates a GUID string with 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
std::wstring createGUID32_wstr()
{
	std::string guid = createGUID32();
	std::wstring guid_wstr = string2wstring(guid);
	return guid_wstr;
}

std::string createGUID32()
{
	std::random_device              rd;
	std::mt19937                    gen(rd());
	std::uniform_int_distribution<> dis(0, 15);
	std::uniform_int_distribution<> dis2(8, 11);
	std::basic_stringstream<char> uuid_strs;
	uuid_strs << std::uppercase;

	size_t i;
	uuid_strs << std::hex;
	for (i = 0; i < 8; i++) { uuid_strs << dis(gen); }
	uuid_strs << "-";
	for (i = 0; i < 4; i++){uuid_strs << dis(gen); }
	uuid_strs << "-4";
	for (i = 0; i < 3; i++){uuid_strs << dis(gen);}
	uuid_strs << "-";
	uuid_strs << dis2(gen);
	for (i = 0; i < 3; i++){uuid_strs << dis(gen);}
	uuid_strs << "-";
	for (i = 0; i < 12; i++){uuid_strs << dis(gen);}
	std::string str = uuid_strs.str();	
	return str;
}

static const char base16mask[] = { char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(0), char(1), char(2), char(3), char(4), char(5), char(6), char(7), char(8), char(9), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(10), char(11), char(12), char(13), char(14), char(15), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(10), char(11), char(12), char(13), char(14), char(15), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1) };
static const char base64mask[] = { char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(-1), char(-1), char(-1), char(63), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(0), char(1), char(2), char(3), char(4), char(5), char(6), char(7), char(8), char(9), char(-1), char(-1), char(-1), char(-1), char(-1), char(-1), \
				 char(-1), char(10), char(11), char(12), char(13), char(14), char(15), char(16), char(17), char(18), char(19), char(20), char(21), char(22), char(23), char(24), \
				 char(25), char(26), char(27), char(28), char(29), char(30), char(31), char(32), char(33), char(34), char(35), char(-1), char(-1), char(-1), char(-1), char(62), \
				 char(-1), char(36), char(37), char(38), char(39), char(40), char(41), char(42), char(43), char(44), char(45), char(46), char(47), char(48), char(49), char(50), \
				 char(51), char(52), char(53), char(54), char(55), char(56), char(57), char(58), char(59), char(60), char(61), char(-1), char(-1), char(-1), char(-1), char(-1) };


///@brief Compresses a GUID string
///@details Expects a string with exactly 36 characters including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
///@returns an IFC GUID string with 22 characters, for example: "3n0m0Cc6L4xhvkpCU0k1GZ"
std::wstring compressGUID_wstr(const std::wstring& in)
{
	static constexpr std::array<wchar_t, 64> base64Chars = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'_','$'
	};
	
	std::wstring temp;
	std::wstring result;
	result.resize(23);
	result[0] = '0';

	temp.push_back('0');

	// remove dashes
	for (size_t ii = 0; ii < in.length(); ++ii)
	{
		if (in[ii] != '-')
		{
			temp.push_back(in[ii]);
		}
	}

	// compress
	int n = 0;
	for (size_t ii_out = 0, ii = 0; ii < 32; ii += 3)
	{
		n = base16mask[temp[ii]] << 8;
		n += base16mask[temp[ii + 1]] << 4;
		n += base16mask[temp[ii + 2]];
		result[ii_out + 1] = base64Chars[n % 64];
		result[ii_out] = base64Chars[n / 64];
		ii_out += 2;
	}
	result.resize(22);
	return result;
}
std::string compressGUID(const std::string& in)
{
	static constexpr std::array<char, 64> base64Chars = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'_','$'
	};

	std::string temp;
	std::string result;
	result.resize(23);
	result[0] = '0';

	temp.push_back('0');

	// remove dashes
	for (size_t ii = 0; ii < in.length(); ++ii)
	{
		if (in[ii] != '-')
		{
			temp.push_back(in[ii]);
		}
	}

	// compress
	int n = 0;
	for (size_t ii_out = 0, ii = 0; ii < 32; ii += 3)
	{
		n = base16mask[temp[ii]] << 8;
		n += base16mask[temp[ii + 1]] << 4;
		n += base16mask[temp[ii + 2]];
		result[ii_out + 1] = base64Chars[n % 64];
		result[ii_out] = base64Chars[n / 64];
		ii_out += 2;
	}
	result.resize(22);
	return result;
}

///@brief Decompresses an IFC GUID string
///@details Expects a string with exactly 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@returns GUID string with 36 characters, including dashes, for example: "F103000C-9865-44EE-BE6E-CCC780B81423"
std::wstring decompressGUID_wstr(const std::wstring& in )
{
	static constexpr std::array<wchar_t, 16> base16Chars = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	wchar_t temp[32 + 2];
	size_t ii_out = 0;
	int n = 0;
	int t = 0;

	for (size_t ii = 0; ii < 22; ii += 2)
	{
		n = base64mask[in[ii]] << 6;
		n += base64mask[in[ii + 1]];
		t = n / 16;
		temp[ii_out + 2] = base16Chars[n % 16];
		temp[ii_out + 1] = base16Chars[t % 16];
		temp[ii_out] = base16Chars[t / 16];
		ii_out += 3;
	}
	temp[ii_out] = '\0';

	// add dashes: F103000C-9865-44EE-BE6E-CCC780B81423
	std::wstring result;
	for (size_t ii = 1; ii < 36; ++ii)
	{
		if (ii == 9 || ii == 13 || ii == 17 || ii == 21)
		{
			result.push_back('-');
		}

		result.push_back(temp[ii]);
	}

	result.resize(36);
	return result;
}
std::string decompressGUID(const std::string& in)
{
	static constexpr std::array<char, 16> base16Chars = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	char temp[32 + 2];
	size_t ii_out = 0;
	int n = 0;
	int t = 0;

	for (size_t ii = 0; ii < 22; ii += 2)
	{
		n = base64mask[in[ii]] << 6;
		n += base64mask[in[ii + 1]];
		t = n / 16;
		temp[ii_out + 2] = base16Chars[n % 16];
		temp[ii_out + 1] = base16Chars[t % 16];
		temp[ii_out] = base16Chars[t / 16];
		ii_out += 3;
	}
	temp[ii_out] = '\0';

	// add dashes: F103000C-9865-44EE-BE6E-CCC780B81423
	std::string result;
	for (size_t ii = 1; ii < 36; ++ii)
	{
		if (ii == 9 || ii == 13 || ii == 17 || ii == 21)
		{
			result.push_back('-');
		}

		result.push_back(temp[ii]);
	}

	result.resize(36);
	return result;
}

///@brief Create IFC GUID string with 22 characters, for example "3n0m0Cc6L4xhvkpCU0k1GZ"
///@details Use desired character type as template parameter - char or wchar_t.
///IFC uses a different base64 character set than RFC4648 - it starts with digits
///instead of uppercase letters and uses '_' and '$' as last two characters.
std::wstring createBase64Uuid_wstr()
{
	std::wstring guid_uncompressed = createGUID32_wstr();
	std::wstring guid_compressed = compressGUID_wstr(guid_uncompressed);
	return guid_compressed;
}
std::string createBase64Uuid()
{
	std::string guid_uncompressed = createGUID32();
	std::string guid_compressed = compressGUID(guid_uncompressed);
	return guid_compressed;
}
