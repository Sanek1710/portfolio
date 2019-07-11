#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <lm.h>
#include <iostream>
#include <AccCtrl.h>
#include <LM.h>
#include <sddl.h>
#include <Ntsecapi.h>
#include <LsaLookup.h>

#define SID_DEF_SIZE 28

using namespace std;

namespace DLLFunctions
{
	typedef	NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetUserEnumFunc)
		(
			_In_opt_    LPCWSTR,
			_In_        DWORD,
			_In_        DWORD,
			_Outptr_result_buffer_(_Inexpressible_("varies")) LPBYTE *,
			_In_        DWORD,
			_Out_       LPDWORD,
			_Out_       LPDWORD,
			_Inout_opt_ PDWORD
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetUserChangePasswordFunc)
		(
			_In_opt_ IN  LPCWSTR,
			_In_opt_ IN  LPCWSTR,
			_In_ IN  LPCWSTR,
			_In_ IN  LPCWSTR
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetUserAddFunc)
		(
			_In_opt_  LPCWSTR,
			_In_      DWORD,
			_In_      LPBYTE,
			_Out_opt_ LPDWORD
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetUserDelFunc)
		(
			_In_opt_  LPCWSTR,
			_In_      LPCWSTR
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetLocalGroupEnumFunc)
		(
			_In_opt_    LPCWSTR,
			_In_        DWORD,
			_Outptr_result_buffer_(_Inexpressible_("varies")) LPBYTE*,
			_In_        DWORD,
			_Out_       LPDWORD,
			_Out_       LPDWORD,
			_Inout_opt_ PDWORD_PTR
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION*	DLLNetLocalGroupDelFunc)
		(
			_In_opt_  LPCWSTR,
			_In_      LPCWSTR
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION *DLLNetLocalGroupAddFunc)
		(
			_In_opt_  LPCWSTR,
			_In_      DWORD,
			_In_reads_(_Inexpressible_("varies"))  LPBYTE,
			_Out_opt_ LPDWORD
			);

	typedef NET_API_STATUS
	(NET_API_FUNCTION *DLLNetApiBufferFreeFunc)
		(
			_Frees_ptr_opt_ LPVOID 
			);

	typedef NET_API_STATUS 
		(NET_API_FUNCTION *DLLNetGroupAddUserFunc)
		(
			_In_opt_  LPCWSTR   servername OPTIONAL,
			_In_      LPCWSTR   GroupName,
			_In_      LPCWSTR   username
		);

	typedef NET_API_STATUS 
		(NET_API_FUNCTION *DLLNetGroupDelUserFunc)
		(
			_In_opt_  LPCWSTR   servername OPTIONAL,
			_In_      LPCWSTR   GroupName,
			_In_      LPCWSTR   Username
		);

	DLLNetLocalGroupEnumFunc	  LoadedNetLocalGroupEnum;
	DLLNetLocalGroupDelFunc		  LoadedNetLocalGroupDel;
	DLLNetLocalGroupAddFunc		  LoadedNetLocalGroupAdd;
	DLLNetUserEnumFunc			  LoadedNetUserEnum;
	DLLNetUserChangePasswordFunc  LoadedNetUserChangePassword;
	DLLNetUserAddFunc			  LoadedNetUserAdd;
	DLLNetUserDelFunc			  LoadedNetUserDel;
	DLLNetApiBufferFreeFunc		  LoadedNetApiBufferFree;
	DLLNetGroupAddUserFunc		  LoadedNetGroupAddUser;
	DLLNetGroupDelUserFunc		  LoadedNetGroupDelUser;

	typedef NTSTATUS
	(NTAPI* DLLLsaAddAccountRightsFunc)(
		_In_ LSA_HANDLE ,
		_In_ PSID ,
		_In_reads_(CountOfRights) PLSA_UNICODE_STRING ,
		_In_ ULONG 
		);


	typedef NTSTATUS
	(NTAPI* DLLLsaRemoveAccountRightsFunc)(
		_In_ LSA_HANDLE ,
		_In_ PSID ,
		_In_ BOOLEAN ,
		_In_reads_opt_(CountOfRights) PLSA_UNICODE_STRING ,
		_In_ ULONG 
		);

	typedef ULONG
	(NTAPI* DLLLsaNtStatusToWinErrorFunc)(
		_In_ NTSTATUS 
		);


	typedef NTSTATUS
	(NTAPI* DLLLsaEnumerateAccountRightsFunc)(
		_In_ LSA_HANDLE ,
		_In_ PSID ,
		_Outptr_result_buffer_(*CountOfRights) PLSA_UNICODE_STRING *,
		_Out_ PULONG 
		);

	typedef NTSTATUS
	(NTAPI* DLLLsaOpenPolicyFunc)(
		_In_opt_ PLSA_UNICODE_STRING ,
		_In_ PLSA_OBJECT_ATTRIBUTES ,
		_In_ ACCESS_MASK ,
		_Out_ PLSA_HANDLE 
		);

	typedef PVOID LSA_HANDLE, *PLSA_HANDLE;

	DLLLsaAddAccountRightsFunc		  LoadedLsaAddAccountRights;
	DLLLsaRemoveAccountRightsFunc	  LoadedLsaRemoveAccountRights;
	DLLLsaNtStatusToWinErrorFunc	  LoadedLsaNtStatusToWinError;
	DLLLsaEnumerateAccountRightsFunc  LoadedLsaEnumerateAccountRights;
	DLLLsaOpenPolicyFunc			  LoadedLsaOpenPolicy;

	int LoadWinapiFunctions()
	{
		HINSTANCE hNetApiDLL;          // Handle to DLL  
		HINSTANCE hAdvapi32DLL;          // Handle to DLL  

		hNetApiDLL = LoadLibrary(L"netapi32.dll");
		if (hNetApiDLL != NULL)
		{
			LoadedNetLocalGroupEnum = (DLLNetLocalGroupEnumFunc)GetProcAddress(hNetApiDLL, "NetLocalGroupEnum");
			LoadedNetLocalGroupDel = (DLLNetLocalGroupDelFunc)GetProcAddress(hNetApiDLL, "NetLocalGroupDel");
			LoadedNetLocalGroupAdd = (DLLNetLocalGroupAddFunc)GetProcAddress(hNetApiDLL, "NetLocalGroupAdd");
			LoadedNetUserEnum = (DLLNetUserEnumFunc)GetProcAddress(hNetApiDLL, "NetUserEnum");
			LoadedNetUserChangePassword = (DLLNetUserChangePasswordFunc)GetProcAddress(hNetApiDLL, "NetUserChangePassword");
			LoadedNetUserAdd = (DLLNetUserAddFunc)GetProcAddress(hNetApiDLL, "NetUserAdd");
			LoadedNetUserDel = (DLLNetUserDelFunc)GetProcAddress(hNetApiDLL, "NetUserDel");
			LoadedNetApiBufferFree = (DLLNetApiBufferFreeFunc)GetProcAddress(hNetApiDLL, "NetApiBufferFree");
			LoadedNetGroupAddUser  = (DLLNetGroupAddUserFunc) GetProcAddress(hNetApiDLL, "NetGroupAddUser");
			LoadedNetGroupDelUser  = (DLLNetGroupDelUserFunc) GetProcAddress(hNetApiDLL, "NetGroupDelUser");

			if (
				!LoadedNetLocalGroupEnum
				|| !LoadedNetLocalGroupDel
				|| !LoadedNetLocalGroupAdd
				|| !LoadedNetUserEnum
				|| !LoadedNetUserChangePassword
				|| !LoadedNetUserAdd
				|| !LoadedNetUserDel
				|| !LoadedNetApiBufferFree
				|| !LoadedNetGroupAddUser
				|| !LoadedNetGroupDelUser
				)
			{
				cout << "  netapi32.dll load functions error" << endl;
				FreeLibrary(hNetApiDLL);
				exit(0);
			}
		}
		else
		{
			cout << "  netapi32.dll open error" << endl;
			exit(0);
		}

		hAdvapi32DLL = LoadLibrary(L"advapi32.dll");
		if (hAdvapi32DLL != NULL)
		{
			LoadedLsaAddAccountRights = (DLLLsaAddAccountRightsFunc)GetProcAddress(hAdvapi32DLL, "LsaAddAccountRights");
			LoadedLsaRemoveAccountRights = (DLLLsaRemoveAccountRightsFunc)GetProcAddress(hAdvapi32DLL, "LsaRemoveAccountRights");
			LoadedLsaNtStatusToWinError = (DLLLsaNtStatusToWinErrorFunc)GetProcAddress(hAdvapi32DLL, "LsaNtStatusToWinError");
			LoadedLsaEnumerateAccountRights = (DLLLsaEnumerateAccountRightsFunc)GetProcAddress(hAdvapi32DLL, "LsaEnumerateAccountRights");
			LoadedLsaOpenPolicy = (DLLLsaOpenPolicyFunc)GetProcAddress(hAdvapi32DLL, "LsaOpenPolicy");

			if (
				!LoadedLsaAddAccountRights
				|| !LoadedLsaRemoveAccountRights
				|| !LoadedLsaNtStatusToWinError
				|| !LoadedLsaEnumerateAccountRights
				|| !LoadedLsaOpenPolicy
				)
			{
				cout << "  advapi32.dll load functions error" << endl;
				FreeLibrary(hNetApiDLL);
				FreeLibrary(hAdvapi32DLL);
				exit(0);
			}
		}
		else
		{
			FreeLibrary(hNetApiDLL);
			cout << "  advapi32.dll open error" << endl;
			exit(0);
		}
	}

}

