#pragma once

struct _Assembly;

struct _AssemblyVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _Assembly* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _Assembly* This);
    ULONG(WINAPI* Release)(struct _Assembly* This);
    UINT64 Reserved[12];
    HRESULT(WINAPI* get_FullName)(struct _Assembly* This, BSTR* pRetVal);
    UINT64 Reserved1;
    HRESULT(WINAPI* GetType_2)(struct _Assembly* This, BSTR name, struct _Type** pRetVal);
};

struct _Assembly {
    struct _AssemblyVtbl* lpVtbl;
};

struct _Type;

struct _TypeVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _Type* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _Type* This);
    ULONG(WINAPI* Release)(struct _Type* This);
    HRESULT(WINAPI* GetTypeInfoCount)(struct _Type* This, unsigned long* pcTInfo);
    HRESULT(WINAPI* GetTypeInfo)(struct _Type* This, unsigned long iTInfo, unsigned long lcid, __int64 ppTInfo);
    HRESULT(WINAPI* GetIDsOfNames)(struct _Type* This, GUID* riid, __int64 rgszNames, unsigned long cNames, unsigned long lcid, __int64 rgDispId);
    HRESULT(WINAPI* Invoke)(struct _Type* This, unsigned long dispIdMember, GUID* riid, unsigned long lcid, short wFlags, __int64 pDispParams, __int64 pVarResult, __int64 pExcepInfo, __int64 puArgErr);
    HRESULT(WINAPI* get_ToString)(struct _Type* This, BSTR* pRetVal);
    HRESULT(WINAPI* Equals)(struct _Type* This, VARIANT other, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetHashCode)(struct _Type* This, long* pRetVal);
    HRESULT(WINAPI* GetType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* get_MemberType)(struct _Type* This, LPVOID pRetVal);
    HRESULT(WINAPI* get_name)(struct _Type* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_DeclaringType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* get_ReflectedType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* GetCustomAttributes)(struct _Type* This, struct _Type* attributeType, VARIANT_BOOL inherit, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetCustomAttributes_2)(struct _Type* This, VARIANT_BOOL inherit, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* IsDefined)(struct _Type* This, struct _Type* attributeType, VARIANT_BOOL inherit, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_Guid)(struct _Type* This, GUID* pRetVal);
    HRESULT(WINAPI* get_Module)(struct _Type* This, struct _Module** pRetVal);
    HRESULT(WINAPI* get_Assembly)(struct _Type* This, struct _Assembly** pRetVal);
    HRESULT(WINAPI* get_TypeHandle)(struct _Type* This, struct RuntimeTypeHandle* pRetVal);
    HRESULT(WINAPI* get_FullName)(struct _Type* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_Namespace)(struct _Type* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_AssemblyQualifiedName)(struct _Type* This, BSTR* pRetVal);
    HRESULT(WINAPI* GetArrayRank)(struct _Type* This, long* pRetVal);
    HRESULT(WINAPI* get_BaseType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* GetConstructors)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetInterface)(struct _Type* This, BSTR name, VARIANT_BOOL ignoreCase, struct _Type** pRetVal);
    HRESULT(WINAPI* GetInterfaces)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* FindInterfaces)(struct _Type* This, struct _TypeFilter* filter, VARIANT filterCriteria, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetEvent)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _EventInfo** pRetVal);
    HRESULT(WINAPI* GetEvents)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetEvents_2)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetNestedTypes)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetNestedType)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _Type** pRetVal);
    HRESULT(WINAPI* GetMember)(struct _Type* This, BSTR name, enum MemberTypes Type, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetDefaultMembers)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* FindMembers)(struct _Type* This, enum MemberTypes MemberType, enum BindingFlags bindingAttr, struct _MemberFilter* filter, VARIANT filterCriteria, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetElementType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* IsSubclassOf)(struct _Type* This, struct _Type* c, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* IsInstanceOfType)(struct _Type* This, VARIANT o, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* IsAssignableFrom)(struct _Type* This, struct _Type* c, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetInterfaceMap)(struct _Type* This, struct _Type* interfaceType, struct InterfaceMapping* pRetVal);
    HRESULT(WINAPI* GetMethod)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _Binder* Binder, SAFEARRAY* types, SAFEARRAY* modifiers, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethod_2)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethods)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetField)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _FieldInfo** pRetVal);
    HRESULT(WINAPI* GetFields)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetProperty)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperty_2)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _Binder* Binder, struct _Type* returnType, SAFEARRAY* types, SAFEARRAY* modifiers, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperties)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetMember_2)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetMembers)(struct _Type* This, enum BindingFlags bindingAttr, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* InvokeMember)(struct _Type* This, BSTR name, enum BindingFlags invokeAttr, struct _Binder* Binder, VARIANT Target, SAFEARRAY* args, SAFEARRAY* modifiers, LPVOID culture, SAFEARRAY* namedParameters, VARIANT* pRetVal);
    HRESULT(WINAPI* get_UnderlyingSystemType)(struct _Type* This, struct _Type** pRetVal);
    HRESULT(WINAPI* InvokeMember_2)(struct _Type* This, BSTR name, enum BindingFlags invokeAttr, struct _Binder* Binder, VARIANT Target, SAFEARRAY* args, LPVOID culture, VARIANT* pRetVal);
    HRESULT(WINAPI* InvokeMember_3)(struct _Type* This, BSTR name, enum BindingFlags invokeAttr, struct _Binder* Binder, VARIANT Target, SAFEARRAY* args, VARIANT* pRetVal);
    HRESULT(WINAPI* GetConstructor)(struct _Type* This, enum BindingFlags bindingAttr, struct _Binder* Binder, enum CallingConventions callConvention, SAFEARRAY* types, SAFEARRAY* modifiers, struct _ConstructorInfo** pRetVal);
    HRESULT(WINAPI* GetConstructor_2)(struct _Type* This, enum BindingFlags bindingAttr, struct _Binder* Binder, SAFEARRAY* types, SAFEARRAY* modifiers, struct _ConstructorInfo** pRetVal);
    HRESULT(WINAPI* GetConstructor_3)(struct _Type* This, SAFEARRAY* types, struct _ConstructorInfo** pRetVal);
    HRESULT(WINAPI* GetConstructors_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* get_TypeInitializer)(struct _Type* This, struct _ConstructorInfo** pRetVal);
    HRESULT(WINAPI* GetMethod_3)(struct _Type* This, BSTR name, enum BindingFlags bindingAttr, struct _Binder* Binder, enum CallingConventions callConvention, SAFEARRAY* types, SAFEARRAY* modifiers, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethod_4)(struct _Type* This, BSTR name, SAFEARRAY* types, SAFEARRAY* modifiers, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethod_5)(struct _Type* This, BSTR name, SAFEARRAY* types, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethod_6)(struct _Type* This, BSTR name, struct _MethodInfo** pRetVal);
    HRESULT(WINAPI* GetMethods_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetField_2)(struct _Type* This, BSTR name, struct _FieldInfo** pRetVal);
    HRESULT(WINAPI* GetFields_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetInterface_2)(struct _Type* This, BSTR name, struct _Type** pRetVal);
    HRESULT(WINAPI* GetEvent_2)(struct _Type* This, BSTR name, struct _EventInfo** pRetVal);
    HRESULT(WINAPI* GetProperty_3)(struct _Type* This, BSTR name, struct _Type* returnType, SAFEARRAY* types, SAFEARRAY* modifiers, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperty_4)(struct _Type* This, BSTR name, struct _Type* returnType, SAFEARRAY* types, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperty_5)(struct _Type* This, BSTR name, SAFEARRAY* types, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperty_6)(struct _Type* This, BSTR name, struct _Type* returnType, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperty_7)(struct _Type* This, BSTR name, LPVOID* pRetVal);
    HRESULT(WINAPI* GetProperties_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetNestedTypes_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetNestedType_2)(struct _Type* This, BSTR name, struct _Type** pRetVal);
    HRESULT(WINAPI* GetMember_3)(struct _Type* This, BSTR name, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetMembers_2)(struct _Type* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* get_Attributes)(struct _Type* This, enum TypeAttributes* pRetVal);
    HRESULT(WINAPI* get_IsNotPublic)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsPublic)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedPublic)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedPrivate)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedFamily)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedAssembly)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedFamANDAssem)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsNestedFamORAssem)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAutoLayout)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsLayoutSequential)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsExplicitLayout)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsClass)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsInterface)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsValueType)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAbstract)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsSealed)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsEnum)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsSpecialName)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsImport)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsSerializable)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAnsiClass)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsUnicodeClass)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAutoClass)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsArray)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsByRef)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsPointer)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsPrimitive)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsCOMObject)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_HasElementType)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsContextful)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsMarshalByRef)(struct _Type* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* Equals_2)(struct _Type* This, struct _Type* o, VARIANT_BOOL* pRetVal);
};

