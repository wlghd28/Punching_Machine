#ifndef __MVTOOL_H__
#define __MVTOOL_H__

#include "IMVAPI/IMVApi.h"

class CMvTool
{
public:
	CMvTool();

	~CMvTool();

	static uint64_t getCurTimeMilliSecond();

	static uint64_t getCurTimeMicroSecond();


	bool enter();


	bool tryEnter();


	bool leave();

private:
	HANDLE m_mutexHandle;
};

#endif // __MVTOOL_H__
