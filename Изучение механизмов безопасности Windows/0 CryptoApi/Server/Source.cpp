#define _CRT_SECURE_NO_WARNINGS  
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  

#define F(a) ;//printf("-->%s\n", #a)
#define P(a) ;//printf("-->%s = %d\n", #a, a)

#include <Windows.h>  
#include <winsock2.h>  
#include <mswsock.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "commands.h"

#include <Wincrypt.h>
#pragma comment(lib, "crypt32.lib")

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "mswsock.lib") 

#define MAX_CLIENTS (100)  
#define PORT	4343

typedef struct client_ctx
{
	
	int socket;
	CHAR buf_recv[512];           // Буфер приема   
	CHAR buf_send[512];           // Буфер отправки   
	int sz_recv;         // Принято данных 
	int sz_send_total;   // Данных в буфере отправки  
	int sz_send;         // Данных отправлено 

	// Структуры OVERLAPPED для уведомлений о завершении
	OVERLAPPED overlap_recv;
	OVERLAPPED overlap_send;
	OVERLAPPED overlap_cancel;

	HCRYPTPROV hProv;
	HCRYPTKEY SessionKey;

	DWORD flags_recv; // Флаги для WSARecv  

}client_ctx;

//cani top
// Прослушивающий сокет и все сокеты подключения хранятся  
// в массиве структур (вместе с overlapped и буферами)  
client_ctx g_ctxs[1 + MAX_CLIENTS];
int g_accepted_socket;
HANDLE g_io_port;

// Функция стартует операцию чтения из сокета  

void
schedule_read(DWORD idx)
{
	WSABUF buf;
	buf.buf = g_ctxs[idx].buf_recv + g_ctxs[idx].sz_recv;
	buf.len = sizeof(g_ctxs[idx].buf_recv) - g_ctxs[idx].sz_recv;
	memset(&g_ctxs[idx].overlap_recv, 0, sizeof(OVERLAPPED));

	g_ctxs[idx].flags_recv = 0;
	WSARecv(g_ctxs[idx].socket, &buf, 1, NULL, &g_ctxs[idx].flags_recv, &g_ctxs[idx].overlap_recv, NULL);
}

char *
pack_send_str(DWORD idx, char *send_str, char *src, DWORD src_len, int *to_send_len)
{
	*((int *)send_str) = src_len;
	DWORD len = 4;
	if (!CryptEncrypt(g_ctxs[idx].SessionKey, 0, TRUE, 0, (BYTE*)send_str, &len, len))
	{
		printf("ErrorEnc1\n");
	}

	memcpy(&send_str[4], src, src_len);
	len = src_len;
	if (!CryptEncrypt(g_ctxs[idx].SessionKey, 0, TRUE, 0, (BYTE*)(&send_str[4]), &len, len))
	{
		printf("ErrorEnc2\n");
	}
	*to_send_len = src_len + 4;

	return NULL;
}

// Функция стартует операцию отправки подготовленных данных в сокет 
void
schedule_write(DWORD idx)
{
	WSABUF buf;

	buf.buf = g_ctxs[idx].buf_send + g_ctxs[idx].sz_send;
	buf.len = g_ctxs[idx].sz_send_total - g_ctxs[idx].sz_send;
	memset(&g_ctxs[idx].overlap_send, 0, sizeof(OVERLAPPED));

	WSASend(g_ctxs[idx].socket, &buf, 1, NULL, 0, &g_ctxs[idx].overlap_send, NULL);
}
// Функция добавляет новое принятое подключение клиента  

