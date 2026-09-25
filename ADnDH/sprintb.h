
#ifndef SPRINTB_H_
#define SPRINTB_H_

#include "global.h"

char* sprintbx(char* dest, void* source, BYTE bytes, char separator);
char* sprintb(char* dest, void* source, BYTE bytes);
char* sprintb_dword(char* dest, DWORD dword);
char* sprintb_word(char* dest, WORD word);

#endif /* SPRINTB_H_ */
