#include "pch.h"
#include "JsExternalObject.h"
#include <algorithm>

using namespace Opportunity::ChakraBridge::WinRT;
using ObjPtr = std::pair<object^, nullptr_t>;

void JsExternalObjectImpl::JsFinalizeCallbackImpl(void* data)
{
    auto ptr = static_cast<ObjPtr*>(data);
    delete ptr;
}

object^ JsExternalObjectImpl::ExternalData::get()
{
    ObjPtr* ptr;
    CHAKRA_CALL(JsGetExternalData(Reference.Ref, reinterpret_cast<void**>(&ptr)));
    return ptr->first;
}

void JsExternalObjectImpl::ExternalData::set(object^ value)
{
    ObjPtr* ptr;
    CHAKRA_CALL(JsGetExternalData(Reference.Ref, reinterpret_cast<void**>(&ptr)));
    ptr->first = value;
}

IJsExternalObject^ JsExternalObject::Create(object^ data)
{
    JsValueRef obj;
    auto ptr = new ObjPtr();
    ptr->first = data;
    CHAKRA_CALL(JsCreateExternalObject(ptr, JsExternalObjectImpl::JsFinalizeCallbackImpl, &obj));
    return ref new JsExternalObjectImpl(obj);
}