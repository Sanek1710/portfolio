#define _CRT_SECURE_NO_WARNINGS

#include <comdef.h>
#include <Wbemidl.h>
#include <wincred.h>
#include <strsafe.h>
#include <iostream>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "credui.lib")
#pragma comment(lib, "comsuppw.lib")

#define _WIN32_DCOM
#define UNICODE

using namespace std;

#define	REQ_ERROR		0
#define REQ_OS_V		1
#define REQ_ANTIVIR		2
#define REQ_ANTISPY		3
#define REQ_PRODUCT		4
#define REQ_FIREWALL	5
#define REQ_BIOS		6
#define REQ_MEMORY		7
#define	REQ_USERS		8
#define	REQ_CPU			9
#define	REQ_SERVICE		10
#define	REQ_HELP		11
#define	REQ_EXIT		12



class REQUEST
{
private:
	const wchar_t *desktop;
	
	struct REQ
	{
		const wchar_t *root;
		const wchar_t *clas;
		int count_fields;
		const wchar_t * *fields;
		const wchar_t * *pattern;
	}req[11];

	int req_cur_num;
	int cur_field_num;

public:

	REQUEST(const wchar_t * DESK)
	{
		desktop = DESK;

		req_cur_num = 0;
		cur_field_num = 0;


		int cnt = 2;

		req[REQ_OS_V] = 
		{ 
			L"\\root\\CIMV2", 
			L"Win32_OperatingSystem",
			3,
			new const wchar_t*[3]
			{
				L"Caption", 
				L"Version", 
				L"OSArchitecture"
			},
			new const wchar_t*[3]
			{
				L"    Name: ", 
				L"\n    Version: ", 
				L"\n    Architecture: "
			},
		};

		req[REQ_MEMORY] =
		{
			L"\\root\\CIMV2" ,
			L"Win32_OperatingSystem",
			4,
			new const wchar_t*[4]
			{
				L"FreeVirtualMemory",
				L"TotalVirtualMemorySize",
				L"FreePhysicalMemory",
				L"TotalVisibleMemorySize"
			},
			new const wchar_t*[4]
			{
				L"    VirtualMemory: ",
				L"/",
				L"\n    PhysicalMemory: ",
				L"/",
			},
		};

		req[REQ_PRODUCT] = 
		{ 
			L"\\root\\CIMV2" , 
			L"Win32_Product",
			1,
			new const wchar_t*[1] { L"Name" },
			new const wchar_t*[1] { L"    Name: " },
		};

		req[REQ_ANTIVIR] = 
		{ 
			L"\\root\\SecurityCenter2" ,
			L"AntiVirusProduct",
			2,
			new const wchar_t*[2]
			{
				L"displayName", 
				L"pathToSignedProductExe"
			},
			new const wchar_t*[2]
			{
				L"    Name: ", 
				L"\n    Path: "
			},
		};

		req[REQ_ANTISPY] = 
		{ 
			L"\\root\\SecurityCenter2" ,
			L"AntiSpywareProduct",
			2,
			new const wchar_t*[2]
			{
				L"displayName",
				L"pathToSignedProductExe"
			},
			new const wchar_t*[2]
			{
				L"    Name: ",
				L"\n    Path: "
			},
		};

		cnt = 2;
		req[REQ_FIREWALL] =
		{
			L"\\root\\SecurityCenter2" ,
			L"FirewallProduct",
			cnt,
			new const wchar_t*[cnt]
			{
				L"displayName",
				L"pathToSignedProductExe",
			},
			new const wchar_t*[cnt]
			{
				L"    Name: ",
				L"\n    Path: "
			},
		};

		cnt = 2;
		req[REQ_BIOS] =
		{
			L"\\root\\CIMV2" ,
			L"Win32_BIOS",
			cnt,
			new const wchar_t*[cnt]
			{
				L"Name",
				L"Version",
			},
			new const wchar_t*[cnt]
			{
				L"    Name: ",
				L"\n    Version: ",
			},
		};

		cnt = 1;
		req[REQ_CPU] =
		{
			L"\\root\\CIMV2" ,
			L"Win32_Processor",
			cnt,
			new const wchar_t*[cnt]
			{
				L"Name",
			},
			new const wchar_t*[cnt]
			{
				L"    Name: ",
			},
		};

		cnt = 1;
		req[REQ_USERS] =
		{
			L"\\root\\CIMV2" ,
			L"Win32_UserDesktop",
			cnt,
			new const wchar_t*[cnt]
			{
				L"Element",
			},
			new const wchar_t*[cnt]
			{
				L"    USER: ",
			},
		};

		cnt = 1;
		req[REQ_SERVICE] =
		{
			L"\\root\\CIMV2" ,
			L"Win32_Service",
			cnt,
			new const wchar_t*[cnt]
			{
				L"Name",
			},
			new const wchar_t*[cnt]
			{
				L"    Name: ",
			},
		};

		/*
		cnt = 2;
		req[REQ_] =
		{
			L"root\\XXXXXXXXX" ,
			L"XXXXXXXXXXXXX",
			cnt,
			new const wchar_t*[cnt]
			{
				L"XXXXXXXXXXXXXXXX",
			},
			new const wchar_t*[cnt]
			{
				L"\n    XXXXXXX: ",
			},
		};
		*/
	}

