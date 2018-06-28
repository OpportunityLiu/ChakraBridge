using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript symbol value.
    /// </summary>
    public sealed class JsSymbol : JsValue
    {
        internal JsSymbol(JsValueRef reference) : base(reference)
        {
        }

        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        /// <param name="description">The string description of the symbol. Can be null. </param>
        public JsSymbol(JsString description)
            : this(CreateSymbol(description?.Reference ?? JsValueRef.Invalid))
        {

        }

        private static JsValueRef CreateSymbol(JsValueRef description)
        {
            Native.JsCreateSymbol(description, out var symbol).ThrowIfError();
            return symbol;
        }

        public override string ToString()
        {
            var func = JsContext.GlobalObject["Symbol"].ToJsObject()["prototype"].ToJsObject()["toString"].ToJsFunction();
            return func.Invoke(this).ToString();
        }
    }
}
