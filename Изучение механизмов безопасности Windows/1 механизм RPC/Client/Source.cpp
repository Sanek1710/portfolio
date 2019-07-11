#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include "FileServManager.h"
#pragma comment (lib,"Rpcrt4.lib")

#define FSM_UPLOAD	1
#define FSM_SAVE	2
#define FSM_DELETE	3
#define FSM_EXIT	4
#define FSM_HELP	5

using namespace std;

int	//return 1 - success, 0 - else
ñ_delete_file(const char *filename)
{
	return delete_file((unsigned char *)filename);
}

int //return 1 - success, 0 - else
ñ_upload_file(const char *fname_src, const char *fname_dst)
{
	fstream fin(fname_src, fstream::in | fstream::binary);

	if (fin.is_open())
	{
		fin.seekp(0, fin.end);
		long long size = fin.tellp();
		fin.seekp(0, fin.beg);

		unsigned char buf[1025];
		long long offset = 0;
		int read_size, res = 1;

		while (offset < size)
		{
			read_size = ((size - offset) < 1024) ? (size - offset) : 1024;
			fin.read((char *)buf, read_size);
			res &= upload_file((unsigned char *)fname_dst, buf, read_size);
			if (!res) break;
			offset += read_size;
		}
		fin.close();

		return res;
	}
	return 0;
}

int //return 1 - success, 0 - else
ñ_save_file(const char *fname_src, const char *fname_dst)
{
	fstream fout((char *)fname_dst, fstream::out | fstream::binary);

	if (fout.is_open())
	{
		long long size = size_of_file((unsigned char *)fname_src);
		if (size == -1)
		{
			fout.close();
			remove((char *)fname_dst);
			return 0;
		}

		unsigned char buf[1025];
		long long offset = 0;
		int read_size, res = 1;

		while (offset < size)
		{
			read_size = ((size - offset) < 1024) ? (size - offset) : 1024;
			res &= save_file((unsigned char *)fname_src, buf, offset >> 10, read_size - 1);
			if (res) fout.write((char *)buf, read_size);
			else break;
			offset += read_size;
		}
		fout.close();

		if (!res) remove((char *)fname_dst);

		return res;
	}
	return 0;
}


int //return error_code
login(const unsigned char *username, const unsigned char *password);

void
exit_if_error(RPC_STATUS status)
{
	if (status)
	{
		printf("ERROR\n");
		exit(status);
	}
}


int
read_filename(char *buf, char *fname)
{
	int i = 0;

	while (buf[i] == ' ') buf[i++];
	if (buf[i] == '"')
	{
		i++;

		while (buf[i] != '"' && buf[i] != '\0')
		{
			*(fname++) = buf[i++];
		}
		*fname = '\0';

		if (buf[i] != '"') return -1;
		i++;
	}
	else
	{
		while (buf[i] != ' ' && buf[i] != '\0')
		{
			*(fname++) = buf[i++];
		}
		*fname = '\0';
	}

	return i;
}