struct _Type {
    struct _TypeVtbl* lpVtbl;
};

struct _FieldInfo;

struct _FieldInfoVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _FieldInfo* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _FieldInfo* This);
    ULONG(WINAPI* Release)(struct _FieldInfo* This);
    UINT64 Reserved[16];
    HRESULT(WINAPI* GetValue)(struct _FieldInfo* This, VARIANT obj, VARIANT* pRetVal);
    UINT64 Reserved1[5];
    HRESULT(WINAPI* SetValue_2)(struct _FieldInfo* This, VARIANT obj, VARIANT value);
};

struct _FieldInfo {
    struct _FieldInfoVtbl* lpVtbl;
};

struct _MethodInfo;

struct _MethodInfoVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _MethodInfo* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _MethodInfo* This);
    ULONG(WINAPI* Release)(struct _MethodInfo* This);
    HRESULT(WINAPI* GetTypeInfoCount)(struct _MethodInfo* This, unsigned long* pcTInfo);
    HRESULT(WINAPI* GetTypeInfo)(struct _MethodInfo* This, unsigned long iTInfo, unsigned long lcid, __int64 ppTInfo);
    HRESULT(WINAPI* GetIDsOfNames)(struct _MethodInfo* This, GUID* riid, __int64 rgszNames, unsigned long cNames, unsigned long lcid, __int64 rgDispId);
    HRESULT(WINAPI* Invoke)(struct _MethodInfo* This, unsigned long dispIdMember, GUID* riid, unsigned long lcid, short wFlags, __int64 pDispParams, __int64 pVarResult, __int64 pExcepInfo, __int64 puArgErr);
    HRESULT(WINAPI* get_ToString)(struct _MethodInfo* This, BSTR* pRetVal);
    HRESULT(WINAPI* Equals)(struct _MethodInfo* This, VARIANT other, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetHashCode)(struct _MethodInfo* This, long* pRetVal);
    HRESULT(WINAPI* GetType)(struct _MethodInfo* This, struct _Type** pRetVal);
    HRESULT(WINAPI* get_MemberType)(struct _MethodInfo* This, LPVOID pRetVal);
    HRESULT(WINAPI* get_name)(struct _MethodInfo* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_DeclaringType)(struct _MethodInfo* This, struct _Type** pRetVal);
    HRESULT(WINAPI* get_ReflectedType)(struct _MethodInfo* This, struct _Type** pRetVal);
    HRESULT(WINAPI* GetCustomAttributes)(struct _MethodInfo* This, struct _Type* attributeType, VARIANT_BOOL inherit, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetCustomAttributes_2)(struct _MethodInfo* This, VARIANT_BOOL inherit, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* IsDefined)(struct _MethodInfo* This, struct _Type* attributeType, VARIANT_BOOL inherit, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetParameters)(struct _MethodInfo* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* GetMethodImplementationFlags)(struct _MethodInfo* This, LPVOID pRetVal);
    HRESULT(WINAPI* get_MethodHandle)(struct _MethodInfo* This, LPVOID pRetVal);
    HRESULT(WINAPI* get_Attributes)(struct _MethodInfo* This, LPVOID pRetVal);
    HRESULT(WINAPI* get_CallingConvention)(struct _MethodInfo* This, LPVOID pRetVal);
    HRESULT(WINAPI* Invoke_2)(struct _MethodInfo* This, VARIANT obj, enum BindingFlags invokeAttr, struct _Binder* Binder, SAFEARRAY* parameters, LPVOID culture, VARIANT* pRetVal);
    HRESULT(WINAPI* get_IsPublic)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsPrivate)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsFamily)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAssembly)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsFamilyAndAssembly)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsFamilyOrAssembly)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsStatic)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsFinal)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_Is)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsHideBySig)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsAbstract)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsSpecialName)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* get_IsConstructor)(struct _MethodInfo* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* Invoke_3)(struct _MethodInfo* This, VARIANT obj, SAFEARRAY* parameters, VARIANT* pRetVal);
    HRESULT(WINAPI* get_returnType)(struct _MethodInfo* This, struct _Type** pRetVal);
    HRESULT(WINAPI* get_ReturnTypeCustomAttributes)(struct _MethodInfo* This, LPVOID* pRetVal);
    HRESULT(WINAPI* GetBaseDefinition)(struct _MethodInfo* This, struct _MethodInfo** pRetVal);
};

