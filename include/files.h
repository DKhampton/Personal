#ifndef FILES_H_
#define FILES_H_

#include "global.h"

#define MAX_PATH_LEN 256

typedef struct {
	int FileSize;
	char* FileData;
} sMemFile;

extern sMemFile* loadMemFile(char* filename);
extern void killMemFile(void* filedata);

#endif /* FILES_H_ */


/*

У місті Лева все дуже яскраво,
Несхожі ні на що його обійми,

Але, моя великоока файна жінко,
Кохання є лиш там, де ти...


*/
