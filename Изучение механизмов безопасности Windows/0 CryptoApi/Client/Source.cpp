#define _CRT_SECURE_NO_WARNINGS  
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  



#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "commands.h"

#include <Wincrypt.h>
#pragma comment(lib, "crypt32.lib")

#pragma comment (lib,"ws2_32.lib") 
#define PORT 4343
#define SERVERADDR "127.0.0.1"

int
command_send(SOCKET sock, int command, HCRYPTKEY hSessionKey, char *path_INFO_SECURE)
{
	char cmd_num = command;
	DWORD len = 1;
	if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, (BYTE*)(&cmd_num), &len, len))
	{
		printf("ErrorEnc1\n");
	}

	if (command == INFO_SECURE)
	{
		char send_str[260];
		send_str[0] = cmd_num;
		DWORD str_len = strlen(path_INFO_SECURE) + 1;

		send_str[1] = str_len;
		len = 1;
		if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, (BYTE*)(&send_str[1]), &len, len))
		{
			printf("ErrorEnc1\n");
		}
		strcpy(&send_str[2], path_INFO_SECURE);

		len = str_len;
		if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, (BYTE*)(&send_str[2]), &len, len))
		{
			printf("ErrorEnc1\n");
		}

		return send(sock, send_str, str_len + 3, 0);
	}

	return send(sock, (char*)(&cmd_num), 1, 0);
}

char *
recv_data(SOCKET sock, char *rbuf, int *len, HCRYPTKEY hSessionKey)
{
	recv(sock, rbuf, 4, 0);
	DWORD rlen = 4;
	if (!CryptDecrypt(hSessionKey, 0, TRUE, 0, (BYTE*)(rbuf), &rlen))
	{
		printf("Err = 0x%X\n", GetLastError());
		printf("ErrorEnc1\n");
	}
	*len = *((int*)rbuf);
	recv(sock, rbuf, *len, 0);
	rlen = *len;
	if (!CryptDecrypt(hSessionKey, 0, TRUE, 0, (BYTE*)(rbuf), &rlen))
	{
		printf("ErrorEnc2\n");
	}
	return rbuf;
}

int
read_command(char *path)
{
	char buf[1000];
	int c, len = 0;

	while ((c = getchar()) == ' ');

	while (c != '\n')
	{
		buf[len] = c;
		len++;
		c = getchar();
	}
	buf[len] = '\0';

	if (strlen(buf) == 0) return INFO_HELP;
	if (!strncmp(buf, "back"			, len)) return INFO_BACK	;
	if (!strncmp(buf, "os version"		, len)) return INFO_OS_VERS	;	
	if (!strncmp(buf, "time"			, len)) return INFO_TIME	;
	if (!strncmp(buf, "os time"			, len)) return INFO_OS_TIME	;	
	if (!strncmp(buf, "memory statistic", len)) return INFO_MEM_STAT;	
	if (!strncmp(buf, "disk info"		, len)) return INFO_DISK_MEM;
	if (!strncmp(buf, "secure "			, 7	 )) 
	{ 
		int i = 6;
		while (buf[i] == ' ') i++;
		strncpy(path, &buf[i], 255);
		return INFO_SECURE;	
	}
	if (!strncmp(buf, "help"			, len)) return INFO_HELP	;
	if (!strncmp(buf, "exit"			, len)) return INFO_EXIT	;

	return -1;
}

void 
help()
{
	printf("\tOS version        - get version of OS\n");
	printf("\ttime              - get current real time\n");
	printf("\tos time           - get current OS runtime\n");
	printf("\tmemory statistic  - get statistic of memory\n");
	printf("\tdisk info         - get information about disks\n");
	printf("\tsecure [filename] - get secure rule of file\n");
	printf("\tback              - exit from PC\n");
	printf("\texit              - close console\n");
	printf("\thelp              - help\n");
}

