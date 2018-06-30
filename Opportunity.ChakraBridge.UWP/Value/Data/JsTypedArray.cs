using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Storage.Streams;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A Javascript typed array.
    /// </summary>
    public unsafe class JsTypedArray : JsObject, IEnumerable, IBuffer
    {
        internal JsTypedArray(JsValueRef reference, void* buffer, long bufferLength)
            : base(reference)
        {
            // WinRT disallows abstact classes.
            Debug.Assert(this.GetType() != typeof(JsTypedArray));
            this.Buffer = buffer;
            this.BufferLength = bufferLength;
            this.length = (uint)bufferLength;
        }

        internal static JsTypedArray FromRef(JsValueRef reference)
        {
            switch (RawArray.GetTypedArrayStorage(reference, out var buf, out var buflen))
            {
            case JsTypedArrayType.Int8:
                return new JsInt8Array(reference, buf, buflen);
            case JsTypedArrayType.Uint8:
                return new JsUint8Array(reference, buf, buflen);
            case JsTypedArrayType.Uint8Clamped:
                return new JsUint8ClampedArray(reference, buf, buflen);
            case JsTypedArrayType.Int16:
                return new JsInt16Array(reference, buf, buflen);
            case JsTypedArrayType.Uint16:
                return new JsUint16Array(reference, buf, buflen);
            case JsTypedArrayType.Int32:
                return new JsInt32Array(reference, buf, buflen);
            case JsTypedArrayType.Uint32:
                return new JsUint32Array(reference, buf, buflen);
            case JsTypedArrayType.Float32:
                return new JsFloat32Array(reference, buf, buflen);
            case JsTypedArrayType.Float64:
                return new JsFloat64Array(reference, buf, buflen);
            }
            throw new InvalidOperationException("Unknown array type.");
        }

        //public static JsTypedArray<byte> Create<byte>(JsArrayBuffer buffer, int byteOffset,int length)
        // where byte : unmanaged, IEquatable<byte>, IComparable<byte>, IConvertible, IFormattable
        //{

        //}

        /// <summary>
        /// Gets the type of array.
        /// </summary>
        public JsTypedArrayType ArrayType => RawArray.GetTypedArrayInfo(this.Reference, out var buf, out var offset, out var len);

        /// <summary>
        /// Buffer pointer of the array data.
        /// </summary>
        protected readonly void* Buffer;

        /// <summary>
        /// Length if buffer in bytes.
        /// </summary>
        protected readonly long BufferLength;

        IEnumerator IEnumerable.GetEnumerator() => throw new NotImplementedException();

        uint IBuffer.Capacity => (uint)this.BufferLength;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint length;
        uint IBuffer.Length
        {
            get => this.length;
            set
            {
                if (value > this.BufferLength)
                    throw new ArgumentOutOfRangeException(nameof(value));
                this.length = value;
            }
        }
    }
}
