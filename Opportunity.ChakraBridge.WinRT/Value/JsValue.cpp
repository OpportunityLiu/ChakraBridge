#include "pch.h"
#include "JsValue.h"
#include "JsContext\JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge;

JsValueImpl::JsValueImpl(RawValue ref)
    : Reference(std::move(ref))
{
    if (!Reference.IsValid())
        Throw(E_HANDLE, L"ref for JsValue is JS_INVALID_REFERENCE");
}

object^ JsValueImpl::ToInspectable()
{
    return reinterpret_cast<object^>(Reference.ToInspectable());
}

JsContext^ JsValueImpl::Context::get()
{
    return JsContext::Get(Reference.Context());
}

JsType JsValueImpl::Type::get()
{
    return Reference.Type();
}

JsValueImpl^ JsValue::CreateTyped(RawValue ref)
{
    if (!ref.IsValid())
        return nullptr;
    switch (ref.Type())
    {
    case JsType::Undefined:
        return ref new JsUndefinedImpl(ref);
    case JsType::Null:
        return ref new JsNullImpl(ref);
    case JsType::Number:
        return ref new JsNumberImpl(ref);
    case JsType::String:
        return ref new JsStringImpl(ref);
    case JsType::Boolean:
        return ref new JsBooleanImpl(ref);
    case JsType::Symbol:
        return ref new JsSymbolImpl(ref);
    case JsType::Object:
        if (ref.ObjHasExternalData())
            return ref new JsExternalObjectImpl(ref);
        else
            return ref new JsObjectImpl(ref);
    case JsType::Function:
        return ref new JsFunctionImpl(ref);
    case JsType::Error:
        return ref new JsErrorImpl(ref);
    case JsType::Array:
        return ref new JsArrayImpl(ref);
    case JsType::ArrayBuffer:
        return ref new JsArrayBufferImpl(ref);
    case JsType::DataView:
        return ref new JsDataViewImpl(ref);
    case JsType::TypedArray:
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
    CHAKRA_CALL(JsEquals(to_impl(v1)->Reference.Ref, to_impl(v2)->Reference.Ref, &r));
    return r;
}

bool JsValue::StrictEquals(IJsValue^ v1, IJsValue^ v2)
{
    if (v1 == nullptr)
        return (v2 == nullptr);
    if (v2 == nullptr)
        return false;
    bool r;
    CHAKRA_CALL(JsStrictEquals(to_impl(v1)->Reference.Ref, to_impl(v2)->Reference.Ref, &r));
    return r;
}

IJsValue^ JsValue::Create(object^ inspectable)
{
    NULL_CHECK(inspectable);
    return CreateTyped(RawValue(reinterpret_cast<IInspectable*>(inspectable)));
}

IJsBoolean^ JsValue::ToJsBoolean(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsBoolean^>(value);
    if (cv != nullptr)
        return cv;
    return ref new JsBooleanImpl(to_impl(value)->Reference.ToJsBoolean());
}

IJsNumber^ JsValue::ToJsNumber(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsNumber^>(value);
    if (cv != nullptr)
        return cv;
    return ref new JsNumberImpl(to_impl(value)->Reference.ToJsNumber());
}

IJsString^ JsValue::ToJsString(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsString^>(value);
    if (cv != nullptr)
        return cv;
    return ref new JsStringImpl(to_impl(value)->Reference.ToJsString());
}

IJsObject^ JsValue::ToJsObject(IJsValue^ value)
{
    NULL_CHECK(value);
    auto cv = dynamic_cast<IJsObject^>(value);
    if (cv != nullptr)
        return cv;
    return safe_cast<IJsObject^>(CreateTyped(to_impl(value)->Reference.ToJsObjet()));
}

IJsObject^ JsValue::GlobalObject::get()
{
    return ref new JsObjectImpl(RawValue::GlobalObject());
}
