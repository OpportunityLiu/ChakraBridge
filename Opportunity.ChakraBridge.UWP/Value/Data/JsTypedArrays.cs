using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation.Metadata;
using Windows.Storage.Streams;

namespace Opportunity.ChakraBridge.UWP
{

    /// <summary>
    /// A Javascript Int8 array.
    /// </summary>
    public unsafe sealed class JsInt8Array : JsTypedArray, IList<byte>
    {
        internal JsInt8Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsInt8Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsInt8Array"/>.</returns>
        public static new JsInt8Array Create()
            => (JsInt8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int8, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsInt8Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsInt8Array"/>.</returns>
        [DefaultOverload]
        public static JsInt8Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsInt8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int8, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt8Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsInt8Array"/>.</returns>
        public static JsInt8Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsInt8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int8, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt8Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsInt8Array"/>.</returns>
        public static JsInt8Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsInt8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int8, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(byte);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(byte));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte* Pointer => (byte*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<byte>.IsReadOnly => false;

        public byte this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(byte item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(byte item) => IndexOf(item) >= 0;
        public void CopyTo(byte[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (byte* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public byte[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<byte>();
            var r = new byte[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<byte> GetEnumerator() => ((IEnumerable<byte>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Uint8 array.
    /// </summary>
    public unsafe sealed class JsUint8Array : JsTypedArray, IList<byte>
    {
        internal JsUint8Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsUint8Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsUint8Array"/>.</returns>
        public static new JsUint8Array Create()
            => (JsUint8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsUint8Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsUint8Array"/>.</returns>
        [DefaultOverload]
        public static JsUint8Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsUint8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint8Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsUint8Array"/>.</returns>
        public static JsUint8Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsUint8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint8Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsUint8Array"/>.</returns>
        public static JsUint8Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsUint8Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(byte);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(byte));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte* Pointer => (byte*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<byte>.IsReadOnly => false;

        public byte this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(byte item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(byte item) => IndexOf(item) >= 0;
        public void CopyTo(byte[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (byte* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public byte[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<byte>();
            var r = new byte[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<byte> GetEnumerator() => ((IEnumerable<byte>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Uint8Clamped array.
    /// </summary>
    public unsafe sealed class JsUint8ClampedArray : JsTypedArray, IList<byte>
    {
        internal JsUint8ClampedArray(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsUint8ClampedArray"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsUint8ClampedArray"/>.</returns>
        public static new JsUint8ClampedArray Create()
            => (JsUint8ClampedArray)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8Clamped, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsUint8ClampedArray"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsUint8ClampedArray"/>.</returns>
        [DefaultOverload]
        public static JsUint8ClampedArray Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsUint8ClampedArray)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8Clamped, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint8ClampedArray"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsUint8ClampedArray"/>.</returns>
        public static JsUint8ClampedArray Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsUint8ClampedArray)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8Clamped, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint8ClampedArray"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsUint8ClampedArray"/>.</returns>
        public static JsUint8ClampedArray Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsUint8ClampedArray)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint8Clamped, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(byte);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(byte));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte* Pointer => (byte*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<byte>.IsReadOnly => false;

        public byte this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(byte item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(byte item) => IndexOf(item) >= 0;
        public void CopyTo(byte[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (byte* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(byte item) => throw new InvalidOperationException("The array is fixed size.");
        public byte[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<byte>();
            var r = new byte[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<byte> GetEnumerator() => ((IEnumerable<byte>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Int16 array.
    /// </summary>
    public unsafe sealed class JsInt16Array : JsTypedArray, IList<short>
    {
        internal JsInt16Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsInt16Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsInt16Array"/>.</returns>
        public static new JsInt16Array Create()
            => (JsInt16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int16, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsInt16Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsInt16Array"/>.</returns>
        [DefaultOverload]
        public static JsInt16Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsInt16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int16, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt16Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsInt16Array"/>.</returns>
        public static JsInt16Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsInt16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int16, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt16Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsInt16Array"/>.</returns>
        public static JsInt16Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsInt16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int16, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(short);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(short));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private short* Pointer => (short*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<short>.IsReadOnly => false;

        public short this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(short item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, short item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(short item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(short item) => IndexOf(item) >= 0;
        public void CopyTo(short[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (short* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(short item) => throw new InvalidOperationException("The array is fixed size.");
        public short[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<short>();
            var r = new short[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<short> GetEnumerator() => ((IEnumerable<short>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Uint16 array.
    /// </summary>
    public unsafe sealed class JsUint16Array : JsTypedArray, IList<ushort>
    {
        internal JsUint16Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsUint16Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsUint16Array"/>.</returns>
        public static new JsUint16Array Create()
            => (JsUint16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint16, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsUint16Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsUint16Array"/>.</returns>
        [DefaultOverload]
        public static JsUint16Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsUint16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint16, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint16Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsUint16Array"/>.</returns>
        public static JsUint16Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsUint16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint16, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint16Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsUint16Array"/>.</returns>
        public static JsUint16Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsUint16Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint16, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(ushort);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(ushort));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private ushort* Pointer => (ushort*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<ushort>.IsReadOnly => false;

        public ushort this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(ushort item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, ushort item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(ushort item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(ushort item) => IndexOf(item) >= 0;
        public void CopyTo(ushort[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (ushort* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(ushort item) => throw new InvalidOperationException("The array is fixed size.");
        public ushort[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<ushort>();
            var r = new ushort[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<ushort> GetEnumerator() => ((IEnumerable<ushort>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Int32 array.
    /// </summary>
    public unsafe sealed class JsInt32Array : JsTypedArray, IList<int>
    {
        internal JsInt32Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsInt32Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsInt32Array"/>.</returns>
        public static new JsInt32Array Create()
            => (JsInt32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int32, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsInt32Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsInt32Array"/>.</returns>
        [DefaultOverload]
        public static JsInt32Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsInt32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int32, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt32Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsInt32Array"/>.</returns>
        public static JsInt32Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsInt32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int32, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsInt32Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsInt32Array"/>.</returns>
        public static JsInt32Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsInt32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Int32, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(int);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(int));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private int* Pointer => (int*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<int>.IsReadOnly => false;

        public int this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(int item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, int item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(int item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(int item) => IndexOf(item) >= 0;
        public void CopyTo(int[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (int* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(int item) => throw new InvalidOperationException("The array is fixed size.");
        public int[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<int>();
            var r = new int[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<int> GetEnumerator() => ((IEnumerable<int>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Uint32 array.
    /// </summary>
    public unsafe sealed class JsUint32Array : JsTypedArray, IList<uint>
    {
        internal JsUint32Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsUint32Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsUint32Array"/>.</returns>
        public static new JsUint32Array Create()
            => (JsUint32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint32, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsUint32Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsUint32Array"/>.</returns>
        [DefaultOverload]
        public static JsUint32Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsUint32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint32, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint32Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsUint32Array"/>.</returns>
        public static JsUint32Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsUint32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint32, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsUint32Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsUint32Array"/>.</returns>
        public static JsUint32Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsUint32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Uint32, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(uint);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(uint));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint* Pointer => (uint*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<uint>.IsReadOnly => false;

        public uint this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(uint item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, uint item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(uint item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(uint item) => IndexOf(item) >= 0;
        public void CopyTo(uint[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (uint* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(uint item) => throw new InvalidOperationException("The array is fixed size.");
        public uint[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<uint>();
            var r = new uint[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<uint> GetEnumerator() => ((IEnumerable<uint>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Float32 array.
    /// </summary>
    public unsafe sealed class JsFloat32Array : JsTypedArray, IList<float>
    {
        internal JsFloat32Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsFloat32Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsFloat32Array"/>.</returns>
        public static new JsFloat32Array Create()
            => (JsFloat32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float32, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsFloat32Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsFloat32Array"/>.</returns>
        [DefaultOverload]
        public static JsFloat32Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsFloat32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float32, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsFloat32Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsFloat32Array"/>.</returns>
        public static JsFloat32Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsFloat32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float32, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsFloat32Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsFloat32Array"/>.</returns>
        public static JsFloat32Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsFloat32Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float32, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(float);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(float));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private float* Pointer => (float*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<float>.IsReadOnly => false;

        public float this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(float item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, float item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(float item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(float item) => IndexOf(item) >= 0;
        public void CopyTo(float[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (float* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(float item) => throw new InvalidOperationException("The array is fixed size.");
        public float[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<float>();
            var r = new float[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<float> GetEnumerator() => ((IEnumerable<float>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }

    /// <summary>
    /// A Javascript Float64 array.
    /// </summary>
    public unsafe sealed class JsFloat64Array : JsTypedArray, IList<double>
    {
        internal JsFloat64Array(JsValueRef reference, void* buffer, long bufferLength) : base(reference, buffer, bufferLength) { }

        /// <summary>
        /// Create new instance of <see cref="JsFloat64Array"/>.
        /// </summary>
        /// <returns>A new instance of <see cref="JsFloat64Array"/>.</returns>
        public static new JsFloat64Array Create()
            => (JsFloat64Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float64, JsValueRef.Invalid, 0, 0));

        /// <summary>
        /// Create new instance of <see cref="JsFloat64Array"/>.
        /// </summary>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="JsFloat64Array"/>.</returns>
        [DefaultOverload]
        public static JsFloat64Array Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            return (JsFloat64Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float64, JsValueRef.Invalid, 0, (uint)length));
        }

        /// <summary>
        /// Create new instance of <see cref="JsFloat64Array"/>.
        /// </summary>
        /// <param name="typedArray">The array to copy values into the new array.</param>
        /// <returns>A new instance of <see cref="JsFloat64Array"/>.</returns>
        public static JsFloat64Array Create(JsTypedArray typedArray)
        {
            if (typedArray == null)
                throw new ArgumentNullException(nameof(typedArray));
            return (JsFloat64Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float64, typedArray?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Create new instance of <see cref="JsFloat64Array"/>.
        /// </summary>
        /// <param name="obj">The <see cref="JsObject"/> to convert to array.</param>
        /// <returns>A new instance of <see cref="JsFloat64Array"/>.</returns>
        public static JsFloat64Array Create(JsObject obj)
        {
            if (obj == null)
                throw new ArgumentNullException(nameof(obj));
            return (JsFloat64Array)FromRef(RawArray.CreateTyped(JsTypedArrayType.Float64, obj?.Reference ?? JsValueRef.Invalid, 0, 0));
        }

        /// <summary>
        /// Returns a number value of the element size for the different typed array objects.
        /// </summary>
        public static int BytesPerElement => sizeof(double);

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / sizeof(double));

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private double* Pointer => (double*)this.Buffer;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool ICollection<double>.IsReadOnly => false;

        public double this[int index]
        {
            get
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                return Pointer[index];
            }
            set
            {
                if (unchecked((uint)index >= (uint)Count))
                    throw new ArgumentOutOfRangeException(nameof(index));
                Pointer[index] = value;
            }
        }

        public int IndexOf(double item)
        {
            var start = Pointer;
            var end = start + Count;
            for (var p = start; p < end; p++)
            {
                if (*p == item)
                    return (int)(p - start);
            }
            return -1;
        }
        public void Insert(int index, double item) => throw new InvalidOperationException("The array is fixed size.");
        public void RemoveAt(int index) => throw new InvalidOperationException("The array is fixed size.");
        public void Add(double item) => throw new InvalidOperationException("The array is fixed size.");
        public void Clear() => throw new InvalidOperationException("The array is fixed size.");
        public bool Contains(double item) => IndexOf(item) >= 0;
        public void CopyTo(double[] array, int arrayIndex)
        {
            if (array is null)
                throw new ArgumentNullException(nameof(array));
            if (arrayIndex < 0 || array.Length < arrayIndex + Count)
                throw new ArgumentOutOfRangeException(nameof(arrayIndex));
            fixed (double* dst = &array[arrayIndex])
            {
                System.Buffer.MemoryCopy(this.Buffer, dst, (array.Length - arrayIndex) * BytesPerElement, this.BufferLength);
            }
        }
        public bool Remove(double item) => throw new InvalidOperationException("The array is fixed size.");
        public double[] ToArray()
        {
            if (Count == 0)
                return Array.Empty<double>();
            var r = new double[Count];
            CopyTo(r, 0);
            return r;
        }
        public IEnumerator<double> GetEnumerator() => ((IEnumerable<double>)ToArray()).GetEnumerator();
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}