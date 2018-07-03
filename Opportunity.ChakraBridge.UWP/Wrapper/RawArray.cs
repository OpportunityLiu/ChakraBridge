using System;
using System.Diagnostics;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawArray
    {
        /// <summary>
        /// Creates a JavaScript array object.
        /// </summary>
        /// <param name="length">The initial length of the array.</param>
        /// <returns>The new array object.</returns>
        /// <exception cref="ArgumentOutOfRangeException"></exception>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef Create(long length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            Native.JsCreateArray(checked((uint)length), out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        /// Creates a JavaScript typed array object. 
        /// </summary>
        /// <param name="arrayType">The type of the array to create. </param>
        /// <param name="arrayBuffer">The base array of the new array. Use <see cref="Native.JS_INVALID_REFERENCE"/> if no base array. </param>
        /// <param name="byteOffset">The offset in bytes from the start of <paramref name="arrayBuffer"/> for result typed array to reference. 
        /// Only applicable when baseArray is an ArrayBuffer object. Must be 0 otherwise. </param>
        /// <param name="elementLength">The number of elements in the array.
        /// Only applicable when creating a new typed array without baseArray (baseArray is <see cref="Native.JS_INVALID_REFERENCE"/>) or when baseArray is an ArrayBuffer object. Must be 0 otherwise. </param>
        /// <returns></returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsValueRef CreateTyped(JsTypedArrayType arrayType, JsValueRef arrayBuffer, long byteOffset,
            long elementLength)
        {
            checked
            {
                Native.JsCreateTypedArray(arrayType, arrayBuffer, (uint)byteOffset, (uint)elementLength, out var result).ThrowIfError();
                return result;
            }
        }

        /// <summary>
        /// Obtains frequently used properties of a typed array.
        /// </summary>
        /// <param name="typedArray">The typed array instance. </param>
        /// <param name="arrayBuffer">The <c>ArrayBuffer</c> backstore of the array. </param>
        /// <param name="byteOffset">The offset in bytes from the start of arrayBuffer referenced by the array. </param>
        /// <param name="byteLength">The number of bytes in the array. </param>
        /// <returns>The type of the array. </returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsTypedArrayType GetTypedArrayInfo(JsValueRef typedArray, out JsValueRef arrayBuffer, out long byteOffset, out long byteLength)
        {
            Native.JsGetTypedArrayInfo(typedArray, out var arrayType, out arrayBuffer, out var off, out var len)
                .ThrowIfError();
            byteOffset = off;
            byteLength = len;
            return arrayType;
        }

        public static unsafe JsTypedArrayType GetTypedArrayStorage(JsValueRef typedArray, out void* buffer, out long bufferLength)
        {
            Native.JsGetTypedArrayStorage(typedArray, out var buf, out var bufLen, out var type, out var eleSize).ThrowIfError();
            buffer = buf.ToPointer();
            bufferLength = bufLen;
            return type;
        }

        public static unsafe JsValueRef CreateArrayBuffer(long bufferLength)
        {
            Native.JsCreateArrayBuffer(checked((uint)bufferLength), out var r).ThrowIfError();
            return r;
        }

        public static unsafe JsValueRef CreateExternalArrayBuffer(IntPtr data, long byteLength, JsObjectFinalizeCallbackPtr finalizeCallback, IntPtr callbackState)
        {
            Native.JsCreateExternalArrayBuffer(data, checked((uint)byteLength), finalizeCallback, callbackState, out var r).ThrowIfError();
            return r;
        }

        public static unsafe void GetArrayBufferStorage(JsValueRef arrayBuffer, out void* buffer, out long bufferLength)
        {
            Native.JsGetArrayBufferStorage(arrayBuffer, out var buf, out var bufLen).ThrowIfError();
            buffer = buf.ToPointer();
            bufferLength = bufLen;
        }

        public static unsafe JsValueRef CreateDataView(JsValueRef arrayBuffer, long byteOffset, long length)
        {
            checked
            {
                Native.JsCreateDataView(arrayBuffer, (uint)byteOffset, (uint)length, out var r).ThrowIfError();
                return r;
            }
        }

        public static unsafe void GetDataViewStorage(JsValueRef dataView, out void* buffer, out long bufferLength)
        {
            Native.JsGetDataViewStorage(dataView, out var buf, out var bufLen).ThrowIfError();
            buffer = buf.ToPointer();
            bufferLength = bufLen;
        }
    }
}
