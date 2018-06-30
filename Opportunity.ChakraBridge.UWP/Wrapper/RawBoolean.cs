namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawBoolean
    {
        public static bool ToBoolean(JsValueRef booleanValue)
        {
            Native.JsBooleanToBool(booleanValue, out var value).ThrowIfError();
            return value;
        }

        /// <summary>
        /// Creates a <c>Boolean</c> value from a <c>bool</c> value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        public static JsValueRef FromBoolean(bool value)
        {
            Native.JsBoolToBoolean(value, out var reference).ThrowIfError();
            return reference;
        }
    }
}
