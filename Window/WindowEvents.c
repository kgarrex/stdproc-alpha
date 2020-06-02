enum {
	EVENT_NOT_HANDLED = 0,
	EVENT_HANDLED,
};

inline int AppWindow_OnIdle(HWND Wnd) {
	//printf("OnIdle\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnNull(HWND Wnd) {
	printf("OnNull\n");
	return EVENT_HANDLED;
}

inline int AppWindow_OnCreate(HWND Wnd, CREATESTRUCT* CS) {
	//printf("OnCreate\n");
	//ShowScrollBar(Wnd, SB_BOTH, 1);
	ShowWindow(Wnd, SW_SHOWNORMAL);
	return EVENT_HANDLED;
}

inline int AppWindow_OnShow(HWND Wnd, uint x ) {
	//printf("OnShow\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnHide(HWND Wnd, uint x) {
	//printf("OnHide\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnSize(HWND Wnd, UINT ResizeType, short Width, short Height) {
	printf("OnSize\n");
	return EVENT_HANDLED;
} 

inline int AppWindow_OnDestroy(HWND Wnd) {
	printf("OnDestroy\n");
	PostQuitMessage(EVENT_NOT_HANDLED);
	return EVENT_HANDLED;
}

inline int AppWindow_OnMove(HWND Wnd, COORD XY) {
	printf("OnMove\n");
	return EVENT_HANDLED;
}

inline int AppWindow_OnMoving(HWND Wnd, RECT* Rect) {
	//printf("OnMoving\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnClose(HWND Wnd) {
	//printf("OnClose\n");
	//DestroyWindow(Wnd);
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnQuit(HWND Wnd) {
	//printf("OnQuit\n");
	return EVENT_NOT_HANDLED;
}

//Render, ReIllustrate, etc...
inline int AppWindow_OnPaint(HWND Wnd) {
	PAINTSTRUCT Paint;
	BeginPaint(Wnd, &Paint);
	printf("OnPaint\n");
	EndPaint(Wnd, &Paint);
	return 1;
}

inline int AppWindow_OnKey(HWND Wnd) {
	//printf("OnKey\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnExitSizeMove(HWND Wnd) {
	//printf("OnExitSizeMove\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnClickClose(HWND Wnd, COORD MouseXY) {
	//printf("OnClickClose\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnClickMinimize(HWND Wnd, COORD XY) {
	//printf("OnClickMinimize\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnClickMaximize(HWND Wnd, COORD XY) {
	//printf("OnClickMaximize\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnMouseMove(HWND Wnd, COORD Coord, uint x) {
	//printf("OnMouseMove\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnLeftMouse(HWND Wnd, COORD Coord, BOOL x) {
	//printf("OnLeftMouse\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnLeftMouseDouble(HWND Wnd, COORD Coord) {
	//printf("OnLeftMouseDouble\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnRightMouse(HWND Wnd, COORD Coord, BOOL x) {
	//printf("OnRightMouse\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnRightMouseDouble(HWND Wnd, COORD Coord) {
	//printf("OnRightMouseDouble\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnMiddleMouse(HWND Wnd, COORD Coord, BOOL x) {
	//printf("OnMiddleMouse\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnMiddleMouseDouble(HWND Wnd, COORD Coord) {
	//printf("OnMiddleMouseDouble\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnMouseWheelUp(HWND Wnd, ushort x, COORD Coord) {
	//printf("OnMouseWheelUp\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnMouseWheelDown(HWND Wnd, ushort x, COORD Coord) {
	//printf("OnMouseWheelDown\n");
	return EVENT_NOT_HANDLED;
}

inline int AppWindow_OnFocus(HWND Wnd, HWND Wnd2, BOOL x) {
	printf("OnFocus\n");
	return 1;
}

//inline int AppWindow_OnVerticalScrollDown(){}
//inline int AppWindow_OnVerticalScrollUp(){}
//inline int AppWindow_OnHorizontalScrollLeft(){}
//inline int AppWindow_OnHorizonatalScrollRight(){}

//int AppWindow_OnTimer(uint x, TIMERPROC timer) {
inline int AppWindow_OnTimer(HWND Wnd, WPARAM WParam, LPARAM LParam) {
	//printf("OnTimer\n");
	return EVENT_NOT_HANDLED;
}
