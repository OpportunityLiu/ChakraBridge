namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics.CodeAnalysis;
    using System.Runtime.InteropServices;

    /// <summary>
    /// Native interfaces.
    /// </summary>
    internal static class Native
    {
        const string DllName = "Chakra.dll";

        /// <summary>
        /// An invalid property identifier. 
        /// </summary>
        public static readonly IntPtr JS_INVALID_PROPERTYID = IntPtr.Zero;

        /// <summary>
        /// An invalid reference. 
        /// </summary>
        public static readonly IntPtr JS_INVALID_REFERENCE = IntPtr.Zero;

        /// <summary>
        /// An invalid runtime handle. 
        /// </summary>
        public static readonly IntPtr JS_INVALID_RUNTIME_HANDLE = IntPtr.Zero;

        /// <summary>
        /// An empty source context. 
        /// </summary>
        public static readonly UIntPtr JS_SOURCE_CONTEXT_NONE = UIntPtr.Subtract(UIntPtr.Zero, 1);

        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <param name="attributes">The attributes of the runtime to be created.</param>
        /// <param name="threadService">The thread service for the runtime. Can be null.</param>
        /// <param name="runtime">The runtime created.</param>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallbackPtr threadService, out JsRuntimeHandle runtime);

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

        /// <summary>
        /// Sets a memory allocation callback for specified runtime
        /// </summary>
        /// <remarks>
        /// <para>
        /// Registering a memory allocation callback will cause the runtime to call back to the host 
        /// whenever it acquires memory from, or releases memory to, the OS. The callback routine is
        /// called before the runtime memory manager allocates a block of memory. The allocation will
        /// be rejected if the callback returns false. The runtime memory manager will also invoke the
        /// callback routine after freeing a block of memory, as well as after allocation failures. 
        /// </para>
        /// <para>
        /// The callback is invoked on the current runtime execution thread, therefore execution is 
        /// blocked until the callback completes.
        /// </para>
        /// <para>
        /// The return value of the callback is not stored; previously rejected allocations will not
        /// prevent the runtime from invoking the callback again later for new memory allocations.
        /// </para>
        /// </remarks>
        /// <param name="runtime"></param>
        /// <param name="callbackState">
        /// User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="allocationCallback">
        /// Memory allocation callback to be called for memory allocation events.
        /// </param>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeMemoryAllocationCallback(JsRuntimeHandle runtime, IntPtr callbackState, JsMemoryAllocationCallbackPtr allocationCallback);

        /// <summary>
        /// Sets a callback function that is called by the runtime before garbage collection.
        /// </summary>
        /// <remarks>
        /// <para>
        /// The callback is invoked on the current runtime execution thread, therefore execution is 
        /// blocked until the callback completes.
        /// </para>
        /// <para>
        /// The callback can be used by hosts to prepare for garbage collection. For example, by 
        /// releasing unnecessary references on Chakra objects.
        /// </para>
        /// </remarks>
        /// <param name="runtime"></param>
        /// <param name="callbackState">
        /// User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="beforeCollectCallback">The callback function being set.</param>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetRuntimeBeforeCollectCallback(JsRuntimeHandle runtime, IntPtr callbackState, JsBeforeCollectCallbackPtr beforeCollectCallback);

        /// <summary>
        /// Adds a reference to a script context.
        /// </summary>
        /// <remarks>
        /// Calling AddRef ensures that the context will not be freed until Release is called.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        [DllImport(DllName, EntryPoint = "JsAddRef")]
        internal static extern JsErrorCode JsContextAddRef(JsContextRef reference, out uint count);

        /// <summary>
        /// Adds a reference to the object.
        /// </summary>
        /// <remarks>
        /// This only needs to be called on objects that are not going to be stored somewhere on 
        /// the stack. Calling AddRef ensures that the JavaScript object the value refers to will not be freed 
        /// until Release is called
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsAddRef(JsValueRef reference, out uint count);

        /// <summary>
        /// Releases a reference to a script context.
        /// </summary>
        /// <remarks>
        /// Removes a reference to a context that was created by AddRef.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        [DllImport(DllName, EntryPoint = "JsRelease")]
        internal static extern JsErrorCode JsContextRelease(JsContextRef reference, out uint count);

        /// <summary>
        /// Releases a reference to the object.
        /// </summary>
        /// <remarks>
        /// Removes a reference that was created by AddRef.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsRelease(JsValueRef reference, out uint count);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateContext(JsRuntimeHandle runtime, out JsContextRef newContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetCurrentContext(out JsContextRef currentContext);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetCurrentContext(JsContextRef context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetRuntime(JsContextRef context, out JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStartDebugging();

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIdle(out uint nextIdleTick);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseScript(string script, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunScript(string script, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsSerializeScript(string script, byte[] buffer, ref ulong bufferSize);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsParseSerializedScript(string script, byte[] buffer, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsRunSerializedScript(string script, byte[] buffer, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);

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
        internal static extern JsErrorCode JsGetStringLength(JsValueRef stringValue, out int length);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsPointerToString(string value, UIntPtr stringLength, out JsValueRef stringValue);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsStringToPointer(JsValueRef value, out IntPtr stringValue, out UIntPtr stringLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsConvertValueToString(JsValueRef value, out JsValueRef stringValue);

        //[DllImport(DllName)]
        //internal static extern JsErrorCode JsVariantToValue([MarshalAs(UnmanagedType.Struct)] ref object var, out JsValueRef value);

        //[DllImport(DllName)]
        //internal static extern JsErrorCode JsValueToVariant(JsValueRef obj, [MarshalAs(UnmanagedType.Struct)] out object var);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetGlobalObject(out JsValueRef globalObject);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateObject(out JsValueRef obj);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateExternalObject([MarshalAs(UnmanagedType.IInspectable)] object data, JsObjectFinalizeCallbackPtr finalizeCallback, out JsValueRef obj);

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
        internal static extern JsErrorCode JsGetExternalData(JsValueRef obj, [MarshalAs(UnmanagedType.IInspectable)] out object externalData);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetExternalData(JsValueRef obj, [MarshalAs(UnmanagedType.IInspectable)] object externalData);

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

        /// <summary>
        /// Sets the runtime of the current context to an exception state.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If the runtime of the current context is already in an exception state, this API will 
        /// throw <c>JsErrorInExceptionState</c>.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="exception">
        /// The JavaScript exception to set for the runtime of the current context.
        /// </param>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetException(JsValueRef exception);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsDisableRuntimeExecution(JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsEnableRuntimeExecution(JsRuntimeHandle runtime);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsIsRuntimeExecutionDisabled(JsRuntimeHandle runtime, out bool isDisabled);

        /// <summary>
        /// Sets a callback function that is called by the runtime before garbage collection of an object. 
        /// </summary>
        /// <param name="reference"></param>
        /// <param name="callbackState"></param>
        /// <param name="beforeCollectCallback">The callback function being set. Use <see langword="null"/> to clear previously registered callback. </param>
        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetObjectBeforeCollectCallback(JsValueRef reference, IntPtr callbackState, JsObjectBeforeCollectCallbackPtr beforeCollectCallback);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsCreateNamedFunction(JsValueRef name, JsNativeFunctionPtr nativeFunction, IntPtr callbackState, out JsValueRef function);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        internal static extern JsErrorCode JsProjectWinRTNamespace(string namespaceName);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsInspectableToObject([MarshalAs(UnmanagedType.IInspectable)] object inspectable, out JsValueRef value);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsObjectToInspectable(JsValueRef value, [MarshalAs(UnmanagedType.IInspectable)] out object inspectable);

        //[DllImport(DllName)]
        //internal static extern JsErrorCode JsSetProjectionEnqueueCallback(JsProjectionEnqueueCallbackPtr projectionEnqueueCallback, IntPtr context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetPromiseContinuationCallback(JsPromiseContinuationCallbackPtr promiseContinuationCallback, IntPtr callbackState);

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
        internal static extern JsErrorCode JsCreateExternalArrayBuffer(IntPtr data, uint byteLength, JsObjectFinalizeCallbackPtr finalizeCallback, IntPtr callbackState, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetTypedArrayInfo(JsValueRef typedArray, out JsTypedArrayType arrayType, out JsValueRef arrayBuffer, out uint byteOffset, out uint byteLength);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextOfObject(JsValueRef obj, out JsContextRef context);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsGetContextData(JsContextRef context, [MarshalAs(UnmanagedType.IInspectable)] out object data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsSetContextData(JsContextRef context, [MarshalAs(UnmanagedType.IInspectable)] object data);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsParseSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackPtr scriptLoadCallback,
            JsSerializedScriptUnloadCallbackPtr scriptUnloadCallback, byte[] buffer, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);

        [DllImport(DllName)]
        internal static extern JsErrorCode JsRunSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackPtr scriptLoadCallback,
            JsSerializedScriptUnloadCallbackPtr scriptUnloadCallback, byte[] buffer, JsSourceContextImpl sourceContext, string sourceUrl, out JsValueRef result);
    }
}
