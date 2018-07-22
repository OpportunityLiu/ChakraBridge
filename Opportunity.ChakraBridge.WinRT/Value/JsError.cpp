#include "pch.h"
#include "JsError.h"

using namespace Opportunity::ChakraBridge::WinRT;

string^ JsErrorImpl::Message::get()
{
    auto p = RawGetProperty(Reference, L"message");
    try
    {
        return RawStringToPointer(p);
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Message::set(string^ value)
{
    auto v = RawPointerToString(value);
    RawSetProperty(Reference, L"message", v);
}

string^ JsErrorImpl::Name::get()
{
    auto p = RawGetProperty(Reference, L"name");
    try
    {
        return RawStringToPointer(p);
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Name::set(string^ value)
{
    auto v = RawPointerToString(value);
    RawSetProperty(Reference, L"name", v);
}

string^ JsErrorImpl::Stack::get()
{
    auto p = RawGetProperty(Reference, L"stack");
    try
    {
        return RawStringToPointer(p);
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Stack::set(string^ value)
{
    auto v = RawPointerToString(value);
    RawSetProperty(Reference, L"Stack", v);
}

#define CREATE_ERROR_WITH_JSSTRING(methodName, implName)                          \
IJsError^ JsError::methodName(IJsString^ message)                                 \
{                                                                                 \
    auto ref = message == nullptr ? RawUndefined() : to_impl(message)->Reference; \
    JsValueRef r;                                                                 \
    CHAKRA_CALL(implName(ref, &r));                                               \
    return ref new JsErrorImpl(r);                                                \
}

#define CREATE_ERROR_WITH_STRING(methodName, implName)                            \
IJsError^ JsError::methodName(string^ message)                                    \
{                                                                                 \
    auto ref = RawPointerToString(message);                                       \
    JsValueRef r;                                                                 \
    CHAKRA_CALL(implName(ref, &r));                                               \
    return ref new JsErrorImpl(r);                                                \
}

CREATE_ERROR_WITH_JSSTRING(CreateError, JsCreateError);
CREATE_ERROR_WITH_STRING(CreateError, JsCreateError);

CREATE_ERROR_WITH_JSSTRING(CreateRangeError, JsCreateRangeError);
CREATE_ERROR_WITH_STRING(CreateRangeError, JsCreateRangeError);

CREATE_ERROR_WITH_JSSTRING(CreateReferenceError, JsCreateReferenceError);
CREATE_ERROR_WITH_STRING(CreateReferenceError, JsCreateReferenceError);

CREATE_ERROR_WITH_JSSTRING(CreateSyntaxError, JsCreateSyntaxError);
CREATE_ERROR_WITH_STRING(CreateSyntaxError, JsCreateSyntaxError);

CREATE_ERROR_WITH_JSSTRING(CreateTypeError, JsCreateTypeError);
CREATE_ERROR_WITH_STRING(CreateTypeError, JsCreateTypeError);

CREATE_ERROR_WITH_JSSTRING(CreateUriError, JsCreateURIError);
CREATE_ERROR_WITH_STRING(CreateUriError, JsCreateURIError);
