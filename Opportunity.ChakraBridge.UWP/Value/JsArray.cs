using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///     A JavaScript array value.
    /// </summary>
    [DebuggerDisplay("[{ToString(),nq}]")]
    public class JsArray : JsObject, IList<JsValue>
    {
        /// <summary>
        ///     Creates a JavaScript array object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="length">The initial length of the array.</param>
        /// <returns>The new array object.</returns>
        public static JsArray Create(uint length)
        {
            Native.JsCreateArray(length, out var reference).ThrowIfError();
            return new JsArray(reference);
        }

        internal JsArray(JsValueRef reference) : base(reference)
        {
        }

        public JsValue this[int index] { get => GetIndexedProperty((JsNumber)index); set => SetIndexedProperty((JsNumber)index, value); }

        public int Count => this["length"].ToInt32();

        bool ICollection<JsValue>.IsReadOnly => false;

        public void Add(JsValue item) => this["push"].ToJsFunction().Invoke(this, item);
        public void Clear() => this["length"] = (JsNumber)0;
        public bool Contains(JsValue item) => IndexOf(item) >= 0;
        public void CopyTo(JsValue[] array, int arrayIndex)
        {
            var c = Count;
            for (var i = 0; i < Count; i++, arrayIndex++)
            {
                array[arrayIndex] = this[i];
            }
        }
        public IEnumerator<JsValue> GetEnumerator()
        {
            var c = Count;
            for (var i = 0; i < Count; i++)
            {
                yield return this[i];
            }
        }
        public int IndexOf(JsValue item) => this["indexOf"].ToJsFunction().Invoke(this, item).ToInt32();
        public void Insert(int index, JsValue item) => this["splice"].ToJsFunction().Invoke(this, (JsNumber)index, (JsNumber)0, item);
        public bool Remove(JsValue item)
        {
            var i = IndexOf(item);
            if (i < 0)
                return false;
            RemoveAt(i);
            return true;
        }
        public void RemoveAt(int index) => this["splice"].ToJsFunction().Invoke(this, (JsNumber)index, (JsNumber)1);
    }
}