void
crypt_init_cli(SOCKET s, HCRYPTPROV *hProvExch, HCRYPTKEY *hSessionKey)
{
	HCRYPTKEY hKey;
	if (!CryptAcquireContext(hProvExch, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		Error("CryptAcquireContext");
		return;
	}

	if (!CryptGenKey(*hProvExch, AT_KEYEXCHANGE, CRYPT_ARCHIVABLE, &hKey))
	{
		Error("CryptGenKey");
		return;
	}

	DWORD dwLen;
	char PubKeyBuf[150];

	if (!CryptExportKey(hKey, NULL, PUBLICKEYBLOB, 0, (BYTE *)PubKeyBuf, &dwLen))
	{
		Error("CryptExportKey");
		return;
	}
	//CRYPT//printf("dwLen, %d\n", dwLen);

	//sn(PubKeyBuf, dwLen);
	send(s, PubKeyBuf, dwLen, 0);
	///////////////////////////////////////////////////////////////////SERVER PART
	//		HCRYPTPROV hProvExchSER;
	//		HCRYPTKEY hSessionKeySER;
	//		crypt_init_serv(&hProvExchSER, &SERW_KEY);
	//SERVER PART/////////////////////////////////////////////////////////////////

	//rc(PubKeyBuf, 128);
	recv(s, PubKeyBuf, 128, 0);

	if (!CryptDecrypt(hKey, 0, TRUE, 0, (BYTE*)PubKeyBuf, &dwLen))
	{
		DWORD Er = GetLastError();
		printf("ERROR = %d\n", Er);
		printf("%s\n", PubKeyBuf);
		Error("CryptEncryptD");
		return;
	}

	//CRYPT//printf("len = %d\n\n", dwLen);

	if (!CryptImportKey(*hProvExch, (BYTE *)PubKeyBuf, dwLen, 0, 0, hSessionKey))
	{
		Error("ImprotKeyPrivate");
	}

	if (!CryptDestroyKey(hKey))
	{
		Error("DestroyKeyExch");
	}
}

int 
read_ip_addr()
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
	return *((int *)&c);
}


int 
main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	char buff[1024];
	printf("CLIENT\n");

	// Шаг 1 - инициализация библиотеки Winsock
	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}


	HCRYPTPROV hProv;
	HCRYPTKEY hSessionKey;

	// Шаг 4 - чтение и передача сообщений
	int nsize;

	int len;
	char rbuf[1000] = { 0 };
	int cmd;
	SOCKET my_sock;
	char path[256];

	while (1)
	{
		int ADDRESS = read_ip_addr();
		printf("SERVER IP: %d.%d.%d.%d\n", ((UCHAR*)&ADDRESS)[0], ((UCHAR*)&ADDRESS)[1], ((UCHAR*)&ADDRESS)[2], ((UCHAR*)&ADDRESS)[3]);
		my_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (my_sock <= 0)
		{
			printf("Socket() error %d\n", WSAGetLastError());
			return -1;
		}

		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(PORT);
		dest_addr.sin_addr.s_addr = ADDRESS;

		int times = 10;
		printf("trying to connect...\n");
		while (times && connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
		{
			times--;
			printf("\tConnecting error %d/10\n", 10 - times);
			//return -1;
		}
		if (!times)
		{
			closesocket(my_sock);
			continue;
		}
		printf("\tConnecting success\n");

		crypt_init_cli(my_sock, &hProv, &hSessionKey);
		while (1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
			printf("Enter command> ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);
			cmd = read_command(path);
			if (cmd == -1)
			{
				printf("incorrect command\n");
				continue;
			}
			else if (cmd == INFO_HELP)
			{
				help();
				continue;
			}
			else if (cmd == INFO_BACK)
			{
				CryptDestroyKey(hSessionKey);
				closesocket(my_sock);
				break;
			}
			else if (cmd == INFO_EXIT) goto END_PROG;
			else if (cmd == INFO_SECURE)
			{
				printf("%s : ", path);
			}

			command_send(my_sock, cmd, hSessionKey, path);
			recv_data(my_sock, rbuf, &len, hSessionKey);
			parse_command(rbuf, len, cmd);
		}
	}
END_PROG:
	CryptDestroyKey(hSessionKey);
	CryptReleaseContext(hProv, 0);
	closesocket(my_sock);
	WSACleanup();
	return 0;
}