using namespace DLLFunctions;



class LSAProv
{
public:
	LSAProv()
	{
		GetLSAHandle();
		UIL.ui = NULL;
	}

	~LSAProv()
	{
		UIL.clear();
		GIL.clear();
	}

	LSA_HANDLE GetLSAHandle()
	{
		LSA_UNICODE_STRING compName = { 0 };
		LSA_OBJECT_ATTRIBUTES lsaOA = { 0 };
		NTSTATUS stat;

		stat = LoadedLsaOpenPolicy(&compName, &lsaOA, POLICY_ALL_ACCESS, &hPolicy);
		ULONG err = LoadedLsaNtStatusToWinError(stat);

		if (err != ERROR_SUCCESS)
		{
			cout << "  Open policy error" << endl;
			return 0;
		}

		return hPolicy;
	}

	DWORD GetUserInfo()
	{
		UIL.clear();
		NET_API_STATUS ret_status; // код возврата из функции
								   // получаем информацию о пользователях

		ret_status = LoadedNetUserEnum(
			NULL, // имя сервера
			0, // узнаем только имена пользователей
			FILTER_NORMAL_ACCOUNT, // перечисляем пользователей, зарегистрированных на компьютере
			(LPBYTE*)&UIL.ui, // адрес информации о пользователях
			MAX_PREFERRED_LENGTH, // перечисляем всех пользователей
			&UIL.entries_read, // количество прочитанных пользователей
			&UIL.total_entries, // общее количество пользователей
			NULL); // индексации нет

		// проверяем на успешное завершение
		if (ret_status != NERR_Success)
		{
			printf("  Net user get info failed.\n");
			printf("  Net API Status: %d\n", ret_status);
			UIL.clear();
			return ret_status;
		}
	}

