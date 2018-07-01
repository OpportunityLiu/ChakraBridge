using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript boolean value.
    /// </summary>
    public sealed class JsBoolean : JsValue
    {
        internal JsBoolean(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Retrieves the <see cref="bool"/> value of a <see cref="JsBoolean"/> value. 
        /// </summary>
        /// <returns>The converted value. </returns>
        /// <remarks>Requires an active script context.</remarks>
        public bool Value => RawBoolean.ToBoolean(this.Reference);

        /// <summary>
        /// Creates a <see cref="JsBoolean"/> value from a <see cref="bool"/> value.
        /// </summary>
        /// <param name="booleanValue">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsBoolean FromBoolean(bool booleanValue) => new JsBoolean(RawBoolean.FromBoolean(booleanValue));

        /// <summary>
        /// Convert the <see cref="JsBoolean"/> to its string representaion.
        /// </summary>
        /// <returns>A <see cref="string"/> represent the value of <see cref="JsBoolean"/>.</returns>
        public override string ToString() => Value.ToString();
    }
}
