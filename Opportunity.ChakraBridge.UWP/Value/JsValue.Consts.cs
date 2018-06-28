﻿namespace Opportunity.ChakraBridge.UWP
{
    partial class JsValue
    {
        /// <summary>
        ///     Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsBoolean True
        {
            get
            {
                Native.JsGetTrueValue(out var value).ThrowIfError();
                return new JsBoolean(value);
            }
        }

        /// <summary>
        ///     Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsBoolean False
        {
            get
            {
                Native.JsGetFalseValue(out var value).ThrowIfError();
                return new JsBoolean(value);
            }
        }

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