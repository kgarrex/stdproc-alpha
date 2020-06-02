PRIVATE void
create_thread_pool(struct p_config *cnf)
{
va_list args;
int thread_idx = 0, i = 1, main_thread_id;
struct thread *thread = 0;
uint32 flags = 0;
BOOL sync_threads = 0; /* temp, move to pconfig */
uint32 thread_count = 0;

INIT_TIMER(1);
/* get memory for thread handles */
process.thread_pool.hthreads = alloc_mem(64);

/* calculate total thread count */
thread_count = PROCESSOR_COUNT-1;
if(cnf->attr & GUI_PA) thread_count++;
if(cnf->attr & CONSOLE_PA) thread_count++;

//main thread is the first thread
process.thread_pool.head = thread = alloc_mem(SIZE_THREAD);
THREAD_ID = main_thread_id = GetCurrentThreadId();
THREAD_INDEX = thread_idx++;
THREAD_HANDLE = OpenThread(0, 1, THREAD_ID);

//if(sync_threads) flags |= CREATE_SUSPENDED;
while(thread_count--)
    {
	thread->next = alloc_mem(SIZE_THREAD);
	thread = thread->next;
    THREAD_HANDLE = CreateThread
    (0,  //security attributes
     CREATE_SUSPENDED,    //stacksize, use default
     thread_engine,
     thread,
     flags,
     &THREAD_ID);
    assert(THREAD_HANDLE != INVALID_HANDLE_VALUE);
	process.thread_pool.hthreads[i++] = THREAD_HANDLE;
	THREAD_INDEX = thread_idx++;
    }
thread->next = 0;
/* synchronize thread starts */
thread = process.thread_pool.head;
if(cnf->attr & GUI_PA)
	{
	THREAD_MODE = THREAD_MODE_WINDOW_INIT;
	printf("Starting thread %d\n", THREAD_INDEX);
	if(main_thread_id != THREAD_ID)
	    {
	    ResumeThread(THREAD_HANDLE);
		}
	thread = thread->next;
	cnf->attr &= ~GUI_PA;
	}
if(cnf->attr & CONSOLE_PA)
	{
	THREAD_MODE = THREAD_MODE_CONSOLE_INIT;	
	printf("Starting thread %d\n", THREAD_INDEX);
	if(main_thread_id != THREAD_ID)
	    {
	    ResumeThread(THREAD_HANDLE);
		}
	thread = thread->next;
	cnf->attr &= ~CONSOLE_PA;
	}
do
	{
	THREAD_MODE = THREAD_MODE_IO;
	printf("Starting thread %d\n", THREAD_INDEX);
	if(main_thread_id != THREAD_ID)
	    {
	    ResumeThread(THREAD_HANDLE);
		}
	} while(thread = thread->next);
}

