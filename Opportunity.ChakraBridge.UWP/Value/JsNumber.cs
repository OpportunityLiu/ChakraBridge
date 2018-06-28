namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript number value.
    /// </summary>
    public sealed class JsNumber : JsValue
    {
        internal JsNumber(JsValueRef reference) : base(reference)
        {
        }

        public override JsNumber ToJsNumber() => this;

        public override int ToInt32()
        {
            Native.JsNumberToInt(this.Reference, out var value).ThrowIfError();
            return value;
        }

        public override double ToDouble()
        {
            Native.JsNumberToDouble(this.Reference, out var value).ThrowIfError();
            return value;
        }

        /// <summary>
        ///     Creates a <c>Number</c> value from a <c>double</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        public static JsNumber FromDouble(double value)
        {
            Native.JsDoubleToNumber(value, out var reference).ThrowIfError();
            return new JsNumber(reference);
        }

        /// <summary>
        ///     Creates a <c>Number</c> value from a <c>int</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        public static JsNumber FromInt32(int value)
        {
            Native.JsIntToNumber(value, out var reference).ThrowIfError();
            return new JsNumber(reference);
        }

        public static implicit operator double(JsNumber value)
            => value.ToDouble();

        public static explicit operator int(JsNumber value)
            => value.ToInt32();

        public static implicit operator JsNumber(int value)
            => FromInt32(value);

        public static implicit operator JsNumber(double value)
            => FromDouble(value);
    }
}
