#include "pch.h"
#include "JsValue.h"

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge;

JsValueImpl::JsValueImpl(JsValueRef ref)
    : Reference(ref)
{
    if (Reference == JS_INVALID_REFERENCE)
        throw ref new Platform::InvalidArgumentException("ref for JsValue is JS_INVALID_REFERENCE");
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
    }
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}

IJsObject^ JsValue::GlobalObject::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetGlobalObject(&ref));
    return ref new JsObjectImpl(ref);
}
