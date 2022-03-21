#include <algorithm>
#include <iostream>

#include "Utils.h"

wchar_t* CharArrayToLPCWSTR(const char* charArray, const int& buffer_size)
{
	wchar_t* wString = new wchar_t[buffer_size];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, buffer_size);

	return wString;
}

int Median(std::vector<int>& vector)
{
	std::sort(vector.begin(), vector.end());
	
	return vector[vector.size() / 2];;
}