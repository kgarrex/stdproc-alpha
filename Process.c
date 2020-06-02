#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Iphlpapi.lib")

#include "core.h"
#include "process.h"

void create_thread_pool(struct p_config*);
void* create_async_packet_pool(struct async_packet_pool*, void*, uint32);
DWORD WINAPI thread_engine(struct thread *thread); 
void* init_process();

struct process process = {0};

#include "mempool.c"
#include "Window\WindowEngine.c"
#include "async_packet.c"
#include "network.c"
#include "thread_pool.c"
//#include "Odbc.c"
//#include "COM\COM.c"
//#include "ACE.c"
//#include "MAPICode.c"

/*
 * NOTE: May want to move process startup code inside of thread procedure.
 * This will allow the window code to run immediately to tell the user that
 * the process is starting up. This, of course, is depenedent on how long
 * process startup code takes.
 */

inline void __stdcall
thread_shutdown_proc(struct thread *thread)
{
thread->op_mode = THREAD_MODE_SHUTDOWN;
} 
void pass_off_thread_work() {
/*
PopAsyncPacket(AsyncPacket, 0);
THREAD_MODE = THREAD_MODE_IO;
AsyncPacket->Handler.ThreadMode = CurrentMode;
AsyncPacket->Handler.Proc = 0;
if(!PostQueuedCompletionStatus(
	Process.IOCPHandle,
	0,
	(ULONG_PTR)AsyncPacket,
	(LPOVERLAPPED)AsyncPacket))
{
printf("Error %d: Could not post asynchronous packet\n", GetLastError());
return 0;    
}
*/
}

#define PACK2_UINT8(hi, lo)\
    (uint16)((((uint8)hi) << 8) | ((uint8)lo))

EXTERNAL void pconfig(struct p_config*);

EXTERNAL void pconfig(struct p_config *cnf)
{
/* cnf->attr |= GUI_PA; */
cnf->scale    = 1;
cnf->ratio[0] = 1; //32
cnf->ratio[1] = 1; //64
cnf->ratio[2] = 3;
cnf->ratio[3] = 3;
cnf->ratio[4] = 3;
cnf->ratio[5] = 2;
cnf->ratio[6] = 1;
cnf->ratio[7] = 2; //4096
}

PRIVATE void init()
{
SYSTEM_INFO sysinfo = {0};
OSVERSIONINFO osinfo = {0};
struct p_config cnf = {0};
void* pool = 0;

GetSystemInfo(&sysinfo);
process.hinstance      = GetModuleHandle(0);
PROCESSOR_COUNT        = sysinfo.dwNumberOfProcessors;
PAGE_SIZE              = sysinfo.dwPageSize;
ALLOCATION_GRANULARITY = sysinfo.dwAllocationGranularity;
OS_VERSION             = 0;
IO_COMPLETION_PORT     = CreateIoCompletionPort
 (INVALID_HANDLE_VALUE, 0, 0, PROCESSOR_COUNT);

//get_os_version_info();

pconfig(&cnf);
process.apqsize = 16;

//Create the process memory pool
INIT_TIMER(1);
/*
create_memory_pool
(sysinfo.dwAllocationGranularity,
 1, //scale
 &process.mem_base,

 );
*/

pool = mem_alloc(KB(1));
assert(pool);
create_async_packet_pool(&process.async_packet_pool, pool, KB(1));
create_thread_pool(&cnf);
}

void* init_process()
{
INIT_TIMER(1);
struct async_packet *async_packet = 0;
DWORD error = 0;
if(PAGE_SIZE) return 0; /* check if process has been initialized */

init();
assert(process.thread_pool.head);
thread_engine(process.thread_pool.head);


//Initialize the network
/*
AsyncPacket = pop_async_packet(NetworkAsyncHandler, 0, SOCKET_INIT);
PostQueuedCompletionStatus(
	IO_COMPLETION_PORT, 0,
	(ULONG_PTR)AsyncPacket,
	(WSAOVERLAPPED*)AsyncPacket); 
*/

//Initialize COM
/*
AsyncPacket = pop_async_packet(COMAsyncHandler, 0, COM_INIT);
PostQueuedCompletionStatus(
	IOCP, 0,
	(ULONG_PTR)AsyncPacket,
	(OVERLAPPED*)AsyncPacket);
*/

/*
//Initialize MAPI
AsyncPacket = pop_async_packet(MAPIAsyncHandler, 0, MAPI_INIT);
PostQueuedCompletionStatus(
	IOCP, 0,
	(ULONG_PTR)AsyncPacket,
	(OVERLAPPED*)AsyncPacket);
*/

/*
//Initialize ODBC
AsyncPacket = pop_async_packet(ODBCAsyncHandler, THREAD_MODE_IO, ODBC_INIT);
PostQueuedCompletionStatus(
	IOCP, 0,
	(ULONG_PTR)AsyncPacket,
	(OVERLAPPED*)AsyncPacket);
*/

/*
//Initialize ACE Database Engine
AsyncPacket = pop_async_packet(ACEAsyncHandler, THREAD_MODE_IO, ACE_INIT);
PostQueuedCompletionStatus(
	IOCP, 0,
	(ULONG_PTR)AsyncPacket,
	(OVERLAPPED*)AsyncPacket);
*/

error = WaitForMultipleObjectsEx
(process.thread_pool.thread_count-1,
 (HANDLE*)(&process.thread_pool.hthreads[1]),
 1, INFINITE, 0);
if(error == WAIT_FAILED)
	printf("Error %d: Failed to wait for threads to exit\n", GetLastError());    
CloseHandle(process.console.input);
printf("Exiting Process...\n");
ExitProcess(0);
return 0;
}

#define APQ thread->async_packet_queue
#define APQ_SIZE APQ.size
#define APQ_ENTRIES APQ.entries
DWORD __stdcall thread_engine(struct thread *thread)
{
struct async_packet *async_packet;
int thread_mode = THREAD_MODE;
DWORD result = 0;
//Thread->nPriority = GetThreadPriority(Thread->ThreadHandle);
APQ.entries = mem_alloc(BYTESIZE(OVERLAPPED_ENTRY) * process.apqsize);
while(INFINITE)
{
    switch(THREAD_MODE)
    {
    #include "thread_modes\Io.ThreadMode.c"
    #include "thread_modes\Window.ThreadMode.c"
    //#include "ThreadModes\Console.ThreadMode.c"
    //#include "thread_modes\Shutdown.ThreadMode.c"
    //#include "ThreadModes\Pause.ThreadMode.c"
    } //end switch
} //end while

}
#undef APQ
