// superdel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ntddk.h>

#define NT_DEVICE_NAME              L"\\Device\\360SuperKill"
#define DOS_DEVICE_NAME            L"\\DosDevices\\360SuperKill"

NTSTATUS NTAPI VfatBuildRequest (PDEVICE_OBJECT DeviceObject,
	PIRP Irp);
VOID
	SKillUnloadDriver(
	IN PDRIVER_OBJECT  DriverObject
	)
{
	PDEVICE_OBJECT  deviceObject = DriverObject->DeviceObject;
	UNICODE_STRING  uniSymLink;

	RtlInitUnicodeString(&uniSymLink, DOS_DEVICE_NAME);

	IoDeleteSymbolicLink(&uniSymLink);

	IoDeleteDevice(deviceObject);
}


HANDLE
	SkillIoOpenFile(
	IN PCWSTR FileName,
	IN ACCESS_MASK DesiredAccess,
	IN ULONG ShareAccess
	)
{
	NTSTATUS            ntStatus;
	UNICODE_STRING      uniFileName;
	OBJECT_ATTRIBUTES  objectAttributes;
	HANDLE              ntFileHandle;
	IO_STATUS_BLOCK    ioStatus;

	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return 0;
	}

	RtlInitUnicodeString(&uniFileName, FileName);

	InitializeObjectAttributes(&objectAttributes, &uniFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	ntStatus = IoCreateFile(&ntFileHandle,
		DesiredAccess,
		&objectAttributes,
		&ioStatus,
		0,
		FILE_ATTRIBUTE_NORMAL,
		ShareAccess,
		FILE_OPEN,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING);

	if (!NT_SUCCESS(ntStatus))
	{
		return 0;
	}

	return ntFileHandle;
}
NTSTATUS
	SkillSetFileCompletion(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp,
	IN PVOID Context
	)
{
	Irp->UserIosb->Status = Irp->IoStatus.Status;
	Irp->UserIosb->Information = Irp->IoStatus.Information;

	KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);

	IoFreeIrp(Irp);

	return STATUS_MORE_PROCESSING_REQUIRED;
}

BOOLEAN
	SKillDeleteFile(
	IN HANDLE  FileHandle
	)
{
	NTSTATUS        ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT    fileObject;
	PDEVICE_OBJECT  DeviceObject;
	PIRP            Irp;
	KEVENT          event;
	FILE_DISPOSITION_INFORMATION  FileInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PSECTION_OBJECT_POINTERS pSectionObjectPointer;



	ntStatus = ObReferenceObjectByHandle(FileHandle,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&fileObject,
		NULL);

	if (!NT_SUCCESS(ntStatus))
	{
		return FALSE;
	} 

	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = IoAllocateIrp(DeviceObject->StackSize, TRUE);

	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}

	KeInitializeEvent(&event, SynchronizationEvent, FALSE);

	FileInformation.DeleteFile = TRUE;

	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &event;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;

	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;

	IoSetCompletionRoutine(
		Irp,
		SkillSetFileCompletion,
		&event,
		TRUE,
		TRUE,
		TRUE);
	pSectionObjectPointer = fileObject->SectionObjectPointer;
	pSectionObjectPointer->ImageSectionObject = 0;
	pSectionObjectPointer->DataSectionObject = 0;

	IoCallDriver(DeviceObject, Irp);   

	KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	ObDereferenceObject(fileObject);

	return TRUE;
}

NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
	)
{
	UNICODE_STRING                uniDeviceName;
	UNICODE_STRING                uniSymLink;
	NTSTATUS                        ntStatus;
	PDEVICE_OBJECT                deviceObject = NULL;
	HANDLE              hFileHandle;

	RtlInitUnicodeString(&uniDeviceName, NT_DEVICE_NAME);
	RtlInitUnicodeString(&uniSymLink, DOS_DEVICE_NAME);

	ntStatus = IoCreateDevice(
		DriverObject,
		0,
		&uniDeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&deviceObject);

	if (!NT_SUCCESS(ntStatus))
	{
		return ntStatus;
	}

	ntStatus = IoCreateSymbolicLink(&uniSymLink, &uniDeviceName);

	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(deviceObject);
		return ntStatus;
	}

	DriverObject->DriverUnload = SKillUnloadDriver;

	hFileHandle = SkillIoOpenFile(L"\\??\\c:\\calc.exe",
		FILE_READ_ATTRIBUTES,
		FILE_SHARE_DELETE);

	DbgPrint("hFileHandle:%08X/n",hFileHandle);

	if (hFileHandle!=NULL)
	{
		SKillDeleteFile(hFileHandle);
		ZwClose(hFileHandle);
	}
	return STATUS_SUCCESS;
} 

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

