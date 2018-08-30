
#include "global.h"
#include "files.h"

sMemFile* loadMemFile(char* filename) {
	FILE* aFile;
	if (!(aFile = fopen(filename, "rb"))) GlobalError("File problem");
	NewObject(tmpFile, sMemFile);
	if (!(tmpFile->FileSize = fseek(aFile, 0, SEEK_END))) GlobalError("File empty");
	rewind(aFile);
	AddString(tmpFile->FileData, tmpFile->FileSize);
	if (tmpFile->FileSize != fread(tmpFile->FileData,1, tmpFile->FileSize, aFile)) GlobalError("File read error");
	return tmpFile;
}