	void PrintUserList()
	{
		GetUserInfo();

		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		int j = 0;
		while (j < UIL.total_entries)
		{
			dwDomainSize = 1024;
			if (!LookupAccountName(NULL, UIL.ui[j].usri0_name, SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
			{
				if (cbSID != SID_DEF_SIZE)
				{
					SID = (DWORD*)realloc(SID, cbSID);
					j++;
				}
			}

			ConvertSidToStringSid(SID, &strSID);
			wprintf(L"  %-20s", UIL.ui[j].usri0_name);
			wcout << " :" << " " << strSID << endl;
			j++;
		}
	}

	void AddUser(wstring Name, wstring Password)
	{
		USER_INFO_1 ui;
		DWORD dwError = 0;
		NET_API_STATUS nStatus;

		ui.usri1_name = (LPWSTR)Name.data();
		ui.usri1_password = (LPWSTR)Password.data();

		ui.usri1_priv = USER_PRIV_USER;
		ui.usri1_home_dir = NULL;
		ui.usri1_comment = NULL;
		ui.usri1_flags = UF_SCRIPT;
		ui.usri1_script_path = NULL;

		nStatus = LoadedNetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
		//nStatus =LoadedNetUserDel(argv[1], (LPCWSTR)usr_name);

		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"User %s has been successfully added\n", Name.data());
		}
		else if (nStatus == NERR_NoNetworkResource)
		{
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		}
	}