int
read_command(char *fname1, char *fname2)
{
	char buf[1000];
	int c, len = 0;
	int offs = 0;
	int i, j;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
	printf("Enter command> ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);

	while ((c = getchar()) == ' ');

	while (c != '\n')
	{
		buf[len] = c;
		len++;
		c = getchar();
	}
	buf[len] = '\0';

	if (strlen(buf) == 0) return FSM_HELP;
	if (!strncmp(buf, "upload", 6))
	{
		i = 6;

		offs = read_filename(&buf[i], fname1);
		i += offs;
		if (offs < 0) return -1;

		offs = read_filename(&buf[i], fname2);
		i += offs;
		if (offs < 0) return -1;

		return FSM_UPLOAD;
	}
	if (!strncmp(buf, "save", 4))
	{
		i = 4;

		offs = read_filename(&buf[i], fname1);
		i += offs;
		if (offs < 0) return -1;

		offs = read_filename(&buf[i], fname2);
		i += offs;
		if (offs < 0) return -1;

		return FSM_SAVE;
	}
	if (!strncmp(buf, "delete", 6))
	{
		i = 6;

		offs = read_filename(&buf[i], fname1);
		i += offs;
		if (offs < 0) return -1;

		return FSM_DELETE;
	}

	if (!strncmp(buf, "help", len)) return FSM_HELP;
	if (!strncmp(buf, "exit", len)) return FSM_EXIT;

	return -1;
}

void
help()
{
	printf("\tupload [fname1] [fname2]  - client[fname1] -> server[fname2]\n");
	printf("\tsave   [fname1] [fname2]  - server[fname1] -> client[fname2]\n");
	printf("\tdelete [fname1]           - server[fname1] -> X(delete)\n");
	printf("\texit                      - close console\n");
	printf("\thelp                      - help\n");
}

int
read_ip_addr(char *ADDR_buf)
{
	char c[7];

	while (1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
		printf("Enter IP address> ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);

		if (scanf("%d.%d.%d.%d", c + 0, c + 1, c + 2, c + 3) != 4 || getchar() != '\n')
		{
			while (getchar() != '\n');
			printf("incorrect format\n");
			continue;
		}

		break;
	}
	sprintf(ADDR_buf, "%u.%u.%u.%u", (unsigned char)c[0], (unsigned char)c[1], (unsigned char)c[2], (unsigned char)c[3]);

	return *((int *)&c);
}

int
main()
{
	hFileServManager;
	RPC_STATUS status = 0;
	RPC_CSTR szStringBinding = NULL;
	RPC_CSTR Proto = (RPC_CSTR)"ncacn_ip_tcp";
	char	 Ip_addr[64];
	RPC_CSTR Port = (RPC_CSTR)"4343";

	read_ip_addr((char*)Ip_addr);
	cout << Ip_addr << endl;

	status |= RpcStringBindingComposeA(NULL, Proto, (RPC_CSTR)Ip_addr, Port, NULL, &szStringBinding);
	status |= RpcBindingFromStringBindingA(szStringBinding, &hFileServManager);

	exit_if_error(status);
	char path1[1024], path2[1024];
	unsigned char login_buf[1024], password_buf[1024];
	int c, i;

	while (1)
		RpcTryExcept
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
		printf("Enter login> ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);

		i = 0;
		while ((c = getchar()) != '\n') login_buf[i++] = c;
		login_buf[i] = '\0';

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
		printf("Enter password> ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);

		i = 0;
		while ((c = getchar()) != '\n') password_buf[i++] = c;
		password_buf[i] = '\0';

		if (login(login_buf, password_buf)) cout << "wrong username or password\n";
		else
		{
			cout << "login success\n";
			break;
		}
	}
		RpcExcept(1)
	{
		cout << "connection error\n";
	}
	RpcEndExcept

		while (1)
		{
			int cmd = read_command(path1, path2);

			if (cmd == -1)
			{
				cout << "incorrect command\n";
				continue;
			}
			else if (cmd == FSM_EXIT) break;
			else if (cmd == FSM_HELP) help();
			else
			{
				int success = 1;

				RpcTryExcept
				{
					if (cmd == FSM_UPLOAD)		success = ñ_upload_file(path1, path2);
					else if (cmd == FSM_SAVE)	success = ñ_save_file(path1, path2);
					else if (cmd == FSM_DELETE)	success = ñ_delete_file(path1);
				}
					RpcExcept(1)
				{
					if (cmd == FSM_UPLOAD)		printf("ERROR : upload 0x%X\n", RpcExceptionCode());
					else if (cmd == FSM_SAVE)	printf("ERROR : save   0x%X\n", RpcExceptionCode());
					else if (cmd == FSM_DELETE)	printf("ERROR : delete 0x%X\n", RpcExceptionCode());
				}
				RpcEndExcept

					if (!success) cout << "ERROR : filename\n";
			}
		}

	status |= RpcStringFreeA(&szStringBinding);
	status |= RpcBindingFree(&hFileServManager);
	exit_if_error(status);
}

void * __RPC_USER
midl_user_allocate(size_t _Size)
{
	return malloc(_Size);
}

void __RPC_USER
midl_user_free(void* _Block)
{
	free(_Block);
}