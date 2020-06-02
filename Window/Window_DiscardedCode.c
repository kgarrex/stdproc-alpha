struct {
		void (*AppWindow_OnIdle) (HWND);
		void (*AppWindow_OnNull) (HWND);
		void (*AppWindow_OnCreate) (HWND, CREATESTRUCT*);
		void (*AppWindow_OnSize) (HWND, UINT, short Width, short Height);
		void (*AppWindow_OnDestroy) (HWND);
		void (*AppWindow_OnMove) (HWND, COORD XY);
		void (*AppWindow_OnMoving) (HWND, RECT*);
		void (*AppWindow_OnClose) (HWND);
		void (*AppWindow_OnPaint) (HWND);

	/* SYSTEM COMMAND EVENTS */
		void (*AppWindow_OnClickClose) (HWND, COORD MouseXY);
		void (*AppWindow_OnClickMinimize) (HWND, COORD);
		void (*AppWindow_OnClickMaximize) (HWND, COORD);

	/* Mouse Button Position */
		void (*AppWindow_OnMouseMove) 			(HWND, COORD, uint);
		void (*AppWindow_OnLeftMouse) 		(HWND, COORD, BOOL);
		void (*AppWindow_OnLeftMouseDouble) 	(HWND, COORD);
		void (*AppWindow_OnRightMouse) 		(HWND, COORD, BOOL);
		void (*AppWindow_OnRightMouseDouble) (HWND, COORD);
		void (*AppWindow_OnMiddleMouse) 		(HWND, COORD, BOOL);
		void (*AppWindow_OnMiddleMouseDouble) (HWND, COORD);
		void (*AppWindow_OnMouseWheelUp) (HWND, ushort, COORD);
		void (*AppWindow_OnMouseWheelDown) (HWND, ushort, COORD);

		void (*AppWindow_OnFocus) (HWND, HWND, BOOL);
		void (*AppWindow_OnTimer) (uint, TIMERPROC);
} Events;

Window->Events.OnIdle 				= OnIdle;
Window->Events.OnNull 				= OnNull;
Window->Events.OnCreate 			= OnCreate;
Window->Events.OnSize 				= OnSize;
Window->Events.OnDestroy 			= OnDestroy;
Window->Events.OnMove 				= OnMove;
Window->Events.OnMoving 			= OnMoving;
Window->Events.OnClose 				= OnClose;
Window->Events.OnPaint 				= OnPaint;
Window->Events.OnClickClose 		= OnClickClose;
Window->Events.OnClickMinimize 		= OnClickMinimize;
Window->Events.OnClickMaximize 		= OnClickMaximize;
Window->Events.OnMouseMove 			= OnMouseMove;
Window->Events.OnLeftMouse 			= OnLeftMouse;
Window->Events.OnLeftMouseDouble 	= OnLeftMouseDouble;
Window->Events.OnRightMouse 		= OnRightMouse;
Window->Events.OnRightMouseDouble 	= OnRightMouseDouble;
Window->Events.OnMiddleMouse 		= OnMiddleMouse;
Window->Events.OnMiddleMouseDouble 	= OnMiddleMouseDouble;
Window->Events.OnMouseWheelUp 		= OnMouseWheelUp;
Window->Events.OnMouseWheelDown 	= OnMouseWheelDown;
Window->Events.OnFocus 				= OnFocus;
Window->Events.OnTimer 				= OnTimer;


