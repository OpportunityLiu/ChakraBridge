using System;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawProperty
    {
        #region Property

        /// <summary>
        /// Gets an object's property.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="id">The ID of the property.</param>
        /// <returns>The value of the property.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GetProperty(JsValueRef obj, JsPropertyId id)
        {
            if (!id.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(id));
            Native.JsGetProperty(obj, id, out var propertyReference).ThrowIfError();
            return propertyReference;
        }

        /// <summary>
        /// Sets an object's property.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The new value of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <remarks>Requires an active script context.</remarks>
        public static void SetProperty(JsValueRef obj, JsPropertyId id, JsValueRef value, bool useStrictRules)
        {
            if (!id.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(id));
            Native.JsSetProperty(obj, id, value, useStrictRules).ThrowIfError();
        }

        /// <summary>
        /// Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="id">The ID of the property.</param>
        /// <param name="propertyDescriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static bool DefineProperty(JsValueRef obj, JsPropertyId id, JsValueRef propertyDescriptor)
        {
            if (!id.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(id));
            Native.JsDefineProperty(obj, id, propertyDescriptor, out var result).ThrowIfError();
            return result;
        }

        /// <summary>
        /// Deletes an object's property.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="id">The ID of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <returns>Whether the property was deleted.</returns>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// <para>The property set will follow strict mode rules.</para>
        /// </remarks>
        public static JsValueRef DeleteProperty(JsValueRef obj, JsPropertyId id, bool useStrictRules)
        {
            if (!id.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(id));
            Native.JsDeleteProperty(obj, id, useStrictRules, out var returnReference).ThrowIfError();
            return returnReference;
        }

        /// <summary>
        /// Determines whether an object has a property.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="id">The ID of the property.</param>
        /// <returns>Whether the object (or a prototype) has the property.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static bool HasProperty(JsValueRef obj, JsPropertyId id)
        {
            if (!id.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(id));
            Native.JsHasProperty(obj, id, out var hasProperty).ThrowIfError();
            return hasProperty;
        }

        #endregion Property

        #region IndexedProperty

        /// <summary>
        /// Test if an object has a value at the specified index.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="index">The index to test.</param>
        /// <returns>Whether the object has an value at the specified index.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static bool HasIndexedProperty(JsValueRef obj, JsValueRef index)
        {
            Native.JsHasIndexedProperty(obj, index, out var hasProperty).ThrowIfError();
            return hasProperty;
        }

        /// <summary>
        /// Retrieve the value at the specified index of an object.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="index">The index to retrieve.</param>
        /// <returns>The retrieved value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GetIndexedProperty(JsValueRef obj, JsValueRef index)
        {
            Native.JsGetIndexedProperty(obj, index, out var propertyReference).ThrowIfError();
            return propertyReference;
        }

        /// <summary>
        /// Set the value at the specified index of an object.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="index">The index to set.</param>
        /// <param name="value">The value to set.</param>
        /// <remarks>Requires an active script context.</remarks>
        public static void SetIndexedProperty(JsValueRef obj, JsValueRef index, JsValueRef value)
        {
            Native.JsSetIndexedProperty(obj, index, value).ThrowIfError();
        }

        /// <summary>
        /// Delete the value at the specified index of an object.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="index">The index to delete.</param>
        /// <remarks>Requires an active script context.</remarks>
        public static void DeleteIndexedProperty(JsValueRef obj, JsValueRef index)
        {
            Native.JsDeleteIndexedProperty(obj, index).ThrowIfError();
        }

        #endregion IndexedProperty

        /// <summary>
        /// Gets the list of all symbol properties on the object. 
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <returns>An array of property symbols. </returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GetOwnPropertySymbols(JsValueRef obj)
        {
            Native.JsGetOwnPropertySymbols(obj, out var propertySymbols).ThrowIfError();
            return propertySymbols;
        }

        /// <summary>
        /// Gets the list of all properties on the object.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <returns>An array of property names.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GetOwnPropertyNames(JsValueRef obj)
        {
            Native.JsGetOwnPropertyNames(obj, out var propertyNamesReference).ThrowIfError();
            return propertyNamesReference;
        }

        /// <summary>
        /// Gets a property descriptor for an object's own property.
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef GetOwnPropertyDescriptor(JsValueRef obj, JsPropertyId propertyId)
        {
            if (!propertyId.IsValid)
                throw new ArgumentException("Property id is invalid.", nameof(propertyId));
            Native.JsGetOwnPropertyDescriptor(obj, propertyId, out var descriptorReference).ThrowIfError();
            return descriptorReference;
        }
    }
}
