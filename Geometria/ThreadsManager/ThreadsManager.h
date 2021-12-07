#pragma once

#include <functional>
#include <thread>
#include "geometria.h"

typedef unsigned long int threadID;

struct ThreadPair {
	threadID id;
	std::thread *th;
};

class ThreadsManager {
private:
	static threadID __tid;
	static std::list <ThreadPair> __tlist;
public:
	template <class Func, class... Args>
	static threadID CreateThread(Func&& func, Args&&... args);
	static void StartThread(threadID id);
	static void StopThread(threadID id);
	static std::thread *GetThread(threadID id);
};