void
crypt_init_serv(SOCKET s, HCRYPTPROV *hProvExch, HCRYPTKEY *hSessionKey)
{

	if (!CryptAcquireContext(hProvExch, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0))
	{
		Error("CryptAcquireContext");
		return;
	}
	char PubKeyBuf[150];

	DWORD dwLen = 148;

	//rc(PubKeyBuf, dwLen);
	recv(s, PubKeyBuf, dwLen, 0);

	HCRYPTKEY hKeyPub;
	if (!CryptImportKey(*hProvExch, (BYTE *)PubKeyBuf, dwLen, 0, 0, &hKeyPub))
	{
		Error("ImprotKeyPube");
		return;
	}

	if (!CryptGenKey(*hProvExch, CALG_RC4, CRYPT_EXPORTABLE, hSessionKey)) //генерируем ключ для RC4
	{
		Error("CryptGenKey CALG_RC4");
		return;
	}

	dwLen = 0;
	CryptExportKey(*hSessionKey, 0, OPAQUEKEYBLOB, 0, NULL, &dwLen);

	char SessKeyBuf[128];
	//CRYPT//printf("SessKeyBuf Len = %d\n", dwLen);
	if (!CryptExportKey(*hSessionKey, 0, OPAQUEKEYBLOB, 0, (BYTE *)SessKeyBuf, &dwLen))
	{
		DWORD Er = GetLastError();
		printf("ERROR = %X\n", Er);
		Error("CryptExportKeySes");
		return;
		//. тут что-то делаем
	}

	//CRYPT//printf("SessKeyBuf Lens = %d\n", dwLen);

	if (!CryptEncrypt(hKeyPub, 0, TRUE, 0, (BYTE*)SessKeyBuf, &dwLen, 128))
	{
		DWORD Er = GetLastError();
		printf("ERROR = 0x%X\n", Er);
		printf("%s\n", SessKeyBuf);
		Error("CryptEncryptK");
		return;
	}
	//CRYPT//printf("len = %d\n\n", dwLen);

	//sn(SessKeyBuf, dwLen);
	send(s, SessKeyBuf, dwLen, 0);

	if (!CryptDestroyKey(hKeyPub))
	{
		Error("DestroyKeyExch");
	}
}

void
add_accepted_connection()
{
	DWORD i; // Поиск места в массиве g_ctxs для вставки нового подключения  
	for (i = 0; i < sizeof(g_ctxs) / sizeof(g_ctxs[0]); i++)
	{
		if (g_ctxs[i].socket == 0)
		{
			unsigned int ip = 0;
			struct sockaddr_in* local_addr = 0, *remote_addr = 0;

			int local_addr_sz, remote_addr_sz;
			GetAcceptExSockaddrs(g_ctxs[0].buf_recv, g_ctxs[0].sz_recv, sizeof(struct sockaddr_in) + 16,
				sizeof(struct sockaddr_in) + 16, (struct sockaddr **) &local_addr,
				&local_addr_sz, (struct sockaddr **) &remote_addr, &remote_addr_sz);

			if (remote_addr) ip = ntohl(remote_addr->sin_addr.s_addr);

			printf
			(" connection %u created, remote IP: %u.%u.%u.%u\n",
				i,
				(ip >> 24) & 0xff,
				(ip >> 16) & 0xff,
				(ip >> 8) & 0xff,
				(ip) & 0xff
			);
			g_ctxs[i].socket = g_accepted_socket;
			// Связь сокета с портом IOCP, в качестве key используется индекс массива  
			if (NULL == CreateIoCompletionPort((HANDLE)g_ctxs[i].socket, g_io_port, i, 0))
			{
				printf("CreateIoCompletionPort error: %x\n", GetLastError());
				return;
			}
			crypt_init_serv(g_ctxs[i].socket, &g_ctxs[i].hProv, &g_ctxs[i].SessionKey);
			// Ожидание данных от сокета  
			schedule_read(i);
			return;
		}
	}
	// Место не найдено => нет ресурсов для принятия соединения  
	closesocket(g_accepted_socket);
	g_accepted_socket = 0;
}
// Функция стартует операцию приема соединения  

