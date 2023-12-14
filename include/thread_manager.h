#ifndef THREAD_MANAGER
#define THREAD_MANAGER

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

#endif // THREAD_MANAGER