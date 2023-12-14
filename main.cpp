#include "project.h"

#include "include/thread_manager.h"

int main()
{
  Project project;
  ThreadManager threadManager(project);
  return 0;
}