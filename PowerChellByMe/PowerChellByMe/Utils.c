#include "pch.h"

LPWSTR GenerateTempPathW
(
	_In_ LPWSTR lpFileName,
	_In_ LPWSTR lpExtension,
	_In_ LPWSTR lpPrefixString
)
{
	WCHAR wszTempPath[MAX_PATH];
	WCHAR wszTempName[0x100];
	LPWSTR lpResult = NULL;

	GetTempPathW(MAX_PATH, wszTempPath);
	lpResult = ALLOC(MAX_PATH * sizeof(WCHAR));
	if (lpFileName != NULL) {
		wsprintfW(lpResult, L"%s%s", wszTempPath, lpFileName);
	}
	else {
		GetTempFileNameW(wszTempPath, lpPrefixString, 0, lpResult);
		DeleteFileW(lpResult);
		if (lpExtension != NULL) {
			lstrcatW(lpResult, lpExtension);
		}
	}

	return lpResult;
}

VOID FreeAllocatedHeap
(
	_In_ LPVOID lpBuffer
)
{
	if (lpBuffer != NULL) {
		RtlFreeHeap(GetProcessHeap(), 0, lpBuffer);
	}
}

BOOL WriteToFile
(
	_In_ LPWSTR wszPath,
	_In_ PBYTE  pBuffer,
	_In_ DWORD  dwBufferSize
)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	BOOL bResult = FALSE;
	DWORD dwNumberOfBytesWritten = 0;

	hFile = CreateFileW(wszPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto CLEANUP;
	}

	if (!WriteFile(hFile, pBuffer, dwBufferSize, &dwNumberOfBytesWritten, NULL)) {
		goto CLEANUP;
	}

	bResult = TRUE;
CLEANUP:
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
	}

	return bResult;
}

BOOL IsFileExist
(
	_In_ LPWSTR lpPath
)
{
	DWORD dwFileAttr = GetFileAttributesW(lpPath);
	if (dwFileAttr != INVALID_FILE_ATTRIBUTES && !(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
		return TRUE;
	}

	return FALSE;
}