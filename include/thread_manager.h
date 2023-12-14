#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "project.h"
#include "thread"

class ThreadManager
{
public:
  ThreadManager(Project &project);
  ~ThreadManager();
  
private:
  std::thread renderThread;
  std::thread soundThread;

  void run_render();
  void run_sound();
};

#endif // THREAD_MANAGER_H