using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript string value.
    /// </summary>
    [DebuggerDisplay("{Value}")]
    public sealed class JsString : JsValue
    {
        internal JsString(JsValueRef refernce) : base(refernce) { }

        /// <summary>
        /// Gets the length of a <see cref="JsString"/> value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The length of the string.</returns>
        public int Length => RawString.GetStringLength(this.Reference);

        internal string Value => ToString();

        /// <summary>
        /// Creates a <see cref="JsString"/> value from a <see cref="string"/>.
        /// </summary>
        /// <param name="stringValue">The <see cref="string"/> to convert to a <see cref="JsString"/> value.</param>
        /// <returns>The new <see cref="JsString"/> value.</returns>
        /// <exception cref="ArgumentNullException"></exception>
        /// <remarks>Requires an active script context.</remarks>
        public static JsString FromString(string stringValue) => new JsString(RawString.FromString(stringValue));

        /// <summary>
        /// Retrieves the string pointer of the <see cref="JsString"/> value.
        /// </summary>
        /// <returns>The string.</returns>
        /// <remarks>
        /// <para>
        /// This function retrieves the string pointer of the <see cref="JsString"/> value. It will fail with 
        /// <see cref="JsErrorCode.InvalidArgument"/> if the type of the value is not <see cref="JsString"/>.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        public override sealed string ToString()
        {
            return RawString.ToString(this.Reference);
        }
    }
}
