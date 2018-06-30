namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript number value.
    /// </summary>
    public sealed class JsNumber : JsValue
    {
        internal JsNumber(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Retrieves the <see cref="int"/> value of a number value.
        /// </summary>
        /// <returns>The <see cref="int"/> value.</returns>
        /// <remarks>
        /// This function retrieves the value of a number value and converts to an <see cref="int"/> value.
        /// It will fail with <see cref="JsErrorCode.InvalidArgument"/> if the type of the value is not number.
        /// </remarks>
        public int ToInt32() => RawNumber.ToInt32(this.Reference);

        /// <summary>
        /// Retrieves the <see cref="double"/> value of a number value.
        /// </summary>
        /// <returns>The <see cref="double"/> value.</returns>
        /// <remarks>
        /// This function retrieves the value of a number value. It will fail with
        /// <see cref="JsErrorCode.InvalidArgument"/> if the type of the value is not number.
        /// </remarks>
        public double ToDouble() => RawNumber.ToDouble(this.Reference);

        internal double Value => ToDouble();

        /// <summary>
        /// Creates a <see cref="JsNumber"/> value from a <see cref="double"/> value.
        /// </summary>
        /// <param name="doubleValue">The value to be converted.</param>
        /// <returns>The new <see cref="JsNumber"/> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsNumber FromDouble(double doubleValue) => new JsNumber(RawNumber.FromDouble(doubleValue));

        /// <summary>
        /// Creates a <see cref="JsNumber"/> value from a <see cref="int"/> value.
        /// </summary>
        /// <param name="int32Value">The value to be converted.</param>
        /// <returns>The new <see cref="JsNumber"/> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsNumber FromInt32(int int32Value) => new JsNumber(RawNumber.FromInt32(int32Value));

        /// <summary>
        /// Convert the <see cref="JsNumber"/> to its string representaion.
        /// </summary>
        /// <returns>A <see cref="string"/> represent the value of <see cref="JsNumber"/>.</returns>
        public override string ToString() => ToDouble().ToString();
    }
}