	void DelUser(wstring Name)
	{
		DWORD dwError = 0;
		NET_API_STATUS nStatus;

		nStatus = LoadedNetUserDel(NULL, Name.data());

		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"User %s has been successfully deleted\n", Name.data());
		}
		else if (nStatus == NERR_NoNetworkResource)
		{
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		}
	}

	void ChangeUserPassword(wstring Name, wstring OldPassword, wstring NewPassword)
	{
		USER_INFO_1 ui;
		DWORD dwError = 0;
		NET_API_STATUS nStatus;

		nStatus = LoadedNetUserChangePassword(NULL, Name.data(), OldPassword.data(), NewPassword.data());

		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"  Password successfully changed\n");
		}
		else
		{
			fprintf(stderr, "  Password change error\n");
		}
	}

	void PrintAllUserPrivilege()
	{
		GetUserInfo();

		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		int j = 0;
		while (j < UIL.total_entries)
		{
			dwDomainSize = 1024;
			if (!LookupAccountName(NULL, UIL.ui[j].usri0_name, SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
			{
				if (cbSID != SID_DEF_SIZE)
				{
					SID = (DWORD*)realloc(SID, cbSID);
					j++;
				}
			}

			ConvertSidToStringSid(SID, &strSID);
			wcout << UIL.ui[j].usri0_name << ":" << endl;
			wcout << "    SID      : " << strSID << endl;

			// а теперь привилегии. для начала выведем что есть
			ULONG cPrivs = 0;
			PLSA_UNICODE_STRING pPrivs;

			stat = LoadedLsaEnumerateAccountRights(hPolicy, SID, &pPrivs, &cPrivs);

			if (cPrivs != 0)
			{
				wcout << "    Privilege: " << endl;
				for (ULONG i = 0; i < cPrivs; i++)
				{
					wcout << "        " << i + 1 << ": " << pPrivs[i].Buffer << endl;
				}
			}
			else wcout << "    Privilege: none" << endl;
			cout << endl;
			j++;
		}
	}

	void PrintUserPrivilege(wstring Name)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		dwDomainSize = 1024;
		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			if (cbSID != SID_DEF_SIZE)
			{
				SID = (DWORD*)realloc(SID, cbSID);
				return;
			}
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		// а теперь привилегии. для начала выведем что есть
		ULONG cPrivs = 0;
		PLSA_UNICODE_STRING pPrivs;

		stat = LoadedLsaEnumerateAccountRights(hPolicy, SID, &pPrivs, &cPrivs);

		if (cPrivs != 0)
		{
			wcout << "    Privilege: " << endl;
			for (ULONG i = 0; i < cPrivs; i++)
			{
				wcout << "        " << i + 1 << ": " << pPrivs[i].Buffer << endl;
			}
		}
		else wcout << "    Privilege: none" << endl;
		cout << endl;
	}

	void UserAddRight(wstring Name, wstring Right)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			cout << "  UserAddRight: User name Error" << endl;
			return;
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		ULONG cPrivs = 1;
		PLSA_UNICODE_STRING pPrivs;

		stat = LoadedLsaAddAccountRights(hPolicy, SID, &StringToLsaUnicodeString(Right.data()), cPrivs);

		if (stat != ERROR_SUCCESS)
		{
			ULONG winError = LoadedLsaNtStatusToWinError(stat);
			printf("  Net user get info failed.\n");
			printf("  Net API Status: %d\n", stat);
		}
		else
		{
			wcout << "  Right " << Right.data() << " added success" << endl;
		}
	}

	void UserDelRight(wstring Name, wstring Right)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			cout << "  UserAddRight: User name Error" << endl;
			return;
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		ULONG cPrivs = 1;
		PLSA_UNICODE_STRING pPrivs;


		stat = LoadedLsaRemoveAccountRights(hPolicy, SID, FALSE, &StringToLsaUnicodeString(Right.data()), cPrivs);

		if (stat != ERROR_SUCCESS)
		{
			ULONG winError = LoadedLsaNtStatusToWinError(stat);
			printf("  Net user get info failed.\n");
			printf("  Net API Status: %d\n", stat);
		}
		else
		{
			wcout << "  Right " << Right.data() << " deleted success" << endl;
		}

		//LsaRemoveAccountRights
	}


	void PrintGroupList()
	{
		GetGroupInfo();

		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		int j = 0;
		while (j < GIL.total_entries)
		{
			dwDomainSize = 1024;
			if (!LookupAccountName(NULL, GIL.gi[j].grpi0_name, SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
			{
				if (cbSID != SID_DEF_SIZE)
				{
					SID = (DWORD*)realloc(SID, cbSID);
					j++;
				}
			}

			ConvertSidToStringSid(SID, &strSID);
			wprintf(L"  %-30s", GIL.gi[j].grpi0_name);
			wcout << " :" << " " << strSID << endl;
			j++;
		}
	}

	DWORD GetGroupInfo()
	{
		GIL.clear();
		NET_API_STATUS ret_status; // код возврата из функции
								   // получаем информацию о пользователях
		ret_status = LoadedNetLocalGroupEnum(
			NULL, // имя сервера
			0, // узнаем только имена пользователей
			(LPBYTE*)&GIL.gi, // адрес информации о пользователях
			MAX_PREFERRED_LENGTH, // перечисляем всех пользователей
			&GIL.entries_read, // количество прочитанных пользователей
			&GIL.total_entries, // общее количество пользователей
			NULL); // индексации нет

		// проверяем на успешное завершение
		if (ret_status != NERR_Success)
		{
			printf("  NetGroupEnum failed.\n");
			printf("  Net API Status: %d\n", ret_status);
			GIL.clear();
			return ret_status;
		}
	}

	void AddGroup(wstring Name)
	{
		GROUP_INFO_0 gi;
		DWORD dwError = 0;
		NET_API_STATUS nStatus;

		gi.grpi0_name = (LPWSTR)Name.data();

		nStatus = LoadedNetLocalGroupAdd(NULL, 0, (LPBYTE)&gi, &dwError);

		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"  Group %s has been successfully added\n", Name.data());
		}
		else if (nStatus == NERR_NoNetworkResource)
		{
			fprintf(stderr, "  A system error has occurred: %d\n", nStatus);
		}
	}

	void DelGroup(wstring Name)
	{
		DWORD dwError = 0;
		NET_API_STATUS nStatus;

		nStatus = LoadedNetLocalGroupDel(NULL, Name.data());

		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"Group %s has been successfully deleted\n", Name.data());
		}
		else if (nStatus == NERR_NoNetworkResource)
		{
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		}
	}

	void PrintAllGroupPrivilege()
	{
		GetGroupInfo();

		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		int j = 0;
		while (j < GIL.total_entries)
		{
			dwDomainSize = 1024;
			if (!LookupAccountName(NULL, GIL.gi[j].grpi0_name, SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
			{
				if (cbSID != SID_DEF_SIZE)
				{
					SID = (DWORD*)realloc(SID, cbSID);
					j++;
				}
			}

			ConvertSidToStringSid(SID, &strSID);
			wcout << GIL.gi[j].grpi0_name << ":" << endl;
			wcout << "    SID      : " << strSID << endl;

			// а теперь привилегии. для начала выведем что есть
			ULONG cPrivs = 0;
			PLSA_UNICODE_STRING pPrivs;

			stat = LoadedLsaEnumerateAccountRights(hPolicy, SID, &pPrivs, &cPrivs);
			if (cPrivs != 0)
			{
				wcout << "    Privilege: " << endl;
				for (ULONG i = 0; i < cPrivs; i++)
				{
					wcout << "        " << i + 1 << ": " << pPrivs[i].Buffer << endl;
				}
			}
			else wcout << "    Privilege: none" << endl;
			cout << endl;
			j++;
		}
	}

	void PrintGroupPrivilege(wstring Name)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		dwDomainSize = 1024;
		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			if (cbSID != SID_DEF_SIZE)
			{
				SID = (DWORD*)realloc(SID, cbSID);
				return;
			}
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		// а теперь привилегии. для начала выведем что есть
		ULONG cPrivs = 0;
		PLSA_UNICODE_STRING pPrivs;

		stat = LoadedLsaEnumerateAccountRights(hPolicy, SID, &pPrivs, &cPrivs);

		if (cPrivs != 0)
		{
			wcout << "    Privilege: " << endl;
			for (ULONG i = 0; i < cPrivs; i++)
			{
				wcout << "        " << i + 1 << ": " << pPrivs[i].Buffer << endl;
			}
		}
		else wcout << "    Privilege: none" << endl;
		cout << endl;
	}

	void GroupAddRight(wstring Name, wstring Right)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			cout << "  GroupAddRight: Group name Error" << endl;
			return;
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		ULONG cPrivs = 1;
		PLSA_UNICODE_STRING pPrivs;

		stat = LoadedLsaAddAccountRights(hPolicy, SID, &StringToLsaUnicodeString(Right.data()), cPrivs);

		if (stat != ERROR_SUCCESS)
		{
			ULONG winError = LoadedLsaNtStatusToWinError(stat);
			printf("  Net user get info failed.\n");
			printf("  Net API Status: %d\n", stat);
		}
		else
		{
			wcout << "  Right " << Right.data() << " added success" << endl;
		}
	}

	void GroupDelRight(wstring Name, wstring Right)
	{
		DWORD cbSID = SID_DEF_SIZE;
		DWORD *SID = (DWORD*)malloc(cbSID);
		WCHAR* strSID;
		WCHAR szDomain[1024];
		DWORD dwDomainSize = 1024;
		SID_NAME_USE SIDNameUse;
		NTSTATUS stat;

		if (!LookupAccountName(NULL, Name.data(), SID, &cbSID, szDomain, &dwDomainSize, &SIDNameUse))
		{
			cout << "  GroupAddRight: Group name Error" << endl;
			return;
		}

		ConvertSidToStringSid(SID, &strSID);
		wcout << Name.data() << ":" << endl;
		wcout << "    SID      : " << strSID << endl;

		ULONG cPrivs = 1;
		PLSA_UNICODE_STRING pPrivs;


		stat = LoadedLsaRemoveAccountRights(hPolicy, SID, FALSE, &StringToLsaUnicodeString(Right.data()), cPrivs);

		if (stat != ERROR_SUCCESS)
		{
			ULONG winError = LoadedLsaNtStatusToWinError(stat);
			printf("  Net user get info failed.\n");
			printf("  Net API Status: %d\n", stat);
		}
		else
		{
			wcout << "  Right " << Right.data() << " deleted success" << endl;
		}

		//LsaRemoveAccountRights
	}

	void AddGroupUser(wstring GrName, wstring UsName)
	{
		NET_API_STATUS ret_status; // код возврата из функции
						   // получаем информацию о пользователях

		ret_status = LoadedNetGroupAddUser(NULL, GrName.data(), UsName.data());

		if (ret_status != NERR_Success)
		{
			cout << "  Cannot add user in group." << endl;
		}
		else
		{
			cout << "  added success" << endl;
		}
	}

	void DelGroupUser(wstring GrName, wstring UsName)
	{
		NET_API_STATUS ret_status; // код возврата из функции
				   // получаем информацию о пользователях

		ret_status = LoadedNetGroupDelUser(NULL, GrName.data(), UsName.data());

		if (ret_status != NERR_Success)
		{
			cout << "  Cannot add user in group." << endl;
		}
		else
		{
			cout << "  deleted success" << endl;
		}
	}

