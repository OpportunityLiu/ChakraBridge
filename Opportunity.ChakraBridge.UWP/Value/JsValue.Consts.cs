namespace Opportunity.ChakraBridge.UWP
{
    partial class JsValue
    {
        /// <summary>
        ///     Gets the value of <c>undefined</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsUndefined Undefined
        {
            get
            {
                Native.JsGetUndefinedValue(out var value).ThrowIfError();
                return new JsUndefined(value);
            }
        }

        /// <summary>
        ///     Gets the value of <c>null</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsNull Null
        {
            get
            {
                Native.JsGetNullValue(out var value).ThrowIfError();
                return new JsNull(value);
            }
        }
    }
}
