#include "glib_src.h"

void glibSetEvent(EventHandles * events, EventHandle handle, u_int type)
{
	if (type & EVENT_SHOWN)
		events->shown = handle;
	if (type & EVENT_CLOSING)
		events->closing = handle;
	if (type & EVENT_CLOSED)
		events->closed = handle;
	if (type & EVENT_RESIZE)
		events->resize = handle;
	if (type & EVENT_MOVED)
		events->moved = handle;
}

bool glibRunEvent(EventHandles * events, EventArgs * args)
{
	switch (args->msg)
	{
	case EVENT_SHOWN:
			if (events->shown != NULL)
				events->shown(args);
			else return false;
			break;
	case EVENT_CLOSING:
		if (events->closing != NULL)
			events->closing(args);
		else return false;
		break;
	case EVENT_CLOSED:
		if (events->closed != NULL)
			events->closed(args);
		else return false;
		break;
	case EVENT_RESIZE:
		if (events->resize != NULL)
			events->resize(args);
		else return false;
		break;
	case EVENT_MOVED:
		if (events->moved != NULL)
			events->moved(args);
		else return false;
		break;
	}
	return true;
}
