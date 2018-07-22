#include "pch.h"
#include "JsSymbol.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsSymbolImpl::~JsSymbolImpl()
{
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
}

JsSymbolImpl::JsSymbolImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

string^ JsSymbolImpl::ToString()
{
    auto tostrFunc = RawGetProperty(RawGlobalObject(), L"Symbol", L"prototype", L"toString");
    JsValueRef strref = RawCallFunction(tostrFunc, this->Reference);
    return RawStringToPointer(strref);
}

IJsSymbol^ InnerCreate(JsValueRef description)
{
    JsValueRef r;
    CHAKRA_CALL(JsCreateSymbol(description, &r));
    return ref new JsSymbolImpl(r);
}

IJsSymbol^ JsSymbol::Create(IJsValue^ description)
{
    if (description == nullptr)
        return Create();
    return InnerCreate(to_impl(description)->Reference);
}

IJsSymbol^ JsSymbol::Create(string^ description)
{
    if (description == nullptr)
        return Create();
    return InnerCreate(RawPointerToString(description));
}

IJsSymbol^ JsSymbol::Create()
{
    return InnerCreate(JS_INVALID_REFERENCE);
}
