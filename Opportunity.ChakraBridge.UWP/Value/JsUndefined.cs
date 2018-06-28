namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript undefined value.
    /// </summary>
    public sealed class JsUndefined : JsValue
    {
        internal JsUndefined(JsValueRef refernce) : base(refernce)
        {
        }
    }
}
