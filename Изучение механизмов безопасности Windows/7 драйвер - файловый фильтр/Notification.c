#include "FsFilter.h"

NTSTATUS FsFilterAttachToFileSystemDevice(
    __in PDEVICE_OBJECT DeviceObject
);

VOID FsFilterDetachFromFileSystemDevice(
    __in PDEVICE_OBJECT DeviceObject
);

NTSTATUS FsFilterEnumerateFileSystemVolumes(
    __in PDEVICE_OBJECT DeviceObject
);

VOID FsFilterNotificationCallback(
    __in PDEVICE_OBJECT DeviceObject,
    __in BOOLEAN FsActive
)
{
	if (FsActive)
		FsFilterAttachToFileSystemDevice(DeviceObject);
    else
		FsFilterDetachFromFileSystemDevice(DeviceObject);
}

NTSTATUS FsFilterAttachToFileSystemDevice(
    __in PDEVICE_OBJECT DeviceObject
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PDEVICE_OBJECT filterDeviceObject = NULL;

    if (!FsFilterIsAttachedToDevice(DeviceObject))
    {
        status = FsFilterAttachToDevice(DeviceObject, &filterDeviceObject);
        if (!NT_SUCCESS(status))
			return status;

        status = FsFilterEnumerateFileSystemVolumes(DeviceObject);
        if (!NT_SUCCESS(status)) 
        {
            FsFilterDetachFromDevice(filterDeviceObject);
            return status;
        }
    }   

    return STATUS_SUCCESS;
}

VOID FsFilterDetachFromFileSystemDevice(
    __in PDEVICE_OBJECT DeviceObject
)
{
    PDEVICE_OBJECT device = NULL;

    for (device = DeviceObject->AttachedDevice; NULL != device; device = device->AttachedDevice) 
    {
        if (FsFilterIsMyDeviceObject(device)) 
        {
			FsFilterDetachFromDevice(device);
            break;
        }
    }
}

NTSTATUS FsFilterEnumerateFileSystemVolumes(
    __in PDEVICE_OBJECT DeviceObject
)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG numDevices = 0;
    ULONG i = 0;
    PDEVICE_OBJECT devList[DEVOBJ_LIST_SIZE];

    status = IoEnumerateDeviceObjectList(
        DeviceObject->DriverObject,
        devList,
        sizeof(devList),
        &numDevices
	);

    if (!NT_SUCCESS(status))
		return status;

    numDevices = min(numDevices, RTL_NUMBER_OF(devList));

    for (i = 0; i < numDevices; ++i) 
    {
        if (devList[i] != DeviceObject
			&& devList[i]->DeviceType == DeviceObject->DeviceType
			&& !FsFilterIsAttachedToDevice(devList[i]))
			status = FsFilterAttachToDevice(devList[i], NULL);
        ObDereferenceObject(devList[i]);
    }

    return STATUS_SUCCESS;
}