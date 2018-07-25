#include "pch.h"
#include "JsExternalObject.h"
#include <algorithm>

using namespace Opportunity::ChakraBridge::WinRT;

using EOP = struct EO
{
    object^ Object;
}*;

void JsExternalObjectImpl::JsFinalizeCallbackImpl(void* data)
{
    auto ptr = static_cast<EOP>(data);
    delete ptr;
}

object^ JsExternalObjectImpl::ExternalData::get()
{
    EOP ptr = nullptr;
    CHAKRA_CALL(JsGetExternalData(Reference.Ref, reinterpret_cast<void**>(&ptr)));
    return ptr->Object;
}

void JsExternalObjectImpl::ExternalData::set(object^ value)
{
    EOP ptr = nullptr;
    CHAKRA_CALL(JsGetExternalData(Reference.Ref, reinterpret_cast<void**>(&ptr)));
    ptr->Object = value;
}

IJsExternalObject^ JsExternalObject::Create(object^ data)
{
    JsValueRef obj;
    auto ptr = new EO();
    ptr->Object = data;
    CHAKRA_CALL(JsCreateExternalObject(ptr, JsExternalObjectImpl::JsFinalizeCallbackImpl, &obj));
    return ref new JsExternalObjectImpl(obj);
}