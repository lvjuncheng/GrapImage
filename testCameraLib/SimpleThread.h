#ifndef _SimpleThread_H
#define _SimpleThread_H

class CSimpleThread
{
public:
	CSimpleThread();
	~CSimpleThread();

public:
	bool Start(LPTHREAD_START_ROUTINE fun, void* data);
	void WaitStop();
	bool HasFinish();
	bool GetExitCode(unsigned *code);
	void Terminate();
	void SimpleSuspendThread();
	void SimpleResumeThread();
	void SimpleStopThread();

protected:
	HANDLE m_hThread;
public:
	bool m_bRun;
};
#endif