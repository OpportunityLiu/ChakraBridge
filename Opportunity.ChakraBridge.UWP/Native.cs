namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics.CodeAnalysis;
    using System.Runtime.InteropServices;

    /// <summary>
    ///     Native interfaces.
    /// </summary>
    public static class Native
    {
        const string DllName = "Chakra.dll";

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, out JsRuntime runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCollectGarbage(JsRuntime handle);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDisposeRuntime(JsRuntime handle);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntimeMemoryUsage(JsRuntime runtime, out UIntPtr memoryUsage);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntimeMemoryLimit(JsRuntime runtime, out UIntPtr memoryLimit);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeMemoryLimit(JsRuntime runtime, UIntPtr memoryLimit);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeMemoryAllocationCallback(JsRuntime runtime, IntPtr callbackState, JsMemoryAllocationCallback allocationCallback);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeBeforeCollectCallback(JsRuntime runtime, IntPtr callbackState, JsBeforeCollectCallback beforeCollectCallback);

        [DllImport(DllName, EntryPoint = "JsAddRef")]
        internal static extern JsErrorCode JsContextAddRef(JsContext reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsAddRef(JsValueReference reference, out uint count);

        [DllImport(DllName, EntryPoint = "JsRelease")]
        internal static extern JsErrorCode JsContextRelease(JsContext reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsRelease(JsValueReference reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateContext(JsRuntime runtime, out JsContext newContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetCurrentContext(out JsContext currentContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetCurrentContext(JsContext context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntime(JsContext context, out JsRuntime runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStartDebugging();

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIdle(out uint nextIdleTick);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseScript(string script, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunScript(string script, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsSerializeScript(string script, byte[] buffer, ref ulong bufferSize);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseSerializedScript(string script, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunSerializedScript(string script, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsGetPropertyIdFromName(string name, out JsPropertyId propertyId);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsGetPropertyNameFromId(JsPropertyId propertyId, out string name);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetUndefinedValue(out JsValueReference undefinedValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetNullValue(out JsValueReference nullValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTrueValue(out JsValueReference trueValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetFalseValue(out JsValueReference falseValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsBoolToBoolean(bool value, out JsValueReference booleanValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsBooleanToBool(JsValueReference booleanValue, out bool boolValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToBoolean(JsValueReference value, out JsValueReference booleanValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetValueType(JsValueReference value, out JsValueType type);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDoubleToNumber(double doubleValue, out JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIntToNumber(int intValue, out JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsNumberToDouble(JsValueReference value, out double doubleValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToNumber(JsValueReference value, out JsValueReference numberValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetStringLength(JsValueReference sringValue, out int length);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsPointerToString(string value, UIntPtr stringLength, out JsValueReference stringValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStringToPointer(JsValueReference value, out IntPtr stringValue, out UIntPtr stringLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToString(JsValueReference value, out JsValueReference stringValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsVariantToValue([MarshalAs(UnmanagedType.Struct)] ref object var, out JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsValueToVariant(JsValueReference obj, [MarshalAs(UnmanagedType.Struct)] out object var);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetGlobalObject(out JsValueReference globalObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateObject(out JsValueReference obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateExternalObject(IntPtr data, JsObjectFinalizeCallback finalizeCallback, out JsValueReference obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToObject(JsValueReference value, out JsValueReference obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPrototype(JsValueReference obj, out JsValueReference prototypeObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetPrototype(JsValueReference obj, JsValueReference prototypeObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetExtensionAllowed(JsValueReference obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsPreventExtension(JsValueReference obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetProperty(JsValueReference obj, JsPropertyId propertyId, out JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertyDescriptor(JsValueReference obj, JsPropertyId propertyId, out JsValueReference propertyDescriptor);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertyNames(JsValueReference obj, out JsValueReference propertyNames);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetProperty(JsValueReference obj, JsPropertyId propertyId, JsValueReference value, bool useStrictRules);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasProperty(JsValueReference obj, JsPropertyId propertyId, out bool hasProperty);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDeleteProperty(JsValueReference obj, JsPropertyId propertyId, bool useStrictRules, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDefineProperty(JsValueReference obj, JsPropertyId propertyId, JsValueReference propertyDescriptor, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasIndexedProperty(JsValueReference obj, JsValueReference index, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetIndexedProperty(JsValueReference obj, JsValueReference index, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetIndexedProperty(JsValueReference obj, JsValueReference index, JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDeleteIndexedProperty(JsValueReference obj, JsValueReference index);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsEquals(JsValueReference obj1, JsValueReference obj2, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStrictEquals(JsValueReference obj1, JsValueReference obj2, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasExternalData(JsValueReference obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetExternalData(JsValueReference obj, out IntPtr externalData);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetExternalData(JsValueReference obj, IntPtr externalData);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateArray(uint length, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCallFunction(JsValueReference function, JsValueReference[] arguments, ushort argumentCount, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConstructObject(JsValueReference function, JsValueReference[] arguments, ushort argumentCount, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateFunction(JsNativeFunction nativeFunction, IntPtr externalData, out JsValueReference function);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateRangeError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateReferenceError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateSyntaxError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateTypeError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateURIError(JsValueReference message, out JsValueReference error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasException(out bool hasException);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetAndClearException(out JsValueReference exception);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetException(JsValueReference exception);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDisableRuntimeExecution(JsRuntime runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsEnableRuntimeExecution(JsRuntime runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIsRuntimeExecutionDisabled(JsRuntime runtime, out bool isDisabled);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetObjectBeforeCollectCallback(JsValueReference reference, IntPtr callbackState, JsObjectBeforeCollectCallback beforeCollectCallback);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateNamedFunction(JsValueReference name, JsNativeFunction nativeFunction, IntPtr callbackState, out JsValueReference function);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsProjectWinRTNamespace(string namespaceName);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsInspectableToObject([MarshalAs(UnmanagedType.IInspectable)] object inspectable, out JsValueReference value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetProjectionEnqueueCallback(JsProjectionEnqueueCallback projectionEnqueueCallback, IntPtr context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetPromiseContinuationCallback(JsPromiseContinuationCallback promiseContinuationCallback, IntPtr callbackState);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateArrayBuffer(uint byteLength, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateTypedArray(JsTypedArrayType arrayType, JsValueReference arrayBuffer, uint byteOffset,
            uint elementLength, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateDataView(JsValueReference arrayBuffer, uint byteOffset, uint byteOffsetLength, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetArrayBufferStorage(JsValueReference arrayBuffer, out IntPtr buffer, out uint bufferLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTypedArrayStorage(JsValueReference typedArray, out IntPtr buffer, out uint bufferLength, out JsTypedArrayType arrayType, out int elementSize);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetDataViewStorage(JsValueReference dataView, out IntPtr buffer, out uint bufferLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPropertyIdType(JsPropertyId propertyId, out JsPropertyIdType propertyIdType);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateSymbol(JsValueReference description, out JsValueReference symbol);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetSymbolFromPropertyId(JsPropertyId propertyId, out JsValueReference symbol);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPropertyIdFromSymbol(JsValueReference symbol, out JsPropertyId propertyId);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertySymbols(JsValueReference obj, out JsValueReference propertySymbols);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsNumberToInt(JsValueReference value, out int intValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetIndexedPropertiesToExternalData(JsValueReference obj, IntPtr data, JsTypedArrayType arrayType, uint elementLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetIndexedPropertiesExternalData(JsValueReference obj, IntPtr data, out JsTypedArrayType arrayType, out uint elementLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasIndexedPropertiesExternalData(JsValueReference obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsInstanceOf(JsValueReference obj, JsValueReference constructor, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateExternalArrayBuffer(IntPtr data, uint byteLength, JsObjectFinalizeCallback finalizeCallback, IntPtr callbackState, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTypedArrayInfo(JsValueReference typedArray, out JsTypedArrayType arrayType, out JsValueReference arrayBuffer, out uint byteOffset, out uint byteLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextOfObject(JsValueReference obj, out JsContext context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextData(JsContext context, out IntPtr data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetContextData(JsContext context, IntPtr data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsParseSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsRunSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueReference result);
    }
}
