using System;
using System.Diagnostics;
using Windows.Foundation.Metadata;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// The reference of a script context.
    /// </summary>
    [DebuggerDisplay(@"[{Disp,nq}]")]
    internal readonly struct JsContextRef : IEquatable<JsContextRef>
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private string Disp => this.Value.GetDebugDisp(Native.JS_INVALID_REFERENCE, "(INVALID_REFERENCE)");

        /// <summary>
        /// Gets the runtime that the context belongs to.
        /// </summary>
        internal JsRuntimeHandle Runtime
        {
            get
            {
                Native.JsGetRuntime(this, out var handle).ThrowIfError();
                return handle;
            }
        }

        /// <summary>
        /// Gets the internal data set on <see cref="JsContextRef"/>. 
        /// </summary>
        /// <remarks>Requires an active script context. </remarks>
        public void GetData([Variant]out object data)
        {
            Native.JsGetContextData(this, out data).ThrowIfError();
        }

        /// <summary>
        /// Sets the internal data of <see cref="JsContextRef"/>. 
        /// </summary>
        /// <remarks>Requires an active script context. </remarks>
        public void SetData([Variant]object value) => Native.JsSetContextData(this, value).ThrowIfError();

        /// <summary>
        /// Gets a value indicating whether the value is valid.
        /// </summary>
        public bool IsValid => this.Value != Native.JS_INVALID_REFERENCE;

        /// <summary>
        /// Gets a value indicating whether the value is useable.
        /// </summary>
        public bool IsRuntimeValid
        {
            get
            {
                try
                {
                    return this.IsValid && this.Runtime.IsValid;
                }
                catch (JsUsageException ex) when (ex.ErrorCode == JsErrorCode.InvalidArgument)
                {
                    return false;
                }
            }
        }

        /// <summary>
        /// Add reference to the script context.
        /// </summary>
        internal uint AddRef()
        {
            Native.JsContextAddRef(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        /// Releases reference to the script context.
        /// </summary>
        internal uint Release()
        {
            Native.JsContextRelease(this, out var count).ThrowIfError();
            return count;
        }

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        internal readonly IntPtr Value;

        /// <summary>
        /// Initializes a new instance of the <see cref="JsContextRef"/> struct. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        internal JsContextRef(IntPtr reference)
        {
            this.Value = reference;
        }

        public bool Equals(JsContextRef other) => this.Value == other.Value;

        public override bool Equals(object obj) => obj is JsContextRef other && Equals(other);

        public override int GetHashCode() => this.Value.GetHashCode();

        public static bool operator ==(JsContextRef r1, JsContextRef r2) => r1.Value == r2.Value;
        public static bool operator !=(JsContextRef r1, JsContextRef r2) => r1.Value != r2.Value;

        /// <summary>
        /// Gets or sets the current script context on the thread.
        /// </summary>
        public static JsContextRef Current
        {
            get
            {
                Native.JsGetCurrentContext(out var reference).ThrowIfError();
                return reference;
            }
            set => Native.JsSetCurrentContext(value).ThrowIfError();
        }

        /// <summary>
        /// Gets an invalid value.
        /// </summary>
        public static JsContextRef Invalid => new JsContextRef(Native.JS_INVALID_REFERENCE);
    }
}
