namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.InteropServices;

    /// <summary>
    ///     A JavaScript value.
    /// </summary>
    /// <remarks>
    ///     A JavaScript value is one of the following types of values: Undefined, Null, Boolean, 
    ///     String, Number, or Object.
    /// </remarks>
    internal readonly struct JsValueReference
    {
        /// <summary>
        /// The reference.
        /// </summary>
        private readonly IntPtr reference;

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsValueReference"/> struct.
        /// </summary>
        /// <param name="reference">The reference.</param>
        private JsValueReference(IntPtr reference)
        {
            this.reference = reference;
        }

        /// <summary>
        ///     Gets an invalid value.
        /// </summary>
        public static JsValueReference Invalid { get; } = new JsValueReference(IntPtr.Zero);

        /// <summary>
        ///     Gets the value of <c>undefined</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsValueReference Undefined
        {
            get
            {
                Native.JsGetUndefinedValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        ///     Gets the value of <c>null</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsValueReference Null
        {
            get
            {
                Native.JsGetNullValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        ///     Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsValueReference True
        {
            get
            {
                Native.JsGetTrueValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        ///     Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsValueReference False
        {
            get
            {
                Native.JsGetFalseValue(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        ///     Gets the global object in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public static JsValueReference GlobalObject
        {
            get
            {
                Native.JsGetGlobalObject(out var value).ThrowIfError();
                return value;
            }
        }

        /// <summary>
        ///     Gets a value indicating whether the value is valid.
        /// </summary>
        public bool IsValid => this.reference != IntPtr.Zero;

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
                Native.JsGetValueType(this, out var type).ThrowIfError();
                return type;
            }
        }

        /// <summary>
        ///     Gets the length of a <c>String</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The length of the string.</returns>
        public int StringLength
        {
            get
            {
                Native.JsGetStringLength(this, out var length).ThrowIfError();
                return length;
            }
        }

        /// <summary>
        ///     Gets or sets the prototype of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public JsValueReference Prototype
        {
            get
            {
                Native.JsGetPrototype(this, out var prototypeReference).ThrowIfError();
                return prototypeReference;
            }
            set => Native.JsSetPrototype(this, value).ThrowIfError();
        }

        /// <summary>
        ///     Gets a value indicating whether an object is extensible or not.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public bool IsExtensionAllowed
        {
            get
            {
                Native.JsGetExtensionAllowed(this, out var allowed).ThrowIfError();
                return allowed;
            }
        }

        /// <summary>
        ///     Gets a value indicating whether an object is an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public bool HasExternalData
        {
            get
            {
                Native.JsHasExternalData(this, out var hasExternalData).ThrowIfError();
                return hasExternalData;
            }
        }

        /// <summary>
        ///     Gets or sets the data in an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public IntPtr ExternalData
        {
            get
            {
                Native.JsGetExternalData(this, out var data).ThrowIfError();
                return data;
            }
            set => Native.JsSetExternalData(this, value).ThrowIfError();
        }

        /// <summary>
        ///     Creates a <c>Boolean</c> value from a <c>bool</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        public static JsValueReference FromBoolean(bool value)
        {
            Native.JsBoolToBoolean(value, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a <c>Number</c> value from a <c>double</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        public static JsValueReference FromDouble(double value)
        {
            Native.JsDoubleToNumber(value, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a <c>Number</c> value from a <c>int</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The new <c>Number</c> value.</returns>
        public static JsValueReference FromInt32(int value)
        {
            Native.JsIntToNumber(value, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a <c>String</c> value from a string pointer.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The string  to convert to a <c>String</c> value.</param>
        /// <returns>The new <c>String</c> value.</returns>
        public static JsValueReference FromString(string value)
        {
            Native.JsPointerToString(value, new UIntPtr((uint)value.Length), out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new <c>Object</c>.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The new <c>Object</c>.</returns>
        public static JsValueReference CreateObject()
        {
            Native.JsCreateObject(out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new <c>Object</c> that stores some external data.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="data">External data that the object will represent. May be null.</param>
        /// <param name="finalizer">
        ///     A callback for when the object is finalized. May be null.
        /// </param>
        /// <returns>The new <c>Object</c>.</returns>
        public static JsValueReference CreateExternalObject(IntPtr data, JsObjectFinalizeCallback finalizer)
        {
            Native.JsCreateExternalObject(data, finalizer, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <returns>The new function object.</returns>
        public static JsValueReference CreateFunction(JsNativeFunction function)
        {
            Native.JsCreateFunction(function, IntPtr.Zero, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="callbackData">Data to be provided to all function callbacks.</param>
        /// <returns>The new function object.</returns>
        public static JsValueReference CreateFunction(JsNativeFunction function, IntPtr callbackData)
        {
            Native.JsCreateFunction(function, callbackData, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a JavaScript array object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="length">The initial length of the array.</param>
        /// <returns>The new array object.</returns>
        public static JsValueReference CreateArray(uint length)
        {
            Native.JsCreateArray(length, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateError(JsValueReference message)
        {
            Native.JsCreateError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateRangeError(JsValueReference message)
        {
            Native.JsCreateRangeError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateReferenceError(JsValueReference message)
        {
            Native.JsCreateReferenceError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateSyntaxError(JsValueReference message)
        {
            Native.JsCreateSyntaxError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateTypeError(JsValueReference message)
        {
            Native.JsCreateTypeError(message, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        public static JsValueReference CreateUriError(JsValueReference message)
        {
            Native.JsCreateURIError(message, out var reference).ThrowIfError();
            return reference;
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
            Native.JsAddRef(this, out var count).ThrowIfError();
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
            Native.JsRelease(this, out var count).ThrowIfError();
            return count;
        }

        /// <summary>
        ///     Retrieves the <c>bool</c> value of a <c>Boolean</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public bool ToBoolean()
        {
            Native.JsBooleanToBool(this, out var value).ThrowIfError();
            return value;
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
        public double ToDouble()
        {
            Native.JsNumberToDouble(this, out var value).ThrowIfError();
            return value;
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
        public int ToInt32()
        {
            Native.JsNumberToInt(this, out var value).ThrowIfError();
            return value;
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
        public new string ToString()
        {
            Native.JsStringToPointer(this, out var buffer, out var length).ThrowIfError();
            return Marshal.PtrToStringUni(buffer, (int)length);
        }

        /// <summary>
        ///     Converts the value to <c>Boolean</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public JsValueReference ConvertToBoolean()
        {
            JsValueReference booleanReference;
            Native.JsConvertValueToBoolean(this, out booleanReference).ThrowIfError();
            return booleanReference;
        }

        /// <summary>
        ///     Converts the value to <c>Number</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public JsValueReference ConvertToNumber()
        {
            JsValueReference numberReference;
            Native.JsConvertValueToNumber(this, out numberReference).ThrowIfError();
            return numberReference;
        }

        /// <summary>
        ///     Converts the value to <c>String</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public JsValueReference ConvertToString()
        {
            JsValueReference stringReference;
            Native.JsConvertValueToString(this, out stringReference).ThrowIfError();
            return stringReference;
        }

        /// <summary>
        ///     Converts the value to <c>Object</c> using regular JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        public JsValueReference ConvertToObject()
        {
            JsValueReference objectReference;
            Native.JsConvertValueToObject(this, out objectReference).ThrowIfError();
            return objectReference;
        }

        /// <summary>
        ///     Sets an object to not be extensible.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public void PreventExtension()
        {
            Native.JsPreventExtension(this).ThrowIfError();
        }

        /// <summary>
        ///     Gets a property descriptor for an object's own property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        public JsValueReference GetOwnPropertyDescriptor(JsPropertyId propertyId)
        {
            JsValueReference descriptorReference;
            Native.JsGetOwnPropertyDescriptor(this, propertyId, out descriptorReference).ThrowIfError();
            return descriptorReference;
        }

        /// <summary>
        ///     Gets the list of all properties on the object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>An array of property names.</returns>
        public JsValueReference GetOwnPropertyNames()
        {
            JsValueReference propertyNamesReference;
            Native.JsGetOwnPropertyNames(this, out propertyNamesReference).ThrowIfError();
            return propertyNamesReference;
        }

        /// <summary>
        ///     Determines whether an object has a property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>Whether the object (or a prototype) has the property.</returns>
        public bool HasProperty(JsPropertyId propertyId)
        {
            bool hasProperty;
            Native.JsHasProperty(this, propertyId, out hasProperty).ThrowIfError();
            return hasProperty;
        }

        /// <summary>
        ///     Gets an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="id">The ID of the property.</param>
        /// <returns>The value of the property.</returns>
        public JsValueReference GetProperty(JsPropertyId id)
        {
            JsValueReference propertyReference;
            Native.JsGetProperty(this, id, out propertyReference).ThrowIfError();
            return propertyReference;
        }

        /// <summary>
        ///     Sets an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The new value of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        public void SetProperty(JsPropertyId id, JsValueReference value, bool useStrictRules)
        {
            Native.JsSetProperty(this, id, value, useStrictRules).ThrowIfError();
        }

        /// <summary>
        ///     Deletes an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="propertyId">The ID of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <returns>Whether the property was deleted.</returns>
        public JsValueReference DeleteProperty(JsPropertyId propertyId, bool useStrictRules)
        {
            JsValueReference returnReference;
            Native.JsDeleteProperty(this, propertyId, useStrictRules, out returnReference).ThrowIfError();
            return returnReference;
        }

        /// <summary>
        ///     Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="propertyId">The ID of the property.</param>
        /// <param name="propertyDescriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        public bool DefineProperty(JsPropertyId propertyId, JsValueReference propertyDescriptor)
        {
            bool result;
            Native.JsDefineProperty(this, propertyId, propertyDescriptor, out result).ThrowIfError();
            return result;
        }

        /// <summary>
        ///     Test if an object has a value at the specified index.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to test.</param>
        /// <returns>Whether the object has an value at the specified index.</returns>
        public bool HasIndexedProperty(JsValueReference index)
        {
            bool hasProperty;
            Native.JsHasIndexedProperty(this, index, out hasProperty).ThrowIfError();
            return hasProperty;
        }

        /// <summary>
        ///     Retrieve the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to retrieve.</param>
        /// <returns>The retrieved value.</returns>
        public JsValueReference GetIndexedProperty(JsValueReference index)
        {
            JsValueReference propertyReference;
            Native.JsGetIndexedProperty(this, index, out propertyReference).ThrowIfError();
            return propertyReference;
        }

        /// <summary>
        ///     Set the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to set.</param>
        /// <param name="value">The value to set.</param>
        public void SetIndexedProperty(JsValueReference index, JsValueReference value)
        {
            Native.JsSetIndexedProperty(this, index, value).ThrowIfError();
        }

        /// <summary>
        ///     Delete the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to delete.</param>
        public void DeleteIndexedProperty(JsValueReference index)
        {
            Native.JsDeleteIndexedProperty(this, index).ThrowIfError();
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
        public bool Equals(JsValueReference other)
        {
            bool equals;
            Native.JsEquals(this, other, out equals).ThrowIfError();
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
        public bool StrictEquals(JsValueReference other)
        {
            bool equals;
            Native.JsStrictEquals(this, other, out equals).ThrowIfError();
            return equals;
        }

        /// <summary>
        ///     Invokes a function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation, if any.</returns>
        public JsValueReference CallFunction(params JsValueReference[] arguments)
        {
            JsValueReference returnReference;

            if (arguments.Length > ushort.MaxValue)
            {
                throw new ArgumentOutOfRangeException("arguments");
            }

            Native.JsCallFunction(this, arguments, (ushort)arguments.Length, out returnReference).ThrowIfError();
            return returnReference;
        }

        /// <summary>
        ///     Invokes a function as a constructor.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation.</returns>
        public JsValueReference ConstructObject(params JsValueReference[] arguments)
        {
            JsValueReference returnReference;

            if (arguments.Length > ushort.MaxValue)
            {
                throw new ArgumentOutOfRangeException("arguments");
            }

            Native.JsConstructObject(this, arguments, (ushort)arguments.Length, out returnReference).ThrowIfError();
            return returnReference;
        }
    }
}
