#ifndef PROCESS_H
#define PROCESS_H

#include <winsock2.h>
#include <windows.h>

//C runtime headers
#include <stdio.h>
#include <assert.h>

//Network headers
#include <mswsock.h>
#include <Iphlpapi.h>

//Native Windows headers
#include <ntstatus.h>

//ODBC headers
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

/* process attributes */
#define GUI_PA     0x0001
#define CONSOLE_PA 0x0002
#define SERVICE_PA 0x0004
#define NETWORK_PA 0x0008
#define COM_PA     0x0010
#define ODBC_PA    0x0020 

struct async_packet
{
union
    { 
    struct {
        struct {
            union {
                WSAOVERLAPPED wsaovl_s;
                OVERLAPPED ovl_s;    
                };
            DWORD op_type;
            char data[8];
            } object;
        struct {
            int (__cdecl *fn)(struct async_packet*);
            int thread_mode;    
            } handler;
        };
    struct async_packet *next;
    };
CRITICAL_SECTION crit_sect;
};
#define SIZE_ASYNC_PACKET sizeof(struct async_packet)
struct p_config 
{
uint32 attr;
uint16 scale;
sint16 ratio[8];
ULONG  apqsize;
};
#define SIZE_PCONFIG sizeof(struct p_config)

struct process_memory_schema
{
uint16 scale;
sint16 ratio[8];
};

struct memory_pool_arena
{
void *next;
};

struct app_window
{
HWND hwnd;
HDC dc;
MSG msg;
WNDCLASSEX cls;
ULONG width;
ULONG height;
struct ScrollBar* scroll_bar;
};
#define SIZE_APP_WINDOW sizeof(struct app_window)

#define PROCESSOR_COUNT        process.processor_count
#define PAGE_SIZE              process.page_size
#define ALLOCATION_GRANULARITY process.allocation_granularity
#define OS_VERSION             process.os_version
#define IO_COMPLETION_PORT     process.hiocp
#define IOCP                   IO_COMPLETION_PORT

struct process
{
HINSTANCE hinstance; 
uint8   processor_count;
uint16  page_size;
uint32  allocation_granularity;
int     os_version;
uint16  os_external_version;
uint16  os_internal_version;
ULONG   apqsize; /* size of each thread's apq */

HANDLE hiocp,
       hevent;
    struct
    {
    HWND hwnd;
    HANDLE output;
    HANDLE input;
    INPUT_RECORD *console_records;
    } console;

    struct
    {
    struct thread *head;
    HANDLE *hthreads;
    int thread_count;
    } thread_pool;

    struct async_packet_pool
    {
    void *head;
    short size;
    struct async_packet *next;
    CRITICAL_SECTION crit_sect;
    } async_packet_pool;

void *mem_base;

    struct
    {
    void *next;
    uint16 arena_size;
    void *low_address;
    void *high_address;
    uint16 arena_count;
    CRITICAL_SECTION crit_sect;    
    } mem[8];

struct app_window* app_window;

}; /* struct process */
#define SIZE_PROCESS sizeof(struct process)


#define THREAD_HANDLE thread->hthread
#define THREAD_PRIORITY thread->priority
#define THREAD_INDEX thread->index
#define THREAD_ID thread->thread_id
#define THREAD_MODE thread->op_mode
#define THREAD_COUNT process.thread_pool.thread_count
struct thread
{
HANDLE hthread;
struct thread  *next;
FILETIME creation_time;
FILETIME exit_time;
FILETIME kernel_time;
FILETIME user_time;
int priority;
int index;
int thread_id;
short op_mode;

void* ThreadBuf;
int nThreadBufSize;
struct async_packet *async_packet;
struct
    {
    OVERLAPPED_ENTRY *entries;
    ULONG pending;
    } async_packet_queue;

RIO_CQ RIOCompletionQueue;

struct
    {
    union
        {
        SOCKET socket;
        SOCKET *next;    
        };
    int count;
    } socket_pool;
};
#define SIZE_THREAD sizeof(struct thread)

struct scroll_bar
{
HBRUSH hbrush; 
}; 

enum
{
THREAD_MODE_CURRENT = 0,
THREAD_MODE_IO,
THREAD_MODE_CONSOLE_INIT,
THREAD_MODE_CONSOLE,
THREAD_MODE_SHUTDOWN,
THREAD_MODE_WINDOW_INIT,
THREAD_MODE_WINDOW,
THREAD_MODE_PAUSE,
THREAD_MODE_LOAD_SERVICES,
};

struct ODBCDatabase
{
SQLHENV SQLEnv;
SQLHDBC SQLConn;
SQLHSTMT SQLStmt;
SQLINTEGER ODBCVersion;
};

GLOBAL struct process process;
#endif
