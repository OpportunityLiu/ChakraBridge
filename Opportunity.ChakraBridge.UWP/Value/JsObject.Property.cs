using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.ComponentModel;
using Opportunity.ChakraBridge.UWP;
using Windows.Foundation.Metadata;

[assembly: DebuggerDisplay(@"Count = {Length}", Target = typeof(JsObject.Property[]))]
namespace Opportunity.ChakraBridge.UWP
{
    partial class JsObject
    {
        /// <summary>
        /// Determines whether an object has a property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <returns>Whether the object (or a prototype) has the property.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("ConatinsName")]
        public bool Contains(string id) => RawProperty.HasProperty(this.Reference, id);
        /// <summary>
        /// Determines whether an object has a property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <returns>Whether the object (or a prototype) has the property.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("ConatinsSymbol")]
        public bool Contains(JsSymbol id) => RawProperty.HasProperty(this.Reference, id);

        /// <summary>
        /// Test if an object has a value at the specified index.
        /// </summary>
        /// <param name="index">The index to test.</param>
        /// <returns>Whether the object has an value at the specified index.</returns>
        /// <exception cref="ArgumentNullException"><paramref name="index"/> is <see langword="null"/>.</exception>
        /// <remarks>Requires an active script context.</remarks>
        public bool ContainsAt(JsValue index)
            => RawProperty.HasIndexedProperty(this.Reference, (index ?? throw new ArgumentNullException(nameof(index))).Reference);

        /// <summary>
        /// Gets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <returns>The value of the property.</returns>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// </remarks>
        [DefaultOverload]
        [Overload("GetByName")]
        public JsValue Get(string id) => CreateTyped(RawProperty.GetProperty(this.Reference, id));
        /// <summary>
        /// Gets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <returns>The value of the property.</returns>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// </remarks>
        [Overload("GetBySymbol")]
        public JsValue Get(JsSymbol id) => CreateTyped(RawProperty.GetProperty(this.Reference, id));
        /// <summary>
        /// Sets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The value of the property</param>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// <para>The property set will follow strict mode rules.</para>
        /// </remarks>
        [DefaultOverload]
        [Overload("SetByName")]
        public void Set(string id, JsValue value) => Set(id, value, true);
        /// <summary>
        /// Sets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The value of the property</param>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// <para>The property set will follow strict mode rules.</para>
        /// </remarks>
        [Overload("SetBySymbol")]
        public void Set(JsSymbol id, JsValue value) => Set(id, value, true);
        /// <summary>
        /// Sets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The new value of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("SetByNameWithStrictRules")]
        public void Set(string id, JsValue value, bool useStrictRules)
            => RawProperty.SetProperty(this.Reference, id, value?.Reference ?? RawValue.Null, useStrictRules);
        /// <summary>
        /// Sets an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="value">The new value of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("SetBySymbolWithStrictRules")]
        public void Set(JsSymbol id, JsValue value, bool useStrictRules)
            => RawProperty.SetProperty(this.Reference, id, value?.Reference ?? RawValue.Null, useStrictRules);

        /// <summary>
        /// Gets the value at the specified index of an object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="index">The index of the object.</param>
        /// <returns>The value of the property.</returns>
        /// <exception cref="ArgumentNullException"><paramref name="index"/> is <see langword="null"/>.</exception>
        public JsValue GetAt(JsValue index)
            => CreateTyped(RawProperty.GetIndexedProperty(this.Reference, (index ?? throw new ArgumentNullException(nameof(index))).Reference));

        /// <summary>
        /// Sets the value at the specified index of an object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="index">The index of the object.</param>
        /// <param name="value">The value of the property</param>
        /// <exception cref="ArgumentNullException"><paramref name="index"/> is <see langword="null"/>.</exception>
        public void SetAt(JsValue index, JsValue value)
        {
            if (index is null)
                throw new ArgumentNullException(nameof(index));
            RawProperty.SetIndexedProperty(this.Reference, index.Reference, value?.Reference ?? RawValue.Null);
        }

        /// <summary>
        /// Deletes an object's property.
        /// </summary>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// <para>The property set will follow strict mode rules.</para>
        /// </remarks>
        /// <param name="id">The ID of the property.</param>
        /// <returns>Whether the property was deleted.</returns>
        [DefaultOverload]
        [Overload("RemoveByName")]
        public bool Remove(string id) => Remove(id, true);
        /// <summary>
        /// Deletes an object's property.
        /// </summary>
        /// <remarks>
        /// <para>Requires an active script context.</para>
        /// <para>The property set will follow strict mode rules.</para>
        /// </remarks>
        /// <param name="id">The ID of the property.</param>
        /// <returns>Whether the property was deleted.</returns>
        [Overload("RemoveBySymbol")]
        public bool Remove(JsSymbol id) => Remove(id, true);

