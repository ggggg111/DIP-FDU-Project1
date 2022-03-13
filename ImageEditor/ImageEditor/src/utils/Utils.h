#ifndef __UTILS_H__
#define __UTILS_H__

template<typename T>
T** Array2D(const int& width, const int& height)
{
	T** ret = new T *[height];
	for (int i = 0; i < height; ++i)
		ret[i] = new T[width];
	
	return ret;
}

#endif /* __UTILS_H__ */