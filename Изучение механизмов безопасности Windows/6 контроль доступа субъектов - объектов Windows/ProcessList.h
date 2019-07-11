#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <psapi.h>
#include <Aclapi.h>
#include <Sddl.h>

namespace ProcManeger
{
#ifdef _CPP_
	using namespace std;
#endif

	struct USER_INFO
	{
		CHAR Name[MAX_PATH];
		CHAR Domain[MAX_PATH];
		LPSTR SidStr;

	#ifdef _CPP_
		void print()
		{

		}
	#endif

	};

	struct PROC_POLICY_INFO
	{
		PROCESS_MITIGATION_ASLR_POLICY ASLR;
		PROCESS_MITIGATION_DEP_POLICY DEP;

	#ifdef _CPP_
		void print()
		{

		}
	#endif
	};

	struct PROC_PARENT_NAME
	{
		CHAR Name[MAX_PATH];

	#ifdef _CPP_
		void print()
		{
			cout << Name << endl;
		}
	#endif
	};

	enum ftype
	{
		fnofile,
		ffile,
		fdir,
		fkey
	};


	HANDLE GetProcessHandle(DWORD procId)
	{
		return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procId);
	}

	HANDLE GetProcessToken(HANDLE hProcess)
	{
		HANDLE hToken = NULL;

		if (hProcess == NULL) return NULL;

		if (!OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES | TOKEN_ADJUST_DEFAULT, &hToken))
		{
			return NULL;
		}

		return hToken;
	}

	bool GetProcPolicyInfo(HANDLE procHandle, PROC_POLICY_INFO& ProcPolicyInfo)
	{
		bool Result = false;

		Result = true;

		GetProcessMitigationPolicy(
			procHandle,
			ProcessASLRPolicy,
			(PVOID)&ProcPolicyInfo.ASLR,
			sizeof(ProcPolicyInfo.ASLR)
		);

		GetProcessMitigationPolicy(
			procHandle,
			ProcessDEPPolicy,
			(PVOID)&ProcPolicyInfo.DEP,
			sizeof(ProcPolicyInfo.DEP)
		);

		return Result;
	}

	int IntegrityLevelToIndex(DWORD IntegrityLevel)
	{
		if (IntegrityLevel == SECURITY_MANDATORY_LOW_RID)
		{
			return 0;
		}
		else if (IntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID &&
			IntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
		{
			// Medium Integrity
			return 1;
		}
		else if (IntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
		{
			// High Integrity
			return 2;
		}
		else if (IntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID)
		{
			// System Integrity
			return 3;
		}
		else return 3;
	}

	DWORD IndexToIntegrityLevel(int Index)
	{
		switch (Index)
		{
		case 0:  return SECURITY_MANDATORY_LOW_RID;
		case 1:  return SECURITY_MANDATORY_MEDIUM_RID;
		case 2:  return SECURITY_MANDATORY_HIGH_RID;
		case 3:  return SECURITY_MANDATORY_SYSTEM_RID;
		default: return SECURITY_MANDATORY_UNTRUSTED_RID;
		}
	}

	bool GetProcUsername(HANDLE procToken, USER_INFO& UserInfo)
	{
		bool Result = false;

		DWORD dwLength = TOKEN_USER_MAX_SIZE;
		PTOKEN_USER pTU = (PTOKEN_USER)malloc(TOKEN_USER_MAX_SIZE);

		if (GetTokenInformation(procToken, TokenUser, pTU, dwLength, &dwLength))
		{
			SID_NAME_USE SidType;
			DWORD dwSize = MAX_PATH;

			if (LookupAccountSidA(NULL, pTU->User.Sid, UserInfo.Name, &dwSize, UserInfo.Domain, &dwSize, &SidType))
			{
				ConvertSidToStringSidA(pTU->User.Sid, &UserInfo.SidStr);
				Result = true;
				printf("  - user: %s\\%s\n", UserInfo.Domain, UserInfo.Name);
			}
		}

		return Result;
	}

	bool GetProcessNameByPID(HANDLE procHandle, PROC_PARENT_NAME& ProcParentName)
	{
		bool Result = false;

		if (GetModuleFileNameExA(procHandle, 0, ProcParentName.Name, MAX_PATH))
		{
			Result = true;
		}

		return 0;
	}

	bool GetProcessParentId(DWORD procId, DWORD& ParentId)
	{
		ParentId = 0;

		HANDLE const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, procId);
		PROCESSENTRY32 peProcessEntry;

		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnapshot, &peProcessEntry);

		ParentId = peProcessEntry.th32ParentProcessID;

		CloseHandle(hSnapshot);

		return true;
	}

	bool GetProcessPrivelegeList(HANDLE procToken, PTOKEN_PRIVILEGES& pTP)
	{
		bool Result = false;
		DWORD dwLength = 0;

		GetTokenInformation(procToken, TokenPrivileges, NULL, dwLength, &dwLength);
		pTP = (PTOKEN_PRIVILEGES)malloc(dwLength);

		if (!GetTokenInformation(procToken, TokenPrivileges, pTP, dwLength, &dwLength))
		{
			free(pTP);
			pTP = NULL;
		}
		else
		{
			Result = true;
		}

		return Result;
	}

