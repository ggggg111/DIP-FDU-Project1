#ifndef __HDR_LOADER_H__
#define __HDR_LOADER_H__

#include <list>
#include <string>

class HDRLoader
{
public:
	static void LoadHDRImage(const std::list<const std::string&>& paths);
};

#endif /* __HDR_LOADER_H__ */