#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

using namespace std;

#define TASK_DEFENDER		(DWORD)(1)
#define TASK_PING			(DWORD)(2)

#define EXE_PATH			(L"C:\\Users\\user\\Desktop\\new\\1.exe")

struct TASK
{
	BSTR taskName;
	TASK_STATE taskState;

	const char* state_to_str()
	{
		if (taskState == TASK_STATE_UNKNOWN)
		{
			return "UNKNOWN";
		}
		else if (taskState == TASK_STATE_DISABLED)
		{
			return "DISABLED";
		}
		else if (taskState == TASK_STATE_QUEUED)
		{
			return "QUEUED";
		}
		else if (taskState == TASK_STATE_READY)
		{
			return "READY";
		}
		else if (taskState == TASK_STATE_RUNNING)
		{
			return "RUNNING";
		}
		else return "";
	}
};

struct TaskList
{
	TASK *tasks;
	DWORD count;
	DWORD max_count;
	
	void init() 
	{
		count = 0;
		max_count = 32;
		tasks = (TASK*)malloc(max_count * sizeof(TASK));
	}

	void add(BSTR taskName, TASK_STATE taskState)
	{
		tasks[count].taskName = taskName;
		tasks[count].taskState = taskState;
		count++;

		if (count == max_count)
		{
			max_count += 32;
			tasks = (TASK*)realloc(tasks, max_count * sizeof(TASK));
		}
	}

	void clear()
	{
		for (int i = 0; i < count; i++)
		{
			SysFreeString(tasks[i].taskName);
		}
	}

	void free()
	{
		clear();
		std::free(tasks);
	}

	void out()
	{
		for (int i = 0; i < count; i++)
		{
			printf("Task: %S\n", tasks[i].taskName);
			printf("  - State: %s\n", tasks[i].state_to_str());
		}
	}
};

class TaskManager
{
public:
	TaskManager()
	{
		pRootFolder = NULL;
		pService = NULL;
	}

	int get_root_folder()
	{
		hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(hr))
		{
			printf("\nCoInitializeEx failed: %x", hr);
			return 1;
		}

