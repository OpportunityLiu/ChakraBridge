namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.ComponentModel;
    using System.Diagnostics;

    internal readonly struct JsSourceContextImpl
    {
        private readonly UIntPtr Value;

        internal JsSourceContextImpl(UIntPtr value) => this.Value = value;
        internal unsafe JsSourceContextImpl(ulong value) : this(new UIntPtr(unchecked((void*)value))) { }
        internal JsSourceContextImpl(JsSourceContext value) : this(value.Value) { }

        internal JsSourceContext ToContext() => JsSourceContextExtension.FromUInt64(this.Value.ToUInt64());

        public static JsSourceContextImpl operator +(JsSourceContextImpl left, int right)
            => new JsSourceContextImpl(left.Value + right);

        public static JsSourceContextImpl operator -(JsSourceContextImpl left, int right)
            => new JsSourceContextImpl(left.Value - right);

        public static implicit operator JsSourceContext(JsSourceContextImpl v) => v.ToContext();
        public static implicit operator JsSourceContextImpl(JsSourceContext v) => new JsSourceContextImpl(v);
    }

    /// <summary>
    /// A cookie that identifies a script for debugging purposes.
    /// </summary>
    [DebuggerDisplay(@"[{JsSourceContextExtension.GetDisp(Value),nq}]")]
    public readonly struct JsSourceContext
    {
        /// <summary>
        /// The context.
        /// </summary>
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        public readonly ulong Value;
    }

    /// <summary>
    /// Extension methods for <see cref="JsSourceContext"/>.
    /// </summary>
    public static class JsSourceContextExtension
    {
        private static string GetDisp(ulong value)
            => value == Native.JS_SOURCE_CONTEXT_NONE.ToUInt64() ? "(SOURCE_CONTEXT_NONE)" : value.ToString();

        /// <summary>
        /// Gets an empty source context.
        /// </summary>
        public static JsSourceContext None => FromUInt64(Native.JS_SOURCE_CONTEXT_NONE.ToUInt64());


        /// <summary>
        /// Subtracts an offset from the value of the source context.
        /// </summary>
        /// <param name="left">The source context to subtract the offset from.</param>
        /// <param name="right">The offset to subtract.</param>
        /// <returns>A new source context that reflects the subtraction of the offset from the context.</returns>
        public static JsSourceContext Subtract(this JsSourceContext left, int right) => (JsSourceContextImpl)left - right;

        /// <summary>
        /// Decrements the value of the source context.
        /// </summary>
        /// <param name="left">The source context to decrement.</param>
        /// <returns>A new source context that reflects the decrementing of the context.</returns>
        public static JsSourceContext Decrement(this JsSourceContext left) => (JsSourceContextImpl)left - 1;

        /// <summary>
        /// Adds an offset from the value of the source context.
        /// </summary>
        /// <param name="left">The source context to add the offset to.</param>
        /// <param name="right">The offset to add.</param>
        /// <returns>A new source context that reflects the addition of the offset to the context.</returns>
        public static JsSourceContext Add(this JsSourceContext left, int right) => (JsSourceContextImpl)left + right;

        /// <summary>
        /// Increments the value of the source context.
        /// </summary>
        /// <param name="left">The source context to increment.</param>
        /// <returns>A new source context that reflects the incrementing of the context.</returns>
        public static JsSourceContext Increment(this JsSourceContext left) => (JsSourceContextImpl)left + 1;

        /// <summary>
        /// Creates a new source context. 
        /// </summary>
        /// <param name="cookie">
        /// The cookie for the source context.
        /// </param>
        /// <returns>The new source context.</returns>
        public unsafe static JsSourceContext FromUInt64(ulong cookie) => *(JsSourceContext*)&cookie;

        /// <summary>
        /// Checks for equality between source contexts.
        /// </summary>
        /// <returns>Whether the two source contexts are the same.</returns>
        public static bool Equals(this JsSourceContext left, JsSourceContext right) => left.Value == right.Value;
    }
}