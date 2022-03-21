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
	std::cout << "Vector len: " << vector.size() << std::endl;
	for (auto el : vector) std::cout << el << std::endl;
	std::cout << "Vector before: " << vector[0] << " " << vector[1] << " " << vector[2] << " " << vector[3] << " "
		<< vector[4] << " " << vector[5] << " " << vector[6] << " " << vector[7] << " " << vector[8] << std::endl;
	std::sort(vector.begin(), vector.end());
	std::cout << "Vector after: " << vector[0] << " " << vector[1] << " " << vector[2] << " " << vector[3] << " "
		<< vector[4] << " " << vector[5] << " " << vector[6] << " " << vector[7] << " " << vector[8] << std::endl;
	int median = vector[vector.size() / 2];
	std::cout << "Median: " << median << std::endl;
	return median;

	/*std::cout << "Vector is: " << std::endl;
	for (auto& el : vector)
	{
		std::cout << el << std::endl;
	}

	int n = vector.size() / 2;

	std::nth_element(vector.begin(), vector.begin() + n, vector.end());
	int vector_n = vector[n];

	int ret  = -1;

	if (vector.size() % 2 == 1)
	{
		return ret = vector_n;
	}
	else
	{
		std::nth_element(vector.begin(), vector.begin() + n - 1, vector.end());
		ret = 0.5 * (vector_n + vector[n - 1]);
	}

	std::cout << "Ret: " << ret << std::endl;

	return ret;*/

	/*int n = vector.size() / 2;

	std::nth_element(vector.begin(), vector.begin() + n, vector.end());
	int vector_n = vector[n];
	
	if (vector.size() % 2 == 1)
	{
		return vector_n;
	}
	else
	{
		std::nth_element(vector.begin(), vector.begin() + n - 1, vector.end());
		return 0.5 * (vector_n + vector[n - 1]);
	}*/
}