        /// <summary>
        /// Deletes an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <returns>Whether the property was deleted.</returns>
        /// <remarks> Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("RemoveByNameWithStrictRules")]
        public bool Remove(string id, bool useStrictRules)
        {
            var r = RawProperty.DeleteProperty(this.Reference, id, useStrictRules);
            return RawBoolean.ToBoolean(r);
        }

        /// <summary>
        /// Deletes an object's property.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="useStrictRules">The property set should follow strict mode rules.</param>
        /// <returns>Whether the property was deleted.</returns>
        /// <remarks> Requires an active script context.</remarks>
        [Overload("RemoveBySymbolWithStrictRules")]
        public bool Remove(JsSymbol id, bool useStrictRules)
        {
            var r = RawProperty.DeleteProperty(this.Reference, id, useStrictRules);
            return RawBoolean.ToBoolean(r);
        }

        /// <summary>
        /// Delete the value at the specified index of an object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="index">The index to delete.</param>
        /// <exception cref="ArgumentNullException"><paramref name="index"/> is <see langword="null"/>.</exception>
        public void RemoveAt(JsValue index)
            => RawProperty.DeleteIndexedProperty(this.Reference, (index ?? throw new ArgumentNullException(nameof(index))).Reference);

        /// <summary>
        /// Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="descriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("AddByName")]
        public bool Add(string id, JsObject descriptor)
            => RawProperty.DefineProperty(this.Reference, id, (descriptor ?? throw new ArgumentNullException(nameof(descriptor))).Reference);

        /// <summary>
        /// Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <param name="id">The ID of the property.</param>
        /// <param name="descriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("AddBySymbol")]
        public bool Add(JsSymbol id, JsObject descriptor)
            => RawProperty.DefineProperty(this.Reference, id, (descriptor ?? throw new ArgumentNullException(nameof(descriptor))).Reference);

        /// <summary>
        /// Gets the list of all symbol properties on the object. 
        /// </summary>
        /// <returns>An array of property symbols. </returns>
        /// <remarks>Requires an active script context.</remarks>
        public JsArray GetOwnPropertySymbols() => new JsArray(RawProperty.GetOwnPropertySymbols(this.Reference));

        /// <summary>
        /// Gets the list of all properties on the object.
        /// </summary>
        /// <returns>An array of property names.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public JsArray GetOwnPropertyNames() => new JsArray(RawProperty.GetOwnPropertyNames(this.Reference));

        /// <summary>
        /// Gets a property descriptor for an object's own property.
        /// </summary>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("GetOwnPropertyDescriptorByName")]
        public JsObject GetOwnPropertyDescriptor(string propertyId)
            => (JsObject)CreateTyped(RawProperty.GetOwnPropertyDescriptor(this.Reference, propertyId));

        /// <summary>
        /// Gets a property descriptor for an object's own property.
        /// </summary>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("GetOwnPropertyDescriptorBySymbol")]
        public JsObject GetOwnPropertyDescriptor(JsSymbol propertyId)
            => (JsObject)CreateTyped(RawProperty.GetOwnPropertyDescriptor(this.Reference, propertyId));

        [DebuggerDisplay(@"{Value}", Name = @"[{Key.ToString(),nq}]", Type = @"{Value.GetType().ToString(),nq}")]
        internal sealed class Property
        {
            public Property(JsPropertyIdRef key, JsValue value)
            {
                this.key = key;
                this.Value = value;
            }

            [DebuggerBrowsable(DebuggerBrowsableState.Never)]
            private readonly JsPropertyIdRef key;

            [DebuggerBrowsable(DebuggerBrowsableState.Never)]
            public JsPropertyId Key => new JsPropertyId(this.key);

            [DebuggerBrowsable(DebuggerBrowsableState.RootHidden)]
            public JsValue Value;
        }

        private Property[] Properties
        {
            get
            {
                var nk = GetOwnPropertyNames();
                var sk = GetOwnPropertySymbols();
                var nkc = nk.Count;
                var skc = sk.Count;
                var r = new Property[nk.Count + sk.Count];
                for (var i = 0; i < nkc; i++)
                {
                    var k = nk[i].ToString();
                    var p = JsPropertyId.FromString(k);
                    r[i] = new Property(p.Reference, this.Get(k));
                }
                for (var i = 0; i < skc; i++)
                {
                    var s = (JsSymbol)sk[i];
                    var p = JsPropertyId.FromSymbol(s);
                    r[i + nkc] = new Property(p.Reference, this.Get(s));
                }
                return r;
            }
        }
    }
}
