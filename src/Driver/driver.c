#include <ntddk.h>
#include "../Shared/protocol.h"

NTSTATUS DispatchIoctl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG ioctlCode = stack->Parameters.DeviceIoControl.IoControlCode;
    PVOID inputBuffer = Irp->AssociatedIrp.SystemBuffer;
    ULONG inputLength = stack->Parameters.DeviceIoControl.InputBufferLength;
    
    if (ioctlCode == IOCTL_EXEC_SCRIPT) {
        if (inputLength >= sizeof(EXEC_SCRIPT)) {
            PEXEC_SCRIPT execInfo = (PEXEC_SCRIPT)inputBuffer;
            // 這裡放執行腳本的邏輯 (例如：寫入 Roblox 記憶體並呼叫函數)
            DbgPrint("Driver: Received Script from PID %d\n", execInfo->Pid);
            DbgPrint("Script: %s\n", execInfo->Script);
        }
    }
    
    // 簡單處理讀寫 (範例)
    if (ioctlCode == IOCTL_READ_MEMORY || ioctlCode == IOCTL_WRITE_MEMORY) {
        PRW_MEMORY rwInfo = (PRW_MEMORY)inputBuffer;
        // 使用 MmCopyVirtualMemory 進行讀寫
        // ...
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNICODE_STRING devName;
    PDEVICE_OBJECT devObj;
    
    RtlInitUnicodeString(&devName, L"\\Device\\RobloxDriver");
    IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &devObj);
    
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    DriverObject->DriverUnload = [](PDRIVER_OBJECT obj) {
        PDEVICE_OBJECT dev = obj->DeviceObject;
        if (dev) IoDeleteDevice(dev);
    };
    
    DbgPrint("Driver Loaded!\n");
    return STATUS_SUCCESS;
}
