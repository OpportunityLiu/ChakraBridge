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

        public override string ToString()
        {
            var func = JsContext.GlobalObject["Symbol"].ToJsObject()["prototype"].ToJsObject()["toString"].ToJsFunction();
            return func.Invoke(this).ToString();
        }
    }
}
