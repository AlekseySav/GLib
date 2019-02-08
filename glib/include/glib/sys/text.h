#pragma once

#define EX_DrawText _FUNCTION_HANDLE(DrawText)

#define LANGUAGE_DEFAULT	0x0000	//default ascii lang
#define LANGUAGE_ENGLISH	0x0001
#define LANGUAGE_RUSSIAN	0x0002

EXTERN void glibSetLanguage(u_int32 lang);

#define glibSetDefaultLanguage() glibSetLanguage(LANGUAGE_ENGLISH)

EXTERN void glibDrawText(Image im, int32 x, int32 y, char * str, u_int32 format);

typedef wchar_t w_char;
EXTERN w_char glibConvertCharacter(char c);
EXTERN void glibConvertString(char * c, w_char * buf);
