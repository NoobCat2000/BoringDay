#pragma once

#define ALLOC(X) RtlAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, X)
#define FREE(X) FreeAllocatedHeap(X)
#define REALLOC(X, Y) RtlReAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, X, Y)
#define LOG_ERROR(F, E) wprintf(L"%lls.%d: %lls failed at %lls (Error: 0x%08x)\n", PathFindFileNameW(__FILEW__), __LINE__, L ## F, __FUNCTIONW__, E);

LPWSTR GenerateTempPathW
(
	_In_ LPWSTR lpFileName,
	_In_ LPWSTR lpExtension,
	_In_ LPWSTR lpPrefixString
);

VOID FreeAllocatedHeap
(
	_In_ LPVOID lpBuffer
);

BOOL WriteToFile
(
	_In_ LPWSTR wszPath,
	_In_ PBYTE  pBuffer,
	_In_ DWORD  dwBufferSize
);