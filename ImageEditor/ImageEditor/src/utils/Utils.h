#pragma once

#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <vector>
#include <Windows.h>

#define RELEASE_ARRAY2D(arr, h) \
	for (int i = 0; i < h; ++i) \
	{ \
		delete[] arr[i]; \
	} \
	delete[] arr;

#define CLAMP(value, min_value, max_value) \
	if (value < min_value) value = min_value; \
	if (value > max_value) value = max_value;

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

template<typename Base, typename T>
inline bool IsInstanceOf(const T* ptr)
{
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif /* __UTILS_H__ */