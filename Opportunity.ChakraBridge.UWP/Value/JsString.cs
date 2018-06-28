using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript string value.
    /// </summary>
    [DebuggerDisplay("{ToString()}")]
    public sealed class JsString : JsValue
    {
        internal JsString(JsValueRef refernce) : base(refernce)
        {
        }

        /// <summary>
        ///     Gets the length of a <c>String</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The length of the string.</returns>
        public int Length
        {
            get
            {
                Native.JsGetStringLength(this.Reference, out var length).ThrowIfError();
                return length;
            }
        }

        public override JsString ToJsString() => this;

        public override string ToString()
        {
            Native.JsStringToPointer(this.Reference, out var buffer, out var length).ThrowIfError();
            return Marshal.PtrToStringUni(buffer, (int)length);
        }

        /// <summary>
        ///     Creates a <c>String</c> value from a string pointer.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The string  to convert to a <c>String</c> value.</param>
        /// <returns>The new <c>String</c> value.</returns>
        public static JsString FromString(string value)
        {
            if (value == null)
                throw new ArgumentNullException(nameof(value));
            Native.JsPointerToString(value, new UIntPtr((uint)value.Length), out var reference).ThrowIfError();
            return new JsString(reference);
        }

        public static implicit operator string(JsString value)
            => value.ToString();

        public static implicit operator JsString(string value)
            => FromString(value);
    }
}
