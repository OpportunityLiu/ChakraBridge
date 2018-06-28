namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript null value.
    /// </summary>
    public sealed class JsNull : JsValue
    {
        internal JsNull(JsValueRef reference) : base(reference)
        {
        }
    }
}
