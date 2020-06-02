
#include "WindowMacros.h"
#include "WindowEvents.c"

enum {
	NOT_HANDLED = 0,
	SUCCESS,
	CANNOT_CREATE_WINDOW
};

long __stdcall
WindowEngine(HWND Wnd, UINT Message, WPARAM WParam, LPARAM LParam) {
	static COORD coord = {0};
	int result;

	switch (Message) {

	DEFINE_EVENT_PARAM0(WM_PAINT, AppWindow_OnPaint);

	DEFINE_EVENT_PARAM0(WM_NULL, AppWindow_OnNull);

	DEFINE_EVENT_PARAM2(WM_SETFOCUS, AppWindow_OnFocus, (HWND)WParam, 1);
	DEFINE_EVENT_PARAM2(WM_KILLFOCUS, AppWindow_OnFocus, (HWND)WParam, 0);
	
/*
	case WM_KEYDOWN: {
		AppWindow_OnKeyDown(
			Wnd,
				
		);
	}
*/

    case WM_VSCROLL: {
	switch(LOWORD(WParam)) {
	case SB_THUMBPOSITION:
		//hiword equals the scrollbox position
	case SB_THUMBTRACK:	
		//hiword equals the scrollbox position
	case SB_BOTTOM:
	case SB_ENDSCROLL:
	case SB_LINEDOWN:
	case SB_LINEUP:
	case SB_PAGEDOWN:
	case SB_PAGEUP:
	case SB_TOP:
		break;
	}
    } return 0;

    case WM_HSCROLL: {
        switch(LOWORD(WParam)) {
        case SB_THUMBPOSITION:
		//hiword equals the scrollbox position
        case SB_THUMBTRACK:
		//hiwork equals the scrollbox position	
        case SB_ENDSCROLL:
        case SB_LEFT:
        case SB_RIGHT:
        case SB_LINELEFT:
        case SB_LINERIGHT:
        case SB_PAGELEFT:
        case SB_PAGERIGHT:
            break;
        }
    } return 0;

    //Windows updates the scrollbar back ground through this message
    case WM_CTLCOLORSCROLLBAR: {
	//AppWindow_OnDrawScrollBar();
	
    } return 0; //return a handle to the brush


/* HANDLE MOUSE CLICKS */
	DEFINE_MOUSE_EVENT1(WM_MOUSEMOVE, AppWindow_OnMouseMove, (UINT)WParam);

	DEFINE_MOUSE_EVENT1(WM_LBUTTONDOWN, AppWindow_OnLeftMouse, 1);
	DEFINE_MOUSE_EVENT1(WM_LBUTTONUP, AppWindow_OnLeftMouse, 0);
	DEFINE_MOUSE_EVENT0(WM_LBUTTONDBLCLK, AppWindow_OnLeftMouseDouble);

	DEFINE_MOUSE_EVENT1(WM_RBUTTONDOWN, AppWindow_OnRightMouse, 1);
	DEFINE_MOUSE_EVENT1(WM_RBUTTONUP, AppWindow_OnRightMouse, 0);
	DEFINE_MOUSE_EVENT0(WM_RBUTTONDBLCLK, AppWindow_OnRightMouseDouble);

	DEFINE_MOUSE_EVENT1(WM_MBUTTONDOWN, AppWindow_OnMiddleMouse, 1);
	DEFINE_MOUSE_EVENT1(WM_MBUTTONUP, AppWindow_OnMiddleMouse, 0);
	DEFINE_MOUSE_EVENT0(WM_MBUTTONDBLCLK, AppWindow_OnMiddleMouseDouble);

	case WM_MOUSEWHEEL:
	if(HISWORD_DW(WParam) > 0) {
		coord.X = LOSWORD_DW(LParam);
		coord.Y = HISWORD_DW(LParam);
		if(AppWindow_OnMouseWheelUp
			(Wnd,
			LOUWORD_DW(WParam),
			coord)) return 0;
	}
	else {
		coord.X = LOSWORD_DW(LParam);
		coord.Y = HISWORD_DW(LParam);
		if(AppWindow_OnMouseWheelDown
			(Wnd,
			LOUWORD_DW(WParam),
			coord)) return 0;
	}
	break;

	//case WM_MOUSELEAVE:


    DEFINE_EVENT_PARAM3(WM_SIZE, AppWindow_OnSize, (UINT)WParam, LOUWORD_DW(LParam), HIUWORD_DW(LParam));

case WM_MOVE: {
	coord.X = LOSWORD_DW(LParam);
	coord.Y = HISWORD_DW(LParam);
	if(AppWindow_OnMove(Wnd, coord)) return 0;
	GetWindowLongPtr(Wnd,  
} break;

case WM_MOVING: {
	if(AppWindow_OnMoving(Wnd, (RECT*)LParam)) return 1;
} break;

case WM_SHOWWINDOW: {
	if(WParam)
		if(AppWindow_OnShow(Wnd, LParam)) return 0;
	else
		if(AppWindow_OnHide(Wnd, LParam)) return 0;
} break;


case WM_CREATE: {
	if(AppWindow_OnCreate(Wnd, (CREATESTRUCT*) LParam)) return 0;
	/*
	else if(result == CANNOT_CREATE_WINDOW)
		return -1;
	*/
}
break;

DEFINE_EVENT_PARAM0(WM_CLOSE, AppWindow_OnClose);
DEFINE_EVENT_PARAM0(WM_DESTROY, AppWindow_OnDestroy);

case WM_KEYDOWN: {
	if(AppWindow_OnKey(Wnd)) return 0;
} break;
	
	DEFINE_EVENT_PARAM0(WM_EXITSIZEMOVE, AppWindow_OnExitSizeMove);

	/* HANDLE SYSCOMMAND MESSAGES */
	case WM_SYSCOMMAND: {
		if (!(HISWORD_DW(LParam) > 0)) break; /* using accelerator or mnemonic */
		
		coord.X = LOSWORD_DW(LParam);
		coord.Y = HISWORD_DW(LParam);
		switch(0xFFF0 & WParam) {
		case SC_CLOSE: {
			if(AppWindow_OnClickClose(Wnd, coord)) return 0;
		} break;

		case SC_MAXIMIZE: {
			if(AppWindow_OnClickMaximize(Wnd, coord)) return 0;
		} break;

		case SC_MINIMIZE: {
			if(AppWindow_OnClickMinimize(Wnd, coord)) return 0;
		} break;

		} //end switch
	} break;

	case WM_TIMER: {
		if(AppWindow_OnTimer(Wnd, WParam, LParam)) return 0;
	} break;

	case WM_QUIT: {
		if(AppWindow_OnQuit(Wnd)) return 0;				  
	} break;
	
	} /* end switch */

DEFAULT_WINDOW_PROCEDURE:
	return DefWindowProc (Wnd, Message, WParam, LParam);
}