	void set_desktop(const wchar_t *desk)
	{
		desktop = desk;
	}

	void set_request(int num)
	{
		if (num < 0 || 10 < num) req_cur_num = REQ_ERROR;
		req_cur_num = num;
	}

	const wchar_t *get_desktop()
	{
		return desktop;
	}

	wchar_t *get_req_class()
	{
		wchar_t *req_class; 
		
		if (req_cur_num < 1 || 10 < req_cur_num)
		{
			req_class = new wchar_t[100]{ L"" };
		}
		else
		{
			req_class = new wchar_t[100]{ L"Select * from " };
			wcscat(req_class, req[req_cur_num].clas);
		}

		return req_class;
	}

	wchar_t *get_req_root()
	{
		wchar_t *req_root;

		if (req_cur_num < 1 || 10 < req_cur_num)
		{
			req_root = new wchar_t[100]{ L"" };
		}
		else
		{
			req_root = new wchar_t[100]{ L"\\\\" };
			wcscat(req_root, desktop);
			wcscat(req_root, req[req_cur_num].root);
		}

		return req_root;
	}

	const wchar_t *cur_field()
	{
		if (cur_field_num >= req[req_cur_num].count_fields) cur_field_num = 0;
		return req[req_cur_num].fields[cur_field_num];
	}

	const wchar_t *next_field()
	{
		cur_field_num++;

		if (cur_field_num >= req[req_cur_num].count_fields)
		{
			cur_field_num = 0;
			return NULL;
		}

		return req[req_cur_num].fields[cur_field_num];
	}

	void out_field(wchar_t *val)
	{
		wcout << req[req_cur_num].pattern[cur_field_num] << val;
	}

};

void COM_init(HRESULT &hres)
{
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "ERROR 0x" << hex << hres << ": COM_init" << endl;
		exit(0);
	}
}

void set_COM_secure(HRESULT &hres)
{
	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IDENTIFY,
		NULL,
		EOAC_NONE,
		NULL
	);

	if (FAILED(hres))
	{
		cout << "ERROR 0x" << hex << hres << ": set_COM_secure" << endl;
		CoUninitialize();
		exit(0);
	}
}

void create_WMI_locator(HRESULT &hres, IWbemLocator *&pLoc)
{
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		cout << "ERROR 0x" << hex << hres << ": create_WMI_locator" << endl;
		CoUninitialize();
		exit(0);
	}
}

