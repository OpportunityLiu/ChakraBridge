#include "pch.h"
#include "JsDataView.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsDataViewImpl::JsDataViewImpl(JsValueRef ref)
    : JsObjectImpl(ref)
{
    CHAKRA_CALL(JsGetDataViewStorage(ref, &BufferPtr, &BufferLen));
}

IJsArrayBuffer^ JsDataViewImpl::Buffer::get()
{
    return safe_cast<IJsArrayBuffer^>(JsValue::CreateTyped(RawGetProperty(Reference, L"buffer")));
}

IJsDataView::IBuffer^ JsDataViewImpl::Data::get()
{
    return CreateNativeBuffer(this);
}

uint32 JsDataViewImpl::ByteLength::get()
{
    return BufferLen;
}

uint32 JsDataViewImpl::ByteOffset::get()
{
    return static_cast<uint32>(RawNumberToInt(RawGetProperty(Reference, L"byteOffset")));;
}

IJsDataView^ JsDataView::Create(IJsArrayBuffer^ buffer)
{
    return Create(buffer, 0);
}

IJsDataView^ JsDataView::Create(IJsArrayBuffer^ buffer, uint32 byteOffset)
{
    NULL_CHECK(buffer);
    auto buflen = to_impl(buffer)->ByteLength;
    if (buflen < byteOffset)
        throw ref new Platform::InvalidArgumentException(L"byteOffset is greater than buffer.ByteLength.");
    return Create(buffer, 0, buflen - byteOffset);
}

IJsDataView^ JsDataView::Create(IJsArrayBuffer^ buffer, uint32 byteOffset, uint32 byteLength)
{
    NULL_CHECK(buffer);
    auto bufferImpl = to_impl(buffer);
    auto buflen = bufferImpl->ByteLength;
    if (byteOffset + byteLength > buflen || byteOffset > buflen || byteLength > buflen)
        throw ref new Platform::InvalidArgumentException(L"(byteOffset + byteLength) is greater than buffer.ByteLength.");
    JsValueRef r;
    CHAKRA_CALL(JsCreateDataView(bufferImpl->Reference, byteOffset, byteLength, &r));
    return ref new JsDataViewImpl(r);
}
