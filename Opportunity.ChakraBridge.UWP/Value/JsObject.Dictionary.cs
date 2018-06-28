using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    partial class JsObject : IDictionary<JsPropertyId, JsValue>
    {
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
}
