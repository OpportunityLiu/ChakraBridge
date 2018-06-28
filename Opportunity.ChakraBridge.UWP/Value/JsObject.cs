using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{

    /// <summary>
    ///     A JavaScript object value.
    /// </summary>
    public class JsObject : JsValue, IDictionary<JsPropertyId, JsValue>
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
        public JsObject(IntPtr data, JsObjectFinalizeCallback finalizer)
            : this(CreateExternalObject(data, finalizer))
        {
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
        ///     Gets a value indicating whether an object is an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public bool HasExternalData
        {
            get
            {
                Native.JsHasExternalData(this.Reference, out var hasExternalData).ThrowIfError();
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
                Native.JsGetExternalData(this.Reference, out var data).ThrowIfError();
                return data;
            }
            set => Native.JsSetExternalData(this.Reference, value).ThrowIfError();
        }

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<KeyValuePair<JsPropertyId, JsValue>>.IsReadOnly => false;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        ICollection<JsPropertyId> IDictionary<JsPropertyId, JsValue>.Keys
            => Enumerable.Select<JsValue, JsPropertyId>(GetOwnPropertyNames(), v => v.ToString()).ToArray();

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        ICollection<JsValue> IDictionary<JsPropertyId, JsValue>.Values => this.Select(kv => kv.Value).ToArray();

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        int ICollection<KeyValuePair<JsPropertyId, JsValue>>.Count => GetOwnPropertyNames()["length"].ToInt32();

        public JsValue this[JsPropertyId key]
        {
            get => GetProperty(key);
            set => SetProperty(key, value, true);
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

        private static JsValueRef CreateExternalObject(IntPtr data, JsObjectFinalizeCallback finalizer)
        {
            Native.JsCreateExternalObject(data, finalizer, out var reference).ThrowIfError();
            return reference;
        }

        private static JsValueRef CreateObject()
        {
            Native.JsCreateObject(out var reference).ThrowIfError();
            return reference;
        }

        void IDictionary<JsPropertyId, JsValue>.Add(JsPropertyId key, JsValue value)
            => SetProperty(key, value, true);
        bool IDictionary<JsPropertyId, JsValue>.ContainsKey(JsPropertyId key)
            => HasProperty(key);
        bool IDictionary<JsPropertyId, JsValue>.Remove(JsPropertyId key)
            => DeleteProperty(key, true);
        bool IDictionary<JsPropertyId, JsValue>.TryGetValue(JsPropertyId key, out JsValue value)
        {
            value = this[key];
            return true;
        }
        void ICollection<KeyValuePair<JsPropertyId, JsValue>>.Add(KeyValuePair<JsPropertyId, JsValue> item)
            => SetProperty(item.Key, item.Value, true);
        void ICollection<KeyValuePair<JsPropertyId, JsValue>>.Clear()
        {
            foreach (var item in GetOwnPropertyNames())
            {
                this.DeleteIndexedProperty(item);
            }
        }
        bool ICollection<KeyValuePair<JsPropertyId, JsValue>>.Contains(KeyValuePair<JsPropertyId, JsValue> item)
            => this[item.Key].StrictEquals(item.Value);
        void ICollection<KeyValuePair<JsPropertyId, JsValue>>.CopyTo(KeyValuePair<JsPropertyId, JsValue>[] array, int arrayIndex)
        {
            foreach (var item in GetOwnPropertyNames())
            {
                array[arrayIndex++] = new KeyValuePair<JsPropertyId, JsValue>(item.ToString(), this.GetIndexedProperty(item));
                this.DeleteIndexedProperty(item);
            }
        }
        bool ICollection<KeyValuePair<JsPropertyId, JsValue>>.Remove(KeyValuePair<JsPropertyId, JsValue> item)
            => throw new NotImplementedException();
        IEnumerator<KeyValuePair<JsPropertyId, JsValue>> IEnumerable<KeyValuePair<JsPropertyId, JsValue>>.GetEnumerator()
        {
            foreach (var item in GetOwnPropertyNames())
            {
                yield return new KeyValuePair<JsPropertyId, JsValue>(item.ToString(), this.GetIndexedProperty(item));
            }
        }
        IEnumerator IEnumerable.GetEnumerator() => ((IEnumerable<KeyValuePair<JsPropertyId, JsValue>>)this).GetEnumerator();

        private List<KeyValuePair<JsPropertyId, JsValue>> Properties => this.ToList();
    }

    /// <summary>
    ///     A finalization callback.
    /// </summary>
    /// <param name="data">
    ///     The external data that was passed in when creating the object being finalized.
    /// </param>
    public delegate void JsObjectFinalizeCallback(IntPtr data);
}
