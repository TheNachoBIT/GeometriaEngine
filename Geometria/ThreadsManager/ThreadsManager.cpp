#include "ThreadsManager.h"

typedef unsigned long int threadID;
std::list<ThreadPair> ThreadsManager::__tlist;

template <class Func, class... Args>
threadID ThreadsManager::CreateThread(Func &&func, Args&&... args) {
	std::thread *th(func, args);

	__tid++;
	ThreadPair pair = ThreadPair();
	pair.id = __tid;
	pair.th = th;

	__tlist.push_back(pair);
}

void ThreadsManager::StartThread(threadID id) {
	std::list<ThreadPair>::iterator it;

	for (it = __tlist.begin(); it != __tlist.end(); it++) {
		if (it->id == id) {
			it->th->join();
			break;
		}
	}
}

void ThreadsManager::StopThread(threadID id) {
	std::list<ThreadPair>::iterator it;

	for (it = __tlist.begin(); it != __tlist.end(); it++) {
		if (it->id == id) {
			it->th->~thread();
			delete it->th;
			__tlist.erase(it);
			break;
		}
	}
}

std::thread *ThreadsManager::GetThread(threadID id) {
	std::list<ThreadPair>::iterator it;

	for (it = __tlist.begin(); it != __tlist.end(); it++) {
		if (it->id == id) {
			return it->th;
			break;
		}
	}

	return NULL;
}
