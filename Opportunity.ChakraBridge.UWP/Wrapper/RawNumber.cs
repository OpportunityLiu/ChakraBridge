namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawNumber
    {
        /// <summary>
        /// Retrieves the <see cref="int"/> value of a number value.
        /// </summary>
        /// <param name="numberRef">The number value to convert to an <see cref="int"/> value.</param>
        /// <returns>The <see cref="int"/> value.</returns>
        /// <remarks>
        /// This function retrieves the value of a number value and converts to an <see cref="int"/> value.
        /// It will fail with <see cref="JsErrorCode.InvalidArgument"/> if the type of the value is not number.
        /// </remarks>
        public static int ToInt32(JsValueRef numberRef)
        {
            Native.JsNumberToInt(numberRef, out var value).ThrowIfError();
            return value;
        }

        /// <summary>
        /// Retrieves the <see cref="double"/> value of a number value.
        /// </summary>
        /// <param name="numberRef">The number value to convert to a <see cref="double"/> value.</param>
        /// <returns>The <see cref="double"/> value.</returns>
        /// <remarks>
        /// This function retrieves the value of a number value. It will fail with
        /// <see cref="JsErrorCode.InvalidArgument"/> if the type of the value is not number.
        /// </remarks>
        public static double ToDouble(JsValueRef numberRef)
        {
            Native.JsNumberToDouble(numberRef, out var value).ThrowIfError();
            return value;
        }

        /// <summary>
        /// Creates a <c>Number</c> value from a <c>double</c> value.
        /// </summary>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef FromDouble(double value)
        {
            Native.JsDoubleToNumber(value, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a <c>Number</c> value from a <c>int</c> value.
        /// </summary>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef FromInt32(int value)
        {
            Native.JsIntToNumber(value, out var reference).ThrowIfError();
            return reference;
        }

    }
}
