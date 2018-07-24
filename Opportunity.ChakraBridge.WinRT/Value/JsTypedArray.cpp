#include "pch.h"
#include "JsTypedArray.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsTypedArrayImpl::JsTypedArrayImpl(RawValue ref, uint8*const bufferPtr, const uint32 bufferLen, const JsArrayType arrType, const uint32 elementSize)
    : JsObjectImpl(std::move(ref)), BufferPtr(bufferPtr), BufferLen(bufferLen), ArrType(arrType), ElementSize(elementSize) {}

IJsArrayBuffer^ JsTypedArrayImpl::Buffer::get()
{
    JsValueRef buf;
    CHAKRA_CALL(JsGetTypedArrayInfo(Reference.Ref, nullptr, &buf, nullptr, nullptr));
    return safe_cast<IJsArrayBuffer^>(JsValue::CreateTyped(buf));
}

IBuffer^ JsTypedArrayImpl::Data::get()
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
    CHAKRA_CALL(JsGetTypedArrayInfo(Reference.Ref, nullptr, nullptr, &offset, nullptr));
    return offset;
}

JsArrayType JsTypedArrayImpl::ArrayType::get()
{
    return ArrType;
}

uint32 JsTypedArrayImpl::BytesPerElement::get()
{
    return ElementSize;
}

void JsTypedArrayImpl::ThrowForFixedSize()
{
    Throw(E_NOTIMPL, L"The typed array is fixed size.");
}

uint32 JsTypedArrayImpl::ArraySize::get()
{
    return BufferLen / ElementSize;
}

void JsTypedArrayImpl::BoundCheck(uint32 index)
{
    if (index >= ArraySize)
        Throw(E_INVALIDARG, L"index is larger than the size of the typed array.");
}

JsTypedArrayImpl^ JsTypedArray::CreateTyped(RawValue ref)
{
    using AT = JsArrayType;
    uint8* bufferPtr;
    unsigned int bufferLen;
    AT arrType;
    int elementSize;
    CHAKRA_CALL(JsGetTypedArrayStorage(ref.Ref, &bufferPtr, &bufferLen, reinterpret_cast<::JsTypedArrayType*>(&arrType), &elementSize));
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
    Throw(E_NOTIMPL, L"Unknown array type.");
}

uint32 GetSize(JsArrayType arrType)
{
    using AT = JsArrayType;

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
    Throw(E_NOTIMPL, L"Unknown array type.");
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType)
{
    return CreateTyped(RawValue::CreateTypedArray(arrayType, RawValue::Invalid(), 0, 0));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, uint32 length)
{
    return CreateTyped(RawValue::CreateTypedArray(arrayType, RawValue::Invalid(), 0, length));
}

IJsTypedArray^ JsTypedArray::Create(JsArrayType arrayType, IJsValue^ arrayLike)
{
    NULL_CHECK(arrayLike);
    return CreateTyped(RawValue::CreateTypedArray(arrayType, to_impl(arrayLike)->Reference, 0, 0));
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
    return CreateTyped(RawValue::CreateTypedArray(arrayType, to_impl(buffer)->Reference, byteOffset, length));
}
