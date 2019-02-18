#if !defined(_GLIB_SYS_TEXT) && defined(_GLIB)
#define _GLIB_SYS_TEXT

#define EX_DrawText _FUNCTION_HANDLE(DrawText)

#define LANGUAGE_DEFAULT	0x0000	//default ascii symbols
#define LANGUAGE_ENGLISH	0x0001
#define LANGUAGE_RUSSIAN	0x0002

EXTERN void glibSetLanguage(u_int32 lang);

#define glibSetDefaultLanguage() glibSetLanguage(LANGUAGE_DEFAULT)

EXTERN void glibDrawText(Image im, int32 x, int32 y, char * str, u_int32 format);

typedef wchar_t w_char;
EXTERN w_char glibConvertCharacter(char c);
EXTERN void glibConvertString(char * c, w_char * buf);

EXTERN u_int32 glib_language;

#endif
