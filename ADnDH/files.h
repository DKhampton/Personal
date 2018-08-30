#ifndef FILES_H_
#define FILES_H_

#define MAX_PATH_LEN 256

typedef struct {
	int FileSize;
	char* FileData;
} sMemFile;

sMemFile* loadMemFile(char* filename);

#endif /* FILES_H_ */


/*

У місті Лева все дуже яскраво,
Несхожі ні на що його обійми,

Але, моя великоока файна жінко,
Кохання є лиш там, де ти...


*/
