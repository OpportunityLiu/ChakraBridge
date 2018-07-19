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
    const wchar_t* str;
    size_t len;
    CHAKRA_CALL(JsStringToPointer(strref, &str, &len));
    return ref new string(str, static_cast<unsigned int>(len));
}
