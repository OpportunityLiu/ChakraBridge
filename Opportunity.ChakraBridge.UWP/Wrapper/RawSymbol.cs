namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawSymbol
    {
        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        /// <param name="description">The string description of the symbol. Can be null. </param>
        public static JsValueRef CreateSymbol(JsValueRef description)
        {
            Native.JsCreateSymbol(description, out var symbol).ThrowIfError();
            return symbol;
        }
    }
}
