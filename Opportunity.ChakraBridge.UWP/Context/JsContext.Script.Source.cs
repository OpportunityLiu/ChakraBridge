using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Metadata;
using Windows.Storage.Streams;

namespace Opportunity.ChakraBridge.UWP
{
    partial class JsContext
    {
        /// <summary>
        /// Parses a serialized script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by script contexts that have debugging enabled.
        /// </param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("ParseSerializedScriptWithContext")]
        public unsafe static JsFunction ParseScript(string script, IBuffer buffer, JsSourceContext sourceContext, string sourceName)
        {
            var pointer = buffer.GetPointer();
            Native.JsParseSerializedScript(script, pointer, sourceContext, sourceName ?? "", out var result).ThrowIfError();
            return new JsFunction(result);
        }

        /// <summary>
        /// Parses a script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <param name="script">The script to parse.</param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by script contexts that have debugging enabled.
        /// </param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("ParseScriptWithContext")]
        public static JsFunction ParseScript(string script, JsSourceContext sourceContext, string sourceName)
        {
            if (script is null)
                throw new ArgumentNullException(nameof(script));
            Native.JsParseScript(script, sourceContext, sourceName ?? "", out var result).ThrowIfError();
            return new JsFunction(result);
        }

        private static JsSerializedScriptLoadSourceCallback LoadSourceCallback;
        private static readonly JsSerializedScriptLoadSourceCallbackPtr OnLoadSource = (JsSourceContextImpl sourceContext, out string scriptBuffer) =>
        {
            scriptBuffer = default;
            if (LoadSourceCallback == null)
                return false;
            try
            {
                return LoadSourceCallback(sourceContext, out scriptBuffer);
            }
            catch
            {
                return false;
            }
        };
        private static JsSerializedScriptUnloadCallback UnloadSourceCallback;
        private static readonly JsSerializedScriptUnloadCallbackPtr OnUnloadSource = (JsSourceContextImpl sourceContext) =>
        {
            UnloadSourceCallback?.Invoke(sourceContext);
        };

        /// <summary>
        /// Parses a serialized script and returns a function representing the script.
        /// Provides the ability to lazy load the script source only if/when it is needed. 
        /// </summary>
        /// <param name="scriptLoadCallback">Callback called when the source code of the script needs to be loaded. </param>
        /// <param name="scriptUnloadCallback">Callback called when the serialized script and source code are no longer needed. </param>
        /// <param name="buffer">The serialized script. </param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by debuggable script contexts. 
        /// This context will passed into scriptLoadCallback and scriptUnloadCallback. 
        /// </param>
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
        [Overload("ParseSerializedScriptWithCallback")]
        public unsafe static JsFunction ParseScript(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, IBuffer buffer, JsSourceContext sourceContext, string sourceUrl)
        {
#pragma warning disable IDE0016 // 使用 "throw" 表达式
            if (scriptLoadCallback == null)
                throw new ArgumentNullException(nameof(scriptLoadCallback));
            if (scriptUnloadCallback == null)
                throw new ArgumentNullException(nameof(scriptUnloadCallback));
            var pointer = buffer.GetPointer();
#pragma warning restore IDE0016 // 使用 "throw" 表达式
            LoadSourceCallback = scriptLoadCallback;
            UnloadSourceCallback = scriptUnloadCallback;
            sourceUrl = sourceUrl ?? "";
            Native.JsParseSerializedScriptWithCallback(OnLoadSource, OnUnloadSource, pointer, sourceContext, sourceUrl, out var r).ThrowIfError();
            return new JsFunction(r);
        }

        /// <summary>
        /// Runs a serialized script.
        /// Provides the ability to lazy load the script source only if/when it is needed.
        /// </summary>
        /// <param name="scriptLoadCallback">Callback called when the source code of the script needs to be loaded. </param>
        /// <param name="scriptUnloadCallback">Callback called when the serialized script and source code are no longer needed. </param>
        /// <param name="buffer">The serialized script. </param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by debuggable script contexts. 
        /// This context will passed into scriptLoadCallback and scriptUnloadCallback. 
        /// </param>
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
        public unsafe static JsValue RunScript(JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            JsSerializedScriptUnloadCallback scriptUnloadCallback, IBuffer buffer, JsSourceContext sourceContext, string sourceUrl)
        {
#pragma warning disable IDE0016 // 使用 "throw" 表达式
            if (scriptLoadCallback == null)
                throw new ArgumentNullException(nameof(scriptLoadCallback));
            if (scriptUnloadCallback == null)
                throw new ArgumentNullException(nameof(scriptUnloadCallback));
            var pointer = buffer.GetPointer();
#pragma warning restore IDE0016 // 使用 "throw" 表达式
            LoadSourceCallback = scriptLoadCallback;
            UnloadSourceCallback = scriptUnloadCallback;
            sourceUrl = sourceUrl ?? "";
            Native.JsRunSerializedScriptWithCallback(OnLoadSource, OnUnloadSource, pointer, sourceContext, sourceUrl, out var result).ThrowIfError();
            var r = JsValue.CreateTyped(result);
            HandlePromiseContinuation();
            return r;
        }

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
        [Overload("RunSerializedScriptWithContext")]
        public static unsafe JsValue RunScript(string script, IBuffer buffer, JsSourceContext sourceContext, string sourceName)
        {
            var pointer = buffer.GetPointer();
            Native.JsRunSerializedScript(script, pointer, sourceContext, sourceName ?? "", out var result).ThrowIfError();
            var r = JsValue.CreateTyped(result);
            HandlePromiseContinuation();
            return r;
        }

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <param name="sourceContext">
        /// A cookie identifying the script that can be used by script contexts that have debugging enabled.
        /// </param>
        /// <param name="sourceName">The location the script came from.</param>
        /// <returns>The result of the script, if any.</returns>
        [Overload("RunScriptWithContext")]
        public static JsValue RunScript(string script, JsSourceContext sourceContext, string sourceName)
        {
            if (script == null)
                throw new ArgumentNullException(nameof(script));
            Native.JsRunScript(script, sourceContext, sourceName ?? "", out var result).ThrowIfError();
            var r = JsValue.CreateTyped(result);
            HandlePromiseContinuation();
            return r;
        }
    }

    /// <summary>
    /// Called by the runtime when it is finished with all resources related to the script execution.
    /// The caller should free the source if loaded, the byte code, and the context at this time.
    /// </summary>
    /// <param name="sourceContext">The context passed to Js[Parse|Run]SerializedScriptWithCallback</param>
    public delegate void JsSerializedScriptUnloadCallback(JsSourceContext sourceContext);
    internal delegate void JsSerializedScriptUnloadCallbackPtr(JsSourceContextImpl sourceContext);

    /// <summary>
    /// Called by the runtime to load the source code of the serialized script.
    /// The caller must keep the script buffer valid until the JsSerializedScriptUnloadCallback.
    /// </summary>
    /// <param name="sourceContext">The context passed to Js[Parse|Run]SerializedScriptWithCallback</param>
    /// <param name="scriptBuffer">The script returned.</param>
    /// <returns>
    /// true if the operation succeeded, false otherwise.
    /// </returns>
    public delegate bool JsSerializedScriptLoadSourceCallback(JsSourceContext sourceContext, out string scriptBuffer);
    internal delegate bool JsSerializedScriptLoadSourceCallbackPtr(JsSourceContextImpl sourceContext, out string scriptBuffer);
}
