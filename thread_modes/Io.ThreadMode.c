
case THREAD_MODE_IO:
{
unsigned short i = 0, entries;
result = GetQueuedCompletionStatusEx (
 process.hiocp,        /* io completion port handle */
 APQ.entries,          /* array of struct _OVERLAPPED entries */
 process.apqsize,      /* number of the overlapped structs in array */
 (PULONG)&APQ.pending, /* pointer to number of entries removed */
 INFINITE,             /* number of milliseconds to wait */
 1);                   /* boolean for alertable io */
if(result == 0)
    {
    result = GetLastError();
    if(result == WAIT_IO_COMPLETION) break; //0x00C0 check if an APC was dequeued
    else
        {
        //Error occured, functionfailed, check why
        printf("Error %d: GetQueuedCompletionStatusEx failed in Thread %d\n",
            result, thread->index);
        //ExitProcess(0);
        }
    }
entries = APQ.pending;
printf("Processing %d entries on thread %d...\n",
 entries, thread->index);
while(i < entries)
    {
    async_packet = (struct async_packet*)
        (APQ.entries[i]).lpCompletionKey;

    if(!async_packet) continue;
    //Check if the threads mode has been changed
    if(async_packet->handler.thread_mode != 0 &&
    async_packet->handler.thread_mode!= thread_mode)
	{
        //if thread mode has already been changed once in this completion
        //pass off this packet to another thread
        if(THREAD_MODE == thread_mode)
            THREAD_MODE = async_packet->handler.thread_mode;
        else
            {
            PostQueuedCompletionStatus
		(process.hiocp, 0, (ULONG_PTR)async_packet,
	        (OVERLAPPED*)async_packet);
            goto PROCESS_NEXT_ASYNC_PACKET;
            }
        }
    //Run the handler procedure if applicable
    if(async_packet->handler.fn) 
	    {
        async_packet->handler.fn
	    ((struct async_packet*)APQ.entries[i].lpOverlapped);
        }
push_async_packet(async_packet);

PROCESS_NEXT_ASYNC_PACKET:
        APQ.pending--; i++;
    }
async_packet = 0;
thread_mode = THREAD_MODE;
} break;
