namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Concurrent;
    using System.Collections.Generic;
    using System.Threading;
    using Windows.Foundation.Metadata;

    /// <summary>
    /// A Chakra runtime.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Each Chakra runtime has its own independent execution engine, JIT compiler, and garbage 
    /// collected heap. As such, each runtime is completely isolated from other runtimes.
    /// </para>
    /// <para>
    /// Runtimes can be used on any thread, but only one thread can call into a runtime at any 
    /// time.
    /// </para>
    /// <para>
    /// NOTE: A JsRuntime, unlike other objects in the Chakra hosting API, is not 
    /// garbage collected since it contains the garbage collected heap itself. A runtime will 
    /// continue to exist until Dispose is called.
    /// </para>
    /// </remarks>
    public sealed partial class JsRuntime : IDisposable
    {
        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <param name="attributes">The attributes of the runtime to be created.</param>
        /// <returns>The runtime created.</returns>
        [Overload("CreateWithAttributes")]
        public static JsRuntime Create(JsRuntimeAttributes attributes)
        {
            Native.JsCreateRuntime(attributes, ThreadServiceCallback, out var handle).ThrowIfError();
            return new JsRuntime(handle);
        }

        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <returns>The runtime created.</returns>
        [DefaultOverload]
        [Overload("Create")]
        public static JsRuntime Create()
        {
            return Create(JsRuntimeAttributes.None);
        }

        /// <summary>
        /// Gets the current memory usage for a runtime.
        /// </summary>
        /// <remarks>
        /// Memory usage can be always be retrieved, regardless of whether or not the runtime is active
        /// on another thread.
        /// </remarks>
        public ulong MemoryUsage
        {
            get
            {
                Native.JsGetRuntimeMemoryUsage(this.Handle, out var memoryUsage).ThrowIfError();
                return memoryUsage.ToUInt64();
            }
        }

        /// <summary>
        /// Gets or sets the current memory limit for a runtime.
        /// </summary>
        /// <remarks>
        /// The memory limit of a runtime can be always be retrieved, regardless of whether or not the 
        /// runtime is active on another thread.
        /// </remarks>
        public ulong MemoryLimit
        {
            get
            {
                Native.JsGetRuntimeMemoryLimit(this.Handle, out var memoryLimit).ThrowIfError();
                return memoryLimit.ToUInt64();
            }
            set => Native.JsSetRuntimeMemoryLimit(this.Handle, new UIntPtr(value)).ThrowIfError();
        }

        /// <summary>
        /// Gets or sets a value indicating whether script execution is enabled in the runtime.
        /// </summary>
        public bool IsEnabled
        {
            get
            {
                Native.JsIsRuntimeExecutionDisabled(this.Handle, out var isDisabled).ThrowIfError();
                return !isDisabled;
            }
            set
            {
                if (value)
                    Native.JsEnableRuntimeExecution(this.Handle).ThrowIfError();
                else
                    Native.JsDisableRuntimeExecution(this.Handle).ThrowIfError();
            }
        }

        internal readonly Dictionary<JsValueRef, JsNativeFunction> NativeFunctions = new Dictionary<JsValueRef, JsNativeFunction>();

        internal readonly Dictionary<JsValueRef, JsObjectBeforeCollectCallback> ObjectCollectingCallbacks = new Dictionary<JsValueRef, JsObjectBeforeCollectCallback>();

        #region IDisposable Support

        void Dispose(bool disposing)
        {
            if (!this.Handle.IsValid)
                return;
            lock (RuntimeDictionary)
            {
                if (!this.Handle.IsValid)
                    return;
                var currentContext = JsContextRef.Current;
                if (currentContext.IsValid && currentContext.Runtime == this.Handle)
                    JsContextRef.Current = default;
                Native.JsDisposeRuntime(this.Handle).ThrowIfError();
                RuntimeDictionary.Remove(this.Handle);
                this.Handle = JsRuntimeHandle.Invalid;
            }
        }

        /// <summary>
        /// Disposes a runtime.
        /// </summary>
        ~JsRuntime()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(false);
        }


        /// <summary>
        /// Disposes a runtime.
        /// </summary>
        /// <remarks>
        /// Once a runtime has been disposed, all resources owned by it are invalid and cannot be used.
        /// If the runtime is active (i.e. it is set to be current on a particular thread), it cannot 
        /// be disposed.
        /// </remarks>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
