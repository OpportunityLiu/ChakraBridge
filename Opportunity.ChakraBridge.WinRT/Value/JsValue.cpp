#include "pch.h"
#include "JsValue.h"
#include "JsContext\JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge;

JsValueImpl::JsValueImpl(JsValueRef ref)
    : Reference(ref)
{
    if (Reference == JS_INVALID_REFERENCE)
        throw ref new Platform::InvalidArgumentException("ref for JsValue is JS_INVALID_REFERENCE");
}

object^ JsValueImpl::ToInspectable()
{
    IInspectable* v;
    CHAKRA_CALL(JsObjectToInspectable(Reference, &v));
    return reinterpret_cast<object^>(v);
}

JsContext^ JsValueImpl::Context::get()
{
    JsContextRef ref;
    CHAKRA_CALL(JsGetContextOfObject(Reference, &ref));
    return JsContext::Get(ref);
}

WinRT::JsValueType JsValueImpl::Type::get()
{
    ::JsValueType type;
    CHAKRA_CALL(JsGetValueType(Reference, &type));
    return static_cast<WinRT::JsValueType>(type);
}

JsValueImpl^ JsValue::CreateTyped(JsValueRef ref)
{
    if (ref == JS_INVALID_REFERENCE)
        return nullptr;
    ::JsValueType type;
    CHAKRA_CALL(JsGetValueType(ref, &type));
    switch (type)
    {
    case ::JsNull:
        return ref new JsNullImpl(ref);
    case ::JsUndefined:
        return ref new JsUndefinedImpl(ref);
    case ::JsBoolean:
        return ref new JsBooleanImpl(ref);
    case ::JsNumber:
        return ref new JsNumberImpl(ref);
    case ::JsString:
        return ref new JsStringImpl(ref);
    case ::JsObject:
        return ref new JsObjectImpl(ref);
    }
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}

bool JsValue::Equals(IJsValue^ v1, IJsValue^ v2)
{
    if (v1 == nullptr)
        return (v2 == nullptr);
    if (v2 == nullptr)
        return false;
    bool r;
    CHAKRA_CALL(JsEquals(to_impl(v1)->Reference, to_impl(v2)->Reference, &r));
    return r;
}

bool JsValue::StrictEquals(IJsValue^ v1, IJsValue^ v2)
{
    if (v1 == nullptr)
        return (v2 == nullptr);
    if (v2 == nullptr)
        return false;
    bool r;
    CHAKRA_CALL(JsStrictEquals(to_impl(v1)->Reference, to_impl(v2)->Reference, &r));
    return r;
}

IJsValue^ JsValue::Of(object^ inspectable)
{
    if (inspectable == nullptr)
        throw ref new Platform::InvalidArgumentException("inspectable is null.");
    JsValueRef v;
    CHAKRA_CALL(JsInspectableToObject(reinterpret_cast<IInspectable*>(inspectable), &v));
    return CreateTyped(v);
}

IJsBoolean^ JsValue::ToJsBoolean(IJsValue^ value)
{
    if (value == nullptr)
        throw ref new Platform::InvalidArgumentException("value is null.");
    auto cv = dynamic_cast<IJsBoolean^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToBoolean(to_impl(value)->Reference, &ref));
    return ref new JsBooleanImpl(ref);
}

IJsNumber^ JsValue::ToJsNumber(IJsValue^ value)
{
    if (value == nullptr)
        throw ref new Platform::InvalidArgumentException("value is null.");
    auto cv = dynamic_cast<IJsNumber^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToNumber(to_impl(value)->Reference, &ref));
    return ref new JsNumberImpl(ref);
}

IJsString^ JsValue::ToJsString(IJsValue^ value)
{
    if (value == nullptr)
        throw ref new Platform::InvalidArgumentException("value is null.");
    auto cv = dynamic_cast<IJsString^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToString(to_impl(value)->Reference, &ref));
    return ref new JsStringImpl(ref);
}

IJsObject^ JsValue::ToJsObject(IJsValue^ value)
{
    if (value == nullptr)
        throw ref new Platform::InvalidArgumentException("value is null.");
    auto cv = dynamic_cast<IJsObject^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToObject(to_impl(value)->Reference, &ref));
    return safe_cast<IJsObject^>(CreateTyped(ref));
}

IJsObject^ JsValue::GlobalObject::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetGlobalObject(&ref));
    return ref new JsObjectImpl(ref);
}
