#include "FsFilter.h"

NTSTATUS FsFilterAttachToDevice(
    __in PDEVICE_OBJECT DeviceObject,
    __out_opt PDEVICE_OBJECT* pFilterDeviceObject
 )
{
    NTSTATUS status = STATUS_SUCCESS;
    PDEVICE_OBJECT filterDeviceObject = NULL;
    PFSFILTER_DEVICE_EXTENSION pDevExt = NULL;

    status = IoCreateDevice(
        deviceDriverObject,
        sizeof(FSFILTER_DEVICE_EXTENSION),
        NULL,
        DeviceObject->DeviceType,
        0,
        FALSE,
        &filterDeviceObject
	);
    if (!NT_SUCCESS(status))
		return status;

    pDevExt = (PFSFILTER_DEVICE_EXTENSION)filterDeviceObject->DeviceExtension;
    if (FlagOn(DeviceObject->Flags, DO_BUFFERED_IO))
		SetFlag(filterDeviceObject->Flags, DO_BUFFERED_IO);
    if (FlagOn(DeviceObject->Flags, DO_DIRECT_IO))
		SetFlag(filterDeviceObject->Flags, DO_DIRECT_IO);
    if (FlagOn(DeviceObject->Characteristics, FILE_DEVICE_SECURE_OPEN))
		SetFlag(filterDeviceObject->Characteristics, FILE_DEVICE_SECURE_OPEN);

    for (ULONG i = 0; i < 8; ++i)
    {
        LARGE_INTEGER interval;

        status = IoAttachDeviceToDeviceStackSafe(
            filterDeviceObject,
            DeviceObject,
            &pDevExt->AttachedToDeviceObject
		);
        if (NT_SUCCESS(status))
			break;

        interval.QuadPart = (500 * DELAY_ONE_MILLISECOND);
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }

    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(filterDeviceObject);
        filterDeviceObject = NULL;
    }
    else
    {
        ClearFlag(filterDeviceObject->Flags, DO_DEVICE_INITIALIZING);
        if (NULL != pFilterDeviceObject)
			*pFilterDeviceObject = filterDeviceObject;
    }

    return status;
}

void FsFilterDetachFromDevice(
    __in PDEVICE_OBJECT DeviceObject
 )
{    
    PFSFILTER_DEVICE_EXTENSION pDevExt = (PFSFILTER_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
    
    IoDetachDevice(pDevExt->AttachedToDeviceObject);
    IoDeleteDevice(DeviceObject);
}

BOOLEAN FsFilterIsAttachedToDevice(
    __in PDEVICE_OBJECT DeviceObject
)
{
    PDEVICE_OBJECT nextDevObj = NULL;
    PDEVICE_OBJECT currentDevObj = IoGetAttachedDeviceReference(DeviceObject);

    do 
    {
        if (FsFilterIsMyDeviceObject(currentDevObj)) 
        {
            ObDereferenceObject(currentDevObj);
            return TRUE;
        }

        nextDevObj = IoGetLowerDeviceObject(currentDevObj);
        ObDereferenceObject(currentDevObj);
        currentDevObj = nextDevObj;
    } while (NULL != currentDevObj);

    return FALSE;
}