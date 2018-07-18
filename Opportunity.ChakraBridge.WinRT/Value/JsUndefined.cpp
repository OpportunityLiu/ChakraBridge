#include "pch.h"
#include "JsUndefined.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsUndefinedImpl::JsUndefinedImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

IJsUndefined^ JsUndefined::Instance::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetUndefinedValue(&ref));
    return ref new JsUndefinedImpl(ref);
}