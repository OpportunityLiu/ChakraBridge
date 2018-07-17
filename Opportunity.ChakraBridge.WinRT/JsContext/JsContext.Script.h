#pragma once
#include "JsEnum.h"
#include "JsRuntime\JsRuntime.h"
#include "Native\ThrowHelper.h"
#include "Value\JsValue.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// Called by the runtime to load the source code of the serialized script.
    /// </summary>
    /// <param name="scriptBuffer">The script returned.</param>
    /// <returns>
    /// true if the operation succeeded, false otherwise.
    /// </returns>
    public delegate bool JsSerializedScriptLoadSourceCallback(Platform::String^* scriptBuffer);

    partial ref class JsContext sealed
    {
    private:
        using IBuffer = Windows::Storage::Streams::IBuffer;
        static JsSourceContext SourceContext;
        static JsValueRef PromiseContinuation;
        static void CALLBACK JsContext::JsPromiseContinuationCallbackImpl(_In_ JsValueRef task, _In_opt_ void *callbackState);
    internal:
        static void HandlePromiseContinuation();

    public:
        /// <summary>
        /// Parses a script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to parse.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [Windows::Foundation::Metadata::DefaultOverload]
        [Windows::Foundation::Metadata::Overload("ParseScript")]
        static JsValue^ ParseScript(Platform::String^ script)
        {
            return ParseScript(script, nullptr);
        }

        /// <summary>
        /// Parses a serialized script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [Windows::Foundation::Metadata::Overload("ParseSerializedScript")]
        static JsValue^ ParseScript(IBuffer^ buffer)
        {
            return ParseScript(static_cast<Platform::String^>(nullptr), buffer, nullptr);
        }

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <returns>The result of the script, if any.</returns>
        [Windows::Foundation::Metadata::DefaultOverload]
        [Windows::Foundation::Metadata::Overload("RunScript")]
        static JsValue^ RunScript(Platform::String^ script)
        {
            return RunScript(script, nullptr);
        }

        /// <summary>
        /// Runs a serialized script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>The result of the script, if any.</returns>
        [Windows::Foundation::Metadata::Overload("RunSerializedScript")]
        static JsValue^ RunScript(IBuffer^ buffer)
        {
            return RunScript(static_cast<Platform::String^>(nullptr), buffer, nullptr);
        }

        /// <summary>
        /// Serializes a parsed script to a buffer than can be reused.
        /// </summary>
        /// <remarks>
        /// <para>
        /// SerializeScript parses a script and then stores the parsed form of the script in a 
        /// runtime-independent format. The serialized script then can be deserialized in any
        /// runtime without requiring the script to be re-parsed.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="script">The script to serialize.</param>
        /// <returns>
        /// The size of the buffer, in bytes, required to hold the serialized script.
        /// </returns>
        static IBuffer^ SerializeScript(Platform::String^ script);

        /// <summary>
        /// Parses a serialized script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Windows::Foundation::Metadata::Overload("ParseSerializedScriptWithSource")]
        static JsValue^ ParseScript(Platform::String^ script, IBuffer^ buffer, Platform::String^ sourceName);

        /// <summary>
        /// Parses a script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Windows::Foundation::Metadata::Overload("ParseScriptWithSource")]
        static JsValue^ ParseScript(Platform::String^ script, Platform::String^ sourceName);

        /// <summary>
        /// Parses a serialized script and returns a function representing the script.
        /// Provides the ability to lazy load the script source only if/when it is needed. 
        /// </summary>
        /// <param name="scriptLoadCallback">Callback called when the source code of the script needs to be loaded. </param>
        /// <param name="scriptUnloadCallback">Callback called when the serialized script and source code are no longer needed. </param>
        /// <param name="buffer">The serialized script. </param>
        /// <param name="sourceUrl">The location the script came from. </param>
        /// <returns>A function representing the script code. </returns>
        /// <remarks>
        /// <para>
        /// Requires an active script context. 
        /// </para>
        /// <para>
        /// The runtime will hold on to the buffer until all instances of any functions created from the buffer are garbage collected.
        /// It will then call scriptUnloadCallback to inform the caller that it is safe to release.
        /// </para>
        /// </remarks>
        [Windows::Foundation::Metadata::Overload("ParseSerializedScriptWithCallback")]
        static JsValue^ ParseScript(JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, Platform::String^ sourceUrl);

        /// <summary>
        /// Runs a serialized script.
        /// Provides the ability to lazy load the script source only if/when it is needed.
        /// </summary>
        /// <param name="scriptLoadCallback">Callback called when the source code of the script needs to be loaded. </param>
        /// <param name="scriptUnloadCallback">Callback called when the serialized script and source code are no longer needed. </param>
        /// <param name="buffer">The serialized script. </param>
        /// <param name="sourceUrl">The location the script came from. </param>
        /// <returns>The result of running the script, if any. This parameter can be null. </returns>
        /// <remarks>
        /// <para>
        /// Requires an active script context. 
        /// </para>
        /// <para>
        /// The runtime will hold on to the buffer until all instances of any functions created from the buffer are garbage collected.
        /// It will then call scriptUnloadCallback to inform the caller that it is safe to release.
        /// </para>
        /// </remarks>
        [Windows::Foundation::Metadata::Overload("RunSerializedScriptWithCallback")]
        static JsValue^ RunScript(JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, Platform::String^ sourceUrl);

        /// <summary>
        /// Runs a serialized script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The source code of the serialized script.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by script contexts that have debugging enabled.
        /// </param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>The result of the script, if any.</returns>
        [Windows::Foundation::Metadata::Overload("RunSerializedScriptWithSource")]
        static JsValue^ RunScript(Platform::String^ script, IBuffer^ buffer, Platform::String^ sourceName);

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>The result of the script, if any.</returns>
        [Windows::Foundation::Metadata::Overload("RunScriptWithSource")]
        static JsValue^ RunScript(Platform::String^ script, Platform::String^ sourceName);
    };
};