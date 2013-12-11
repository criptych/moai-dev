#ifndef MOAIASYNCIMAGELOADTHREAD_H_
#define MOAIASYNCIMAGELOADTHREAD_H_

#include "pch.h"
#include "MOAIImage.h"
#include <threading/Threading.h>

class MOAIImageAsyncLoadThread : public RThread
{
public:
	static MOAIImageAsyncLoadThread* getInstance();
	static void deleteInstance();

	void run();

	void setParams(void* params);

private:
	MOAIImageAsyncLoadThread();

	bool bLoadReady;
	MoaiImageAsyncParams* mParams;

	static MOAIImageAsyncLoadThread* mInstance;

	RCriticalSection mLock;
};


#endif