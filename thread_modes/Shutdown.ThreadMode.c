
case THREAD_MODE_SHUTDOWN:
{
	//perform application cleanup	
	printf("Thread %d ended\n", thread->index);
	if(thread != process.thread_pool.head) ExitThread(0);
} return 0;
