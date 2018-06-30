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
        public static JsValueRef Create(int length)
        {
            if (length < 0)
                throw new ArgumentOutOfRangeException(nameof(length));
            Native.JsCreateArray((uint)length, out var reference).ThrowIfError();
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
        public static JsValueRef CreateTyped(JsTypedArrayType arrayType, JsValueRef arrayBuffer, uint byteOffset,
            uint elementLength)
        {
            Native.JsCreateTypedArray(arrayType, arrayBuffer, byteOffset, elementLength, out var result).ThrowIfError();
            return result;
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
        public static JsTypedArrayType GetTypedArrayInfo(JsValueRef typedArray, out JsValueRef arrayBuffer, out uint byteOffset, out uint byteLength)
        {
            Native.JsGetTypedArrayInfo(typedArray, out var arrayType, out arrayBuffer, out byteOffset, out byteLength).ThrowIfError();
            return arrayType;
        }

        public static unsafe JsTypedArrayType GetTypedArrayStorage(JsValueRef typedArray, out void* buffer, out long bufferLength)
        {
            Native.JsGetTypedArrayStorage(typedArray, out var buf, out var bufLen, out var type, out var eleSize).ThrowIfError();
            buffer = buf.ToPointer();
            bufferLength = bufLen;
            return type;
        }
    }
}
