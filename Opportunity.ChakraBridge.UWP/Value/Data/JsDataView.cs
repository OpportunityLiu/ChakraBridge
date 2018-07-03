using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using Windows.Foundation.Metadata;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A Javascript DataView.
    /// </summary>
    public unsafe sealed class JsDataView : JsObject, IList<byte>
    {
        internal JsDataView(JsValueRef reference)
            : base(reference)
        {
            RawArray.GetDataViewStorage(reference, out var buf, out var len);
            this.Count = (int)len;
            this.Buffer = buf;
        }

        /// <summary>
        /// Create new instance of <see cref="JsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <returns>A new instance of <see cref="JsDataView"/>.</returns>
        [Overload("Create")]
        public static JsDataView Create(JsArrayBuffer buffer)
            => Create(buffer, 0);

        /// <summary>
        /// Create new instance of <see cref="JsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <param name="byteOffset">The offset of the created array to <paramref name="buffer"/>.</param>
        /// <returns>A new instance of <see cref="JsDataView"/>.</returns>
        [Overload("CreateWithOffset")]
        public static JsDataView Create(JsArrayBuffer buffer, int byteOffset)
            => Create(buffer, byteOffset, buffer.Count - byteOffset);

        /// <summary>
        /// Create new instance of <see cref="JsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <param name="byteOffset">The offset of the created array to <paramref name="buffer"/>.</param>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsDataView"/>.</returns>
        [Overload("CreateWithOffsetAndLength")]
        public static JsDataView Create(JsArrayBuffer buffer, int byteOffset, int length)
        {
            if (buffer is null)
                throw new ArgumentNullException(nameof(buffer));
            if (byteOffset + length > buffer.Count)
                throw new ArgumentOutOfRangeException();
            return new JsDataView(RawArray.CreateDataView(buffer.Reference, byteOffset, length));
        }

        /// <summary>
        /// Length if buffer in bytes.
        /// </summary>
        public int Count { get; }

        /// <summary>
        /// Buffer pointer of the array data.
        /// </summary>
        private readonly void* Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<byte>.IsReadOnly => false;

        /// <inheritdoc/>
        public byte this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return ((byte*)this.Buffer)[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                ((byte*)this.Buffer)[index] = value;
            }
        }

        /// <inheritdoc/>
        public int IndexOf(byte item)
        {
            var start = ((byte*)this.Buffer);
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        /// <inheritdoc/>
        public bool Contains(byte item) => IndexOf(item) >= 0;
        /// <inheritdoc/>
        public void CopyTo(byte[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (byte* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex), this.Count);
            }
        }
        /// <summary>
        /// Copy data of this array to a new array.
        /// </summary>
        /// <returns>A copy of this array.</returns>
        public byte[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<byte>();
            var r = new byte[Count];
            CopyTo(r, 0);
            return r;
        }
        /// <inheritdoc/>
        public IEnumerator<byte> GetEnumerator() => ((IEnumerable<byte>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        /// <summary>Invalid operation.</summary>
        /// <exception href="InvalidOperationException">The array is fixed size.</exception>
        public void Insert(int index, byte item) => throw new InvalidOperationException("The array is fixed size.");
        /// <summary>Invalid operation.</summary>
        /// <exception href="InvalidOperationException">The array is fixed size.</exception>
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        /// <summary>Invalid operation.</summary>
        /// <exception href="InvalidOperationException">The array is fixed size.</exception>
        public void Add(byte item) => throw new InvalidOperationException("The array is fixed size.");
        /// <summary>Invalid operation.</summary>
        /// <exception href="InvalidOperationException">The array is fixed size.</exception>
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        /// <summary>Invalid operation.</summary>
        /// <exception href="InvalidOperationException">The array is fixed size.</exception>
        public bool Remove(byte item) => throw new InvalidOperationException("The array is fixed size.");
    }
}
