#include "pch.h"
#include "JsBoolean.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsBooleanImpl::JsBooleanImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

bool JsBooleanImpl::ToBoolean()
{
    bool v;
    CHAKRA_CALL(JsBooleanToBool(Reference, &v));
    return v;
}

Platform::String^ JsBooleanImpl::ToString()
{
    return ToBoolean() ? "true" : "false";
}

IJsBoolean^ JsBoolean::True::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetTrueValue(&ref));
    return ref new JsBooleanImpl(ref);
}

IJsBoolean^ JsBoolean::False::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetFalseValue(&ref));
    return ref new JsBooleanImpl(ref);
}

IJsBoolean^ JsBoolean::OfBoolean(bool value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsBoolToBoolean(value, &ref));
    return ref new JsBooleanImpl(ref);
}