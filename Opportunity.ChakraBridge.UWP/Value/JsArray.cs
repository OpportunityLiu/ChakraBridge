using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript array value.
    /// </summary>
    [DebuggerDisplay("[{ToString(),nq}]")]
    public class JsArray : JsObject, IList<JsValue>
    {
        internal JsArray(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Creates a JavaScript array object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="length">The initial length of the array.</param>
        /// <exception cref="ArgumentOutOfRangeException"></exception>
        /// <returns>A JavaScript array object.</returns>
        public static JsArray Create(int length) => new JsArray(RawArray.Create(length));

        /// <inheritdoc/>
        public JsValue this[int index]
        {
            get => CreateTyped(RawProperty.GetIndexedProperty(this.Reference, RawNumber.FromInt32(index)));
            set => RawProperty.SetIndexedProperty(this.Reference, RawNumber.FromInt32(index), value?.Reference ?? RawValue.Null);
        }

        /// <summary>
        /// Gets <c>length</c> property of the array.
        /// </summary>
        public int Count => RawNumber.ToInt32(RawProperty.GetProperty(this.Reference, "length"));

        bool ICollection<JsValue>.IsReadOnly => false;

        /// <summary>
        /// Invokes <c>push</c> method of the array.
        /// </summary>
        public void Add(JsValue item)
        {
            var push = RawProperty.GetProperty(this.Reference, "push");
            RawFuction.Invoke(push, this.Reference, item?.Reference ?? RawValue.Null);
        }

        /// <summary>
        /// Sets <c>length</c> property of the array to <c>0</c>.
        /// </summary>
        public void Clear() => RawProperty.SetProperty(this.Reference, "length", RawNumber.FromInt32(0), true);

        /// <summary>
        /// Invokes <c>indexOf</c> method of the array.
        /// </summary>
        public bool Contains(JsValue item) => IndexOf(item) >= 0;
        /// <summary>
        /// Invokes <c>indexOf</c> method of the array.
        /// </summary>
        public int IndexOf(JsValue item)
        {
            var indexOf = RawProperty.GetProperty(this.Reference, "indexOf");
            return RawNumber.ToInt32(RawFuction.Invoke(indexOf, this.Reference, item?.Reference ?? RawValue.Null));
        }

        /// <inheritdoc/>
        public void CopyTo(JsValue[] array, int arrayIndex)
        {
            var c = Count;
            for (var i = 0; i < Count; i++, arrayIndex++)
            {
                array[arrayIndex] = this[i];
            }
        }
        /// <inheritdoc/>
        public IEnumerator<JsValue> GetEnumerator()
        {
            var c = Count;
            for (var i = 0; i < Count; i++)
            {
                yield return this[i];
            }
        }
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        /// <summary>
        /// Invokes <c>splice</c> method of the array.
        /// </summary>
        public void Insert(int index, JsValue item)
        {
            var splice = RawProperty.GetProperty(this.Reference, "splice");
            RawFuction.Invoke(splice, this.Reference, RawNumber.FromInt32(index), RawNumber.FromInt32(0), item?.Reference ?? RawValue.Null);
        }
        /// <summary>
        /// Invokes <c>indexOf</c> and <c>splice</c> method of the array.
        /// </summary>
        public bool Remove(JsValue item)
        {
            var i = IndexOf(item);
            if (i < 0)
                return false;
            RemoveAt(i);
            return true;
        }
        /// <summary>
        /// Invokes <c>splice</c> method of the array.
        /// </summary>
        public void RemoveAt(int index)
        {
            var splice = RawProperty.GetProperty(this.Reference, "splice");
            RawFuction.Invoke(splice, this.Reference, RawNumber.FromInt32(index), RawNumber.FromInt32(1));
        }
    }
}
