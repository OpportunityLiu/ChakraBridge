#include "pch.h"
#include "JsValue.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsValue::JsValue(JsValueRef ref)
    : Reference(ref)
{
    if (Reference == JS_INVALID_REFERENCE)
        throw ref new Platform::InvalidArgumentException("ref for JsValue is JS_INVALID_REFERENCE");
}
