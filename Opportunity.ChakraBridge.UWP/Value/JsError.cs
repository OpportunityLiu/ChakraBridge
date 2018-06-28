using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript error object.
    /// </summary>
    public class JsError : JsObject
    {
        internal JsError(JsValueRef reference) : base(reference)
        {
        }

        /// <summary>
        ///     Creates a new JavaScript error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateError(JsValue message)
        {
            Native.JsCreateError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateRangeError(JsValue message)
        {
            Native.JsCreateRangeError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateReferenceError(JsValue message)
        {
            Native.JsCreateReferenceError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateSyntaxError(JsValue message)
        {
            Native.JsCreateSyntaxError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateTypeError(JsValue message)
        {
            Native.JsCreateTypeError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        ///     Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateUriError(JsValue message)
        {
            Native.JsCreateURIError(message.Reference, out var reference).ThrowIfError();
            return new JsError(reference);
        }
    }
}
