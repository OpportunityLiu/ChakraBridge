#include "pch.h"
#include "JsValue.h"
#include "JsObject.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsValueImpl::JsValueImpl(JsValueRef ref)
    : Reference(ref)
{
    if (Reference == JS_INVALID_REFERENCE)
        throw ref new Platform::InvalidArgumentException("ref for JsValue is JS_INVALID_REFERENCE");
}

JsValueImpl^ JsValue::CreateTyped(JsValueRef ref)
{
    if (ref == JS_INVALID_REFERENCE)
        return nullptr;
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}

IJsObject^ JsValue::GlobalObject::get()
{
    JsValueRef ref;
    CHAKRA_CALL(JsGetGlobalObject(&ref));
    return ref new JsObjectImpl(ref);
}
