enum {
CONSOLE_INPUT_INDEX,
};

case THREAD_MODE_CONSOLE_INIT:
{
SECURITY_ATTRIBUTES Security = {0};
Security.bInheritHandle = 1;
/* Allocate a console */
//FreeConsole();
AllocConsole();
Process.Console.WndHandle = GetConsoleWindow();

/*
ntstatus = NtCreateFile
(&hcons,
 FILE_READ_DATA | FILE_WRITE_DATA,
 pobjattr,
 piostatusblock,
 RESERVED,
 FILE_ATTRIBUTE_NORMAL,
 FILE_SHARE_WRITE | FILE_SHARE_READ,
 FILE_CREATE,
 createoptions,
 eabuf,
 eabufsz);
if(ntstatus != STATUS_SUCCESS)
{
//report error
}
*/

Process.Console.Input = CreateFile
("CONIN$",
 GENERIC_READ|GENERIC_WRITE,
 FILE_SHARE_READ,
 &Security,
 OPEN_EXISTING, 0, 0);
Process.Console.Output = CreateFile
("CONOUT$",
 GENERIC_READ | GENERIC_WRITE,
 FILE_SHARE_WRITE,
 &Security,
 OPEN_EXISTING, 0, 0);
Process.Console.ConsoleRecords = PopMemoryArena(sizeof(INPUT_RECORD) * 12);
	
    if(!SetConsoleMode(Process.Console.Input,
    ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT))
    {
    printf("Error %d: Could not SetConsoleMode\n", GetLastError());
    return 0;
    }
THREAD_MODE = THREAD_MODE_CONSOLE;
CurrentMode = THREAD_MODE_CONSOLE;
}

#define NUM_HANDLES 1
case THREAD_MODE_CONSOLE:
{
//Run the console code here
int Result = 0;
int WakeMask = 0, WaitFlags = 0; 
INPUT_RECORD* InputRecord;
DWORD NumWritten;
HANDLE WaitHandles[NUM_HANDLES];
WaitHandles[0] = Process.Console.Input;
//WakeMask = QS_HOTKEY | QS_KEY | QS_MOUSEBUTTON | QS_MOUSEMOVE
//	| QS_PAINT | QS_POSTMESSAGE | QS_RAWINPUT | QS_SENDMESSAGE | QS_TIMER;
WaitFlags = MWMO_ALERTABLE | MWMO_INPUTAVAILABLE;
Result = MsgWaitForMultipleObjectsEx
    (NUM_HANDLES, WaitHandles, INFINITE, WakeMask, WaitFlags);
	
switch(Result) {
case WAIT_FAILED:
{
    printf("Error 0x%x: WaitForMultipleObjectsEx failed\n", GetLastError());
    break;
}
case WAIT_IO_COMPLETION: //APC Completion
{
    break;
}
case NUM_HANDLES:
{
    printf("Recieved Message\n");
    break;
}
case CONSOLE_INPUT_INDEX:
{
    printf("Running console mode\n");
    if(!ReadConsoleInput(
    Process.Console.Input,
    Process.Console.ConsoleRecords,
    12,
    &NumWritten))
    {
        printf("Error %d: Could not read console input\n", GetLastError());
        return 0;
    }
    printf("Reading %d console inputs on thread %d\n",
    NumWritten, Thread->nIndex);
    InputRecord = Process.Console.ConsoleRecords;
    for(int i = 0; i != NumWritten; i++) {
        switch((InputRecord[i]).EventType) {
        case KEY_EVENT:
            printf("KEY_EVENT\n");
            break;
        case MOUSE_EVENT:
            printf("MOUSE_EVENT\n");
            break;
        case WINDOW_BUFFER_SIZE_EVENT:
            printf("WINDOW_BUFFER_SIZE_EVENT\n");
            break;
        case FOCUS_EVENT:
            printf("FOCUS_EVENT\n");
            break;
        case MENU_EVENT:
            printf("MENU_EVENT\n");
            break;
        }
    }
}
} //end switch

} break;
#undef NUM_HANDLES
