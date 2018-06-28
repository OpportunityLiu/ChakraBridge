using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript boolean value.
    /// </summary>
    [DebuggerDisplay("{ToBoolean()}")]
    public sealed class JsBoolean : JsValue
    {
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
