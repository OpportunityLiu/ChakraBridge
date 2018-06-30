using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawValue
    {
        /// <summary>
        /// Gets the value of <c>undefined</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef Undefined
        {
            get
            {
                Native.JsGetUndefinedValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        /// Gets the value of <c>null</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef Null
        {
            get
            {
                Native.JsGetNullValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        /// Gets the global object in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GlobalObject
        {
            get
            {
                Native.JsGetGlobalObject(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        /// Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef True
        {
            get
            {
                Native.JsGetTrueValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        /// Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef False
        {
            get
            {
                Native.JsGetFalseValue(out var value).ThrowIfError();
                return value;
            }
        }

        public static JsValueType GetType(JsValueRef value)
        {
            Native.JsGetValueType(value, out var type).ThrowIfError();
            return type;
        }
    }
}
