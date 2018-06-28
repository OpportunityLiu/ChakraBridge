namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;
    using Windows.Storage.Streams;
    partial struct JsContext : IEquatable<JsContext>
    {
        /// <summary>
        ///     The reference.
        /// </summary>
        private readonly IntPtr Reference;

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsContext"/> struct. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        internal JsContext(IntPtr reference)
        {
            this.Reference = reference;
        }

        /// <summary>
        ///     Gets the runtime that the context belongs to.
        /// </summary>
        public JsRuntime Runtime
        {
            get
            {
                var handle = RuntimeHandle;
                if (!handle.IsValid)
                    throw new JsUsageException(JsErrorCode.InvalidArgument, "Not a valid context.");
                return JsRuntime.RuntimeDictionary[RuntimeHandle];
            }
        }

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        internal JsRuntimeHandle RuntimeHandle
        {
            get
            {
                var r = JsErrorCode.NoError;
                var handle = JsRuntimeHandle.Invalid;
                r = Native.JsGetRuntime(this, out handle);
                if (r == JsErrorCode.InvalidArgument)
                    return JsRuntimeHandle.Invalid;
                r.ThrowIfError();
                return handle;
            }
        }

        /// <summary>
        ///     Gets a value indicating whether the context is a valid context or not.
        /// </summary>
        public bool IsValid => this.Reference != IntPtr.Zero && RuntimeHandle.IsValid;

        public bool Equals(JsContext other) => this.Reference == other.Reference;

        public override bool Equals(object obj) => obj is JsContext other && Equals(other);

        public override int GetHashCode() => this.Reference.GetHashCode();

        /// <summary>
        ///  The internal data set on <see cref="JsContext"/>. 
        /// </summary>
        public IntPtr Data
        {
            get
            {
                Native.JsGetContextData(this, out var data).ThrowIfError();
                return data;
            }
            set => Native.JsSetContextData(this, value).ThrowIfError();
        }

        /// <summary>
        ///     Gets an invalid context.
        /// </summary>
        public static JsContext Invalid { get; } = new JsContext(IntPtr.Zero);

        public static bool operator ==(JsContext h1, JsContext h2) => h1.Reference == h2.Reference;
        public static bool operator !=(JsContext h1, JsContext h2) => h1.Reference != h2.Reference;
    }
}
