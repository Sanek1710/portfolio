#define _CRT_SECURE_NO_WARNINGS

#define ERR(err) cout << "ERROR " << GetLastError() << " (" <<  __LINE__ << "): " #err << endl

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

char ServiceName[] = "0 Backup Service";

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatus = 0;

void 
ControlHandler(DWORD request) {
	if (request == SERVICE_CONTROL_SHUTDOWN || request == SERVICE_CONTROL_STOP)
	{
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &ServiceStatus);
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus, &ServiceStatus);
		return;
	}

	SetServiceStatus(hServiceStatus, &ServiceStatus);
	return;
}

inline bool 
operator !=(FILETIME &ft1, FILETIME &ft2)
{
	return !(ft1.dwHighDateTime == ft2.dwHighDateTime 
			 && ft1.dwLowDateTime == ft2.dwLowDateTime);
}

void
BackupService()
{
	FILETIME PrevWriteTime = { 0, 0 }, PrevAccessTime = { 0, 0 };
	FILETIME LastWriteTime, LastAccessTime;

	string ToArchDir;
	string ArchName;
	string FileMask;
	string Command("\"C:\\Program Files\\7-Zip\\7z\" a -tzip -mx=1");

	ifstream ConfigFile("D:\\BackupService\\config.txt");

	if (!ConfigFile.is_open())
	{
		return;
	}

	getline(ConfigFile, ToArchDir);
	getline(ConfigFile, ArchName);

	Command += " " + ArchName;

	while (!ConfigFile.eof())
	{
		getline(ConfigFile, FileMask);
		Command += ' ' + FileMask;
	}
	ConfigFile.close();

	SetCurrentDirectoryA(ToArchDir.data());
	system(Command.data());

	DWORD dwBytesReturned = 0;

	while (TRUE)
	{
		HANDLE hToArchDir = CreateFileA(
			ToArchDir.data(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			NULL);

		if (ReadDirectoryChangesW(hToArchDir,
								  NULL,
								  0,
								  TRUE,
								  FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY |
								  FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | 
								  FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_CREATION,
								  &dwBytesReturned,
								  NULL, NULL)
			)
		{
			system(Command.data());
		}

		CloseHandle(hToArchDir);
		Sleep(5000);
	}
}

void WINAPI 
ServiceMain(DWORD argc, TCHAR* argv[])
{
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	ServiceStatus.dwControlsAccepted = 0;
	ServiceStatus.dwWin32ExitCode = NO_ERROR;
	ServiceStatus.dwServiceSpecificExitCode = NO_ERROR;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hServiceStatus = RegisterServiceCtrlHandlerA(ServiceName, (LPHANDLER_FUNCTION)ControlHandler);
	
	if (hServiceStatus == (SERVICE_STATUS_HANDLE)0) return;

	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	SetServiceStatus(hServiceStatus, &ServiceStatus);
	ServiceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &ServiceStatus);

	BackupService();
}

int 
ServiceInstall() 
{
	SC_HANDLE hSCManager = OpenSCManagerA(0, 0, SC_MANAGER_CREATE_SERVICE);
	if (!hSCManager) 
	{
		ERR(hSCManager);
		return -1;
	}

	char ServiceFileName[_MAX_PATH + 1];
	GetModuleFileNameA(0, ServiceFileName, _MAX_PATH);

	SC_HANDLE hService = CreateServiceA(
		hSCManager, ServiceName, ServiceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		ServiceFileName,
		NULL, NULL, NULL, NULL, NULL
	);

	if (!hService) 
	{
		ERR(hService);
		CloseServiceHandle(hSCManager);
		return -1;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	cout << "Service installed" << endl;
	return 0;
}

int 
ServiceStart()
{
	SERVICE_STATUS_PROCESS ServiceStatusProc;
	DWORD dwBytesNeeded;

	SC_HANDLE hSCManager = OpenSCManagerA(0, 0, SC_MANAGER_CONNECT);
	if (!hSCManager) 
	{
		ERR(OpenSCManagerA);
		return -1;
	}

	SC_HANDLE hService = OpenServiceA(hSCManager, ServiceName, SERVICE_ALL_ACCESS);
	if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ServiceStatusProc, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
	{
		ERR(QueryServiceStatusEx);
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return -1;
	}

	if (ServiceStatusProc.dwCurrentState != SERVICE_STOPPED && ServiceStatusProc.dwCurrentState != SERVICE_STOP_PENDING)
	{
		ERR(Already run);

		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return -1;
	}

	if (!StartServiceA(hService, NULL, NULL))
	{
		ERR(StartServiceA);
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return -1;
	}

	cout << "Service started" << endl;
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 0;
}

int 
ServiceRemove() 
{
	SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) 
	{
		ERR(OpenSCManagerA);
		return -1;
	}

	SC_HANDLE hService = OpenServiceA(hSCManager, ServiceName, SERVICE_STOP | DELETE);
	if (!hService) 
	{
		ERR(OpenServiceA);
		CloseServiceHandle(hSCManager);
		return -1;
	}

	DeleteService(hService);
	cout << "Service removed" << endl;

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 0;
}

int 
ServiceStop()
{
	SC_HANDLE hSCManager = OpenSCManagerA(0, 0, SC_MANAGER_CONNECT);
	
	if (!hSCManager) 
	{
		ERR(OpenSCManagerA);
		printf("Error: Can't open Service Control Manager\n");
		return -1;
	}

	SC_HANDLE hService = OpenServiceA(hSCManager, ServiceName, SERVICE_STOP);
	if (hService)
	{
		SERVICE_STATUS ss;
		if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss))
		{
			ERR(ControlService);
			printf("Error: Stop service\n");
			return -1;
		}
	}
	else
	{
		ERR(OpenServiceA);
		return -1;
	}

	cout << "Service stopped" << endl;

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 0;
}

int 
main(int argc, char* argv[])
{
	if (argc > 1) 
	{
		if (!strcmp((char*)argv[1], "install"))
		{
			ServiceInstall();
		}
		else if (!strcmp((char*)argv[1], "start"))
		{
			ServiceStart();
		}
		else if (!strcmp((char*)argv[1], "stop"))
		{
			ServiceStop();
		}
		else if (!strcmp((char*)argv[1], "remove"))
		{
			ServiceRemove();
		}
	}

	SERVICE_TABLE_ENTRYA ServiceTable[1];
	ServiceTable[0].lpServiceName = (LPSTR)ServiceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;

	StartServiceCtrlDispatcherA(ServiceTable);
	return 0;
}