void auth_window(
	IWbemLocator *pLoc,
	wchar_t *pszName,
	wchar_t *pszPwd,
	wchar_t *pszDomain,
	wchar_t *pszUserName,
	wchar_t *pszAuthority,
	bool	&useToken,
	bool	&useNTLM,
	wchar_t *DESKTOP
	)
{
	BOOL fSave;
	DWORD dwErr;
	CREDUI_INFO cui;

	memset(&cui, 0, sizeof(CREDUI_INFO));
	cui.cbSize = sizeof(CREDUI_INFO);
	cui.hwndParent = NULL;
	cui.pszMessageText = TEXT("Press cancel to use process token");
	cui.pszCaptionText = TEXT("Enter Account Information");
	cui.hbmBanner = NULL;
	fSave = FALSE;


	dwErr = CredUIPromptForCredentials(
		&cui,
		TEXT(""),
		NULL,
		0,
		pszName,
		514,
		pszPwd,
		257,
		&fSave,
		CREDUI_FLAGS_GENERIC_CREDENTIALS |
		CREDUI_FLAGS_ALWAYS_SHOW_UI |
		CREDUI_FLAGS_DO_NOT_PERSIST);

	if (dwErr == ERROR_CANCELLED)
	{
		useToken = true;
	}
	else if (dwErr)
	{
		cout << "Did not get credentials " << dwErr << endl;
		pLoc->Release();
		CoUninitialize();
		exit(0);
	}

	if (!useNTLM)
	{
		StringCchPrintf(pszAuthority, 514, L"kERBEROS:%s", DESKTOP);
	}
}

void authentification(
	IWbemLocator *pLoc,
	IWbemServices *pSvc,
	wchar_t *pszName,
	wchar_t *pszPwd,
	wchar_t *pszDomain,
	wchar_t *pszUserName,
	bool	&useToken,
	COAUTHIDENTITY *&userAcct
)
{
	COAUTHIDENTITY authIdent;
	if (!useToken)
	{
		memset(&authIdent, 0, sizeof(COAUTHIDENTITY));
		authIdent.PasswordLength = wcslen(pszPwd);
		authIdent.Password = (USHORT*)pszPwd;
		LPWSTR slash = wcschr(pszName, L'\\');
		if (slash == NULL)
		{
			cout << "Could not create Auth identity. No domain specified\n";
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			exit(0);
		}
		StringCchCopy(pszUserName, 514, slash + 1);
		authIdent.User = (USHORT*)pszUserName;
		authIdent.UserLength = wcslen(pszUserName);
		StringCchCopyN(pszDomain, 514, pszName, slash - pszName);

		authIdent.Domain = (USHORT*)pszDomain;
		authIdent.DomainLength = slash - pszName;
		authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
		userAcct = &authIdent;
	}
}


void connect_serv(
	IWbemLocator *pLoc,
	IWbemServices *&pSvc,
	wchar_t *pszName,
	wchar_t *pszPwd,
	wchar_t *pszAuthority,
	bool	&useToken,
	bool	&useNTLM,
	COAUTHIDENTITY *&userAcct,
	HRESULT &hres,
	REQUEST &req
) {
	hres = pLoc->ConnectServer(
		_bstr_t(req.get_req_root()),
		_bstr_t(useToken ? NULL : pszName),
		_bstr_t(useToken ? NULL : pszPwd),
		NULL,
		NULL,
		_bstr_t(useNTLM ? NULL : pszAuthority),
		NULL,
		&pSvc
	);

	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;
		pLoc->Release();
		CoUninitialize();
		exit(0);

	}

	hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_DEFAULT,
		RPC_C_AUTHZ_DEFAULT,
		COLE_DEFAULT_PRINCIPAL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		userAcct,
		EOAC_NONE
	);
	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		exit(0);
	}
}



void make_request(
	IWbemLocator *pLoc,
	IWbemServices *pSvc,
	wchar_t *pszName,
	wchar_t *pszPwd,
	wchar_t *pszAuthority,
	bool	&useToken,
	bool	&useNTLM,
	COAUTHIDENTITY *&userAcct,
	HRESULT &hres,
	REQUEST &req
)
{

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(req.get_req_class()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		exit(0);
	}
	hres = CoSetProxyBlanket(
		pEnumerator,
		RPC_C_AUTHN_DEFAULT,
		RPC_C_AUTHZ_DEFAULT,
		COLE_DEFAULT_PRINCIPAL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		userAcct,
		EOAC_NONE
	);
	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket on enumerator. Error code = 0x"
			<< hex << hres << endl;
		pEnumerator->Release();
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		exit(0);
	}

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
									   &pclsObj, &uReturn);
		if (0 == uReturn)
		{
			break;
		}
		VARIANT vtProp;
		// Выбираем поле Name

		for (const wchar_t *curf = req.cur_field(); curf != NULL; curf = req.next_field())
		{
			hr = pclsObj->Get(curf, 0, &vtProp, 0, 0);
			req.out_field(vtProp.bstrVal);
		}
		cout << endl;
		cout << endl;

		VariantClear(&vtProp);
		pclsObj->Release();
		pclsObj = NULL;

	}
	// Очистка
	// ========

	pEnumerator->Release();
	if (pclsObj)
	{
		pclsObj->Release();
	}

}

