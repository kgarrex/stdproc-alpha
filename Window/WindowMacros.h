
#define DEFINE_MOUSE_EVENT0(msg, name) \
	case msg: { \
		coord.X = LOSWORD_DW(LParam); \
		coord.Y = HISWORD_DW(LParam); \
		if(name(Wnd, coord)) return 0; \
	} \
	break

#define DEFINE_MOUSE_EVENT1(msg, name, p1) \
	case msg: { \
		coord.X = LOSWORD_DW(LParam); \
		coord.Y = HISWORD_DW(LParam); \
		if(name(Wnd, coord, p1)) return 0; \
	} \
	break

#define DEFINE_EVENT_PARAM0(msg, name) \
	case msg: { \
		if(name(Wnd)) return 0; \
	} \
	break

#define DEFINE_EVENT_PARAM1(msg, name, p1) \
	case msg: { \
		if(name(Wnd, p1)) return 0; \
	} \
	break

#define DEFINE_EVENT_PARAM2(msg, name, p1, p2) \
	case msg: { \
		if(name(Wnd, p1, p2)) return 0; \
	} \
	break

#define DEFINE_EVENT_PARAM3(msg, name, p1, p2, p3) \
	case msg: { \
		if(name(Wnd, p1, p2, p3)) return 0; \
	} \
	break