private:
	LSA_HANDLE hPolicy;//PolicyHandle;

	struct UserInfoList
	{
		USER_INFO_0 *ui = NULL; // информация о пользователе
		DWORD entries_read; // количество прочитанных пользователей
		DWORD total_entries; // общее количество пользователей

		void clear()
		{
			if (ui != NULL)
			{
				LoadedNetApiBufferFree(ui);
				entries_read = total_entries = 0;
			}
		}
	}UIL;

	struct GroupInfoList
	{
		GROUP_INFO_0 *gi = NULL; // информация о пользователе
		DWORD entries_read; // количество прочитанных пользователей
		DWORD total_entries; // общее количество пользователей

		void clear()
		{
			if (gi != NULL)
			{
				LoadedNetApiBufferFree(gi);
				entries_read = total_entries = 0;
			}
		}
	}GIL;

	LSA_UNICODE_STRING StringToLsaUnicodeString(LPCTSTR string) {
		LSA_UNICODE_STRING lsaString;
		DWORD dwLen = 0;

		dwLen = wcslen(string);
		lsaString.Buffer = (LPWSTR)string;
		lsaString.Length = (USHORT)((dwLen) * sizeof(WCHAR));
		lsaString.MaximumLength = (USHORT)((dwLen + 1) * sizeof(WCHAR));
		return lsaString;
	}
};


