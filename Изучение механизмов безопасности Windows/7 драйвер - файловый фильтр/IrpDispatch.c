#include "FsFilter.h"
#include <stdio.h>
#include <ntstrsafe.h>

NTSTATUS FsFilterDispatchPassThrough(
	__in PDEVICE_OBJECT DeviceObject,
	__in PIRP           Irp
)
{
	PFSFILTER_DEVICE_EXTENSION pDevExt = (PFSFILTER_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(pDevExt->AttachedToDeviceObject, Irp);
}

NTSTATUS FsFilterDispatchCreate(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	//WCHAR SecureDir[] = L"\\Users\\user\\Desktop\\files\\";
	//WCHAR PolicyTableFileStr[] = L"\\??\\C:\\Users\\user\\Desktop\\Driver\\config.txt";
	//WCHAR PidTableFileStr[] =    L"\\??\\C:\\Users\\user\\Desktop\\Driver\\config_pid.txt";
	//copy /Y $(OutDir)$(TargetName)$(TargetExt) \\win-r4nvf9u9d3i\SHARE\MyDriver4.sys
	WCHAR SecureDir[] = L"\\SEC\\";
	WCHAR PolicyTableFileStr[] = L"\\??\\C:\\SEC_conf\\policy.txt";
	WCHAR PidTableFileStr[] = L"\\??\\C:\\SEC_conf\\pid.txt";
	WCHAR *FullFileNameStr, wFileNameStr[20] = { 0 };
	UNICODE_STRING PolicyTableFile, PidTableFile;
	OBJECT_ATTRIBUTES PolicyAttr;
	HANDLE PolicyFile;
	IO_STATUS_BLOCK PolicyStatus;
	LARGE_INTEGER PolicyReadOffset = { 0 };
	CHAR PolicyBuffer[1024] = "", PidBuffer[1024] = "";
	ULONG i = 0, k = 0, PolicyLbl = 0, PidLbl = 0;
	ULONG PID;
	PFILE_OBJECT pFileObject = IoGetCurrentIrpStackLocation(Irp)->FileObject;
	ACCESS_MASK AccessMask;
	ULONG pid = 0, num = 0;
	UNICODE_STRING FileNameUnicode;
	ANSI_STRING FileNameAnsi;
	CHAR *FileNameStr;
	int ERR = 0;

	UNREFERENCED_PARAMETER(ERR);
	if (pFileObject == NULL) return FsFilterDispatchPassThrough(DeviceObject, Irp);
	if (pFileObject->FileName.Length == 0) return FsFilterDispatchPassThrough(DeviceObject, Irp);

	FullFileNameStr = pFileObject->FileName.Buffer;

	while (SecureDir[i] == FullFileNameStr[i]) i++;
	if (SecureDir[i] == '\0')
	{
		while (FullFileNameStr[i] != '\0')
		{
			wFileNameStr[k++] = FullFileNameStr[i++];
		}
		wFileNameStr[k] = '\0';
		DbgPrint("Nice directory\n");
		DbgPrint("FullFileName: %wZ\n", &pFileObject->FileName);
	}
	else
	{
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
	}

	PID = IoGetRequestorProcessId(Irp);

	DbgPrint("FileName: %s\n", wFileNameStr);
	DbgPrint("Pid: %u\n", PID);

	RtlUnicodeStringInit(&PolicyTableFile, PolicyTableFileStr);
	InitializeObjectAttributes(&PolicyAttr, &PolicyTableFile, 0ul, NULL, NULL);
	ZwOpenFile(&PolicyFile, GENERIC_READ, &PolicyAttr, &PolicyStatus, 0ul, 0ul);
	ZwReadFile(PolicyFile, NULL, NULL, NULL, &PolicyStatus, PolicyBuffer, 1023u, &PolicyReadOffset, NULL);
	ZwClose(PolicyFile);

	RtlUnicodeStringInit(&PidTableFile, PidTableFileStr);
	InitializeObjectAttributes(&PolicyAttr, &PidTableFile, 0ul, NULL, NULL);
	ZwOpenFile(&PolicyFile, GENERIC_READ, &PolicyAttr, &PolicyStatus, 0ul, 0ul);
	ZwReadFile(PolicyFile, NULL, NULL, NULL, &PolicyStatus, PidBuffer, 1023u, &PolicyReadOffset, NULL);
	ZwClose(PolicyFile);

	DbgPrint("PidBuffer: %s\n", PidBuffer);
	DbgPrint("PolicyBuffer: %s\n", PolicyBuffer);

	RtlUnicodeStringInit(&FileNameUnicode, wFileNameStr);
	RtlUnicodeStringToAnsiString(&FileNameAnsi, &FileNameUnicode, TRUE);
	FileNameStr = FileNameAnsi.Buffer;
	
	k = 0;
	while (PolicyBuffer[k] != '@')
	{
		i = 0;
		if (PolicyBuffer[k] == FileNameStr[i])
		{
			while (PolicyBuffer[k] == FileNameStr[i])
			{
				k++;
				i++;
			}
			if (PolicyBuffer[k] == ' ' && FileNameStr[i] == '\0')
			{
				k++;
				PolicyLbl = PolicyBuffer[k] - '0';
				DbgPrint("PolicyLbl: %d", PolicyLbl);

				break;
			}
			while (PolicyBuffer[k] != '\n')
			{
				k++;
			}
		}
		k++;
	}

	RtlFreeAnsiString(&FileNameAnsi);

	if (PolicyLbl == 0)
	{
		DbgPrint("There are no file in config: %wZ\n", &pFileObject->FileName);
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
	}

	k = 0;
	while (PidBuffer[k] != '@')
	{
		pid = 0;
		num = 0;
		while (PidBuffer[k] != ' ')
		{
			pid = pid * 10;
			num = PidBuffer[k] - '0';
			pid = pid + num;
			k++;
		}
		DbgPrint("pid: %d ", pid);

		if (pid == PID)
		{
			k++;
			PidLbl = PidBuffer[k] - '0';
			DbgPrint("PidLbl: %d", PidLbl);
		}
		while (PidBuffer[k] != '\n')
		{
			k++;
		}
		k++;
	}
	if (PidLbl == 0)
	{
		DbgPrint("There are no pid in config: %u\n", PID);
		return FsFilterDispatchPassThrough(DeviceObject, Irp);
	}
	
	AccessMask = IoGetCurrentIrpStackLocation(Irp)->Parameters.Create.SecurityContext->DesiredAccess;
	if (AccessMask & FILE_READ_DATA)
	{
		if (PidLbl < PolicyLbl)
		{
			Irp->IoStatus.Status = 0x5;
			IoCompleteRequest(Irp, IO_NO_INCREMENT);
			DbgPrint("Access denied\n");
			return 0x5;
		}
	}
	else if (AccessMask & FILE_WRITE_DATA)
	{
		if (PidLbl > PolicyLbl)
		{
			Irp->IoStatus.Status = 0x5;
			IoCompleteRequest(Irp, IO_NO_INCREMENT);
			DbgPrint("Access denied\n");
			return 0x5;
		}
	}

	return FsFilterDispatchPassThrough(DeviceObject, Irp);
}