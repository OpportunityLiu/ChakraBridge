using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript symbol value.
    /// </summary>
    public sealed class JsSymbol : JsValue
    {
        internal JsSymbol(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        /// <param name="description">The string description of the symbol. Can be <see langword="null"/>.</param>
        public static JsSymbol Create(JsString description)
            => new JsSymbol(RawSymbol.CreateSymbol(description?.Reference ?? JsValueRef.Invalid));

        /// <summary>
        /// Get the debug string of <see cref="JsSymbol"/>. Will invoke <c>Symbol.prototype.toString()</c>
        /// </summary>
        /// <returns>The debug string of <see cref="JsSymbol"/>.</returns>
        public override string ToString()
        {
            var symbol = RawProperty.GetProperty(RawValue.GlobalObject, "Symbol");
            var symbolprototype = RawProperty.GetProperty(symbol, "prototype");
            var symboltoString = RawProperty.GetProperty(symbolprototype, "toString");
            var stringResult = RawFuction.Invoke(symboltoString, this.Reference);
            return RawString.ToString(stringResult);
        }
    }
}
