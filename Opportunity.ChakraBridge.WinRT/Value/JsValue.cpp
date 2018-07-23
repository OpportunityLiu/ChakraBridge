#include "pch.h"
#include "JsValue.h"
#include "JsContext\JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge;

JsValueImpl::JsValueImpl(JsValueRef ref)
    : Reference(ref)
{
    if (Reference == JS_INVALID_REFERENCE)
        throw ref new Platform::InvalidArgumentException(L"ref for JsValue is JS_INVALID_REFERENCE");
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

JsType JsValueImpl::Type::get()
{
    return RawGetValueType(Reference);
}

JsValueImpl^ JsValue::CreateTyped(JsValueRef ref)
{
    if (ref == JS_INVALID_REFERENCE)
        return nullptr;
    ::JsValueType type;
    CHAKRA_CALL(JsGetValueType(ref, &type));
    switch (type)
    {
    case ::JsUndefined:
        return ref new JsUndefinedImpl(ref);
    case ::JsNull:
        return ref new JsNullImpl(ref);
    case ::JsNumber:
        return ref new JsNumberImpl(ref);
    case ::JsString:
        return ref new JsStringImpl(ref);
    case ::JsBoolean:
        return ref new JsBooleanImpl(ref);
    case ::JsSymbol:
        return ref new JsSymbolImpl(ref);
    case ::JsObject:
        bool hasExternalData;
        CHAKRA_CALL(JsHasExternalData(ref, &hasExternalData));
        if (hasExternalData)
            return ref new JsExternalObjectImpl(ref);
        else
            return ref new JsObjectImpl(ref);
    case ::JsFunction:
        return ref new JsFunctionImpl(ref);
    case ::JsError:
        return ref new JsErrorImpl(ref);
    case ::JsArray:
        return ref new JsArrayImpl(ref);
    case ::JsArrayBuffer:
        return ref new JsArrayBufferImpl(ref);
    case ::JsDataView:
        return ref new JsDataViewImpl(ref);
    case ::JsTypedArray:
        return JsTypedArray::CreateTyped(ref);
    }
    return ref new JsObjectImpl(ref);
}

bool JsValue::ReferenceEquals(IJsValue^ v1, IJsValue^ v2)
{
    if (v1 == nullptr)
        return (v2 == nullptr);
    if (v2 == nullptr)
        return false;
    return to_impl(v1)->Reference == to_impl(v2)->Reference;
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

IJsValue^ JsValue::Create(object^ inspectable)
{
    NULL_CHECK(inspectable);
    JsValueRef v;
    CHAKRA_CALL(JsInspectableToObject(reinterpret_cast<IInspectable*>(inspectable), &v));
    return CreateTyped(v);
}

IJsBoolean^ JsValue::ToJsBoolean(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsBoolean^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToBoolean(to_impl(value)->Reference, &ref));
    return ref new JsBooleanImpl(ref);
}

IJsNumber^ JsValue::ToJsNumber(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsNumber^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToNumber(to_impl(value)->Reference, &ref));
    return ref new JsNumberImpl(ref);
}

IJsString^ JsValue::ToJsString(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsString^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToString(to_impl(value)->Reference, &ref));
    return ref new JsStringImpl(ref);
}

IJsObject^ JsValue::ToJsObject(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsObject^>(value);
    if (cv != nullptr)
        return cv;
    JsValueRef ref;
    CHAKRA_CALL(JsConvertValueToObject(to_impl(value)->Reference, &ref));
    return safe_cast<IJsObject^>(CreateTyped(ref));
}

IJsObject^ JsValue::GlobalObject::get()
{
    return ref new JsObjectImpl(RawGlobalObject());
}
