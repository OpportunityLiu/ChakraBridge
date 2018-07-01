using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Metadata;

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

        /// <summary>
        /// Creates a JavaScript value that is a projection of the passed in IInspectable pointer. 
        /// </summary>
        /// <param name="inspectable">A IInspectable to be projected. </param>
        /// <returns>A JavaScript value that is a projection of the IInspectable. </returns>
        /// <remarks><para>The projected value can be used by script to call a WinRT object. Hosts are responsible for enforcing COM threading rules. </para>
        /// <para>Requires an active script context. </para></remarks>
        public static JsValueRef FromInspectable([Variant]object inspectable)
        {
            Native.JsInspectableToObject(inspectable, out var obj).ThrowIfError();
            return obj;
        }

        /// <summary>
        /// Unwraps a JavaScript object to an IInspectable pointer 
        /// </summary>
        /// <param name="obj">A JavaScript value that should be projected to IInspectable. </param>
        /// <param name="inspectable">An IInspectable value of the object. </param>
        /// <remarks><para>Hosts are responsible for enforcing COM threading rules. </para>
        /// <para>Requires an active script context. </para></remarks>
        public static void ToInspectable(JsValueRef obj, [Variant]out object inspectable)
        {
            Native.JsObjectToInspectable(obj, out inspectable).ThrowIfError();
        }
    }
}
