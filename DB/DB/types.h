#pragma once

typedef unsigned char byte;
typedef struct Data {
	char *firstName = NULL;
	char *middleName = NULL;
	char *lastName = NULL;

	bool sex;
	byte age;
	byte weight;
	byte group;
} Data;