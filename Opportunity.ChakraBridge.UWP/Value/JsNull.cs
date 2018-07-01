using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript null value.
    /// </summary>
    public sealed class JsNull : JsValue
    {
        internal JsNull(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Returns <c>"null"</c>.
        /// </summary>
        /// <returns><c>"null"</c></returns>
        public override string ToString() => "null";
    }
}