struct _MethodInfo {
    struct _MethodInfoVtbl* lpVtbl;
};

struct _PropertyInfo;

struct _PropertyInfoVtbl {
    HRESULT(WINAPI* QueryInterface)(LPVOID This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(LPVOID This);
    ULONG(WINAPI* Release)(LPVOID This);
    UINT64 Reserved[16];
    HRESULT(WINAPI* GetValue)(LPVOID This, VARIANT obj, SAFEARRAY* index, VARIANT* pRetVal);
};

struct _PropertyInfo {
    struct _PropertyInfoVtbl* lpVtbl;
};

struct _AppDomain;

struct _AppDomainVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _AppDomain* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _AppDomain* This);
    ULONG(WINAPI* Release)(struct _AppDomain* This);
    HRESULT(WINAPI* GetTypeInfoCount)(struct _AppDomain* This, unsigned long* pcTInfo);
    HRESULT(WINAPI* GetTypeInfo)(struct _AppDomain* This, unsigned long iTInfo, unsigned long lcid, __int64 ppTInfo);
    HRESULT(WINAPI* GetIDsOfNames)(struct _AppDomain* This, GUID* riid, __int64 rgszNames, unsigned long cNames, unsigned long lcid, __int64 rgDispId);
    HRESULT(WINAPI* Invoke)(struct _AppDomain* This, unsigned long dispIdMember, GUID* riid, unsigned long lcid, short wFlags, __int64 pDispParams, __int64 pVarResult, __int64 pExcepInfo, __int64 puArgErr);
    HRESULT(WINAPI* get_ToString)(struct _AppDomain* This, BSTR* pRetVal);
    HRESULT(WINAPI* Equals)(struct _AppDomain* This, VARIANT other, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetHashCode)(struct _AppDomain* This, long* pRetVal);
    HRESULT(WINAPI* GetType)(struct _AppDomain* This, struct _Type** pRetVal);
    HRESULT(WINAPI* InitializeLifetimeService)(struct _AppDomain* This, VARIANT* pRetVal);
    HRESULT(WINAPI* GetLifetimeService)(struct _AppDomain* This, VARIANT* pRetVal);
    HRESULT(WINAPI* get_Evidence)(struct _AppDomain* This, LPVOID* pRetVal);
    HRESULT(WINAPI* add_DomainUnload)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_DomainUnload)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_AssemblyLoad)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_AssemblyLoad)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_ProcessExit)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_ProcessExit)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_TypeResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_TypeResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_ResourceResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_ResourceResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_AssemblyResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_AssemblyResolve)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* add_UnhandledException)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* remove_UnhandledException)(struct _AppDomain* This, LPVOID value);
    HRESULT(WINAPI* DefineDynamicAssembly)(struct _AppDomain* This, LPVOID name, DWORD access, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_2)(struct _AppDomain* This, LPVOID name, DWORD access, BSTR dir, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_3)(struct _AppDomain* This, LPVOID name, DWORD access, LPVOID Evidence, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_4)(struct _AppDomain* This, LPVOID name, DWORD access, LPVOID requiredPermissions, LPVOID optionalPermissions, LPVOID refusedPermissions, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_5)(struct _AppDomain* This, LPVOID name, DWORD access, BSTR dir, LPVOID Evidence, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_6)(struct _AppDomain* This, LPVOID name, DWORD access, BSTR dir, LPVOID requiredPermissions, LPVOID optionalPermissions, LPVOID refusedPermissions, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_7)(struct _AppDomain* This, LPVOID name, DWORD access, LPVOID Evidence, LPVOID requiredPermissions, LPVOID optionalPermissions, LPVOID refusedPermissions, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_8)(struct _AppDomain* This, LPVOID name, DWORD access, BSTR dir, LPVOID Evidence, LPVOID requiredPermissions, LPVOID optionalPermissions, LPVOID refusedPermissions, LPVOID* pRetVal);
    HRESULT(WINAPI* DefineDynamicAssembly_9)(struct _AppDomain* This, LPVOID name, DWORD access, BSTR dir, LPVOID Evidence, LPVOID requiredPermissions, LPVOID optionalPermissions, LPVOID refusedPermissions, VARIANT_BOOL IsSynchronized, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstance)(struct _AppDomain* This, BSTR AssemblyName, BSTR typeName, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstanceFrom)(struct _AppDomain* This, BSTR assemblyFile, BSTR typeName, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstance_2)(struct _AppDomain* This, BSTR AssemblyName, BSTR typeName, SAFEARRAY* activationAttributes, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstanceFrom_2)(struct _AppDomain* This, BSTR assemblyFile, BSTR typeName, SAFEARRAY* activationAttributes, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstance_3)(struct _AppDomain* This, BSTR AssemblyName, BSTR typeName, VARIANT_BOOL ignoreCase, enum BindingFlags bindingAttr, LPVOID Binder, SAFEARRAY* args, LPVOID culture, SAFEARRAY* activationAttributes, LPVOID securityAttributes, LPVOID* pRetVal);
    HRESULT(WINAPI* CreateInstanceFrom_3)(struct _AppDomain* This, BSTR assemblyFile, BSTR typeName, VARIANT_BOOL ignoreCase, enum BindingFlags bindingAttr, LPVOID Binder, SAFEARRAY* args, LPVOID culture, SAFEARRAY* activationAttributes, LPVOID securityAttributes, LPVOID* pRetVal);
    HRESULT(WINAPI* Load)(struct _AppDomain* This, LPVOID assemblyRef, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_2)(struct _AppDomain* This, BSTR assemblyString, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_3)(struct _AppDomain* This, SAFEARRAY* rawAssembly, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_4)(struct _AppDomain* This, SAFEARRAY* rawAssembly, SAFEARRAY* rawSymbolStore, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_5)(struct _AppDomain* This, SAFEARRAY* rawAssembly, SAFEARRAY* rawSymbolStore, LPVOID securityEvidence, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_6)(struct _AppDomain* This, LPVOID assemblyRef, LPVOID assemblySecurity, struct _Assembly** pRetVal);
    HRESULT(WINAPI* Load_7)(struct _AppDomain* This, BSTR assemblyString, LPVOID assemblySecurity, struct _Assembly** pRetVal);
    HRESULT(WINAPI* ExecuteAssembly)(struct _AppDomain* This, BSTR assemblyFile, LPVOID assemblySecurity, long* pRetVal);
    HRESULT(WINAPI* ExecuteAssembly_2)(struct _AppDomain* This, BSTR assemblyFile, long* pRetVal);
    HRESULT(WINAPI* ExecuteAssembly_3)(struct _AppDomain* This, BSTR assemblyFile, LPVOID assemblySecurity, SAFEARRAY* args, long* pRetVal);
    HRESULT(WINAPI* get_FriendlyName)(struct _AppDomain* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_BaseDirectory)(struct _AppDomain* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_RelativeSearchPath)(struct _AppDomain* This, BSTR* pRetVal);
    HRESULT(WINAPI* get_ShadowCopyFiles)(struct _AppDomain* This, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetAssemblies)(struct _AppDomain* This, SAFEARRAY** pRetVal);
    HRESULT(WINAPI* AppendPrivatePath)(struct _AppDomain* This, BSTR Path);
    HRESULT(WINAPI* ClearPrivatePath)(struct _AppDomain* This);
    HRESULT(WINAPI* SetShadowCopyPath)(struct _AppDomain* This, BSTR s);
    HRESULT(WINAPI* ClearShadowCopyPath)(struct _AppDomain* This);
    HRESULT(WINAPI* SetCachePath)(struct _AppDomain* This, BSTR s);
    HRESULT(WINAPI* SetData)(struct _AppDomain* This, BSTR name, VARIANT data);
    HRESULT(WINAPI* GetData)(struct _AppDomain* This, BSTR name, VARIANT* pRetVal);
    HRESULT(WINAPI* SetAppDomainPolicy)(struct _AppDomain* This, LPVOID domainPolicy);
    HRESULT(WINAPI* SetThreadPrincipal)(struct _AppDomain* This, LPVOID principal);
    HRESULT(WINAPI* SetPrincipalPolicy)(struct _AppDomain* This, DWORD policy);
    HRESULT(WINAPI* DoCallBack)(struct _AppDomain* This, LPVOID theDelegate);
    HRESULT(WINAPI* get_DynamicDirectory)(struct _AppDomain* This, BSTR* pRetVal);
};

