#include "pch.h"
#include "JsNull.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsNullImpl::JsNullImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

IJsNull^ JsNull::Instance::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetNullValue(&ref));
    return ref new JsNullImpl(ref);
}