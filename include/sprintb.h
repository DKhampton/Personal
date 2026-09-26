
#ifndef SPRINTB_H_
#define SPRINTB_H_

#include "global.h"

extern char* sprintbx(char* dest, void* source, BYTE bytes, char separator);
extern char* sprintb(char* dest, void* source, BYTE bytes);
extern char* sprintb_dword(char* dest, DWORD dword);
extern char* sprintb_word(char* dest, WORD word);

#endif /* SPRINTB_H_ */