#define TOKEN_PRIVILEGES_SIZE(count)	(sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES)*((count) - 1))

	bool EnablePrivilege(HANDLE procToken, PCWSTR const ppPrivs)
	{
		bool Result = 0;
		TOKEN_PRIVILEGES Priv;

		try {
			Priv.PrivilegeCount = 1;
			Priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT;

			if (!LookupPrivilegeValue(NULL, ppPrivs, &Priv.Privileges[0].Luid)) throw 1;

			Result = (bool)AdjustTokenPrivileges(procToken, FALSE, &Priv, TOKEN_PRIVILEGES_SIZE(1), NULL, NULL);

			//LOGV(Result);
		}
		catch (...)
		{
			//cout << "Error " << endl;
		};

		return Result;
	};

	bool DisablePrivilege(HANDLE procToken, PCWSTR const ppPrivs)
	{
		bool Result = 0;
		TOKEN_PRIVILEGES Priv;

		try {
			Priv.PrivilegeCount = 1;
			Priv.Privileges[0].Attributes = 0;

			if (!LookupPrivilegeValue(NULL, ppPrivs, &Priv.Privileges[0].Luid)) throw 1;

			Result = (bool)AdjustTokenPrivileges(procToken, FALSE, &Priv, TOKEN_PRIVILEGES_SIZE(1), NULL, NULL);

			//LOGV(Result);
		}
		catch (...)
		{
			//cout << "Error " << endl;
		};

		return Result;
	};

	bool GetProcIntegrityLevel(HANDLE procToken, DWORD& IntegrityLevel)
	{
		bool Result = false;

		DWORD dwLength = TOKEN_INTEGRITY_LEVEL_MAX_SIZE;

		PTOKEN_MANDATORY_LABEL pTIL = (PTOKEN_MANDATORY_LABEL)malloc(TOKEN_INTEGRITY_LEVEL_MAX_SIZE);

		if (GetTokenInformation(procToken, TokenIntegrityLevel, pTIL, dwLength, &dwLength))
		{
			Result = true;
			DWORD nSubAuthority = (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1);
			IntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid, nSubAuthority);
			printf("  - IntL: %d\n", IntegrityLevel);
		}

		free(pTIL);

		return Result;
	}

	bool SetProcIntegrityLevel(HANDLE procToken, DWORD IntegrityLevel)
	{
		bool Result = false;

		DWORD dwLength = TOKEN_INTEGRITY_LEVEL_MAX_SIZE;

		PTOKEN_MANDATORY_LABEL pTIL = (PTOKEN_MANDATORY_LABEL)malloc(TOKEN_INTEGRITY_LEVEL_MAX_SIZE);

		SID_IDENTIFIER_AUTHORITY MAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;
		PSID pSid = NULL;
		AllocateAndInitializeSid(&MAuthority, 1, IntegrityLevel, 0, 0, 0, 0, 0, 0, 0, &pSid);
		TOKEN_MANDATORY_LABEL tml = { 0 };
		tml.Label.Sid = pSid;
		tml.Label.Attributes = SE_GROUP_INTEGRITY;

		if (SetTokenInformation(
			procToken,
			TokenIntegrityLevel,
			&tml,
			(sizeof(tml) + GetLengthSid(pSid))))
		{
			Result = true;
		}
		FreeSid(pSid);

		return Result;
	}

	//FILES

	bool GetObjectIntegrityLevel(LPCWSTR ObjPath, DWORD& IntegrityLevel)
	{
		bool Result = false;

		IntegrityLevel = SECURITY_MANDATORY_UNTRUSTED_RID;
		PSECURITY_DESCRIPTOR pSD = NULL;
		PACL acl = 0;
		GetNamedSecurityInfo(
			ObjPath,
			SE_FILE_OBJECT,
			LABEL_SECURITY_INFORMATION,
			0, 0, 0,
			&acl,
			&pSD);

		if (0 != acl && 0 < acl->AceCount)
		{
			SYSTEM_MANDATORY_LABEL_ACE* ace = 0;
			if (GetAce(acl, 0, reinterpret_cast<void**>(&ace)))
			{
				SID* sid = reinterpret_cast<SID*>(&ace->SidStart);
				IntegrityLevel = sid->SubAuthority[0];

				Result = true;
			}
		}

		PWSTR stringSD;
		ULONG stringSDLen = 0;

		ConvertSecurityDescriptorToStringSecurityDescriptor(
			pSD,
			SDDL_REVISION_1,
			LABEL_SECURITY_INFORMATION,
			&stringSD,
			&stringSDLen);
		if (pSD)
		{
			LocalFree(pSD);
		}

		return Result;

	}

	bool SetObjectIntegrityLevel(LPCWSTR ObjPath, DWORD IntegrityLevelIndex)
	{
		bool Result = false;

		LPCWSTR INTEGRITY_SDDL_SACL_W;

		if (IntegrityLevelIndex == 0)      INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;LW)";
		else if (IntegrityLevelIndex == 1) INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;ME)";
		else if (IntegrityLevelIndex == 2) INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;HI)";
		else return Result;

		DWORD dwErr = ERROR_SUCCESS;
		PSECURITY_DESCRIPTOR pSD = NULL;

		PACL pSacl = NULL;
		BOOL fSaclPresent = FALSE;
		BOOL fSaclDefaulted = FALSE;

		if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
			INTEGRITY_SDDL_SACL_W,
			SDDL_REVISION_1,
			&pSD,
			NULL))
		{
			if (GetSecurityDescriptorSacl(
				pSD,
				&fSaclPresent,
				&pSacl,
				&fSaclDefaulted))
			{
				dwErr = SetNamedSecurityInfo(
					(LPWSTR)ObjPath,
					SE_FILE_OBJECT,
					LABEL_SECURITY_INFORMATION,
					NULL,
					NULL,
					NULL,
					pSacl);

				if (dwErr == ERROR_SUCCESS)
				{
					Result = true;
					if (pSD)
					{
						LocalFree(pSD);
					}
					return Result;
				}
			}
		}
		if (pSD)
		{
			LocalFree(pSD);
		}
		return Result;
	}

	bool GetObjectOwnerName(LPCWSTR ObjPath, USER_INFO& Owner)
	{
		bool Result = false;

		PSID  psidOwner = NULL;
		PSID  psidGroup = NULL;
		PACL  pDacl = NULL;
		PACL  pSacl = NULL;
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;

		DWORD err = 0;
		err = GetNamedSecurityInfo(
			ObjPath,
			SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION,
			&psidOwner,
			&psidGroup,
			&pDacl,
			&pSacl,
			&pSecurityDescriptor
		);

		if (err == ERROR_SUCCESS)
		{
			ConvertSidToStringSidA(psidOwner, &Owner.SidStr);
			Result = true;
		}
		else return false;

		DWORD OwnerNameLen = MAX_PATH;
		DWORD DomainNameLen = MAX_PATH;
		SID_NAME_USE SidNameUse;

		if (!LookupAccountSidA(
			NULL,
			psidOwner,
			Owner.Name,
			&OwnerNameLen,
			Owner.Domain,
			&DomainNameLen,
			&SidNameUse
		))
		{
			Result = false;
		}

		return Result;
	}

	struct SECURITY_INFO
	{
		USER_INFO UI;
		ACCESS_MASK Mask;
		DWORD Type;
	};

	struct SECURITY_INFO_LIST
	{
		SECURITY_INFO *list;
		int count;
		int max_count;
		ftype ObjectType;

		void init()
		{
			count = 0;
			max_count = 16;
			list = (SECURITY_INFO*)malloc(max_count * sizeof(SECURITY_INFO));
		}

		void push(SECURITY_INFO SecureInfo)
		{
			list[count] = SecureInfo;
			count++;

			if (count == max_count)
			{
				max_count += 16;
				list = (SECURITY_INFO*)realloc(list, max_count * sizeof(SECURITY_INFO));
			}
		}

	#ifdef _CPP_
		void print()
		{
			for (int i = 0; i < count; i++)
			{
				cout << list[i].UI.Name << endl;
				cout << " - mask: " << hex << list[i].Mask << endl;
				cout << " - type: " << list[i].Type << endl;
			}
		}
	#endif // _CPP_

		void clear()
		{
			count = 0;
		}

		void free()
		{
			std::free(list);
		}
	};

	static SECURITY_INFO_LIST SecurityInfoList;

	bool GetObjectSecurityInfo(LPCWSTR path)
	{
		PACL Dacl = NULL;
		HKEY hKeyBranch;
		char *lost_path;
		char *str = (char *)malloc(2);

		FILE *test;
		DWORD err = 0;
		err = GetNamedSecurityInfo(path,
			SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION,
			NULL,
			NULL,
			&Dacl,
			NULL,
			NULL);

		if (err != ERROR_SUCCESS)
		{
			return false;
			SecurityInfoList.clear();
		}

		ACL_SIZE_INFORMATION asl;
		GetAclInformation(Dacl, &asl, sizeof(asl), AclSizeInformation);

		if (asl.AceCount == 0)
		{
			return false;
		}

		SECURITY_INFO SI;

		PSID pSID;
		SID_NAME_USE SidNameUse;

		DWORD UserNameLen = MAX_PATH;
		DWORD DomainNameLen = MAX_PATH;

		ACCESS_ALLOWED_ACE* pACE;
		for (int i = 0; i < asl.AceCount; i++)
		{
			GetAce(Dacl, i, (LPVOID *)&pACE);
			pSID = &(pACE->SidStart);

			UserNameLen = MAX_PATH;
			DomainNameLen = MAX_PATH;
			LookupAccountSidA(NULL, pSID, SI.UI.Name, &UserNameLen, SI.UI.Domain, &DomainNameLen, &SidNameUse);

			SI.Mask = pACE->Mask;
			SI.Type = ((ACE_HEADER*)pACE)->AceType;

			SecurityInfoList.push(SI);
		}

		return str;
	}


}
