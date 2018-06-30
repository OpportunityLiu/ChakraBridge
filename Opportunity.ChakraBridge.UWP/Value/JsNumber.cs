namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript number value.
    /// </summary>
    public sealed class JsNumber : JsValue
    {
        internal JsNumber(JsValueRef reference) : base(reference) { }

        public int ToInt32() => RawNumber.ToInt32(this.Reference);

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

        public override string ToString() => ToDouble().ToString();
    }
}
