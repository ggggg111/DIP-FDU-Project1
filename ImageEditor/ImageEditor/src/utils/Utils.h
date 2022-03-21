#pragma once

#ifndef __UTILS_H__
#define __UTILS_H__

#include <Windows.h>
#include <vector>

#define RELEASE_ARRAY2D(arr, h) \
	for (int i = 0; i < h; ++i) \
	{ \
		delete[] arr[i]; \
	} \
	delete[] arr;

template<typename T>
T** Array2D(const int& width, const int& height)
{
	T** ret = new T * [height];
	for (int i = 0; i < height; ++i)
		ret[i] = new T[width];

	return ret;
}

wchar_t* CharArrayToLPCWSTR(const char* charArray, const int& buffer_size = 4096);

int Median(std::vector<int>& vector);

#endif /* __UTILS_H__ */