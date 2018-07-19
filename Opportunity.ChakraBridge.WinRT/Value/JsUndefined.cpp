#include "pch.h"
#include "JsUndefined.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsUndefinedImpl::JsUndefinedImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

IJsUndefined^ JsUndefined::Instance::get()
{
    return ref new JsUndefinedImpl(RawUndefined());
}