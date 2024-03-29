#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <fstream>
#include "FileServManager.h"
#pragma comment (lib,"Rpcrt4.lib")
// Server function.
using namespace std;


int	//return 1 - success, 0 - else
delete_file(const unsigned char *filename)
{
	fstream fdel((char*)filename, fstream::in | fstream::binary);

	if (fdel.is_open())
	{
		fdel.close();
		remove((char*)filename);
		return 1;
	}
	return 0;
}

int //return 1 - success, 0 - else
upload_file(const unsigned char *filename, const unsigned char filebuf[1025], int write_size)
{
	FILE *fout = fopen((char*)filename, "ab");
	//fstream fout((char*)filename, fstream:: | fstream::binary);

	if (fout)
	{
		fwrite((char*)filebuf, 1, write_size, fout);
		fclose(fout);
		return 1;
	}
	return 0;
}



long long //return size of file
size_of_file(const unsigned char *filename)
{
	fstream finfo((char*)filename, fstream::in | fstream::binary);

	if (finfo.is_open())
	{
		finfo.seekp(0, finfo.end);
		long long size = finfo.tellp();
		finfo.seekp(0, finfo.beg);

		finfo.close();
		return size;
	}
	return -1;
}


int //return 1 - success, 0 - else
save_file(const unsigned char *filename, unsigned char filebuf[1025], int offset_1024, int read_size)
{
	/*FILE *fin = fopen((char*)filename, "rb");
	if (fin)
	{
		fseek(fin, ((long long)offset_1024) << 10, SEEK_SET);
		
		fread((char*)filebuf, 1, read_size + 1, fin);
		fclose(fin);
		return 1;
	}*/
	
	fstream fin((char*)filename, fstream::in | fstream::binary);

	if (fin.is_open())
	{
		fin.seekp(((long long)offset_1024) << 10, fin.beg);
		fin.read((char*)filebuf, read_size + 1);
		fin.close();
		return 1;
	}
	return 0;
}

int //return error_code
login(const unsigned char *username, const unsigned char *password) 
{
	HANDLE phToken = NULL;
	cout << username << ' ' << password << endl;
	if (!LogonUserA((LPCSTR)username, NULL, (LPCSTR)password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &phToken)
		||
		!ImpersonateLoggedOnUser(phToken))
	{
		return GetLastError();
	}

	cout << username << " has just connected\n";
	return 0;
}


// Naive security callback.
RPC_STATUS CALLBACK 
SecurityCallback(RPC_IF_HANDLE hInterface, void* pBindingHandle)
{
	return RPC_S_OK; // Always allow anyone.
}

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
main()
{
	RPC_STATUS status;
	RPC_CSTR Proto = (RPC_CSTR)"ncacn_ip_tcp";
	RPC_CSTR Port = (RPC_CSTR)"4343";

	status = RpcServerUseProtseqEpA(Proto, RPC_C_PROTSEQ_MAX_REQS_DEFAULT, Port, NULL); // No security.

	exit_if_error(status);

	status = RpcServerRegisterIf2(FileServManager_v1_0_s_ifspec, NULL, NULL, 
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 
		(unsigned)-1, SecurityCallback);
	
	exit_if_error(status);

	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE); //call RpcMgmtStopServerListening tp stop

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