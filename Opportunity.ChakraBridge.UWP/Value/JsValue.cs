namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;
    using Windows.Foundation.Metadata;

    /// <summary>
    /// A JavaScript value.
    /// </summary>
    /// <remarks>
    /// A JavaScript value is one of the following types of values: Undefined, Null, Boolean, 
    /// String, Number, or Object.
    /// </remarks>
    [DebuggerDisplay("{ToString(),nq}")]
    public partial class JsValue
    {
        internal static JsValue CreateTyped(JsValueRef reference)
        {
            switch (RawValue.GetType(reference))
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
                if (RawObject.HasExternalData(reference))
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
            case JsValueType.TypedArray:
                return JsTypedArray.FromRef(reference);
            case JsValueType.ArrayBuffer:
            case JsValueType.DataView:
            default:
                return new JsObject(reference);
            }
        }

        internal readonly JsValueRef Reference;

        internal JsValue(JsValueRef reference)
        {
            // WinRT disallows abstact classes.
            Debug.Assert(this.GetType() != typeof(JsValue));
            this.Reference = reference;
        }

        /// <summary>
        /// Gets the JavaScript type of the value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The type of the value.</returns>
        public JsValueType ValueType => RawValue.GetType(this.Reference);

        /// <summary>
        /// Converts the value to <c>Boolean</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public JsBoolean ToJsBoolean() => this is JsBoolean b ? b : new JsBoolean(RawOperator.ToJsBoolean(this.Reference));

        /// <summary>
        /// Converts the value to <c>Number</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public JsNumber ToJsNumber() => this is JsNumber n ? n : new JsNumber(RawOperator.ToJsNumber(this.Reference));

        /// <summary>
        /// Converts the value to <c>String</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public JsString ToJsString() => this is JsString s ? s : new JsString(RawOperator.ToJsString(this.Reference));

        /// <summary>
        /// Converts the value to <c>Object</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        public JsObject ToJsObject() => this is JsObject o ? o : (JsObject)CreateTyped(RawOperator.ToJsObject(this.Reference));

        /// <summary>
        /// Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "==" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are equal.</returns>
        public static bool JsEquals(JsValue v1, JsValue v2)
            => RawOperator.Equals((v1 ?? throw new ArgumentNullException(nameof(v1))).Reference, (v2 ?? throw new ArgumentNullException(nameof(v2))).Reference);

        /// <summary>
        /// Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "===" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are strictly equal.</returns>
        public static bool JsStrictEquals(JsValue v1, JsValue v2)
            => RawOperator.StrictEquals((v1 ?? throw new ArgumentNullException(nameof(v1))).Reference, (v2 ?? throw new ArgumentNullException(nameof(v2))).Reference);

        /// <summary>
        /// Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "==" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="other">The object to compare.</param>
        /// <returns>Whether the values are equal.</returns>
        public bool JsEqualsTo(JsValue other)
            => RawOperator.Equals(this.Reference, (other ?? throw new ArgumentNullException(nameof(other))).Reference);

        /// <summary>
        /// Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "===" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="other">The object to compare.</param>
        /// <returns>Whether the values are strictly equal.</returns>
        public bool JsStrictEqualsTo(JsValue other)
            => RawOperator.StrictEquals(this.Reference, (other ?? throw new ArgumentNullException(nameof(other))).Reference);

        /// <summary>
        /// Compare reference of two <see cref="JsValue"/>s.
        /// </summary>
        /// <param name="other">Another object to compare.</param>
        /// <returns><see langword="true"/> if <paramref name="other"/> is <see cref="JsValue"/> that has the same reference with this object.</returns>
        [DefaultOverload]
        public bool Equals(JsValue other) => other is JsValue v && this.Reference == v.Reference;

        /// <summary>
        /// Compare reference of two <see cref="JsValue"/>s.
        /// </summary>
        /// <param name="obj">Another object to compare.</param>
        /// <returns><see langword="true"/> if <paramref name="obj"/> is <see cref="JsValue"/> that has the same reference with this object.</returns>
        public override sealed bool Equals(object obj) => obj is JsValue v && this.Reference == v.Reference;

        /// <summary>
        /// Gets hash code of the reference.
        /// </summary>
        /// <returns>The hash code of the reference.</returns>
        public override sealed int GetHashCode() => this.Reference.GetHashCode();

        /// <summary>
        /// Gets the script context that the object belongs to. 
        /// </summary>
        public JsContext Context => JsContext.GetOrCreate(this.Reference.Context);
    }
}