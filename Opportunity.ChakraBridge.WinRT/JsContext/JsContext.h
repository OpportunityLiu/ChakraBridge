#pragma once
#include "alias.h"
#include "JsEnum.h"
#include "JsRuntime\JsRuntime.h"
#include "JsContextScope.h"
#include "Value\JsError.h"
#include "Value\JsFunction.h"
#include <queue>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// Called by the runtime to load the source code of the serialized script.
    /// </summary>
    /// <param name="scriptBuffer">The script returned.</param>
    /// <returns>
    /// true if the operation succeeded, false otherwise.
    /// </returns>
    public delegate bool JsSerializedScriptLoadSourceCallback(string^* scriptBuffer);

    /// <summary>
    /// A script context.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Each script context contains its own global object, distinct from the global object in 
    /// other script contexts.
    /// </para>
    /// <para>
    /// Many Chakra hosting APIs require an "active" script context, which can be set using 
    /// Current. Chakra hosting APIs that require a current context to be set will note 
    /// that explicitly in their documentation.
    /// </para>
    /// </remarks>
    public ref class JsContext sealed
    {
#pragma region Instance
    internal:
        JsContextRef Reference;
        JsContext(JsContextRef ref);
    public:
        virtual ~JsContext();

        /// <summary>
        /// Gets the runtime that the context belongs to.
        /// </summary>
        property JsRuntime^ Runtime
        {
            JsRuntime^ get();
        }

        JsContextScope^ Use(bool disposeContext);

#pragma endregion

#pragma region Static
    internal:
        static JsContext^ Get(JsContextRef reference);
        static JsValueRef LastJsError;
        static void GetAndClearExceptionCore();

    public:
        /// <summary>
        /// Gets a value indicating whether the runtime of the current context is in an exception state.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If a call into the runtime results in an exception (either as the result of running a 
        /// script or due to something like a conversion failure), the runtime is placed into an 
        /// "exception state." All calls into any context created by the runtime (except for the 
        /// exception APIs) will fail with <c>InExceptionState</c> until the exception is 
        /// cleared.
        /// </para>
        /// <para>
        /// If the runtime of the current context is in the exception state when a callback returns 
        /// into the engine, the engine will automatically rethrow the exception.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        static property bool HasException
        {
            bool get();
        }

        /// <summary>
        /// Sets the runtime of the current context to an exception state.
        /// </summary>
        /// <param name="exception">
        /// The JavaScript exception to set for the runtime of the current context.
        /// </param>
        /// <remarks>
        /// <para>
        /// If the runtime of the current context is already in an exception state, this API will 
        /// throw <c>JsErrorInExceptionState</c>.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        static void SetException(IJsError^ exception);

        /// <summary>
        ///     Returns the exception that caused the runtime of the current context to be in the
        ///     exception state and resets the exception state for that runtime.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>
        ///    The exception that caused the runtime of the current context to be in the exception state.
        /// </returns>
        static IJsError^ GetAndClearException();

        /// <summary>
        ///    Last error returned by <see cref="GetAndClearException"/>.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        static property IJsError^ LastError { IJsError^ get(); }


        /// <summary>
        /// Gets or sets the current script context on the thread.
        /// </summary>
        static property JsContext^ Current
        {
            JsContext^ get();
            void set(JsContext^ value);
        };

        /// <summary>
        /// Starts debugging in the current context. 
        /// </summary>
        static void StartDebugging();

        /// <summary>
        /// Tells the runtime to do any idle processing it need to do.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If idle processing has been enabled for the current runtime, calling <c>Idle</c> will 
        /// inform the current runtime that the host is idle and that the runtime can perform 
        /// memory cleanup tasks.
        /// </para>
        /// <para>
        /// <c>Idle</c> will also return the number of system ticks until there will be more idle work
        /// for the runtime to do. Calling <c>Idle</c> before this number of ticks has passed will do
        /// no work.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>
        /// The next system tick when there will be more idle work to do. Returns the 
        /// maximum number of ticks if there no upcoming idle work to do.
        /// </returns>
        static uint32 Idle();

        /// <summary>
        /// Project a WinRT namespace. 
        /// </summary>
        /// <param name="namespaceName">The WinRT namespace to be projected. </param>
        /// <remarks>Requires an active script context.</remarks>
        static void ProjectWinRTNamespace(string^ namespaceName);
#pragma endregion

#pragma region Script
    private:
        using IBuffer = Windows::Storage::Streams::IBuffer;
        std::queue<JsValueRef> PromiseContinuationQueue;
        static JsSourceContext SourceContext;
        static void CALLBACK JsContext::JsPromiseContinuationCallbackImpl(_In_ JsValueRef task, _In_opt_ void *callbackState);
    internal:
        static void HandlePromiseContinuation();

    public:
        /// <summary>
        /// Parses a script and returns a <see ref="IJsFunction"/> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to parse.</param>
        /// <returns>A <see ref="IJsFunction"/> representing the script code. </returns>
        [DefaultOverload]
        [Overload("ParseScript")]
        static IJsFunction^ ParseScript(string^ script)
        {
            return ParseScript(script, nullptr);
        }

        /// <summary>
        /// Parses a serialized script and returns a <see ref="IJsFunction"/> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>A <see ref="IJsFunction"/> representing the script code. </returns>
        [Overload("ParseSerializedScript")]
        static IJsFunction^ ParseScript(IBuffer^ buffer)
        {
            return ParseScript(static_cast<string^>(nullptr), buffer, nullptr);
        }

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <returns>The result of the script, if any.</returns>
        [DefaultOverload]
        [Overload("RunScript")]
        static IJsValue^ RunScript(string^ script)
        {
            return RunScript(script, nullptr);
        }

        /// <summary>
        /// Runs a serialized script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>The result of the script, if any.</returns>
        [Overload("RunSerializedScript")]
        static IJsValue^ RunScript(IBuffer^ buffer)
        {
            return RunScript(static_cast<string^>(nullptr), buffer, nullptr);
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
        static IBuffer^ SerializeScript(string^ script);

        /// <summary>
        /// Parses a serialized script and returns a <see ref="IJsFunction"/> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <see ref="IJsFunction"/> representing the script code. </returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("ParseSerializedScriptWithSource")]
        static IJsFunction^ ParseScript(string^ script, IBuffer^ buffer, string^ sourceName);

        /// <summary>
        /// Parses a script and returns a <see ref="IJsFunction"/> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <see ref="IJsFunction"/> representing the script code. </returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("ParseScriptWithSource")]
        static IJsFunction^ ParseScript(string^ script, string^ sourceName);

        /// <summary>
        /// Parses a serialized script and returns a <see ref="IJsFunction"/> representing the script.
        /// Provides the ability to lazy load the script source only if/when it is needed. 
        /// </summary>
        /// <param name="scriptLoadCallback">Callback called when the source code of the script needs to be loaded. </param>
        /// <param name="scriptUnloadCallback">Callback called when the serialized script and source code are no longer needed. </param>
        /// <param name="buffer">The serialized script. </param>
        /// <param name="sourceUrl">The location the script came from. </param>
        /// <returns>A <see ref="IJsFunction"/> representing the script code. </returns>
        /// <remarks>
        /// <para>
        /// Requires an active script context. 
        /// </para>
        /// <para>
        /// The runtime will hold on to the buffer until all instances of any functions created from the buffer are garbage collected.
        /// It will then call scriptUnloadCallback to inform the caller that it is safe to release.
        /// </para>
        /// </remarks>
        [Overload("ParseSerializedScriptWithCallback")]
        static IJsFunction^ ParseScript(JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, string^ sourceUrl);

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
        [Overload("RunSerializedScriptWithCallback")]
        static IJsValue^ RunScript(JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, string^ sourceUrl);

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
        [Overload("RunSerializedScriptWithSource")]
        static IJsValue^ RunScript(string^ script, IBuffer^ buffer, string^ sourceName);

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>The result of the script, if any.</returns>
        [Overload("RunScriptWithSource")]
        static IJsValue^ RunScript(string^ script, string^ sourceName);
#pragma endregion

    };
}
