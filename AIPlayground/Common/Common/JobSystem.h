#pragma once

#include "Types.h"

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>

struct JobParametersBase
{};

struct Job
{
private:
  typedef std::function<void( JobParametersBase* )> JobFunction;
public:
  Job() :threadCleanUpJob( true ) {};
  ~Job() { delete jobParams; }
  JobFunction taskFunction;
  JobParametersBase* jobParams;
  bool threadCleanUpJob;
};


struct WorkerThread
{
  WorkerThread( uint a_threadID );
  void WorkerMainFunction();

  Job* currentJob;
  std::thread workerThread;
  std::atomic_flag runningFlag;

  uint threadID;
};

class JobSystem
{
  friend struct WorkerThread;
public:
  static void Init( uint a_threadCount );
  static void UnInit();
  static void ScheduleJob( Job* a_jobToAdd );



private:
  JobSystem& operator = ( JobSystem& a_right );
  JobSystem( const JobSystem& );

  static Job* GetAnyAvaidableJob();


  static std::queue<Job*> m_jobs;
  static std::atomic_uint m_jobCount;
  static std::mutex m_jobsMutex;

  static std::vector<WorkerThread*> m_workerThreads;

  static bool m_hasInitBeenCalled;
};