void
schedule_accept()
{
	// Создание сокета для принятия подключения (AcceptEx не создает сокетов)  
	g_accepted_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&g_ctxs[0].overlap_recv, 0, sizeof(OVERLAPPED));
	// Принятие подключения. 
	// Как только операция будет завершена - порт завершения пришлет уведомление. // Размеры буферов должны \
	быть на 16 байт больше размера адреса согласно документации разработчика ОС
	AcceptEx(g_ctxs[0].socket, g_accepted_socket, g_ctxs[0].buf_recv, 0, sizeof(struct sockaddr_in) + 16,
		sizeof(struct sockaddr_in) + 16, NULL, &g_ctxs[0].overlap_recv);
}

int
command_check(DWORD idx)
{
	return (g_ctxs[idx].sz_recv);
}

char *
command_exec(DWORD idx, int *len)//0x739BA943
{
	DWORD rlen = 1;
	char path[256];
	if (!CryptDecrypt(g_ctxs[idx].SessionKey, 0, TRUE, 0, (BYTE*)g_ctxs[idx].buf_recv, (DWORD*)&rlen))
	{
		int ERR = GetLastError();
		printf("ERROR: 0x%X\n", ERR);
		printf("ErrorDec1\n");
	}

	int cmd_num = g_ctxs[idx].buf_recv[0];
	//CRYPT//printf("in command_exec %d\n", cmd_num);

	switch (cmd_num)
	{
	case INFO_OS_VERS	:	return Get_OS_vers		(len);
	case INFO_TIME		:	return Get_time			(len);
	case INFO_OS_TIME	:	return Get_OS_time		(len);
	case INFO_MEM_STAT	:	return Get_mem_stat		(len);
	case INFO_DISK_MEM	:	return Get_disk_mem_info(len);
	case INFO_SECURE	:	
	{
		rlen = 1;
		if (!CryptDecrypt(g_ctxs[idx].SessionKey, 0, TRUE, 0, (BYTE*)(&g_ctxs[idx].buf_recv[1]), (DWORD*)&rlen))
		{
			printf("ErrorDec2\n");
			return NULL;
		}
		rlen = g_ctxs[idx].buf_recv[1];
		if (!CryptDecrypt(g_ctxs[idx].SessionKey, 0, TRUE, 0, (BYTE*)(&g_ctxs[idx].buf_recv[2]), (DWORD*)&rlen))
		{
			printf("ErrorDec3\n");
			return NULL;
		}
		if (rlen > 255) g_ctxs[idx].buf_recv[2 + 255] = '\0';
		strcpy(path, &g_ctxs[idx].buf_recv[2]);
		//printf("path = %s\n", path);
		return Get_secure_info	(path, len);
	}
	default:
		char *error = (char*)malloc(sizeof("No command"));
		strcpy(error, "No command");
		*len = 11;
		return error;
	}

	//char *cmd = &(g_ctxs[idx].buf_recv[6]);
}