struct _AppDomain {
    struct _AppDomainVtbl* lpVtbl;
};

struct _ICollection;

struct _ICollectionVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _ICollection* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _ICollection* This);
    ULONG(WINAPI* Release)(struct _ICollection* This);
    HRESULT(WINAPI* CopyTo)(struct _ICollection* This, LPVOID Array, long index);
    HRESULT(WINAPI* get_Count)(struct _ICollection* This, long* pRetVal);
    HRESULT(WINAPI* get_SyncRoot)(struct _ICollection* This, VARIANT* pRetVal);
    HRESULT(WINAPI* get_IsSynchronized)(struct _ICollection* This, VARIANT_BOOL* pRetVal);
};

struct _ICollection {
    struct _ICollectionVtbl* lpVtbl;
};

struct _IEnumerable;

struct _IEnumerableVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _IEnumerable* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _IEnumerable* This);
    ULONG(WINAPI* Release)(struct _IEnumerable* This);
    HRESULT(WINAPI* GetTypeInfoCount)(struct _IEnumerable* This, unsigned long* pcTInfo);
    HRESULT(WINAPI* GetTypeInfo)(struct _IEnumerable* This, unsigned long iTInfo, unsigned long lcid, __int64 ppTInfo);
    HRESULT(WINAPI* GetIDsOfNames)(struct _IEnumerable* This, GUID* riid, __int64 rgszNames, unsigned long cNames, unsigned long lcid, __int64 rgDispId);
    HRESULT(WINAPI* Invoke)(struct _IEnumerable* This, unsigned long dispIdMember, GUID* riid, unsigned long lcid, short wFlags, __int64 pDispParams, __int64 pVarResult, __int64 pExcepInfo, __int64 puArgErr);
    HRESULT(WINAPI* GetEnumerator)(struct _IEnumerable* This, struct IEnumVARIANT** pRetVal);
};

