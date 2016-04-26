#include "JobSystem.h"
#include "log.h"


std::vector<Job*> JobSystem::m_jobs;
std::mutex JobSystem::m_jobsMutex;
std::vector<WorkerThread*> JobSystem::m_workerThreads;
bool JobSystem::m_hasInitBeenCalled;

std::atomic_int JobSystem::m_jobCount;

std::queue<int32> JobSystem::m_freeIDs;
std::map<uint32 , Job*> JobSystem::m_activeJobs;
std::mutex JobSystem::m_activeJobsMutex;

uint32 JobSystem::m_currentMaxID;

WorkerThread::WorkerThread( uint a_threadID ) :
  workerThread( &WorkerThread::WorkerMainFunction , this ) ,
  threadID( a_threadID )
{
  JobSystemDebugInfo* debugInfo = JobSystemDebugInfo::GI();
  if( debugInfo )
  {
    debugThreadInfo = debugInfo->registerThread();
    if( !debugThreadInfo.expired() )
    {
      auto info = debugThreadInfo.lock();
      info->threadID = threadID;
    }
  }
}

void WorkerThread::WorkerMainFunction()
{
  runningFlag.test_and_set();
  LOGS( "Thread %d is now running" , threadID );

  while( runningFlag.test_and_set() )
  {
    if( JobSystem::m_jobCount > 0 )
    {
      currentJob = JobSystem::GetAnyAvaidableJob();
      if( currentJob )
      {
        jobTimer.Reset();
        jobTimer.Start();
        currentJob->taskFunction( currentJob->jobParams );
        JobSystem::JobCompleted( currentJob );
        if( currentJob->threadCleanUpJob )
          delete currentJob;
        currentJob = nullptr;

        double time = jobTimer.IntervalMS();
        if( !debugThreadInfo.expired() )
        {
          auto threadInfo = debugThreadInfo.lock();
          std::lock_guard<std::mutex> lock( threadInfo->infoLock );
          threadInfo->jobTimeTotal += time;
          threadInfo->jobTimeSamplesCount++;
          threadInfo->jobsCompleted++;

        }
      }

    }
    else
      std::this_thread::yield();
  }
  LOGW( "Thread %d has now ended" , threadID );
}

void JobSystem::Init( uint a_threadCount )
{
  m_hasInitBeenCalled = true;
  m_workerThreads.reserve( a_threadCount );

  for( uint i = 0; i < a_threadCount; i++ )
    m_workerThreads.push_back( new WorkerThread( i ) );
  m_currentMaxID = 1;
}




bool JobSystem::CheckJobConditions( Job* a_job )
{
  if( !a_job )
    return false;
  switch( a_job->jobCondition )
  {
  case JobCondition::NONE:
  {
    return true;
  }
  case JobCondition::ONE_AT_A_TIME:
  {
    if( !a_job->typeHashCode )
    {
      LOGW( "Job doesnt have a hashcode" );
    }
    std::lock_guard<std::mutex> lock( m_activeJobsMutex );
    for( auto activeJob = m_activeJobs.begin(); activeJob != m_activeJobs.end(); activeJob++ )
    {
      if( activeJob->second->typeHashCode == a_job->typeHashCode )
        return false;
    }
    return true;
  }
  default:
    return true;
    break;
  }
}

Job* JobSystem::GetAnyAvaidableJob()
{
<<<<<<< HEAD

=======
>>>>>>> master
  Job* returningJob = nullptr;
  if( !m_jobsMutex.try_lock() )
    return nullptr;
  for( auto job = m_jobs.begin(); job != m_jobs.end(); )
  {
    if( CheckJobConditions( *job ) )
    {
      returningJob = *job;
      m_jobs.erase( job );
      m_jobCount--;
      std::lock_guard<std::mutex> lock( m_activeJobsMutex );
      m_activeJobs[ returningJob->jobID ] = returningJob;
      break;
    }
    job++;
  }
  m_jobsMutex.unlock();
  return returningJob;
}

void JobSystem::JobCompleted( Job* a_job )
{
  m_activeJobsMutex.lock();
  if( a_job &&  m_activeJobs[ a_job->jobID ] )
  {
    m_activeJobs[ a_job->jobID ] = nullptr;
    m_activeJobs.erase( a_job->jobID );
  }
  m_activeJobsMutex.unlock();

}

void JobSystem::UnInit()
{
  if( m_hasInitBeenCalled )
  {
    m_hasInitBeenCalled = false;
    for( uint i = 0; i < m_workerThreads.size(); i++ )
      m_workerThreads[ i ]->runningFlag.clear();

    for( uint i = 0; i < m_workerThreads.size(); i++ )
    {
      if( !m_workerThreads[ i ]->workerThread.joinable() )
        continue;
      else
        m_workerThreads[ i ]->workerThread.join();
    }
    m_workerThreads.clear();
  }
}

uint32 JobSystem::ScheduleJob( Job* a_jobToAdd )
{
  int32 jobID = 0;

  if( m_hasInitBeenCalled )
  {
    if( !a_jobToAdd )
    {
      LOGE( "I recieved a null job!" );
      return jobID;
    }
    std::lock_guard<std::mutex> lock( m_jobsMutex );
    if( !m_freeIDs.empty() )
    {
      jobID = m_freeIDs.front();
      m_freeIDs.pop();
    }
    jobID = m_currentMaxID;
    m_currentMaxID++;
    a_jobToAdd->jobID = jobID;
    m_jobs.push_back( a_jobToAdd );
    m_jobCount++;

  }
  else
    LOGE( "Tried to schedule job but jobsystem has not be initialized" );
  return jobID;
}