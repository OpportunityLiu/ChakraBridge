#include "pch.h"
#include "Native\BufferPointer.h"
#include "JsArrayBuffer.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<void*, RawValue> JsArrayBufferImpl::ExternalBufferKeyMap;
std::unordered_map<RawValue, IBuffer^> JsArrayBufferImpl::ExternalBufferDataMap;

void CALLBACK JsArrayBufferImpl::JsFinalizeCallbackImpl(_In_opt_ void *data)
{
    auto kv = ExternalBufferKeyMap[data];
    ExternalBufferDataMap.erase(kv);
    ExternalBufferKeyMap.erase(data);
}

JsArrayBufferImpl::JsArrayBufferImpl(RawValue ref)
    : JsObjectImpl(std::move(ref))
{
    CHAKRA_CALL(JsGetArrayBufferStorage(Reference.Ref, &BufferPtr, &BufferLen));
}

IBuffer^ JsArrayBufferImpl::Data::get()
{
    auto extBuf = ExternalBufferDataMap.find(Reference);
    if (extBuf != ExternalBufferDataMap.end())
        return extBuf->second;
    return CreateNativeBuffer(this);
}

uint32 JsArrayBufferImpl::ByteLength::get()
{
    return BufferLen;
}

IJsArrayBuffer^ JsArrayBuffer::Create(uint32 length)
{
    return ref new JsArrayBufferImpl(RawValue::CreateArrayBuffer(length));
}

IJsArrayBuffer^ JsArrayBuffer::Create(IBuffer^ buffer)
{
    uint32 buflen;
    auto bufptr = GetPointerOfBuffer(buffer, nullptr, &buflen);
    buffer->Length = buflen;
    auto cb = reinterpret_cast<void*>(buffer);
    const auto r = RawValue::CreateArrayBuffer(bufptr, buflen, JsArrayBufferImpl::JsFinalizeCallbackImpl, cb);
    JsArrayBufferImpl::ExternalBufferKeyMap[cb] = r;
    JsArrayBufferImpl::ExternalBufferDataMap[r] = buffer;
    return ref new JsArrayBufferImpl(r);
}