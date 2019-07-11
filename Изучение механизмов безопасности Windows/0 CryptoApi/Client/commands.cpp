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
	char *str;
	int a = 2, b = 0;

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	const DWORDLONG dwlConditionMask =
		VerSetConditionMask(VerSetConditionMask(VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
			VER_MINORVERSION, VER_GREATER_EQUAL),
			VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

	VerifyVersionInfoW((LPOSVERSIONINFOEXW)&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask);

	str = (char*)malloc(1 * sizeof(char));

	switch ((a << 8) + b)
	{
	case (4 << 8) + 0:	*str = 95;	break;
	case (4 << 8) + 10:	*str = 98;	break;
	case (4 << 8) + 90:	*str = 'M';	break;
	case (5 << 8) + 0:	*str = 200;	break;
	case (5 << 8) + 1:	*str = 'X';	break;
	case (5 << 8) + 2:	*str = 203;	break;
	case (6 << 8) + 0:	*str = 'V';	break;
	case (6 << 8) + 1:	*str = 7;	break;
	case (6 << 8) + 2:	*str = 8;	break;
	case (6 << 8) + 3:	*str = 81;	break;
	default:			*str = 10;	break;
	}
	if (len) *len = 1;

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

char * //return string
Get_secure_info(int *len)
{
	char *str;
	PSECURITY_DESCRIPTOR pSD;
	PACL a;
	char path[14] = "C:\\T2Exe.log";
	GetNamedSecurityInfoA(path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &a, NULL, &pSD);

	ACCESS_ALLOWED_ACE *pACE;
	PSID pSID;

	//GetAclInformation(a, NULL, );
	GetAce(a, 0, (LPVOID *)&pACE);

	pSID = (PSID)&(pACE->SidStart);


	return NULL;
}


const char *
check_mask(ACCESS_MASK mask)
{
	switch (mask)
	{
	case 2032127:	return "GENERIC_ALL";
	case 1179808:	return "GENERIC_EXECUTE";
	case 1179785:	return "GENERIC_READ";
	case 1179926:	return "GENERIC_WRITE";
	case DELETE:	return "DELETE";
	case READ_CONTROL:	return "READ_CONTROL";
	case SYNCHRONIZE:	return "SYNCHRONIZE";
	case WRITE_DAC:	return "WRITE_DAC";
	case WRITE_OWNER:	return "WRITE_OWNER";
	default:	return "EMPTY";
		break;
	}
}

char *
take_file_prior(char *path)
{
	char send_buf[1];
	PSECURITY_DESCRIPTOR pSD;
	PACL Dacl;

	GetNamedSecurityInfoA(path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &Dacl, NULL, &pSD);

	ACL_SIZE_INFORMATION asi;
	GetAclInformation(Dacl, &asi, sizeof(asi), AclSizeInformation);

	void *Ace = NULL;
	PSID Sid;
	ACCESS_MASK Mask;
	DWORD Acc_name_len = 0, Dom_name_len = 0;
	SID_NAME_USE account_type;
	CHAR *Acc_name = NULL, *Dom_name = NULL;

	for (int i = 0; i < Dacl->AceCount; i++)
	{
		GetAce(Dacl, i, &Ace);

		Sid = &((ACCESS_ALLOWED_ACE *)Ace)->SidStart;
		Mask = ((ACCESS_ALLOWED_ACE *)Ace)->Mask;

		if (!LookupAccountSidA(NULL, Sid, NULL, &Acc_name_len, NULL, &Dom_name_len, &account_type))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				Acc_name = (LPSTR)realloc(Acc_name, Acc_name_len);
				Dom_name = (LPSTR)realloc(Dom_name, Dom_name_len);
			}
			else
			{
				printf("error1");
				exit(0);
			}
		}

		if (!LookupAccountSidA(NULL, Sid, Acc_name, &Acc_name_len, Dom_name, &Dom_name_len, &account_type))
		{
			printf("error2");
			exit(0);
		}

		printf("%-19s : ", Acc_name);
		printf("%-12s : ", Dom_name);
		printf("%-11s : ", check_mask(Mask));
		printf("%s\n", (((ACE_HEADER*)Ace)->AceType) ? " Denied" : "Allowed");


		free(Acc_name);
		free(Dom_name);
		Acc_name_len = Dom_name_len = 0;
		Acc_name = Dom_name = NULL;
	}

	exit(0);
	//ACCESS_ALLOWED_ACE* pACE;
	char acc_name[256];
	void *pAce;
	int i;
	for (i = 0; i < asi.AceCount; i++)
	{
		GetAce(Dacl, i, &pAce);
		char domain_name[256];
		PSID pSID;
		SID_NAME_USE peUse;
		DWORD size_domain_name = sizeof(domain_name);
		DWORD size_acc_name = sizeof(acc_name);
		pSID = (PSID)(&(((ACCESS_ALLOWED_ACE*)pAce)->SidStart));

		LookupAccountSidA(NULL, pSID, acc_name, &size_acc_name, domain_name, &size_domain_name, &peUse);

		printf("%-20s : %X\n", acc_name, ((ACCESS_ALLOWED_ACE*)pAce)->Mask);
	}


	return send_buf;
}

