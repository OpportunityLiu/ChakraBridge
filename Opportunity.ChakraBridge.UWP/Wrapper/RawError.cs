namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawError
    {
        /// <summary>
        /// Creates a new JavaScript error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateError(JsValueRef message)
        {
            Native.JsCreateError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateRangeError(JsValueRef message)
        {
            Native.JsCreateRangeError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateReferenceError(JsValueRef message)
        {
            Native.JsCreateReferenceError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateSyntaxError(JsValueRef message)
        {
            Native.JsCreateSyntaxError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateTypeError(JsValueRef message)
        {
            Native.JsCreateTypeError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueRef CreateUriError(JsValueRef message)
        {
            Native.JsCreateURIError(message, out var reference).ThrowIfError();
            return reference;
        }
    }
}
