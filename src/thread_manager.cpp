#include "../include/thread_manager.h"

#include <future>

ThreadManager::ThreadManager(Project& project) 
{
	renderThread = std::thread([&project]() { project.run_render(); });
	soundThread = std::thread([&project]() { project.run_sound(); });
}

ThreadManager::~ThreadManager()
{
	renderThread.join();
	soundThread.join();
}
