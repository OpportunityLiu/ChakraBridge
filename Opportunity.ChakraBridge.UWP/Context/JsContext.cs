using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Windows.Storage.Streams;

namespace Opportunity.ChakraBridge.UWP
{
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
    public sealed partial class JsContext
    {
        /// <summary>
        /// Gets or sets the current script context on the thread.
        /// </summary>
        public static JsContext Current
        {
            get => Get(JsContextRef.Current);
            set
            {
                if (value is null)
                    JsContextRef.Current = JsContextRef.Invalid;
                else
                {
                    JsContextRef.Current = value.Reference;
                    Native.JsSetPromiseContinuationCallback(OnPromiseContinuationCallback, value.Reference.Value).ThrowIfError();
                }
            }
        }

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
        public static bool HasException
        {
            get
            {
                Native.JsHasException(out var hasException).ThrowIfError();
                return hasException;
            }
        }

        /// <summary>
        /// Starts debugging in the current context. 
        /// </summary>
        public static void StartDebugging()
            => Native.JsStartDebugging().ThrowIfError();

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
        public static uint Idle()
        {
            Native.JsIdle(out var ticks).ThrowIfError();
            return ticks;
        }

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
        public static void SetException(JsError exception)
        {
            if (exception == null)
                throw new ArgumentNullException(nameof(exception));
            JsScriptException.SetException(exception.Reference);
        }

        /// <summary>
        /// Project a WinRT namespace. 
        /// </summary>
        /// <param name="namespaceName">The WinRT namespace to be projected. </param>
        /// <remarks>Requires an active script context.</remarks>
        public static void ProjectWinRTNamespace(string namespaceName)
        {
            if (namespaceName == null)
                throw new ArgumentNullException(nameof(namespaceName));
            Native.JsProjectWinRTNamespace(namespaceName).ThrowIfError();
        }
    }
}
