#include "pch.h"
#include "JsArrayBuffer.h"
#include "Native\NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<void*, JsValueRef> JsArrayBufferImpl::ExternalBufferKeyMap;
std::unordered_map<JsValueRef, IJsArrayBuffer::IBuffer^> JsArrayBufferImpl::ExternalBufferDataMap;

void CALLBACK JsArrayBufferImpl::JsFinalizeCallbackImpl(_In_opt_ void *data)
{
    auto kv = ExternalBufferKeyMap[data];
    ExternalBufferDataMap.erase(kv);
    ExternalBufferKeyMap.erase(data);
}

JsArrayBufferImpl::JsArrayBufferImpl(JsValueRef ref)
    : JsObjectImpl(ref)
{
    CHAKRA_CALL(JsGetArrayBufferStorage(ref, &BufferPtr, &BufferLen));
}

IJsArrayBuffer::IBuffer^ JsArrayBufferImpl::Buffer::get()
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
    JsValueRef r;
    CHAKRA_CALL(JsCreateArrayBuffer(length, &r));
    return ref new JsArrayBufferImpl(r);
}

IJsArrayBuffer^ JsArrayBuffer::Create(IJsArrayBuffer::IBuffer^ buffer)
{
    uint32 buflen;
    auto bufptr = GetPointerOfBuffer(buffer, nullptr, &buflen);
    buffer->Length = buflen;
    auto cb = reinterpret_cast<void*>(buffer);
    JsValueRef r;
    CHAKRA_CALL(JsCreateExternalArrayBuffer(bufptr, buflen, JsArrayBufferImpl::JsFinalizeCallbackImpl, cb, &r));
    JsArrayBufferImpl::ExternalBufferKeyMap[cb] = r;
    JsArrayBufferImpl::ExternalBufferDataMap[r] = buffer;
    return ref new JsArrayBufferImpl(r);
}