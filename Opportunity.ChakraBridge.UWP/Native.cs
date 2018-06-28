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
        internal static extern JsErrorCode JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, out JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCollectGarbage(JsRuntimeHandle handle);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDisposeRuntime(JsRuntimeHandle handle);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntimeMemoryUsage(JsRuntimeHandle runtime, out UIntPtr memoryUsage);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntimeMemoryLimit(JsRuntimeHandle runtime, out UIntPtr memoryLimit);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeMemoryLimit(JsRuntimeHandle runtime, UIntPtr memoryLimit);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeMemoryAllocationCallback(JsRuntimeHandle runtime, IntPtr callbackState, JsMemoryAllocationCallback allocationCallback);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeBeforeCollectCallback(JsRuntimeHandle runtime, IntPtr callbackState, JsBeforeCollectCallback beforeCollectCallback);

        [DllImport(DllName, EntryPoint = "JsAddRef")]
        internal static extern JsErrorCode JsContextAddRef(JsContext reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsAddRef(JsValueRef reference, out uint count);

        [DllImport(DllName, EntryPoint = "JsRelease")]
        internal static extern JsErrorCode JsContextRelease(JsContext reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsRelease(JsValueRef reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateContext(JsRuntimeHandle runtime, out JsContext newContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetCurrentContext(out JsContext currentContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetCurrentContext(JsContext context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntime(JsContext context, out JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStartDebugging();

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIdle(out uint nextIdleTick);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseScript(string script, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunScript(string script, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsSerializeScript(string script, byte[] buffer, ref ulong bufferSize);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseSerializedScript(string script, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunSerializedScript(string script, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsGetPropertyIdFromName(string name, out JsPropertyId propertyId);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern unsafe JsErrorCode JsGetPropertyNameFromId(JsPropertyId propertyId, out char* name);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetUndefinedValue(out JsValueRef undefinedValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetNullValue(out JsValueRef nullValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTrueValue(out JsValueRef trueValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetFalseValue(out JsValueRef falseValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsBoolToBoolean(bool value, out JsValueRef booleanValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsBooleanToBool(JsValueRef booleanValue, out bool boolValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToBoolean(JsValueRef value, out JsValueRef booleanValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetValueType(JsValueRef value, out JsValueType type);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDoubleToNumber(double doubleValue, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIntToNumber(int intValue, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsNumberToDouble(JsValueRef value, out double doubleValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToNumber(JsValueRef value, out JsValueRef numberValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetStringLength(JsValueRef sringValue, out int length);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsPointerToString(string value, UIntPtr stringLength, out JsValueRef stringValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStringToPointer(JsValueRef value, out IntPtr stringValue, out UIntPtr stringLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToString(JsValueRef value, out JsValueRef stringValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsVariantToValue([MarshalAs(UnmanagedType.Struct)] ref object var, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsValueToVariant(JsValueRef obj, [MarshalAs(UnmanagedType.Struct)] out object var);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetGlobalObject(out JsValueRef globalObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateObject(out JsValueRef obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateExternalObject(IntPtr data, JsObjectFinalizeCallback finalizeCallback, out JsValueRef obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToObject(JsValueRef value, out JsValueRef obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPrototype(JsValueRef obj, out JsValueRef prototypeObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetPrototype(JsValueRef obj, JsValueRef prototypeObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetExtensionAllowed(JsValueRef obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsPreventExtension(JsValueRef obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetProperty(JsValueRef obj, JsPropertyId propertyId, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertyDescriptor(JsValueRef obj, JsPropertyId propertyId, out JsValueRef propertyDescriptor);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertyNames(JsValueRef obj, out JsValueRef propertyNames);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetProperty(JsValueRef obj, JsPropertyId propertyId, JsValueRef value, bool useStrictRules);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasProperty(JsValueRef obj, JsPropertyId propertyId, out bool hasProperty);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDeleteProperty(JsValueRef obj, JsPropertyId propertyId, bool useStrictRules, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDefineProperty(JsValueRef obj, JsPropertyId propertyId, JsValueRef propertyDescriptor, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasIndexedProperty(JsValueRef obj, JsValueRef index, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetIndexedProperty(JsValueRef obj, JsValueRef index, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetIndexedProperty(JsValueRef obj, JsValueRef index, JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDeleteIndexedProperty(JsValueRef obj, JsValueRef index);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsEquals(JsValueRef obj1, JsValueRef obj2, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStrictEquals(JsValueRef obj1, JsValueRef obj2, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasExternalData(JsValueRef obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetExternalData(JsValueRef obj, out IntPtr externalData);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetExternalData(JsValueRef obj, IntPtr externalData);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateArray(uint length, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCallFunction(JsValueRef function, JsValueRef[] arguments, ushort argumentCount, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConstructObject(JsValueRef function, JsValueRef[] arguments, ushort argumentCount, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateFunction(JsNativeFunctionPtr nativeFunction, IntPtr externalData, out JsValueRef function);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateRangeError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateReferenceError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateSyntaxError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateTypeError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateURIError(JsValueRef message, out JsValueRef error);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasException(out bool hasException);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetAndClearException(out JsValueRef exception);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetException(JsValueRef exception);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDisableRuntimeExecution(JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsEnableRuntimeExecution(JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIsRuntimeExecutionDisabled(JsRuntimeHandle runtime, out bool isDisabled);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetObjectBeforeCollectCallback(JsValueRef reference, IntPtr callbackState, JsObjectBeforeCollectCallback beforeCollectCallback);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateNamedFunction(JsValueRef name, JsNativeFunctionPtr nativeFunction, IntPtr callbackState, out JsValueRef function);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsProjectWinRTNamespace(string namespaceName);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsInspectableToObject([MarshalAs(UnmanagedType.IInspectable)] object inspectable, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsObjectToInspectable(JsValueRef value, [MarshalAs(UnmanagedType.IInspectable)] out object inspectable);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetProjectionEnqueueCallback(JsProjectionEnqueueCallback projectionEnqueueCallback, IntPtr context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetPromiseContinuationCallback(JsPromiseContinuationCallback promiseContinuationCallback, IntPtr callbackState);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateArrayBuffer(uint byteLength, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateTypedArray(JsTypedArrayType arrayType, JsValueRef arrayBuffer, uint byteOffset,
            uint elementLength, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateDataView(JsValueRef arrayBuffer, uint byteOffset, uint byteOffsetLength, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetArrayBufferStorage(JsValueRef arrayBuffer, out IntPtr buffer, out uint bufferLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTypedArrayStorage(JsValueRef typedArray, out IntPtr buffer, out uint bufferLength, out JsTypedArrayType arrayType, out int elementSize);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetDataViewStorage(JsValueRef dataView, out IntPtr buffer, out uint bufferLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPropertyIdType(JsPropertyId propertyId, out JsPropertyIdType propertyIdType);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateSymbol(JsValueRef description, out JsValueRef symbol);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetSymbolFromPropertyId(JsPropertyId propertyId, out JsValueRef symbol);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetPropertyIdFromSymbol(JsValueRef symbol, out JsPropertyId propertyId);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetOwnPropertySymbols(JsValueRef obj, out JsValueRef propertySymbols);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsNumberToInt(JsValueRef value, out int intValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetIndexedPropertiesToExternalData(JsValueRef obj, IntPtr data, JsTypedArrayType arrayType, uint elementLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetIndexedPropertiesExternalData(JsValueRef obj, IntPtr data, out JsTypedArrayType arrayType, out uint elementLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsHasIndexedPropertiesExternalData(JsValueRef obj, out bool value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsInstanceOf(JsValueRef obj, JsValueRef constructor, out bool result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateExternalArrayBuffer(IntPtr data, uint byteLength, JsObjectFinalizeCallback finalizeCallback, IntPtr callbackState, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTypedArrayInfo(JsValueRef typedArray, out JsTypedArrayType arrayType, out JsValueRef arrayBuffer, out uint byteOffset, out uint byteLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextOfObject(JsValueRef obj, out JsContext context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextData(JsContext context, out IntPtr data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetContextData(JsContext context, IntPtr data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsParseSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsRunSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, byte[] buffer, JsSourceContext sourceContext, string sourceUrl, out JsValueRef result);
    }
}
