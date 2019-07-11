#pragma once

#include <Windows.h>
#include <aclapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <VersionHelpers.h>

#define INFO_OS_VERS	1
#define INFO_TIME		2
#define INFO_OS_TIME	3
#define INFO_MEM_STAT	4
#define INFO_DISK_MEM	5	
#define INFO_SECURE		6

namespace typetotype
{
	union MTYPE8
	{
		unsigned char uc;
		char c;
	};

	union MTYPE16
	{
		unsigned short us;
		short s;

		union MTYPE8 T8[2];

		unsigned char uc[2];
		char c[2];
	};

	union MTYPE32
	{
		unsigned long ul;
		long l;
		unsigned int ui;
		int i;
		float f;

		union MTYPE16 T16[2];

		unsigned short us[2];
		short s[2];

		union MTYPE8 T8[4];

		unsigned char uc[4];
		char c[4];
	};

	union MTYPE64
	{
		unsigned long long ull;
		long long ll;
		double d;

		union MTYPE32 T32[2];

		unsigned long ul[2];
		long l[2];
		unsigned int ui[2];
		int i[2];
		float f[2];

		union MTYPE16 T16[4];

		unsigned short us[4];
		short s[4];

		union MTYPE8 T8[8];

		unsigned char uc[8];
		char c[8];
	};
}

char *Get_OS_vers		(int *len);
char *Get_time			(int *len);
char *Get_OS_time		(int *len);
char *Get_mem_stat		(int *len);
char *Get_disk_mem_info	(int *len);
char *Get_secure_info	(char *path, int *len);

void parse_OS_vers		(char *str, int len);
void parse_time			(char *str, int len);
void parse_OS_time		(char *str, int len);
void parse_mem_stat		(char *str, int len);
void parse_disk_mem_info(char *str, int len);
void parse_secure_info	(char *str, int len);

void Error(const char *error);