using System;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript object.
    /// </summary>
    public partial class JsObject : JsValue
    {
        internal JsObject(JsValueRef reference) : base(reference)
        {
        }

        /// <summary>
        ///     Creates a new <c>Object</c>.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public JsObject()
            : this(CreateObject())
        {
        }

        private static JsValueRef CreateObject()
        {
            Native.JsCreateObject(out var reference).ThrowIfError();
            return reference;
        }

        public override JsObject ToJsObject() => this;

        /// <summary>
        ///     Gets or sets the prototype of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public JsObject Prototype
        {
            get
            {
                Native.JsGetPrototype(this.Reference, out var prototypeReference).ThrowIfError();
                return CreateTyped(prototypeReference) as JsObject;
            }
            set => Native.JsSetPrototype(this.Reference, value.Reference).ThrowIfError();
        }

        /// <summary>
        ///     Sets an object to not be extensible.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public void PreventExtension()
        {
            Native.JsPreventExtension(this.Reference).ThrowIfError();
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
                Native.JsGetExtensionAllowed(this.Reference, out var allowed).ThrowIfError();
                return allowed;
            }
        }

        /// <summary>
        /// Gets the list of all symbol properties on the object. 
        /// </summary>
        public JsArray GetOwnPropertySymbols()
        {
            Native.JsGetOwnPropertySymbols(this.Reference, out var propertySymbols).ThrowIfError();
            return new JsArray(propertySymbols);
        }

        /// <summary>
        ///     Gets a property descriptor for an object's own property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        public JsObject GetOwnPropertyDescriptor(JsPropertyId propertyId)
        {
            Native.JsGetOwnPropertyDescriptor(this.Reference, propertyId, out var descriptorReference).ThrowIfError();
            return new JsObject(descriptorReference);
        }

        /// <summary>
        ///     Gets the list of all properties on the object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>An array of property names.</returns>
        public JsArray GetOwnPropertyNames()
        {
            Native.JsGetOwnPropertyNames(this.Reference, out var propertyNamesReference).ThrowIfError();
            return new JsArray(propertyNamesReference);
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
            Native.JsHasProperty(this.Reference, propertyId, out var hasProperty).ThrowIfError();
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
        public JsValue GetProperty(JsPropertyId id)
        {
            Native.JsGetProperty(this.Reference, id, out var propertyReference).ThrowIfError();
            return CreateTyped(propertyReference);
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
        public void SetProperty(JsPropertyId id, JsValue value, bool useStrictRules)
        {
            Native.JsSetProperty(this.Reference, id, value.Reference, useStrictRules).ThrowIfError();
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
        public bool DeleteProperty(JsPropertyId propertyId, bool useStrictRules)
        {
            Native.JsDeleteProperty(this.Reference, propertyId, useStrictRules, out var returnReference).ThrowIfError();
            return new JsBoolean(returnReference).ToBoolean();
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
        public bool DefineProperty(JsPropertyId propertyId, JsObject propertyDescriptor)
        {
            Native.JsDefineProperty(this.Reference, propertyId, propertyDescriptor.Reference, out var result).ThrowIfError();
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
        public bool HasIndexedProperty(JsValue index)
        {
            Native.JsHasIndexedProperty(this.Reference, index.Reference, out var hasProperty).ThrowIfError();
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
        public JsValue GetIndexedProperty(JsValue index)
        {
            Native.JsGetIndexedProperty(this.Reference, index.Reference, out var propertyReference).ThrowIfError();
            return CreateTyped(propertyReference);
        }

        /// <summary>
        ///     Set the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to set.</param>
        /// <param name="value">The value to set.</param>
        public void SetIndexedProperty(JsValue index, JsValue value)
        {
            Native.JsSetIndexedProperty(this.Reference, index.Reference, value.Reference).ThrowIfError();
        }

        /// <summary>
        ///     Delete the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to delete.</param>
        public void DeleteIndexedProperty(JsValue index)
        {
            Native.JsDeleteIndexedProperty(this.Reference, index.Reference).ThrowIfError();
        }

        /// <summary>
        /// Performs JavaScript "instanceof" operator test. 
        /// </summary>
        /// <param name="constructor">The constructor function to test against. </param>
        /// <returns>Whether the "object instanceof constructor" is <see langword="true"/>. </returns>
        public bool InstanceOf(JsFunction constructor)
        {
            Native.JsInstanceOf(this.Reference, constructor.Reference, out var r).ThrowIfError();
            return r;
        }

        /// <summary>
        /// Sets a callback function that is called by the runtime before garbage collection of an object. 
        /// </summary>
        /// <param name="callback">The callback function being set. Use <see langword="null"/> to clear previously registered callback. </param>
        public void SetObjectBeforeCollectCallback(JsObjectBeforeCollectCallback callback)
        {
            if (callback == null)
            {
                Native.JsSetObjectBeforeCollectCallback(this.Reference, default, null).ThrowIfError();
                return;
            }
            var tb = JsContext.Current.Runtime.ObjectBeforeCollectTable;
            var i = tb.GetNextPos();
            Native.JsSetObjectBeforeCollectCallback(this.Reference, i, ObjectBeforeCollectCallback).ThrowIfError();
            tb.Add(i, callback);
        }


        /// <summary>
        ///     A callback called before collecting an object.
        /// </summary>
        /// <remarks>
        ///     Use <c>JsSetObjectBeforeCollectCallback</c> to register this callback.
        /// </remarks>
        /// <param name="reference">The object to be collected.</param>
        /// <param name="callbackState">The state passed to <c>JsSetObjectBeforeCollectCallback</c>.</param>
        private static void ObjectBeforeCollectCallback(JsValueRef reference, IntPtr callbackState)
        {
            var tb = JsContext.Current.Runtime.ObjectBeforeCollectTable;
            var cb = tb.RemoveAt(callbackState);
            cb((JsObject)CreateTyped(reference));
        }
    }

    /// <summary>
    ///     A callback called before collecting an object.
    /// </summary>
    /// <remarks>
    ///     Use <c>JsSetObjectBeforeCollectCallback</c> to register this callback.
    /// </remarks>
    /// <param name="reference">The object to be collected.</param>
    /// <param name="callbackState">The state passed to <c>JsSetObjectBeforeCollectCallback</c>.</param>
    internal delegate void JsObjectBeforeCollectCallbackPtr(JsValueRef reference, IntPtr callbackState);

    /// <summary>
    ///     A callback called before collecting an object.
    /// </summary>
    /// <remarks>
    ///     Use <c>JsSetObjectBeforeCollectCallback</c> to register this callback.
    /// </remarks>
    /// <param name="obj">The object to be collected.</param>
    public delegate void JsObjectBeforeCollectCallback(JsObject obj);
}
