using System;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawString
    {
        /// <summary>
        /// Gets the length of a <c>String</c> value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The length of the string.</returns>
        public static int GetStringLength(JsValueRef stringValue)
        {
            Native.JsGetStringLength(stringValue, out var length).ThrowIfError();
            return length;
        }

        public static string ToString(JsValueRef stringValue)
        {
            Native.JsStringToPointer(stringValue, out var buffer, out var length).ThrowIfError();
            return Marshal.PtrToStringUni(buffer, (int)length);
        }

        /// <summary>
        /// Creates a <c>String</c> value from a string pointer.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="value">The string  to convert to a <c>String</c> value.</param>
        /// <returns>The new <c>String</c> value.</returns>
        /// <exception cref="ArgumentNullException"></exception>
        public static JsValueRef FromString(string value)
        {
            if (value is null)
                throw new ArgumentNullException(nameof(value));
            Native.JsPointerToString(value, new UIntPtr((uint)value.Length), out var reference).ThrowIfError();
            return reference;
        }
    }
}
