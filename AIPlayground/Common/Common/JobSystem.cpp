#include "JobSystem.h"
#include "log.h"


std::queue<Job*> JobSystem::m_jobs;
std::atomic_uint JobSystem::m_jobCount;
std::mutex JobSystem::m_jobsMutex;
std::vector<WorkerThread*> JobSystem::m_workerThreads;
bool JobSystem::m_hasInitBeenCalled;

WorkerThread::WorkerThread( uint a_threadID ) :
  workerThread( &WorkerThread::WorkerMainFunction , this ) ,
  threadID( a_threadID )
{
}

void WorkerThread::WorkerMainFunction()
{
  runningFlag.test_and_set();
  LOGS( "Thread %d is now running" , threadID );
  std::atomic<int> t;
  while( runningFlag.test_and_set() )
  {
    if( JobSystem::m_jobCount > 0 )
    {
      currentJob = JobSystem::GetAnyAvaidableJob();
      if( currentJob )
      {
        currentJob->taskFunction( currentJob->jobParams );
        if( currentJob->threadCleanUpJob )
          delete currentJob;
        currentJob = nullptr;
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
}

Job* JobSystem::GetAnyAvaidableJob()
{
  Job* returningJob;
  if( !m_jobsMutex.try_lock() )
    return nullptr;
  if( m_jobCount > 0 )
  {
    returningJob = m_jobs.front();
    m_jobs.pop();
    m_jobCount--;
    m_jobsMutex.unlock();
    return returningJob;
  }
  m_jobsMutex.unlock();
  return nullptr;
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

void JobSystem::ScheduleJob( Job* a_jobToAdd )
{
  if( m_hasInitBeenCalled )
  {
    if( !a_jobToAdd )
    {
      LOGE( "I recieved a null job!" );
      return;
    }
    std::lock_guard<std::mutex> lock( m_jobsMutex );
    m_jobs.push( a_jobToAdd );
    m_jobCount++;
  }
  else
    LOGE( "Tried to schedule job but jobsystem has not be initialized" );
}