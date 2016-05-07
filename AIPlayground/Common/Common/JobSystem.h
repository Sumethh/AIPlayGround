#pragma once

#include "Types.h"
#include "JobSystemDebugInfo.h"
#include "timer.h"

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <typeinfo>
#include <map>
#include <condition_variable>
enum class JobCondition : int
{
  NONE,
  ONE_AT_A_TIME,
  COUNT
};


struct JobParametersBase
{};

struct Job
{
private:
  typedef std::function<void( JobParametersBase* )> JobFunction;
public:
  Job() :threadCleanUpJob( true ), jobCondition( JobCondition::NONE), typeHashCode(0) {};
  ~Job() { delete jobParams; }
  JobFunction taskFunction;
  JobParametersBase* jobParams;
  bool threadCleanUpJob;
  JobCondition jobCondition;
  uint32 typeHashCode;
  uint32 jobID;
};


struct WorkerThread
{
  WorkerThread( uint a_threadID );
  void WorkerMainFunction();

  Job* currentJob;
  std::thread workerThread;
  std::atomic_flag runningFlag;

  Timer jobTimer;
  uint threadID;


  std::mutex jobQueueLock;
  std::vector<Job*> jobQueue;
  std::atomic<uint> jobCount;
private:
  std::weak_ptr<DebugThreadInfo> debugThreadInfo;
};

class JobSystem
{
  friend struct WorkerThread;
public:
  static void Init( uint a_threadCount );
  static void UnInit();
  static uint32 ScheduleJob( Job* a_jobToAdd );



private:
  JobSystem& operator = ( JobSystem& a_right );
  JobSystem( const JobSystem& );

  static Job* GetAnyAvaidableJob();
  static void JobCompleted( Job* a_job );
  static void AddActiveJob( Job* a_job );
  static bool CheckJobConditions( Job* a_job );

  static std::condition_variable m_jobCondition;

  static std::map<uint32, Job*> m_activeJobs;
  static std::mutex m_activeJobsMutex;

  static std::atomic_int m_jobCount;

  static std::atomic<uint> m_lastScheduledThreadID;

  static std::vector<Job*> m_jobs;
  static std::mutex m_jobsMutex;
  static std::queue<int32> m_freeIDs;
  static std::vector<WorkerThread*> m_workerThreads;
  static uint32 m_currentMaxID;
  static bool m_hasInitBeenCalled;
};