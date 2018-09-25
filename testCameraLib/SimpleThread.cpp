
#include "stdafx.h"
#include "SimpleThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSimpleThread::CSimpleThread()
{
	m_hThread = NULL;
}
CSimpleThread::~CSimpleThread()
{
	m_bRun = false;
	if(m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

bool CSimpleThread::Start(LPTHREAD_START_ROUTINE fun, void *data)
{
	m_bRun = TRUE;
	DWORD   threadId;
	m_hThread = CreateThread( NULL,0,(LPTHREAD_START_ROUTINE)(fun),(LPVOID)data, 0, &threadId );
	// Thread  failed
	if( !m_hThread )
	{
		m_bRun = FALSE;
	}
	return m_bRun;

}
bool CSimpleThread::HasFinish( )
{
	if( m_hThread && m_bRun)
	{
		if( WaitForSingleObject( m_hThread, 1) == WAIT_TIMEOUT )
			return false;
	}
	return true;
}
bool CSimpleThread::GetExitCode(unsigned *code)
{
	bool ret = false;
	if( WaitForSingleObject(m_hThread, INFINITE)== WAIT_OBJECT_0 ) 
	{ 
		DWORD dwCode = -1; 
		if( GetExitCodeThread(m_hThread, &dwCode )) 
			ret = true;
		*code = dwCode;
	}
	return ret; 
};
void CSimpleThread::WaitStop()
{
	if( m_hThread )
	{
		// wait thread exit
		m_bRun=FALSE;
		WaitForSingleObject( m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

}
void CSimpleThread::Terminate()
{
	if (WaitForSingleObject(m_hThread, 1000) == WAIT_TIMEOUT)
	{
		// Terminate thread
		if (!TerminateThread(m_hThread, 0))
		{
			return ;
		}
	}

}
void CSimpleThread::SimpleSuspendThread()
{
	SuspendThread(m_hThread);
}
void CSimpleThread::SimpleResumeThread()
{
	ResumeThread(m_hThread);
}
