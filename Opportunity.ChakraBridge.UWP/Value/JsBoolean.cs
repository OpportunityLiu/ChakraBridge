using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript boolean value.
    /// </summary>
    [DebuggerDisplay("{ToBoolean()}")]
    public sealed class JsBoolean : JsValue
    {
        internal JsBoolean(JsValueRef reference) : base(reference) { }

        public bool ToBoolean() => RawBoolean.ToBoolean(this.Reference);

        /// <summary>
        /// Creates a <c>Boolean</c> value from a <c>bool</c> value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="booleanValue">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        public static JsBoolean FromBoolean(bool booleanValue) => new JsBoolean(RawBoolean.FromBoolean(booleanValue));
    }
}
