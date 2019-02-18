#if !defined(_GLIB_SYS_GRAPHICS_) && defined(_GLIB)
#define _GLIB_SYS_GRAPHICS_

typedef struct Point__
{
	int32 x, y;
} Point;

typedef struct ARGB__
{
	byte a, r, g, b;
} ARGB;

struct Line {
	Point point1, point2;
};

struct Rect {
	Point point1, point2;
};

struct Ellipse {
	Point center;
	u_int radius_x, radius_y;
};

typedef struct Image__
{
	byte * image;
	int32 width;
	int32 height;

	void Release();

	ARGB * GetPixel(Point pos);
	void DrawPoint(Point point, ARGB color);
	
	//start, end - point on target image, sub - left-up point on source image
	void DrawImage(struct Image__ * image, Point start, Point end, Point sub);

	void DrawLine(Line line, ARGB color, long wide = 1L);
	
	void DrawRect(Rect rect, ARGB color, long wide = 1L);
	void DrawEllipse(struct Ellipse ellipse, ARGB color, long wide = 1L);

	void FillRect(Rect rect, ARGB color);
	void FillEllipse(struct Ellipse ellipse, ARGB color);
} * Image;

EXTERN Image glibCreateImage(u_int width, u_int height);
EXTERN Image glibCreateImage(u_int width, u_int height, byte * ptr);
EXTERN Image glibCreateImage(Bitmap bitmap);

EXTERN void glibFillImage(Image im, ARGB color);

EXTERN ARGB glibMerge(ARGB clr, ARGB add);

#endif
