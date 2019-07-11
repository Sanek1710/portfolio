#include "FsFilter.h"

PDRIVER_OBJECT deviceDriverObject;
extern FAST_IO_DISPATCH fastIoDispatchTable;

VOID FsFilterUnload(
	__in PDRIVER_OBJECT DriverObject
)
{
	ULONG numDevices = 0;
	LARGE_INTEGER interval;
	PDEVICE_OBJECT devList[DEVOBJ_LIST_SIZE];

	interval.QuadPart = (5 * DELAY_ONE_SECOND);
	IoUnregisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback);

	for (;;)
	{
		IoEnumerateDeviceObjectList(
			DriverObject,
			devList,
			sizeof(devList),
			&numDevices
		);
		if (0 == numDevices)
			break;

		numDevices = min(numDevices, RTL_NUMBER_OF(devList));
		for (ULONG i = 0; i < numDevices; ++i)
		{
			FsFilterDetachFromDevice(devList[i]);
			ObDereferenceObject(devList[i]);
		}
		KeDelayExecutionThread(KernelMode, FALSE, &interval);
	}
}

NTSTATUS DriverEntry(
	__inout PDRIVER_OBJECT  DriverObject,
	__in    PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(RegistryPath);

	deviceDriverObject = DriverObject;

	for (ULONG i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; ++i)
		DriverObject->MajorFunction[i] = FsFilterDispatchPassThrough;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = FsFilterDispatchCreate;
	DriverObject->FastIoDispatch = &fastIoDispatchTable;

	status = IoRegisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback);
	if (!NT_SUCCESS(status))
		return status;

	DriverObject->DriverUnload = FsFilterUnload;
	return STATUS_SUCCESS;
}

BOOLEAN FsFilterIsMyDeviceObject(
	__in PDEVICE_OBJECT DeviceObject
)
{
	return DeviceObject->DriverObject == deviceDriverObject;
}