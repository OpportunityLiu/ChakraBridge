using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Metadata;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript error object.
    /// </summary>
    public class JsError : JsObject
    {
        internal JsError(JsValueRef reference) : base(reference)
        {
        }

        /// <summary>
        /// <c>message</c> property of Error.
        /// </summary>
        public string Message
        {
            get => this.GetPropertyString("message");
            set => this.SetPropertyString("message", value);
        }

        /// <summary>
        /// <c>name</c> property of Error.
        /// </summary>
        public string Name
        {
            get => this.GetPropertyString("name");
            set => this.SetPropertyString("name", value);
        }

        /// <summary>
        /// Creates a new JavaScript error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateError(JsValue message)
            => new JsError(RawError.CreateError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateError(string message)
            => new JsError(RawError.CreateError(message is null ? RawValue.Undefined : RawString.FromString(message)));

        /// <summary>
        /// Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateRangeError(JsValue message)
            => new JsError(RawError.CreateRangeError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateRangeError(string message)
            => new JsError(RawError.CreateRangeError(message is null ? RawValue.Undefined : RawString.FromString(message)));

        /// <summary>
        /// Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateReferenceError(JsValue message)
            => new JsError(RawError.CreateReferenceError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateReferenceError(string message)
            => new JsError(RawError.CreateReferenceError(message is null ? RawValue.Undefined : RawString.FromString(message)));

        /// <summary>
        /// Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateSyntaxError(JsValue message)
            => new JsError(RawError.CreateSyntaxError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateSyntaxError(string message)
            => new JsError(RawError.CreateSyntaxError(message is null ? RawValue.Undefined : RawString.FromString(message)));

        /// <summary>
        /// Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateTypeError(JsValue message)
            => new JsError(RawError.CreateTypeError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateTypeError(string message)
            => new JsError(RawError.CreateTypeError(message is null ? RawValue.Undefined : RawString.FromString(message)));

        /// <summary>
        /// Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsError CreateUriError(JsValue message)
            => new JsError(RawError.CreateUriError(message?.Reference ?? RawValue.Undefined));

        /// <summary>
        /// Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        public static JsError CreateUriError(string message)
            => new JsError(RawError.CreateUriError(message is null ? RawValue.Undefined : RawString.FromString(message)));
    }
}