struct _IEnumerable {
    struct _IEnumerableVtbl* lpVtbl;
};

struct _IObject;

struct _IObjectVtbl {
    HRESULT(WINAPI* QueryInterface)(struct _IObject* This, REFIID riid, void** ppvObject);
    ULONG(WINAPI* AddRef)(struct _IObject* This);
    ULONG(WINAPI* Release)(struct _IObject* This);
    HRESULT(WINAPI* GetTypeInfoCount)(struct _IObject* This, unsigned long* pcTInfo);
    HRESULT(WINAPI* GetTypeInfo)(struct _IObject* This, unsigned long iTInfo, unsigned long lcid, __int64 ppTInfo);
    HRESULT(WINAPI* GetIDsOfNames)(struct _IObject* This, GUID* riid, __int64 rgszNames, unsigned long cNames, unsigned long lcid, __int64 rgDispId);
    HRESULT(WINAPI* Invoke)(struct _IObject* This, unsigned long dispIdMember, GUID* riid, unsigned long lcid, short wFlags, __int64 pDispParams, __int64 pVarResult, __int64 pExcepInfo, __int64 puArgErr);
    HRESULT(WINAPI* get_ToString)(struct _IObject* This, BSTR* pRetVal);
    HRESULT(WINAPI* Equals)(struct _IObject* This, VARIANT obj, VARIANT_BOOL* pRetVal);
    HRESULT(WINAPI* GetHashCode)(struct _IObject* This, long* pRetVal);
    HRESULT(WINAPI* GetType)(struct _IObject* This, struct _Type** pRetVal);
};

