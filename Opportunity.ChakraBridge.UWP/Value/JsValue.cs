namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;

    /// <summary>
    ///     A JavaScript value.
    /// </summary>
    /// <remarks>
    ///     A JavaScript value is one of the following types of values: Undefined, Null, Boolean, 
    ///     String, Number, or Object.
    /// </remarks>
    [DebuggerDisplay("{ToString(),nq}")]
    public abstract partial class JsValue
    {
        internal static JsValue CreateTyped(JsValueRef reference)
        {
            Native.JsGetValueType(reference, out var type).ThrowIfError();
            switch (type)
            {
            case JsValueType.Undefined:
                return new JsUndefined(reference);
            case JsValueType.Null:
                return new JsNull(reference);
            case JsValueType.Number:
                return new JsNumber(reference);
            case JsValueType.String:
                return new JsString(reference);
            case JsValueType.Boolean:
                return new JsBoolean(reference);
            case JsValueType.Object:
                Native.JsHasExternalData(reference, out var hasExternalData).ThrowIfError();
                if (hasExternalData)
                    return new JsExtenalObject(reference);
                else
                    return new JsObject(reference);
            case JsValueType.Function:
                return new JsFunction(reference);
            case JsValueType.Error:
                return new JsError(reference);
            case JsValueType.Array:
                return new JsArray(reference);
            case JsValueType.Symbol:
                return new JsSymbol(reference);
            case JsValueType.ArrayBuffer:
            case JsValueType.TypedArray:
            case JsValueType.DataView:
            default:
                return new JsObject(reference);
            }
        }

        internal readonly JsValueRef Reference;

        internal JsValue(JsValueRef reference)
        {
            this.Reference = reference;
        }

        /// <summary>
        ///     Gets the JavaScript type of the value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The type of the value.</returns>
        public JsValueType ValueType
        {
            get
            {
                Native.JsGetValueType(this.Reference, out var type).ThrowIfError();
                return type;
            }
        }

        /// <summary>
        ///     Adds a reference to the object.
        /// </summary>
        /// <remarks>
        ///     This only needs to be called on objects that are not going to be stored somewhere on 
        ///     the stack. Calling AddRef ensures that the JavaScript object the value refers to will not be freed 
        ///     until Release is called
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        public uint AddRef()
        {
            Native.JsAddRef(this.Reference, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        ///     Releases a reference to the object.
        /// </summary>
        /// <remarks>
        ///     Removes a reference that was created by AddRef.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        public uint Release()
        {
            Native.JsRelease(this.Reference, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        ///     Retrieves the <c>bool</c> value of a <c>Boolean</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual bool ToBoolean()
        {
            try
            {
                Native.JsBooleanToBool(this.Reference, out var value).ThrowIfError();
                return value;
            }
            catch
            {
                return ToJsBoolean().ToBoolean();
            }
        }

        /// <summary>
        ///     Retrieves the <c>double</c> value of a <c>Number</c> value.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function retrieves the value of a Number value. It will fail with 
        ///     <c>InvalidArgument</c> if the type of the value is not <c>Number</c>.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The <c>double</c> value.</returns>
        public virtual double ToDouble()
        {
            try
            {
                Native.JsNumberToDouble(this.Reference, out var value).ThrowIfError();
                return value;
            }
            catch
            {
                return ToJsNumber().ToDouble();
            }
        }

        /// <summary>
        ///     Retrieves the <c>int</c> value of a <c>Number</c> value.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function retrieves the value of a Number value. It will fail with
        ///     <c>InvalidArgument</c> if the type of the value is not <c>Number</c>.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The <c>int</c> value.</returns>
        public virtual int ToInt32()
        {
            try
            {
                Native.JsNumberToInt(this.Reference, out var value).ThrowIfError();
                return value;
            }
            catch
            {
                return ToJsNumber().ToInt32();
            }
        }

        /// <summary>
        ///     Retrieves the string pointer of a <c>String</c> value.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function retrieves the string pointer of a <c>String</c> value. It will fail with 
        ///     <c>InvalidArgument</c> if the type of the value is not <c>String</c>.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The string.</returns>
        public override string ToString()
        {
            try
            {
                Native.JsStringToPointer(this.Reference, out var buffer, out var length).ThrowIfError();
                return Marshal.PtrToStringUni(buffer, (int)length);
            }
            catch
            {
                return ToJsString();
            }
        }

        /// <summary>
        ///     Converts the value to <c>Boolean</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual JsBoolean ToJsBoolean()
        {
            Native.JsConvertValueToBoolean(this.Reference, out var booleanReference).ThrowIfError();
            return new JsBoolean(booleanReference);
        }

        /// <summary>
        ///     Converts the value to <c>Number</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual JsNumber ToJsNumber()
        {
            Native.JsConvertValueToNumber(this.Reference, out var numberReference).ThrowIfError();
            return new JsNumber(numberReference);
        }

        /// <summary>
        ///     Converts the value to <c>String</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual JsString ToJsString()
        {
            Native.JsConvertValueToString(this.Reference, out var stringReference).ThrowIfError();
            return new JsString(stringReference);
        }

        /// <summary>
        ///     Converts the value to <c>Object</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual JsObject ToJsObject()
        {
            Native.JsConvertValueToObject(this.Reference, out var objectReference).ThrowIfError();
            return new JsObject(objectReference);
        }

        /// <summary>
        ///     Converts the value to <c>Function</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public virtual JsFunction ToJsFunction()
        {
            if (this.ValueType == JsValueType.Function)
                return new JsFunction(this.Reference);
            throw new InvalidOperationException();
        }

        /// <summary>
        ///     Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function is equivalent to the "==" operator in JavaScript.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="other">The object to compare.</param>
        /// <returns>Whether the values are equal.</returns>
        public bool Equals(JsValue other)
        {
            Native.JsEquals(this.Reference, other.Reference, out var equals).ThrowIfError();
            return equals;
        }

        /// <summary>
        ///     Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function is equivalent to the "===" operator in JavaScript.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="other">The object to compare.</param>
        /// <returns>Whether the values are strictly equal.</returns>
        public bool StrictEquals(JsValue other)
        {
            Native.JsStrictEquals(this.Reference, other.Reference, out var equals).ThrowIfError();
            return equals;
        }

        public override bool Equals(object obj) => obj is JsValue v && this.Reference.Value == v.Reference.Value;

        public override int GetHashCode() => this.Reference.GetHashCode();

        public JsContext Context
        {
            get
            {
                Native.JsGetContextOfObject(this.Reference, out var c).ThrowIfError();
                return c;
            }
        }
    }
}