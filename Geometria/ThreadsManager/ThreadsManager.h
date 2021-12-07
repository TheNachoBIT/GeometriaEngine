#pragma once

#include <functional>
#include <thread>
#include <future>
#include "geometria.h"

typedef unsigned long int threadID;

class ThreadsManager {
private:
	struct __threadPair;
	static threadID __tid;
	static std::list <__threadPair> __tlist;
public:
	template <class Func>
	static threadID CreateThread(Func&& func);

	template <class T, class... Args>
	static std::future<T> *StartThread(threadID id, Args&&... args);

	static void StopThread(threadID id);
};