void
io_serv()
{

	WSADATA wsa_data;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0)
	{
		printf("WSAStartup ok\n");
	}
	else
	{
		printf("WSAStartup error\n");
	}

	struct sockaddr_in addr;
	// Создание сокета прослушивания
	SOCKET s = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	// Создание порта завершения  

	g_io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == g_io_port)
	{
		printf("CreateIoCompletionPort error: %x\n", GetLastError());
		return;
	}


	// Обнуление структуры данных для хранения входящих соединений  
	memset(g_ctxs, 0, sizeof(g_ctxs));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{
		printf("error bind()\n");
		return;
	}
	//backlog
	if (listen(s, 1) < 0)
	{
		printf("error listen()\n");
		return;
	}

	printf("Listening: %hu\n", ntohs(addr.sin_port));
	// Присоединение существующего сокета s к порту io_port.  
	// В качестве ключа для прослушивающего сокета используется 0  
	if (NULL == CreateIoCompletionPort((HANDLE)s, g_io_port, 0, 0))
	{
		printf("CreateIoCompletionPort error: %x\n", GetLastError());
		return;
	}
	g_ctxs[0].socket = s;
	// Старт операции принятия подключения. 
	schedule_accept();
	// Бесконечный цикл принятия событий о завершенных операциях
	while (1)
	{
		DWORD transferred;
		ULONG_PTR sock_index;
		OVERLAPPED* lp_overlap;
		// Ожидание событий в течение 1 секунды  
		F(0);
		if (GetQueuedCompletionStatus(g_io_port, &transferred, &sock_index, &lp_overlap, 1000))
		{
			// Поступило уведомление о завершении операции  
			if (sock_index == 0) // ключ 0 - для прослушивающего сокета 
			{
				F(12);
				P(transferred);
				g_ctxs[0].sz_recv += transferred;
				// Принятие подключения и начало принятия следующего 
				add_accepted_connection();
				schedule_accept();
			}
			else
			{
				F(1);
				// Иначе поступило событие по завершению операции от клиента. // Ключ key - индекс в \
				массиве g_ctxs
				if (&g_ctxs[sock_index].overlap_recv == lp_overlap)
				{
					int len;
					// Данные приняты: 
					if (transferred == 0)
					{
						F(2);
						// Соединение разорвано  
						CancelIo((HANDLE)g_ctxs[sock_index].socket);
						PostQueuedCompletionStatus(g_io_port, 0, sock_index,
							&g_ctxs[sock_index].overlap_cancel);
						continue;
					}

					g_ctxs[sock_index].sz_recv += transferred;

					if (command_check(sock_index))
					{
						// Если строка полностью пришла, то сформировать ответ и начать его \
						отправлять

						char *data;
						data = command_exec(sock_index, &len);
						//CRYPT//printf("size to send: %d\n", len);
						pack_send_str(sock_index, g_ctxs[sock_index].buf_send, data, len, &g_ctxs[sock_index].sz_send_total);
						//parse_OS_time(&g_ctxs[sock_index].buf_send[4], len);
						g_ctxs[sock_index].sz_recv = 0;
						free(data);
						//CRYPT//printf("command_executing...\n");
						
						schedule_write(sock_index);
						schedule_read(sock_index);
					}
					else
					{
						// Иначе - ждем данные дальше  
						schedule_read(sock_index);
					}

				}
				else if (&g_ctxs[sock_index].overlap_send == lp_overlap)
				{
					// Данные отправлены  
					g_ctxs[sock_index].sz_send += transferred;
					if (g_ctxs[sock_index].sz_send < g_ctxs[sock_index].sz_send_total && transferred > 0)
					{
						// Если данные отправлены не полностью - продолжить отправлять  
						schedule_write(sock_index);
					}
					else
					{
						g_ctxs[sock_index].sz_send = g_ctxs[sock_index].sz_send_total = 0;
						continue;
						// Данные отправлены полностью, прервать все коммуникации,  
						// добавить в порт событие на завершение работы  

						CancelIo((HANDLE)g_ctxs[sock_index].socket);
						PostQueuedCompletionStatus(g_io_port, 0, sock_index,
							&g_ctxs[sock_index].overlap_cancel);
					}

				}
				else if (&g_ctxs[sock_index].overlap_cancel == lp_overlap)
				{
					F(7);
					// Все коммуникации завершены, сокет может быть закрыт  
					closesocket(g_ctxs[sock_index].socket);
					memset(&g_ctxs[sock_index], 0, sizeof(g_ctxs[sock_index]));
					printf(" connection %u closed\n", sock_index);
					CryptDestroyKey(g_ctxs[sock_index].SessionKey);
					CryptReleaseContext(g_ctxs[sock_index].hProv, 0);
				}
			}
		}
		else
		{
			// Ни одной операции не было завершено в течение заданного времени, программа может  
			// выполнить какие-либо другие действия 
			// ...  
		}
	}
}

int
main()
{
	//char *buf = (char*)malloc(10000000);

	io_serv();
	return 0;
}
