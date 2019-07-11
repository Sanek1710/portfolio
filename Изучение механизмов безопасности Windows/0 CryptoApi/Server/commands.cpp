#define _CRT_SECURE_NO_WARNINGS

#include "commands.h"

using namespace typetotype;

void
Error(const char *error)
{
	printf("ERROR: %s\n", error);
	//	exit(0);
}

char *
Get_OS_vers(int *len)
{
	HKEY hkey;
	char *str;
	DWORD bufsz = 0;
	DWORD err;

	err = RegOpenKeyExA(
		HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0, KEY_READ, &hkey
	);

	if (err != ERROR_SUCCESS) return NULL;

	err = RegGetValueA(
		hkey, NULL, "ProductName",
		RRF_RT_REG_SZ, 0, NULL, &bufsz
	);

	str = (char*)malloc(bufsz);

	err = RegGetValueA(
		hkey, NULL, "ProductName",
		RRF_RT_REG_SZ, 0, str, &bufsz
	);

	if (err != ERROR_SUCCESS) return NULL;

	if (len) *len = bufsz;

	return str;
}

char * //return string
Get_time(int *len)
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	char *str;

	str = (char*)malloc(8 * sizeof(char));

	((MTYPE64*)str)->s[0] = st.wYear;
	((MTYPE64*)str)->s[1] = st.wDay;
	str[4] = (char)st.wMonth;
	str[5] = (char)st.wHour;
	str[6] = (char)st.wMinute;
	str[7] = (char)st.wSecond;

	if (len) *len = 8;

	return str;
}

char * //return string
Get_OS_time(int *len)
{
	int msec = GetTickCount();
	char *str;

	str = (char*)malloc(6 * sizeof(char));

	((MTYPE64*)str)->i[0] = msec / (1000 * 60 * 60);
	str[4] = msec / (1000 * 60) - ((MTYPE64*)str)->i[0] * 60;
	str[5] = msec / (1000) - ((MTYPE64*)str)->i[0] * 60 * 60 - str[4] * 60;

	if (len) *len = 6;

	return str;
}

char * //return string
Get_mem_stat(int *len)
{
	MEMORYSTATUS stat;
	GlobalMemoryStatus(&stat);
	char *str;

	str = (char*)malloc(26 * sizeof(char));

	str[0] = (char)stat.dwLength;
	str[1] = (char)stat.dwMemoryLoad;
	((MTYPE32 *)&str[2])[0].i = stat.dwTotalPhys;
	((MTYPE32 *)&str[2])[1].i = stat.dwAvailPhys;
	((MTYPE32 *)&str[2])[2].i = stat.dwTotalPageFile;
	((MTYPE32 *)&str[2])[3].i = stat.dwAvailPageFile;
	((MTYPE32 *)&str[2])[4].i = stat.dwTotalVirtual;
	((MTYPE32 *)&str[2])[5].i = stat.dwAvailVirtual;

	if (len) *len = 26;

	return str;
}

char * //return string
Get_disk_mem_info(int *len)
{
	char *str = NULL;

	char disk[3] = { ' ', ':', '\0' };
	DWORD drives = GetLogicalDrives();

	int dr_fixed_cnt = 0;
	for (int i = 0; i < 26; i++)
	{
		if ((drives >> i) & (DWORD)1)
		{
			disk[0] = (char)('A' + i);
			if (GetDriveTypeA(disk) == DRIVE_FIXED)
			{
				DWORD s, b, f, c;
				GetDiskFreeSpaceA(disk, &s, &b, &f, &c);
				double free_space = (double)s * (double)b *(double)f / 1024. / 1024. / 1024.;
				str = (char*)realloc(str, 9 * (dr_fixed_cnt + 1) * sizeof(char));

				str[9 * dr_fixed_cnt] = disk[0];
				((MTYPE64 *)&str[9 * dr_fixed_cnt + 1])->d = free_space;

				dr_fixed_cnt++;
			}
		}
	}

	if (*len) *len = 9 * dr_fixed_cnt;

	return str;
}

enum ftype
{
	fnofile,
	ffile,
	fdir,
	fkey
};

char *
check_reg(char *path, HKEY *hKeyBranch)
{
	const char *Branches[6] =
	{
		"HKEY_CLASSES_ROOT", "HKEY_CURRENT_USER", "HKEY_LOCAL_MACHINE", "HKEY_USERS", "HKEY_CURRENT_CONFIG", "HKEY_DYN_DATA"
	};
	HKEY hKeyBranches[6] =
	{
		HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS, HKEY_CURRENT_CONFIG, HKEY_DYN_DATA
	};

	for (int i = 0; i < 6; i++)
	{
		if (!strncmp(path, Branches[i], strlen(Branches[i])))
		{
			*hKeyBranch = hKeyBranches[i];
			char *lost = path + strlen(Branches[i]);
			if (*lost == '\\') lost++;
			return lost;
		}
	}
	return NULL;
}

