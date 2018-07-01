namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;

    [DebuggerDisplay(@"[{Disp,nq}]")]
    internal readonly struct JsValueRef : IEquatable<JsValueRef>
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private string Disp => this.Value.GetDebugDisp(Native.JS_INVALID_REFERENCE, "(INVALID_REFERENCE)");

        /// <summary>
        /// Gets an invalid value.
        /// </summary>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public static JsValueRef Invalid { get; } = new JsValueRef(Native.JS_INVALID_REFERENCE);

        /// <summary>
        /// Gets a value indicating whether the value is valid.
        /// </summary>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public bool IsValid => this.Value != Native.JS_INVALID_REFERENCE;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private readonly IntPtr Value;

        /// <summary>
        /// Gets the script context that the object belongs to. 
        /// </summary>
        public JsContextRef Context
        {
            get
            {
                Native.JsGetContextOfObject(this, out var c).ThrowIfError();
                return c;
            }
        }

        /// <summary>
        /// Adds a reference to a garbage collected object. 
        /// </summary>
        /// <returns>The object's new reference count</returns>
        internal uint AddRef()
        {
            Native.JsAddRef(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        /// Releases a reference to a garbage collected object. 
        /// </summary>
        /// <returns>The object's new reference count</returns>
        internal uint Release()
        {
            Native.JsRelease(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="JsValueRef"/> struct.
        /// </summary>
        /// <param name="value">The reference.</param>
        private JsValueRef(IntPtr value)
        {
            this.Value = value;
        }

        public bool Equals(JsValueRef other) => this.Value == other.Value;

        public override bool Equals(object obj) => obj is JsValueRef other && Equals(other);

        public override int GetHashCode() => this.Value.GetHashCode();

        public static bool operator ==(JsValueRef r1, JsValueRef r2) => r1.Value == r2.Value;
        public static bool operator !=(JsValueRef r1, JsValueRef r2) => r1.Value != r2.Value;
    }
}