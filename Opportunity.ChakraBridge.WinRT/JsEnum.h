#pragma once

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    ///     Attributes of a runtime.
    /// </summary>
    public enum class JsRuntimeAttributes
    {
        /// <summary>
        ///     No special attributes.
        /// </summary>
        None = 0x00000000,
        /// <summary>
        ///     The runtime will not do any work (such as garbage collection) on background threads.
        /// </summary>
        DisableBackgroundWork = 0x00000001,
        /// <summary>
        ///     The runtime should support reliable script interruption. This increases the number of
        ///     places where the runtime will check for a script interrupt request at the cost of a
        ///     small amount of runtime performance.
        /// </summary>
        AllowScriptInterrupt = 0x00000002,
        /// <summary>
        ///     Host will call <c>JsIdle</c>, so enable idle processing. Otherwise, the runtime will
        ///     manage memory slightly more aggressively.
        /// </summary>
        EnableIdleProcessing = 0x00000004,
        /// <summary>
        ///     Runtime will not generate native code.
        /// </summary>
        DisableNativeCodeGeneration = 0x00000008,
        /// <summary>
        ///     Using <c>eval</c> or <c>function</c> constructor will throw an exception.
        /// </summary>
        DisableEval = 0x00000010,
        /// <summary>
        ///     Runtime will enable all experimental features.
        /// </summary>
        EnableExperimentalFeatures = 0x00000020,
        /// <summary>
        ///     Calling <c>JsSetException</c> will also dispatch the exception to the script debugger
        ///     (if any) giving the debugger a chance to break on the exception.
        /// </summary>
        DispatchSetExceptionsToDebugger = 0x00000040
    };

    /// <summary>
    ///     The type of a typed JavaScript array.
    /// </summary>
    public enum class JsTypedArrayType
    {
        /// <summary>
        ///     An int8 array.
        /// </summary>
        Int8,
        /// <summary>
        ///     An uint8 array.
        /// </summary>
        Uint8,
        /// <summary>
        ///     An uint8 clamped array.
        /// </summary>
        Uint8Clamped,
        /// <summary>
        ///     An int16 array.
        /// </summary>
        Int16,
        /// <summary>
        ///     An uint16 array.
        /// </summary>
        Uint16,
        /// <summary>
        ///     An int32 array.
        /// </summary>
        Int32,
        /// <summary>
        ///     An uint32 array.
        /// </summary>
        Uint32,
        /// <summary>
        ///     A float32 array.
        /// </summary>
        Float32,
        /// <summary>
        ///     A float64 array.
        /// </summary>
        Float64
    };

    /// <summary>
    ///     Allocation callback event type.
    /// </summary>
    public enum class JsMemoryEventType
    {
        /// <summary>
        ///     Indicates a request for memory allocation.
        /// </summary>
        Allocate = 0,
        /// <summary>
        ///     Indicates a memory freeing event.
        /// </summary>
        Free = 1,
        /// <summary>
        ///     Indicates a failed allocation event.
        /// </summary>
        Failure = 2
    };

    /// <summary>
    ///     The JavaScript type of a JsValueRef.
    /// </summary>
    public enum class JsValueType
    {
        /// <summary>
        ///     The value is the <c>undefined</c> value.
        /// </summary>
        Undefined = 0,
        /// <summary>
        ///     The value is the <c>null</c> value.
        /// </summary>
        Null = 1,
        /// <summary>
        ///     The value is a JavaScript number value.
        /// </summary>
        Number = 2,
        /// <summary>
        ///     The value is a JavaScript string value.
        /// </summary>
        String = 3,
        /// <summary>
        ///     The value is a JavaScript Boolean value.
        /// </summary>
        Boolean = 4,
        /// <summary>
        ///     The value is a JavaScript object value.
        /// </summary>
        Object = 5,
        /// <summary>
        ///     The value is a JavaScript function object value.
        /// </summary>
        Function = 6,
        /// <summary>
        ///     The value is a JavaScript error object value.
        /// </summary>
        Error = 7,
        /// <summary>
        ///     The value is a JavaScript array object value.
        /// </summary>
        Array = 8,
        /// <summary>
        ///     The value is a JavaScript symbol value.
        /// </summary>
        Symbol = 9,
        /// <summary>
        ///     The value is a JavaScript ArrayBuffer object value.
        /// </summary>
        ArrayBuffer = 10,
        /// <summary>
        ///     The value is a JavaScript typed array object value.
        /// </summary>
        TypedArray = 11,
        /// <summary>
        ///     The value is a JavaScript DataView object value.
        /// </summary>
        DataView = 12,
    };
}
