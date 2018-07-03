using System;
using System.Collections;
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
    public unsafe class JsTypedArray : JsObject, IEnumerable
    {
        internal JsTypedArray(JsValueRef reference, JsTypedArrayType type, int elementSize, void* buffer, long bufferLength)
            : base(reference)
        {
            // WinRT disallows abstact classes.
            Debug.Assert(this.GetType() != typeof(JsTypedArray));
            this.Buffer = buffer;
            this.BufferLength = bufferLength;
            this.ArrayType = type;
            this.ElementSize = elementSize;
        }

        internal static JsTypedArray FromRef(JsValueRef reference)
        {
            var type = RawArray.GetTypedArrayStorage(reference, out var buf, out var buflen);
            switch (type)
            {
            case JsTypedArrayType.Int8:
                return new JsInt8Array(reference, type, buf, buflen);
            case JsTypedArrayType.Uint8:
                return new JsUint8Array(reference, type, buf, buflen);
            case JsTypedArrayType.Uint8Clamped:
                return new JsUint8ClampedArray(reference, type, buf, buflen);
            case JsTypedArrayType.Int16:
                return new JsInt16Array(reference, type, buf, buflen);
            case JsTypedArrayType.Uint16:
                return new JsUint16Array(reference, type, buf, buflen);
            case JsTypedArrayType.Int32:
                return new JsInt32Array(reference, type, buf, buflen);
            case JsTypedArrayType.Uint32:
                return new JsUint32Array(reference, type, buf, buflen);
            case JsTypedArrayType.Float32:
                return new JsFloat32Array(reference, type, buf, buflen);
            case JsTypedArrayType.Float64:
                return new JsFloat64Array(reference, type, buf, buflen);
            }
            throw new InvalidOperationException("Unknown array type.");
        }

        /// <summary>
        /// Size of element in array, count in bytes.
        /// </summary>
        public int ElementSize { get; }

        /// <summary>
        /// Count of elements in the array.
        /// </summary>
        public int Count => (int)(this.BufferLength / ElementSize);

        /// <summary>
        /// Gets the type of array.
        /// </summary>
        public JsTypedArrayType ArrayType { get; }

        /// <summary>
        /// Buffer pointer of the array data.
        /// </summary>
        protected readonly void* Buffer;

        /// <summary>
        /// Length of buffer in bytes.
        /// </summary>
        protected readonly long BufferLength;

        IEnumerator IEnumerable.GetEnumerator() => throw new NotImplementedException();
    }
}
