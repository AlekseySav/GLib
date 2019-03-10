#if !defined(_GLIB_SYS_HANDLES_) && defined(_GLIB)
#define _GLIB_SYS_HANDLES_

struct Element {
	int32 x, y;
	Image im;
};

EXTERN inline Element glibCreateElement(int32 x, int32 y, int32 width, int32 height);

typedef struct Button__ {
	Element magic;
	EventHandles events;
} * Button;

EXTERN Button glibCreateButton(Element e);


#endif
