#include "pch.h"
#include "JsDataView.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsDataViewImpl::JsDataViewImpl(RawValue ref)
    : JsObjectImpl(std::move(ref))
{
    CHAKRA_CALL(JsGetDataViewStorage(Reference.Ref, &BufferPtr, &BufferLen));
}

IJsArrayBuffer^ JsDataViewImpl::Buffer::get()
{
    return safe_cast<IJsArrayBuffer^>(JsValue::CreateTyped(Reference[L"buffer"]));
}

IBuffer^ JsDataViewImpl::Data::get()
{
    return CreateNativeBuffer(this);
}

uint32 JsDataViewImpl::ByteLength::get()
{
    return BufferLen;
}

uint32 JsDataViewImpl::ByteOffset::get()
{
    return static_cast<uint32>(Reference[ L"byteOffset"]().ToInt());
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
        Throw(E_INVALIDARG, L"byteOffset is greater than buffer.ByteLength.");
    return Create(buffer, 0, buflen - byteOffset);
}

IJsDataView^ JsDataView::Create(IJsArrayBuffer^ buffer, uint32 byteOffset, uint32 byteLength)
{
    NULL_CHECK(buffer);
    auto bufferImpl = to_impl(buffer);
    auto buflen = bufferImpl->ByteLength;
    if (byteOffset + byteLength > buflen || byteOffset > buflen || byteLength > buflen)
        Throw(E_INVALIDARG, L"(byteOffset + byteLength) is greater than buffer.ByteLength.");
    return ref new JsDataViewImpl(RawValue::CreateDataView(get_ref(bufferImpl), byteOffset, byteLength));
}
