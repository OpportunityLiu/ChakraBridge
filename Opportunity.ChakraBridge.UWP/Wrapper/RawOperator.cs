namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawOperator
    {
        /// <summary>
        /// Converts the value to <c>Boolean</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public static JsValueRef ToJsBoolean(JsValueRef value)
        {
            Native.JsConvertValueToBoolean(value, out var booleanReference).ThrowIfError();
            return booleanReference;
        }

        /// <summary>
        /// Converts the value to <c>Number</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public static JsValueRef ToJsNumber(JsValueRef value)
        {
            Native.JsConvertValueToNumber(value, out var numberReference).ThrowIfError();
            return numberReference;
        }

        /// <summary>
        /// Converts the value to <c>String</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public static JsValueRef ToJsString(JsValueRef value)
        {
            Native.JsConvertValueToString(value, out var stringReference).ThrowIfError();
            return stringReference;
        }

        /// <summary>
        /// Converts the value to <c>Object</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public static JsValueRef ToJsObject(JsValueRef value)
        {
            Native.JsConvertValueToObject(value, out var objectReference).ThrowIfError();
            return objectReference;
        }

        /// <summary>
        /// Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "==" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are equal.</returns>
        public static bool Equals(JsValueRef v1, JsValueRef v2)
        {
            Native.JsEquals(v1, v2, out var equals).ThrowIfError();
            return equals;
        }

        /// <summary>
        /// Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "===" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are strictly equal.</returns>
        public static bool StrictEquals(JsValueRef v1, JsValueRef v2)
        {
            Native.JsStrictEquals(v1, v2, out var equals).ThrowIfError();
            return equals;
        }
    }
}
