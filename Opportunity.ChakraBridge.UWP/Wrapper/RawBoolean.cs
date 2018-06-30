namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawBoolean
    {
        /// <summary>
        /// Retrieves the bool value of a <c>Boolean</c> value. 
        /// </summary>
        /// <returns>The converted value. </returns>
        /// <remarks>Requires an active script context.</remarks>
        public static bool ToBoolean(JsValueRef booleanValue)
        {
            Native.JsBooleanToBool(booleanValue, out var value).ThrowIfError();
            return value;
        }

        /// <summary>
        /// Creates a <c>Boolean</c> value from a <c>bool</c> value.
        /// </summary>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef FromBoolean(bool value)
        {
            Native.JsBoolToBoolean(value, out var reference).ThrowIfError();
            return reference;
        }
    }
}
