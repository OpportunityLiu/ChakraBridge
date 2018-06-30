namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawNumber
    {
        public static int ToInt32(JsValueRef numberRef)
        {
            Native.JsNumberToInt(numberRef, out var value).ThrowIfError();
            return value;
        }

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
