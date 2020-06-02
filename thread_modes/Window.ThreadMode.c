
#define WndCls process.app_window->cls
#define WndMsg process.app_window->msg
#define WndHandle process.app_window->hwnd
case THREAD_MODE_WINDOW_INIT:
{
process.app_window = alloc_mem(sizeof(*process.app_window));

WndCls.cbSize 		= sizeof(WNDCLASSEX);
WndCls.style 		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_PARENTDC;
WndCls.lpfnWndProc 	= WindowEngine;
WndCls.cbClsExtra 	= 0;
WndCls.cbWndExtra 	= 0;
WndCls.hInstance 	= GetModuleHandle(0);
WndCls.hIcon 		= 0;
WndCls.hCursor 		= LoadCursor(0, IDC_ARROW);
WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
WndCls.lpszMenuName 	= 0;
WndCls.lpszClassName = "MyWindow";
WndCls.hIconSm 		= 0;
RegisterClassEx(&WndCls);

int ExStyle = 0;
int Style = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
WndHandle = CreateWindowEx
(ExStyle,
 WndCls.lpszClassName,
 WndCls.lpszClassName,
 Style,
 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, //starting position
 0, //parent window handle
 0, //menu handle
 GetModuleHandle(0),
 process.app_window);
if(WndHandle == 0)
	{
	printf("Error %d: Could not create window\n", GetLastError());	
	return 0;
	}
thread->op_mode = THREAD_MODE_WINDOW;
thread_mode = THREAD_MODE_WINDOW;
}

case THREAD_MODE_WINDOW:
{
//Look at using Flat scrollbars
while(THREAD_MODE == thread_mode)
    {
	result = MsgWaitForMultipleObjectsEx
	(0, 0, INFINITE,
	QS_INPUT|QS_ALLEVENTS|QS_ALLINPUT|QS_MOUSEBUTTON|QS_MOUSEMOVE,
	MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);
	if(result == WAIT_OBJECT_0)
		{
		while(PeekMessage(&WndMsg, 0, 0, 0, PM_REMOVE))
		    {
			TranslateMessage(&WndMsg);
			DispatchMessage(&WndMsg);
			if(WndMsg.message == WM_QUIT) goto QUIT_WINDOW;
			}
		}
    }
break;
QUIT_WINDOW:
//Loop through every thread and queue the shutdown apc passing the
//thread pointer to the function
struct thread *thread_peer = process.thread_pool.head;
do
    {
	if(thread_peer == thread) continue;
	QueueUserAPC((PAPCFUNC)thread_shutdown_proc,
		thread_peer->hthread, (ULONG_PTR)thread_peer);
    } while (thread_peer = thread_peer->next);
thread->op_mode = THREAD_MODE_SHUTDOWN;
} break;
#undef WndCls
#undef WndMsg
#undef WndHandle