		//  Set general COM security levels.
		hr = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			0,
			NULL);

		if (FAILED(hr))
		{
			printf("\nCoInitializeSecurity failed: %x", hr);
			CoUninitialize();
			return 1;
		}

		//  ------------------------------------------------------
		//  Create an instance of the Task Service. 

		hr = CoCreateInstance(CLSID_TaskScheduler,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ITaskService,
			(void**)&pService);

		if (FAILED(hr))
		{
			printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
			CoUninitialize();
			return 1;
		}

		//  Connect to the task service.
		hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
		if (FAILED(hr))
		{
			printf("ITaskService::Connect failed: %x", hr);
			pService->Release();
			CoUninitialize();
			return 1;
		}

		//  ------------------------------------------------------
		//  Get the pointer to the root task folder.  This folder will hold the
		//  new task that is registered.

		hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

		if (FAILED(hr))
		{
			printf("Cannot get Root Folder pointer: %x", hr);
			CoUninitialize();
			return 1;
		}

		return 0;
	}

	TaskList get_task_list()
	{
		TaskList task_list;
		task_list.init();

		IRegisteredTaskCollection* pTaskCollection = NULL;
		hr = pRootFolder->GetTasks(NULL, &pTaskCollection);

		if (FAILED(hr))
		{
			printf("Cannot get the registered tasks.: %x", hr);
			CoUninitialize();
			return task_list;
		}

		LONG numTasks = 0;
		hr = pTaskCollection->get_Count(&numTasks);

		if (numTasks == 0)
		{
			printf("\nNo Tasks are currently running");
			pTaskCollection->Release();
			CoUninitialize();
			return task_list;
		}

		//printf("Number of Tasks : %d\n", numTasks);

		TASK_STATE taskState;

		for (LONG i = 0; i < numTasks; i++)
		{
			IRegisteredTask* pRegisteredTask = NULL;
			hr = pTaskCollection->get_Item(_variant_t(i + 1), &pRegisteredTask);

			if (SUCCEEDED(hr))
			{
				BSTR taskName = NULL;
				hr = pRegisteredTask->get_Name(&taskName);
				if (SUCCEEDED(hr))
				{
					hr = pRegisteredTask->get_State(&taskState);
					if (SUCCEEDED(hr))
					{
						task_list.add(taskName, taskState);
					}
				}
				pRegisteredTask->Release();
			}
		}

		pTaskCollection->Release();

		return task_list;
	}

	int add_task(LPCWSTR TaskName, DWORD task_type, LPCWSTR path = EXE_PATH)
	{
		LPCWSTR wszTaskName = TaskName;
		

		pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);
		//  Create the task definition object to create the task.
		ITaskDefinition *pTask = NULL;
		hr = pService->NewTask(0, &pTask);

		pService->Release();  // COM clean up.  Pointer is no longer used.
		if (FAILED(hr))
		{
			printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
			pRootFolder->Release();
			CoUninitialize();
			return 1;
		}

		IRegistrationInfo *pRegInfo = NULL;
		hr = pTask->get_RegistrationInfo(&pRegInfo);
		if (FAILED(hr)) {
			printf("\nCannot get identification pointer: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}
		/*hr = pRegInfo->put_Author((BSTR)L"user");
		pRegInfo->Release();
		if (FAILED(hr)) {
			printf("\nCannot put identification info: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}*/
		ITaskSettings *pSettings = NULL;
		hr = pTask->get_Settings(&pSettings);
		if (FAILED(hr)) {
			printf("\nCannot get settings pointer: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}
		hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
		pSettings->Release();
		if (FAILED(hr)) {
			printf("\nCannot put setting info: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}

		LPCWSTR trig_arg_str = L"";

		switch (task_type)
		{
		case TASK_DEFENDER:
			trig_arg_str = L"Defender Trigger";

			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Firewall_2002",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>*[System/EventID=2002]</Select></Query></QueryList>");
			
			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Firewall_2003",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>*[System/EventID=2003]</Select></Query></QueryList>");

			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Defender_WHC_5007",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Defender/WHC'>*[System/EventID=5007]</Select></Query></QueryList>");
			
			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Defender_OP_5007",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Defender/Operational'>*[System/EventID=5007]</Select></Query></QueryList>");
			
			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Defender_WHC_5004",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Defender/WHC'>*[System/EventID=5004]</Select></Query></QueryList>");
			
			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Defender_OP_5004",
				(wchar_t*)L"<QueryList><Query Id='0'><Select Path='Microsoft-Windows-Windows Defender/Operational'>*[System/EventID=5004]</Select></Query></QueryList>");
			break;

		case TASK_PING:
			trig_arg_str = L"Ping Trigger";

			add_trigger(pTask, 
				pRootFolder,
				(wchar_t*)L"Ping_5152",
				(wchar_t*)L"<QueryList><Query Id='1'><Select Path=\"Security\">*[System/EventID=5152] and \
			*[EventData[Data[@Name=\"SourceAddress\"] and Data=\"192.168.190.1\"]] and \
			*[EventData[Data[@Name=\"DestAddress\"] and Data=\"192.168.190.130\"]]</Select></Query></QueryList>");

		default:
			break;
		}

		//  ------------------------------------------------------
		//  Add an action to the task. This task will execute notepad.exe.     
		IActionCollection *pActionCollection = NULL;

		//  Get the task action collection pointer.
		hr = pTask->get_Actions(&pActionCollection);
		if (FAILED(hr))
		{
			printf("\nCannot get Task collection pointer: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}

		IAction *pAction = NULL;
		hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
		pActionCollection->Release();
		if (FAILED(hr)) {
			printf("\nCannot create an exec action: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}
		IExecAction2 *pExecAction = NULL;
		hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
		pAction->Release();
		if (FAILED(hr)) {
			printf("\nQueryInterface call failed for IID_IExecAction2: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}

		hr = pExecAction->put_Path((BSTR)path);
		if (FAILED(hr)) {
			printf("\nCannot put path information: %x", hr);
			pRootFolder->Release();
			pExecAction->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}
		hr = pExecAction->put_Arguments((BSTR)trig_arg_str);
		if (FAILED(hr)) {
			printf("\nCannot put arguments information: %x", hr);
			pRootFolder->Release();
			pExecAction->Release();
			pTask->Release();
			CoUninitialize();
			return 1;
		}

		//  ------------------------------------------------------
		//  Save the task in the root folder.
		IRegisteredTask *pRegisteredTask = NULL;
		hr = pRootFolder->RegisterTaskDefinition(
			_bstr_t(wszTaskName),
			pTask,
			TASK_CREATE_OR_UPDATE,
			_variant_t(_bstr_t(L"")),
			_variant_t(_bstr_t(L"")),
			TASK_LOGON_INTERACTIVE_TOKEN,
			_variant_t(L""),
			&pRegisteredTask);
		if (FAILED(hr))
		{
			printf("\nError saving the Task : %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			printf("4\n");
			system("pause");
			return 1;
		}

		cout << "  Task added success" << endl;

		//  Clean up.
		//pRootFolder->Release();
		//pTask->Release();
		//pRegisteredTask->Release();
		//CoUninitialize();

		return 0;
	}

	~TaskManager()
	{

	}

private:
	HRESULT hr;
	ITaskService *pService;
	ITaskFolder *pRootFolder;

	void add_trigger(ITaskDefinition *pTask, ITaskFolder *pRootFolder, LPWSTR Id, LPWSTR trigger_text)
	{
		HRESULT hr;
		//  ------------------------------------------------------
		//  Get the trigger collection to insert the time trigger.
		ITriggerCollection *pTriggerCollection = NULL;
		hr = pTask->get_Triggers(&pTriggerCollection);
		if (FAILED(hr))
		{
			printf("\nCannot get trigger collection: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return;
		}

		//  Add the event trigger to the task.
		ITrigger *pTrigger = NULL;
		hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pTrigger);
		pTriggerCollection->Release();
		if (FAILED(hr))
		{
			printf("\nCannot create trigger: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return;
		}

		IEventTrigger *pEventTrigger = NULL;
		hr = pTrigger->QueryInterface(
			IID_IEventTrigger,
			(void**)&pEventTrigger
		);

		pTrigger->Release();
		if (FAILED(hr))
		{
			printf("\nQueryInterface call failed for ITimeTrigger: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return;
		}

		hr = pEventTrigger->put_Id(_bstr_t(Id));
		if (FAILED(hr))
			printf("\nCannot put trigger ID: %x", hr);

		hr = pEventTrigger->put_Subscription(trigger_text);


		pEventTrigger->Release();
		if (FAILED(hr))
		{
			printf("\nCannot add start boundary to trigger: %x", hr);
			pRootFolder->Release();
			pTask->Release();
			CoUninitialize();
			return;
		}
	}

};

int main()
{

	TaskManager tm;
	tm.get_root_folder();

	TaskList tl;
	tl.init();

	char *cmd_str = new char[256];

	while (true)
	{
		cout << "Enter the command: ";
		cin >> cmd_str;

		int len = strlen(cmd_str);

		if (!strncmp(cmd_str, "defender", len))
		{
			tm.add_task(L"Defender Sanek Task", TASK_DEFENDER, L"C:\\Users\\sirot\\Desktop\\prog\\prog_list.exe");
		}
		else if (!strncmp(cmd_str, "ping", len))
		{
			tm.add_task(L"Ping Sanek Task", TASK_PING, L"C:\\Users\\sirot\\Desktop\\prog\\out.exe");
		}
		else if (!strncmp(cmd_str, "list", len))
		{
			tl = tm.get_task_list();
			tl.out();
			tl.clear();
		}
		else if (!strncmp(cmd_str, "help", len))
		{
			cout << "defender - set defender trigger" << endl;
			cout << "ping     - set ping trigger" << endl;
			cout << "list     - output active tasks" << endl;
			cout << "help     - output this text" << endl;
		}
		else cout << "incorrect command" << endl;
	}


}