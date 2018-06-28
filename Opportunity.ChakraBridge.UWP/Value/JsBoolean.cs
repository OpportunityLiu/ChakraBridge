using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript boolean value.
    /// </summary>
    [DebuggerDisplay("{ToBoolean()}")]
    public sealed class JsBoolean : JsValue
    {
        /// <summary>
        ///     Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsBoolean True
        {
            get
            {
                Native.JsGetTrueValue(out var value).ThrowIfError();
                return new JsBoolean(value);
            }
        }

        /// <summary>
        ///     Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsBoolean False
        {
            get
            {
                Native.JsGetFalseValue(out var value).ThrowIfError();
                return new JsBoolean(value);
            }
        }

        internal JsBoolean(JsValueRef reference) : base(reference)
        {
        }

        public override JsBoolean ToJsBoolean() => this;
        public override bool ToBoolean()
        {
            Native.JsBooleanToBool(this.Reference, out var value).ThrowIfError();
            return value;
        }

        public static implicit operator bool(JsBoolean value)
            => value.ToBoolean();

        public static implicit operator JsBoolean(bool value)
            => FromBoolean(value);

        /// <summary>
        ///     Creates a <c>Boolean</c> value from a <c>bool</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        public static JsBoolean FromBoolean(bool value)
        {
            Native.JsBoolToBoolean(value, out var reference).ThrowIfError();
            return new JsBoolean(reference);
        }
    }
}