char * //return string
Get_secure_info(char *path, int *len)
{
	//PSECURITY_DESCRIPTOR pSD;
	enum ftype ft;
	PACL Dacl = NULL;
	HKEY hKeyBranch;
	char *lost_path;
	if (len) *len = 2;
	char *str = (char *)malloc(2);


	if (lost_path = check_reg(path, &hKeyBranch))
	{
		HKEY hKey;
		RegOpenKeyA(hKeyBranch, lost_path, &hKey);
		if (!hKey)
		{
			str[0] = fnofile;
			return str;
		}
		GetSecurityInfo(hKey, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, &Dacl, NULL, NULL);
		str[0] = fkey;
	}
	else
	{
		FILE *test;
		if (test = fopen(path, "rb"))
		{
			str[0] = ffile;
			fclose(test);
		}
		else str[0] = fdir;

		GetNamedSecurityInfoA(path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &Dacl, NULL, NULL);
	}
	if (!Dacl)
	{
		str[0] = fnofile;
		return str;
	}
	ft = (enum ftype)str[0];
	////printf("%s : %s\n", path, (ft == ffile) ? "File" : ((ft == fdir) ? "Dir" : "Key"));

	ACL_SIZE_INFORMATION asl;
	GetAclInformation(Dacl, &asl, sizeof(asl), AclSizeInformation);

	ACCESS_ALLOWED_ACE* pACE;
	PSID pSID;
	SID_NAME_USE SNU;
	char *Acc_name = NULL;
	char *Dom_name = NULL;
	DWORD size_Acc_name = 0;
	DWORD size_Dom_name = 0;

	str[1] = asl.AceCount;
	int cur_len = 2;
	for (int i = 0; i < asl.AceCount; i++)
	{
		GetAce(Dacl, i, (LPVOID *)&pACE);
		pSID = &(pACE->SidStart);

		size_Acc_name = 0;
		size_Dom_name = 0;
		LookupAccountSidA(NULL, pSID, NULL, &size_Acc_name, NULL, &size_Dom_name, &SNU);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			str[1] = asl.AceCount = i;
			break;
		}

		Acc_name = (char*)malloc(size_Acc_name);
		Dom_name = (char*)malloc(size_Dom_name);
		LookupAccountSidA(NULL, pSID, Acc_name, &size_Acc_name, Dom_name, &size_Dom_name, &SNU);

		str = (char*)realloc(str, cur_len + strlen(Acc_name) + 1 + sizeof(pACE->Mask) + 1);
		strcpy(&str[cur_len], Acc_name);
		cur_len += strlen(Acc_name) + 1;
		*(int *)(&str[cur_len]) = pACE->Mask;
		cur_len += sizeof(pACE->Mask);
		str[cur_len] = ((ACE_HEADER*)pACE)->AceType;
		cur_len++;

		free(Acc_name);
		free(Dom_name);
		Acc_name = NULL;
		Dom_name = NULL;
	}
	if (len) *len = cur_len;
	return str;
}



void //return
parse_OS_vers(char *str, int len)
{
	char str_vers[6];

	switch (*str)
	{
	case 95:	strcpy(str_vers, "95");		break;
	case 98:	strcpy(str_vers, "98");		break;
	case 'M':	strcpy(str_vers, "Me");		break;
	case 200:	strcpy(str_vers, "2000");	break;
	case 'X':	strcpy(str_vers, "XP");		break;
	case 203:	strcpy(str_vers, "2003");	break;
	case 'V':	strcpy(str_vers, "Vista");	break;
	case 7:	strcpy(str_vers, "7");		break;
	case 8:	strcpy(str_vers, "8");		break;
	case 81:	strcpy(str_vers, "8.1");	break;
	case 10:	strcpy(str_vers, "10");		break;
	default:	printf("I don't know the OS Version :(\n");  return;
	}

	printf("OS Version: Windows %s\n", str_vers);
}

void //return
parse_time(char *str, int len)
{
	printf("Real time: ");
	printf
	("%02d.%02d.%04d %02d:%02d:%02d\n",
		((MTYPE64*)str)->s[1],
		str[4],
		((MTYPE64*)str)->s[0],
		str[5] + 3,
		str[6],
		str[7]
	);
}

void //return
parse_OS_time(char *str, int len)
{
	printf("OS runtime: ");
	printf
	("%3d:%02d:%02d\n",
		((MTYPE64*)str)->i[0],
		str[4],
		str[5]
	);
}

void //return
parse_mem_stat(char *str, int len)
{
	//printf("dwLength        : %d %%\n", str[0]);
	printf("Memory stat info:\n");
	printf(">\tdwMemoryLoad    : %d %%\n", str[1]);
	printf(">\tdwTotalPhys     : %lf\n", ((double)((MTYPE32 *)&str[2])[0].i) / 1024. / 1024. / 1024.);
	printf(">\tdwAvailPhys     : %lf\n", ((double)((MTYPE32 *)&str[2])[1].i) / 1024. / 1024. / 1024.);
	printf(">\tdwTotalPageFile : %lf\n", ((double)((MTYPE32 *)&str[2])[2].i) / 1024. / 1024. / 1024.);
	printf(">\tdwAvailPageFile : %lf\n", ((double)((MTYPE32 *)&str[2])[3].i) / 1024. / 1024. / 1024.);
	printf(">\tdwTotalVirtual  : %lf\n", ((double)((MTYPE32 *)&str[2])[4].i) / 1024. / 1024. / 1024.);
	printf(">\tdwAvailVirtual  : %lf\n", ((double)((MTYPE32 *)&str[2])[5].i) / 1024. / 1024. / 1024.);

}

void //return
parse_disk_mem_info(char *str, int len)
{
	LPDWORD;
	int dr_fixed_cnt = len / 9;

	printf("Disks memory info:\n");

	for (int i = 0; i < dr_fixed_cnt; i++)
	{
		printf
		(">\t%c: %lf Gb\n",
			str[9 * i],
			((MTYPE64 *)&str[9 * i + 1])->d
		);
	}

}

void //return
parse_secure_info(char *str, int len)
{

}