int
read_command()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000000A);
	printf("Enter command> ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00000007);

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


	if (strlen(buf) == 0) return REQ_HELP;
	if (!strncmp(buf, "os version"	, len)) return REQ_OS_V		;
	if (!strncmp(buf, "antivirus"	, len)) return REQ_ANTIVIR	;	
	if (!strncmp(buf, "antispy"		, len)) return REQ_ANTISPY	;	
	if (!strncmp(buf, "programs"	, len)) return REQ_PRODUCT	;
	if (!strncmp(buf, "firewall"	, len)) return REQ_FIREWALL	;
	if (!strncmp(buf, "bios"		, len)) return REQ_BIOS	    ;
	if (!strncmp(buf, "memory"		, len)) return REQ_MEMORY	; 
	if (!strncmp(buf, "users"		, len)) return REQ_USERS	;
	if (!strncmp(buf, "cpu"			, len)) return REQ_CPU		;
	if (!strncmp(buf, "service"		, len)) return REQ_SERVICE	;
	if (!strncmp(buf, "help"		, len)) return REQ_HELP		;
	if (!strncmp(buf, "exit"		, len)) return REQ_EXIT		;
	return REQ_ERROR;		
}																	

void
help()
{
	cout << "    os version"<< endl;
	cout << "    antivirus"	<< endl;
	cout << "    antispy"	<< endl;
	cout << "    programs"	<< endl;
	cout << "    firewall"	<< endl;
	cout << "    bios"		<< endl;
	cout << "    memory"	<< endl;
	cout << "    users"		<< endl;
	cout << "    cpu"		<< endl;
	cout << "    service"	<< endl;
	cout << "    help"		<< endl;
}


int __cdecl main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");

	HRESULT hres;
	COM_init(hres);

	set_COM_secure(hres);

	IWbemLocator *pLoc = NULL;
	create_WMI_locator(hres, pLoc);

	bool	useToken = false;
	bool	useNTLM = true;
	wchar_t pszName[514] = { 0 };
	wchar_t pszPwd[257] = { 0 };
	wchar_t pszDomain[514];
	wchar_t pszUserName[514];
	wchar_t pszAuthority[514];

	wchar_t *DESKTOP = const_cast<wchar_t *>(L"DESKTOP-IGOILCN");

	REQUEST req(DESKTOP);
	//req.set_request(5);
	//wcout << req.get_req_root() << endl;

	auth_window(pLoc, pszName, pszPwd, pszDomain, pszUserName, pszAuthority, useToken, useNTLM, DESKTOP);

	IWbemServices *pSvc = NULL;
	COAUTHIDENTITY *userAcct = NULL;

	authentification(pLoc, pSvc, pszName, pszPwd, pszDomain, pszUserName, useToken, userAcct);


	while (1)
	{
		int request_NUM = read_command();

		if (request_NUM == REQ_ERROR) continue;
		else if (request_NUM == REQ_EXIT) break;
		else if (request_NUM == REQ_HELP)
		{
			help();
			continue;
		}

		req.set_request(request_NUM);
		
		connect_serv(pLoc, pSvc, pszName, pszPwd, pszAuthority, useToken, useNTLM, userAcct, hres, req);

		make_request(pLoc, pSvc, pszName, pszPwd, pszAuthority, useToken, useNTLM, userAcct, hres, req);
	}

	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

	system("pause");

	return 0;
}




