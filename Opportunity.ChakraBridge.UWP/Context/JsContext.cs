namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using Windows.Storage.Streams;

    /// <summary>
    ///     A script context.
    /// </summary>
    /// <remarks>
    ///     <para>
    ///     Each script context contains its own global object, distinct from the global object in 
    ///     other script contexts.
    ///     </para>
    ///     <para>
    ///     Many Chakra hosting APIs require an "active" script context, which can be set using 
    ///     Current. Chakra hosting APIs that require a current context to be set will note 
    ///     that explicitly in their documentation.
    ///     </para>
    /// </remarks>
    public readonly partial struct JsContext
    {
        /// <summary>
        ///     Gets the global object in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsObject GlobalObject
        {
            get
            {
                Native.JsGetGlobalObject(out var value).ThrowIfError();
                return new JsObject(value);
            }
        }

        /// <summary>
        ///     Gets an invalid context.
        /// </summary>
        public static JsContext Invalid { get; } = new JsContext(default);

        /// <summary>
        ///     The reference.
        /// </summary>
        private readonly IntPtr reference;

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsContext"/> struct. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        internal JsContext(IntPtr reference)
        {
            this.reference = reference;
        }

        /// <summary>
        ///     Gets or sets the current script context on the thread.
        /// </summary>
        public static JsContext Current
        {
            get
            {
                Native.JsGetCurrentContext(out var reference).ThrowIfError();
                return reference;
            }
            set => Native.JsSetCurrentContext(value).ThrowIfError();
        }

        /// <summary>
        ///     Gets a value indicating whether the runtime of the current context is in an exception state.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If a call into the runtime results in an exception (either as the result of running a 
        ///     script or due to something like a conversion failure), the runtime is placed into an 
        ///     "exception state." All calls into any context created by the runtime (except for the 
        ///     exception APIs) will fail with <c>InExceptionState</c> until the exception is 
        ///     cleared.
        ///     </para>
        ///     <para>
        ///     If the runtime of the current context is in the exception state when a callback returns 
        ///     into the engine, the engine will automatically rethrow the exception.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
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
        ///     Gets the runtime that the context belongs to.
        /// </summary>
        public JsRuntime Runtime
        {
            get
            {
                Native.JsGetRuntime(this, out var handle).ThrowIfError();
                return JsRuntime.RuntimeDictionary[handle];
            }
        }

        /// <summary>
        ///     Gets a value indicating whether the context is a valid context or not.
        /// </summary>
        public bool IsValid => this.reference != IntPtr.Zero;

        /// <summary>
        ///     Tells the runtime to do any idle processing it need to do.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If idle processing has been enabled for the current runtime, calling <c>Idle</c> will 
        ///     inform the current runtime that the host is idle and that the runtime can perform 
        ///     memory cleanup tasks.
        ///     </para>
        ///     <para>
        ///     <c>Idle</c> will also return the number of system ticks until there will be more idle work
        ///     for the runtime to do. Calling <c>Idle</c> before this number of ticks has passed will do
        ///     no work.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>
        ///     The next system tick when there will be more idle work to do. Returns the 
        ///     maximum number of ticks if there no upcoming idle work to do.
        /// </returns>
        public static uint Idle()
        {
            Native.JsIdle(out var ticks).ThrowIfError();
            return ticks;
        }

        /// <summary>
        ///     Returns the exception that caused the runtime of the current context to be in the 
        ///     exception state and resets the exception state for that runtime.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If the runtime of the current context is not in an exception state, this API will throw
        ///     <c>JsErrorInvalidArgument</c>. If the runtime is disabled, this will return an exception
        ///     indicating that the script was terminated, but it will not clear the exception (the 
        ///     exception will be cleared if the runtime is re-enabled using 
        ///     <c>EnableRuntimeExecution</c>).
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The exception for the runtime of the current context.</returns>
        public static JsError GetAndClearException()
        {
            Native.JsGetAndClearException(out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Sets the runtime of the current context to an exception state.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If the runtime of the current context is already in an exception state, this API will 
        ///     throw <c>JsErrorInExceptionState</c>.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="exception">
        ///     The JavaScript exception to set for the runtime of the current context.
        /// </param>
        public static void SetException(JsError exception)
        {
            Native.JsSetException(exception.Reference).ThrowIfError();
        }

        /// <summary>
        ///     Adds a reference to a script context.
        /// </summary>
        /// <remarks>
        ///     Calling AddRef ensures that the context will not be freed until Release is called.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        public uint AddRef()
        {
            Native.JsContextAddRef(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        ///     Releases a reference to a script context.
        /// </summary>
        /// <remarks>
        ///     Removes a reference to a context that was created by AddRef.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        public uint Release()
        {
            Native.JsContextRelease(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        ///     Starts debugging in the current context. 
        /// </summary>
        public static void StartDebugging()
            => Native.JsStartDebugging().ThrowIfError();
    }
}