void 
help()
{
	cout << "  > (user|group) [name]                         - (user|group) info" << endl;
	cout << "  > (user|group) -all                           - all (user|group) info" << endl;
	cout << "  > (user|group) [name] -add [passw]            - add new (user|group)" << endl;
	cout << "  > (user|group) [name] -del                    - delele  (user|group)" << endl;
	cout << "  > (user|group) [name] -rgt [right ] -add      - add new right to (user|group)" << endl;
	cout << "  > (user|group) [name] -rgt [right ] -del      - delete  right to (user|group)" << endl;
	cout << "  > user         [name] -pwd [old pw] [new pw]  - change user password" << endl;
	cout << "  > group        [name] -addurs [username]      - add user in group" << endl;
	cout << "  > group        [name] -delurs [username]      - del user from group" << endl;
	cout << "  > help                                        - show help" << endl;
}

int 
wmain(int argc, wchar_t *argv[])
{
	setlocale(LC_ALL, "Russian");
	DLLFunctions::LoadWinapiFunctions();
	
	LSAProv LSAP;
	
	const wchar_t *Name		= (argc > 2) ? argv[2] : NULL;
	const wchar_t *Right	= (argc > 4) ? argv[4] : NULL;
	const wchar_t *Passw	= (argc > 4) ? argv[4] : NULL;
	const wchar_t *UsrName	= (argc > 4) ? argv[4] : NULL;
	const wchar_t *NewPassw = (argc > 5) ? argv[5] : NULL;

	if (argc < 2) goto ERROR_END;

	if (argc == 2)
	{
		if (!wcscmp(argv[1], L"help") || !wcscmp(argv[1], L"h"))
		{
			help();
		}
		else if (!wcscmp(argv[1], L"user"))
		{
			LSAP.PrintUserList();
		}
		else if (!wcscmp(argv[1], L"group"))
		{
			LSAP.PrintGroupList();
		}
		else goto ERROR_END;

		return 0;
	}


	if (argc == 3)
	{
		if (!wcscmp(argv[1], L"user"))
		{
			if (!wcscmp(argv[2], L"-all") || !wcscmp(argv[2], L"-a"))
			{
				LSAP.PrintAllUserPrivilege();
			}
			else
			{
				LSAP.PrintUserPrivilege(Name);
			}
		}
		else if (!wcscmp(argv[1], L"group"))
		{
			if (!wcscmp(argv[2], L"-all") || !wcscmp(argv[2], L"-a"))
			{
				LSAP.PrintAllGroupPrivilege();
			}
			else
			{
				LSAP.PrintGroupPrivilege(Name);
			}
		}
		else goto ERROR_END;
	}

	if (argc == 4)
	{
		if (!wcscmp(argv[1], L"user"))
		{
			if (!wcscmp(argv[3], L"-add"))
			{
				LSAP.AddUser(Name, L"*");
			}
			else if(!wcscmp(argv[3], L"-del"))
			{
				LSAP.DelUser(Name);
			}
			else goto ERROR_END;
		}
		else if (!wcscmp(argv[1], L"group"))
		{
			if (!wcscmp(argv[3], L"-add"))
			{
				LSAP.AddGroup(Name);
			}
			else if (!wcscmp(argv[3], L"-del"))
			{
				LSAP.DelGroup(Name);
			}
			else goto ERROR_END;
		}
		else goto ERROR_END;
	}

	if (argc == 5)
	{
		if (!wcscmp(argv[1], L"user"))
		{
			if (!wcscmp(argv[3], L"-add"))
			{
				LSAP.AddUser(Name, Passw);
			}
			else goto ERROR_END;
		}
		else if (!wcscmp(argv[1], L"group"))
		{
			if (!wcscmp(argv[3], L"-addusr"))
			{
				LSAP.AddGroupUser(Name, UsrName);
			}
			else if (!wcscmp(argv[3], L"-delusr"))
			{
				LSAP.DelGroupUser(Name, UsrName);
			}
			else goto ERROR_END;
		}
		else goto ERROR_END;
	}

	if (argc == 6)
	{
		if (!wcscmp(argv[1], L"user"))
		{
			if (!wcscmp(argv[3], L"-rgt"))
			{
				if (!wcscmp(argv[5], L"-add"))
				{
					LSAP.UserAddRight(Name, Right);
				}
				else if (!wcscmp(argv[5], L"-del"))
				{
					LSAP.UserDelRight(Name, Right);
				}
			}
			else if (!wcscmp(argv[3], L"-pwd"))
			{
				LSAP.ChangeUserPassword(Name, Passw, NewPassw);
			}
			else goto ERROR_END;
		}
		else if (!wcscmp(argv[1], L"group"))
		{
			if (!wcscmp(argv[3], L"-rgt"))
			{
				if (!wcscmp(argv[5], L"-add"))
				{
					LSAP.GroupAddRight(Name, Right);
				}
				else if (!wcscmp(argv[5], L"-del"))
				{
					LSAP.GroupDelRight(Name, Right);
				}
			}
			else goto ERROR_END;
		}
		else goto ERROR_END;
	}

	return 0;

ERROR_END:

	cout << "  incorrect command" << endl;

	return -1;
}

