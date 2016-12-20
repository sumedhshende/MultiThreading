// FirstLock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
class CritSec {
	CRITICAL_SECTION m_cs;
public:
	CritSec() {
		InitializeCriticalSection(&m_cs);
	}
	~CritSec() {
		DeleteCriticalSection(&m_cs);
	}
	void Enter() {
		EnterCriticalSection(&m_cs);
	}
	void Leave() {
		LeaveCriticalSection(&m_cs);
	}
};
class Counter {
	CritSec m_cs;
	int _counter;
public:
	Counter() :_counter(0) {}
	int getCount() { return _counter; }
	void increment() { m_cs.Enter();  ++_counter; m_cs.Leave(); }
}g_counter;

DWORD __stdcall CountingThread(void *data) {
	
	for(int i=0; i<10000;i++) {
		g_counter.increment();
	} 
	return 0;
}
const LONG maxThreadCount = 2;

int main()
{
	int count = 0;
	HANDLE threads[maxThreadCount];
	
	for (int i = 0; i < maxThreadCount; i++) {
		threads[i] = CreateThread(nullptr, 0, CountingThread, &count, 0, nullptr);
	}
	
	WaitForMultipleObjects(maxThreadCount, threads, TRUE, INFINITE);
	std::cout << g_counter.getCount() << std::endl;
	getchar();
	return 0;
}

