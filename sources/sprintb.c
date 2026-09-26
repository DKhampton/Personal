
#include "sprintb.h"

char* sprintbx(char* dest, void* source, BYTE bytes, char separator) {

	static char multidanger[38] = { 0 };
	static const char *bit_rep[16] = {
	    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
	    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
	    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
	    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
	};

	char* buffer; char* current;
	buffer = current = dest ? dest : multidanger;

	inline void sprintb_byte(BYTE byte) {
		current += sprintf(current, "%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
	}

	if (!dest && (bytes > 4)) { return NULL; }

	for (int i = 0;;) {
		sprintb_byte(*((BYTE*)(source + bytes - 1 - i)));
		if (++i >= bytes) { break; }
		if (separator) { *current++ = separator; *current = 0; }
	}

	return buffer;
}

char* sprintb(char* dest, void* source, BYTE bytes) {
	return sprintbx(dest,source,bytes,0);
}

char* sprintb_dword(char* dest, DWORD dword) { return sprintb(dest,&dword,4); }
char* sprintb_word(char* dest, WORD word) { return sprintb(dest,&word,2); }
