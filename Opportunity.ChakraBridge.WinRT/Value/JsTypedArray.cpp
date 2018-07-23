#include "pch.h"
#include "JsTypedArray.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsTypedArrayImpl::JsTypedArrayImpl(JsValueRef ref, uint8* bufferPtr, unsigned int bufferLen, JsArrayType arrType, unsigned int elementSize)
    : JsObjectImpl(ref), BufferPtr(bufferPtr), BufferLen(bufferLen), ArrType(arrType), ElementSize(elementSize) {}

IJsArrayBuffer^ JsTypedArrayImpl::Buffer::get()
{
    JsValueRef buf;
    CHAKRA_CALL(JsGetTypedArrayInfo(Reference, nullptr, &buf, nullptr, nullptr));
    return safe_cast<IJsArrayBuffer^>(JsValue::CreateTyped(buf));
}

IJsTypedArray::IBuffer^ JsTypedArrayImpl::Data::get()
{
    return CreateNativeBuffer(this);
}

uint32 JsTypedArrayImpl::ByteLength::get()
{
    return BufferLen;
}

uint32 JsTypedArrayImpl::ByteOffset::get()
{
    uint32 offset;
    CHAKRA_CALL(JsGetTypedArrayInfo(Reference, nullptr, nullptr, &offset, nullptr));
    return offset;
}

JsTypedArrayImpl::JsArrayType JsTypedArrayImpl::ArrayType::get()
{
    return ArrType;
}

uint32 JsTypedArrayImpl::BytesPerElement::get()
{
    return ElementSize;
}

void JsTypedArrayImpl::ThrowForFixedSize()
{
    throw Platform::Exception::CreateException(E_NOTIMPL, L"The typed array is fixed size.");
}

uint32 JsTypedArrayImpl::ArraySize::get()
{
    return BufferLen / ElementSize;
}

void JsTypedArrayImpl::BoundCheck(uint32 index)
{
    if (index >= ArraySize)
        throw Platform::Exception::CreateException(E_INVALIDARG, "index is larger than size of the typed array.");
}

JsTypedArrayImpl^ JsTypedArray::CreateTyped(JsValueRef ref)
{
    using AT = JsArrayType;
    uint8* bufferPtr;
    unsigned int bufferLen;
    AT arrType;
    int elementSize;
    CHAKRA_CALL(JsGetTypedArrayStorage(ref, &bufferPtr, &bufferLen, reinterpret_cast<::JsTypedArrayType*>(&arrType), &elementSize));
#define CASE(name) \
    case AT::name: return ref new JsTypedArrayTempImpl<AT::name>(ref, bufferPtr, bufferLen, arrType, static_cast<unsigned int>(elementSize))

    switch (arrType)
    {
        CASE(Int8);
        CASE(Uint8);
        CASE(Uint8Clamped);
        CASE(Int16);
        CASE(Uint16);
        CASE(Int32);
        CASE(Uint32);
        CASE(Float32);
        CASE(Float64);
    }
#undef CASE
    throw ref new Platform::NotImplementedException("Unknown array type.");
}

uint32 GetSize(JsTypedArray::JsArrayType arrType)
{
    using AT = JsTypedArray::JsArrayType;

#define CASE(name) case AT::name: return sizeof(typename JsTypedArrayTempInfo<AT::name>::t_ele)
    switch (arrType)
    {
        CASE(Int8);
        CASE(Uint8);
        CASE(Uint8Clamped);
        CASE(Int16);
        CASE(Uint16);
        CASE(Int32);
        CASE(Uint32);
        CASE(Float32);
        CASE(Float64);
    }
#undef CASE
    throw ref new Platform::NotImplementedException("Unknown array type.");
}

JsValueRef RawCreateTyped(JsTypedArray::JsArrayType type, JsValueRef base, uint32 byteOffset, uint32 elementLength)
{
    JsValueRef r;
    CHAKRA_CALL(JsCreateTypedArray(static_cast<::JsTypedArrayType>(type), base, byteOffset, elementLength, &r));
    return r;
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType)
{
    return CreateTyped(RawCreateTyped(arrayType, JS_INVALID_REFERENCE, 0, 0));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, uint32 length)
{
    return CreateTyped(RawCreateTyped(arrayType, JS_INVALID_REFERENCE, 0, length));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, IJsValue^ arrayLike)
{
    NULL_CHECK(arrayLike);
    return CreateTyped(RawCreateTyped(arrayType, to_impl(arrayLike)->Reference, 0, 0));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, IJsArrayBuffer^ buffer)
{
    return Create(arrayType, buffer, 0);
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, IJsArrayBuffer^ buffer, uint32 byteOffset)
{
    NULL_CHECK(buffer);
    return Create(arrayType, buffer, byteOffset, (to_impl(buffer)->BufferLen - byteOffset) / GetSize(arrayType));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, IJsArrayBuffer^ buffer, uint32 byteOffset, uint32 length)
{
    NULL_CHECK(buffer);
    return CreateTyped(RawCreateTyped(arrayType, to_impl(buffer)->Reference, byteOffset, length));
}