void //return
parse_OS_vers(char *str, int len)
{
	printf("OS Version: %s\n", str);
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
	printf("\tMemory Load : %d %%\n", str[1]);
	printf("\tPhys        : %u/%u Mb\n", ((MTYPE32 *)&str[2])[1].ui >> 20, ((MTYPE32 *)&str[2])[0].ui >> 20);
	printf("\tPage File   : %u/%u Mb\n", ((MTYPE32 *)&str[2])[3].ui >> 20, ((MTYPE32 *)&str[2])[2].ui >> 20);
	printf("\tVirtual     : %u/%u Mb\n", ((MTYPE32 *)&str[2])[5].ui >> 20, ((MTYPE32 *)&str[2])[4].ui >> 20);

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
		("\t%c: %lf Gb\n",
			str[9 * i],
			((MTYPE64 *)&str[9 * i + 1])->d
		);
	}

}

enum ftype
{
	fnofile,
	ffile,
	fdir,
	fkey
};

void
print_mask(int mask, enum ftype ft)
{
	const char *prefix = "\t\t";
	int cmsk[4];

	if (ft == fkey)
	{
		if (cmsk[0] = ((mask & KEY_ALL_ACCESS) == KEY_ALL_ACCESS)) printf("%sKEY_ALL_ACCESS\n", prefix);
		if (cmsk[0]) mask &= (~KEY_ALL_ACCESS);
		if (cmsk[1] = ((mask & KEY_READ) == KEY_READ)) printf("%sKEY_READ\n", prefix);
		if (cmsk[2] = ((mask & KEY_WRITE) == KEY_WRITE)) printf("%sKEY_WRITE\n", prefix);
		if (cmsk[3] = ((mask & KEY_EXECUTE) == KEY_EXECUTE)) printf("%sKEY_EXECUTE\n", prefix);

		if (cmsk[1]) mask &= (~KEY_READ);
		if (cmsk[2]) mask &= (~KEY_WRITE);
		if (cmsk[3]) mask &= (~KEY_EXECUTE);

		if (cmsk[0] = ((mask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)) printf("%sSTANDARD_RIGHTS_REQUIRED\n", prefix);
		if (cmsk[1] = ((mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)) printf("%sSTANDARD_RIGHTS_ALL\n", prefix);
		if (cmsk[2] = ((mask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)) printf("%sSPECIFIC_RIGHTS_ALL\n", prefix);

		if (cmsk[0]) mask &= (~STANDARD_RIGHTS_REQUIRED);
		if (cmsk[1]) mask &= (~STANDARD_RIGHTS_ALL);
		if (cmsk[2]) mask &= (~SPECIFIC_RIGHTS_ALL);

		if ((mask & GENERIC_READ) == GENERIC_READ) printf("%sGENERIC_READ\n", prefix);
		if ((mask & GENERIC_WRITE) == GENERIC_WRITE) printf("%sGENERIC_WRITE\n", prefix);
		if ((mask & GENERIC_EXECUTE) == GENERIC_EXECUTE) printf("%sGENERIC_EXECUTE\n", prefix);
		if ((mask & GENERIC_ALL) == GENERIC_ALL) printf("%sGENERIC_ALL\n", prefix);
		if ((mask & DELETE) == DELETE) printf("%sDELETE\n", prefix);
		if ((mask & READ_CONTROL) == READ_CONTROL) printf("%sREAD_CONTROL\n", prefix);
		if ((mask & WRITE_DAC) == WRITE_DAC) printf("%sWRITE_DAC\n", prefix);
		if ((mask & WRITE_OWNER) == WRITE_OWNER) printf("%sWRITE_OWNER\n", prefix);
		if ((mask & SYNCHRONIZE) == SYNCHRONIZE) printf("%sSYNCHRONIZE\n", prefix);

		if ((mask & KEY_QUERY_VALUE) == KEY_QUERY_VALUE) printf("%sKEY_QUERY_VALUE\n", prefix);
		if ((mask & KEY_SET_VALUE) == KEY_SET_VALUE) printf("%sKEY_SET_VALUE\n", prefix);
		if ((mask & KEY_CREATE_SUB_KEY) == KEY_CREATE_SUB_KEY) printf("%sKEY_CREATE_SUB_KEY\n", prefix);
		if ((mask & KEY_ENUMERATE_SUB_KEYS) == KEY_ENUMERATE_SUB_KEYS) printf("%sKEY_ENUMERATE_SUB_KEYS\n", prefix);
		if ((mask & KEY_NOTIFY) == KEY_NOTIFY) printf("%sKEY_NOTIFY\n", prefix);
		if ((mask & KEY_CREATE_LINK) == KEY_CREATE_LINK) printf("%sKEY_CREATE_LINK\n", prefix);
		if ((mask & KEY_WOW64_32KEY) == KEY_WOW64_32KEY) printf("%sKEY_WOW64_32KEY\n", prefix);
		if ((mask & KEY_WOW64_64KEY) == KEY_WOW64_64KEY) printf("%sKEY_WOW64_64KEY\n", prefix);
		if ((mask & KEY_WOW64_RES) == KEY_WOW64_RES) printf("%sKEY_WOW64_RES\n", prefix);
	}
	else
	{
		if (cmsk[0] = ((mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)) printf("%sFILE_ALL_ACCESS\n", prefix);
		if (cmsk[0]) mask &= (~FILE_ALL_ACCESS);
		if (cmsk[1] = ((mask & FILE_GENERIC_READ) == FILE_GENERIC_READ)) printf("%sFILE_GENERIC_READ\n", prefix);
		if (cmsk[2] = ((mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE)) printf("%sFILE_GENERIC_WRITE\n", prefix);
		if (cmsk[3] = ((mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)) printf("%sFILE_GENERIC_EXECUTE\n", prefix);

		if (cmsk[1]) mask &= (~FILE_GENERIC_READ);
		if (cmsk[2]) mask &= (~FILE_GENERIC_WRITE);
		if (cmsk[3]) mask &= (~FILE_GENERIC_EXECUTE);

		if (cmsk[0] = ((mask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)) printf("%sSTANDARD_RIGHTS_REQUIRED\n", prefix);
		if (cmsk[1] = ((mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)) printf("%sSTANDARD_RIGHTS_ALL\n", prefix);
		if (cmsk[2] = ((mask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)) printf("%sSPECIFIC_RIGHTS_ALL\n", prefix);

		if (cmsk[0]) mask &= (~STANDARD_RIGHTS_REQUIRED);
		if (cmsk[1]) mask &= (~STANDARD_RIGHTS_ALL);
		if (cmsk[2]) mask &= (~SPECIFIC_RIGHTS_ALL);

		if ((mask & DELETE) == DELETE) printf("%sDELETE\n", prefix);
		if ((mask & READ_CONTROL) == READ_CONTROL) printf("%sREAD_CONTROL\n", prefix);
		if ((mask & WRITE_DAC) == WRITE_DAC) printf("%sWRITE_DAC\n", prefix);
		if ((mask & WRITE_OWNER) == WRITE_OWNER) printf("%sWRITE_OWNER\n", prefix);
		if ((mask & SYNCHRONIZE) == SYNCHRONIZE) printf("%sSYNCHRONIZE\n", prefix);

		//for files
		if (ft == ffile)
		{
			if ((mask & FILE_READ_DATA) == FILE_READ_DATA)	printf("%sFILE_READ_DATA\n", prefix);
			if ((mask & FILE_WRITE_DATA) == FILE_WRITE_DATA)	printf("%sFILE_WRITE_DATA\n", prefix);
			if ((mask & FILE_APPEND_DATA) == FILE_APPEND_DATA)	printf("%sFILE_APPEND_DATA\n", prefix);
			if ((mask & FILE_READ_EA) == FILE_READ_EA)	printf("%sFILE_READ_EA\n", prefix);
			if ((mask & FILE_WRITE_EA) == FILE_WRITE_EA)	printf("%sFILE_WRITE_EA\n", prefix);
			if ((mask & FILE_EXECUTE) == FILE_EXECUTE)	printf("%sFILE_EXECUTE\n", prefix);
			if ((mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES)	printf("%sFILE_READ_ATTRIBUTES\n", prefix);
			if ((mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES)	printf("%sFILE_WRITE_ATTRIBUTES\n", prefix);
			//for directories 
		}
		else
		{
			if ((mask & FILE_LIST_DIRECTORY) == FILE_LIST_DIRECTORY)	printf("%sFILE_LIST_DIRECTORY\n", prefix);
			if ((mask & FILE_ADD_FILE) == FILE_ADD_FILE)	printf("%sFILE_ADD_FILE\n", prefix);
			if ((mask & FILE_ADD_SUBDIRECTORY) == FILE_ADD_SUBDIRECTORY)	printf("%sFILE_ADD_SUBDIRECTORY\n", prefix);
			if ((mask & FILE_READ_EA) == FILE_READ_EA)	printf("%sFILE_READ_EA\n", prefix);
			if ((mask & FILE_WRITE_EA) == FILE_WRITE_EA)	printf("%sFILE_WRITE_EA\n", prefix);
			if ((mask & FILE_TRAVERSE) == FILE_TRAVERSE)	printf("%sFILE_TRAVERSE\n", prefix);
			if ((mask & FILE_DELETE_CHILD) == FILE_DELETE_CHILD)	printf("%sFILE_DELETE_CHILD\n", prefix);
			if ((mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES)	printf("%sFILE_READ_ATTRIBUTES\n", prefix);
			if ((mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES)	printf("%sFILE_WRITE_ATTRIBUTES\n", prefix);
		}

		if (mask & 0xFFE0FE00) printf("%sFILE_OTHER\n", prefix);
	}

}

void //return
parse_secure_info(char *str, int len)
{
	enum ftype ft = (enum ftype)str[0];
	int mask;

	if (ft == fnofile)
	{
		printf("invalid path\n");
		return;
	}

	int rule_cnt = str[1];
	int cur_len = 2;

	printf("%s :\n", (ft == ffile) ? "File" : ((ft == fdir) ? "Directory" : "Registry key"));

	for (int i = 0; i < rule_cnt; i++)
	{
		printf("\t%-20s : ", &str[cur_len]);
		cur_len += strlen(&str[cur_len]) + 1;
		mask = *((int*)(&str[cur_len]));
		cur_len += sizeof(int);
		printf("0x%08X : %s\n", mask, str[cur_len] ? " Denied" : "Allowed");
		cur_len++;
		print_mask(mask, ft);
		printf("\n");
	}
}


void
parse_command(char *str, int len, int cmd_num)
{
	switch (cmd_num)
	{
	case INFO_OS_VERS:	return parse_OS_vers		(str, len);
	case INFO_TIME:		return parse_time			(str, len);
	case INFO_OS_TIME:	return parse_OS_time		(str, len);
	case INFO_MEM_STAT:	return parse_mem_stat		(str, len);
	case INFO_DISK_MEM:	return parse_disk_mem_info	(str, len);
	case INFO_SECURE:	return parse_secure_info	(str, len);
	case INFO_HELP:
	default: return;
	}
}

char *
pack_send_str(char *send_str, char *src, int src_len, int *to_send_len)
{
	*((int *)send_str) = src_len;
	memcpy(&send_str[4], src, src_len);
	*to_send_len = src_len + 4;
	return send_str;
}

