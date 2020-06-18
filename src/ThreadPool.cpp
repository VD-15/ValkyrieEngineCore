#include "ValkyrieEngine/ThreadPool.hpp"
#include "ValkyrieEngine/ValkyrieDebug.hpp"

using namespace vlk;

ThreadPool::Internal ThreadPool::internal;

Thread::Thread()
{
	this->isJoining = false;

	this->thr = std::thread([this]
	{
		std::unique_lock ulock(this->mtx);

		while (true)
		{
			//No function was assigned, go to sleep
			if (!static_cast<bool>(this->fn))
			{
				this->cv.wait(ulock, [this]{ return static_cast<bool>(this->fn) || this->isJoining; });
				if (this->isJoining) return;
			}
			
			this->fn();
			this->fn = ThreadPool::GetNext();
		}
	});
}

Thread::~Thread()
{
	std::unique_lock ulock(this->mtx);
	this->isJoining = true;
	ulock.unlock();

	this->cv.notify_one();
	this->thr.join();
}

void Thread::SetFunc(const std::function<void()>& newFunc)
{
	std::unique_lock ulock(this->mtx);
	this->fn = newFunc;
	ulock.unlock();

	//If we're waiting on the condition variable, notify it.
	this->cv.notify_one();
}

bool Thread::IsIdle()
{
	std::unique_lock ulock(this->mtx, std::try_to_lock);

	if (ulock.owns_lock())
	{
		return !static_cast<bool>(this->fn);
	}
	else
	{
		return false;
	}
}

ThreadPool::Internal::Internal()
{
	this->numThreads = std::thread::hardware_concurrency();
	Size defaultThreads = 4;

	//TODO: Find a way to override thread limit

	if (this->numThreads == 0)
	{
		this->numThreads = defaultThreads;
	}
	else
	{
		Log("Detected " + std::to_string(numThreads) + " threads.");
	}

	this->threads = static_cast<Thread*>(malloc(sizeof(Thread) * numThreads));

	for (Size i = 0; i < this->numThreads; i++)
	{
		new (&this->threads[i]) Thread();
	}
}

ThreadPool::Internal::~Internal()
{
	for (Size i = 0; i < this->numThreads; i++)
	{
		this->threads[i].~Thread();
	}

	free(this->threads);
}

std::function<void()> ThreadPool::GetNext()
{
	std::unique_lock ulock(internal.mtx);

	if (internal.pending.empty())
	{
		internal.cv.notify_all();
		return std::function<void()>();
	}
	else
	{
		std::function<void()> fn(internal.pending.front());
		internal.pending.pop();
		return fn;
	}
}

void ThreadPool::Enqueue(const std::function<void()>& func)
{
	std::unique_lock ulock(internal.mtx);

	//If there is an idle thread, give it a job, can't have it lazing about all day!
	for (Size i = 0; i < internal.numThreads; i++)
	{
		Thread* th = &internal.threads[i];
		if (th->IsIdle())
		{
			th->SetFunc(func);
			return;
		}
	}

	//If there are no threads available, put it in the backlog.
	internal.pending.push(func);
}

void ThreadPool::Wait()
{
	std::unique_lock ulock(internal.mtx);
	internal.cv.wait(ulock), []{ return ThreadPool::IsIdle(); };
}

bool ThreadPool::IsIdle()
{
	std::unique_lock ulock(internal.mtx);
	
	//Still functions in the queue, not idle
	if (!internal.pending.empty()) return false;

	for (Size i = 0; i < internal.numThreads; i++)
	{
		Thread* thr = &internal.threads[i];
		//A thread is still working, not idle.
		if (!thr->IsIdle()) return false;
	}

	//Yup, we're idle.
	return true;
}
