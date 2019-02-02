#include "glib_src.h"

void glibSetEvent(EventHandles * events, EventHandle handle, u_int type)
{
	if (type & EVENT_BASIC)
		events->basic = handle;
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
	if (type & EVENT_DRAW)
		events->draw = handle;
	if (type & EVENT_MOUSEDOWN)
		events->mousedown = handle;
	if (type & EVENT_MOUSEUP)
		events->mouseup = handle;
	if (type & EVENT_DOUBLECLICK)
		events->doubleclick = handle;
}

bool glibRunEvent(EventHandles * events, EventArgs * args)
{
	switch (args->msg)
	{
	case EVENT_BASIC:
		if (events->basic != NULL)
			events->basic(args);
		else return false;
		break;
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
	case EVENT_DRAW:
		if (events->draw != NULL)
			events->draw(args);
		else return false;
		break;
	case EVENT_MOUSEDOWN:
		if (events->mousedown != NULL)
			events->mousedown(args);
		else return false;
		break;
	case EVENT_MOUSEUP:
		if (events->mouseup != NULL)
			events->mouseup(args);
		else return false;
		break;
	case EVENT_DOUBLECLICK:
		if (events->doubleclick != NULL)
			events->doubleclick(args);
		else return false;
		break;
	}
	return true;
}

bool glibCheckEvent(u_int type, EventHandles * events)
{
	switch (type)
	{
	case EVENT_BASIC:
		return events->basic != NULL;
	case EVENT_SHOWN:
		return events->shown != NULL;
	case EVENT_CLOSING:
		return events->closing != NULL;
	case EVENT_CLOSED:
		return events->closed != NULL;
	case EVENT_RESIZE:
		return events->resize != NULL;
	case EVENT_MOVED:
		return events->moved != NULL;
	case EVENT_DRAW:
		return events->draw != NULL;
	case EVENT_MOUSEDOWN:
		return events->mousedown != NULL;
	case EVENT_MOUSEUP:
		return events->mouseup != NULL;
	case EVENT_DOUBLECLICK:
		return events->doubleclick != NULL;
	default:
		return false;
	}
}