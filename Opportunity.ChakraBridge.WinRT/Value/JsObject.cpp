#include "pch.h"
#include "JsObject.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsObjectImpl::~JsObjectImpl()
{
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
}

JsObjectImpl::JsObjectImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

Platform::String ^ Opportunity::ChakraBridge::WinRT::JsObjectImpl::ToString()
{
    throw ref new Platform::NotImplementedException();
    // TODO: �ڴ˴����� return ���
}
