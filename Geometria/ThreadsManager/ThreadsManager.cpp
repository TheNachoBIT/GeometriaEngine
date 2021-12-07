#include "ThreadsManager.h"

typedef unsigned long int threadID;

struct ThreadsManager::__threadPair {
	threadID id;
	void* func;
};

std::list<ThreadsManager::__threadPair> ThreadsManager::__tlist;

template <class Func>
threadID ThreadsManager::CreateThread(Func&& func) {
	__tid++;
	__threadPair pair = __threadPair();
	pair.id = __tid;
	pair.func = func;

	__tlist.push_back(pair);
}

template <class T, class... Args>
std::future<T> *ThreadsManager::StartThread(threadID id, Args&&... args) {
	std::list<__threadPair>::iterator it;

	for (it = __tlist.begin(); it != __tlist.end(); it++) {
		if (it->id == id) {
			return std::async(std::launch::async, it->func, args);
		}
	}

	return NULL;
}

void ThreadsManager::StopThread(threadID id) {
	std::list<__threadPair>::iterator it;

	for (it = __tlist.begin(); it != __tlist.end(); it++) {
		if (it->id == id) {
			__tlist.erase(it);
			break;
		}
	}
}
