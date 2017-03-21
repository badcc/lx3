#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef int8 s8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 r32;
typedef real64 r64;

#define DEBUG_dump(s) (DEBUG_dump_(#s, &s, sizeof(s)))
void DEBUG_dump_(const char *desc, void *addr, int len) {
	printf("%s:\n", desc);
	int j=0;
	uint8 buff[17];
	uint8 *p = (uint8*)addr;
	printf("%p> ", p);
	while (len-->0){
		if (j == 16) {
			j=0;
			printf("%s\n%p> ", buff, p);
		}
		printf("%02x ", *p);
		if (*p<0x20||*p>0x7e)buff[j] = '.';
		else buff[j] = *p;
		buff[++j] = '\0';
		++p;
	}
	while (j++<16) printf("   ");
	printf("%s\n", buff);
}

typedef enum {
	OP_ADD,
	OP_SUB,
} OP_t;

typedef struct {
	u8 op:4;
	u8 a:4;
	u8 b:4;
	u8 c:4;
} VMu;

typedef struct {
	u8 t;
	char _pad1[6];
	union {
		u8 b;
		r64 d;
	} v;
	r64 a;
	r64 b;
	r64 c;
} Value;

int main(int argc, char* argv[]) {
	int reg[16];
	Value r0;
	r0.t = 3;
	r0.v.d = 3.14159;
	// r0.v.b = 'a';
	DEBUG_dump(r0);
	// r0.v.d = 4;
	printf("%lu bytes\n", sizeof(Value));
	reg[0] = 4;
	reg[1] = 2;
	VMu u;
	u.op = OP_ADD;
	u.a = 0;
	u.b = 1;
	u.c = 0;
	switch(u.op){
	case OP_ADD: {reg[u.c]=reg[u.a]+reg[u.b];} break;
	case OP_SUB: {reg[u.c]=reg[u.a]-reg[u.b];} break;
	}
	printf("VMu is %lu bytes\n", sizeof(VMu));
	printf("%d\n", reg[0]);
	return 0;
}