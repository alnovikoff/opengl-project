#include "project.h"

#include "include/thread_manager.h"

int main()
{
  // Project project;
  
  // std::thread renderThread([&project]() {
  //     project.run();
  // });

  // // Start sound playback thread
  // std::thread soundThread([&project]() {
  //     project.run_sound();
  // });

  // // Join threads when the program exits
  // renderThread.join();
  // soundThread.join();

  // renderThread.detach();
  // soundThread.detach();
  Project project;
  ThreadManager threadManager(project);
  return 0;
}