struct _IObject {
    struct _IObjectVtbl* lpVtbl;
};

typedef struct _CLR_CONTEXT {
    ICLRMetaHost* pMetaHost;
    ICLRRuntimeInfo* pRuntimeInfo;
    ICorRuntimeHost* pRuntimeHost;
    IUnknown* pAppDomainThunk;
    struct _AppDomain* pAppDomain;
} CLR_CONTEXT, * PCLR_CONTEXT;

enum BindingFlags {
    BindingFlags_Default = 0,
    BindingFlags_IgnoreCase = 1,
    BindingFlags_DeclaredOnly = 2,
    BindingFlags_Instance = 4,
    BindingFlags_Static = 8,
    BindingFlags_Public = 16,
    BindingFlags_NonPublic = 32,
    BindingFlags_FlattenHierarchy = 64,
    BindingFlags_InvokeMethod = 256,
    BindingFlags_CreateInstance = 512,
    BindingFlags_GetField = 1024,
    BindingFlags_SetField = 2048,
    BindingFlags_GetProperty = 4096,
    BindingFlags_SetProperty = 8192,
    BindingFlags_PutDispProperty = 16384,
    BindingFlags_PutRefDispProperty = 32768,
    BindingFlags_ExactBinding = 65536,
    BindingFlags_SuppressChangeType = 131072,
    BindingFlags_OptionalParamBinding = 262144,
    BindingFlags_IgnoreReturn = 16777216
};

struct _AppDomain* InitializeCommonLanguageRuntime
(
    _In_ LPWSTR lpDomainName,
    _Out_ PBOOL pIsLoadedBefore
);

BOOL CreateInitialRunspaceConfiguration
(
    _In_ struct _AppDomain* pAppDomain,
    _Inout_ VARIANT* pvtRunspaceConfiguration
);

BOOL DisablePowerShellEtwProvider
(
    struct _AppDomain* pAppDomain
);

BOOL PatchTranscriptionOptionFlushContentToDisk
(
    _In_ struct _AppDomain* pAppDomain
);

BOOL PatchAuthorizationManagerShouldRunInternal
(
    _In_ struct _AppDomain* pAppDomain
);

BOOL PatchSystemPolicyGetSystemLockdownPolicy
(
    _In_ struct _AppDomain* pAppDomain
);

LPWSTR StartPowerShell
(
    _In_ struct _AppDomain* pAppDomain,
    _In_ LPWSTR lpScript
);

BOOL PatchAmsiOpenSession();