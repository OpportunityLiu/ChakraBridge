#include "pch.h"
#include "JsError.h"

using namespace Opportunity::ChakraBridge::WinRT;

string^ JsErrorImpl::Message::get()
{
    try
    {
        return Reference[L"message"]().ToString();
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Message::set(string^ value)
{
    Reference[L"message"] = RawValue(value->Data(), value->Length());
}

string^ JsErrorImpl::Name::get()
{
    try
    {
        return Reference[L"name"]().ToString();
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Name::set(string^ value)
{
    Reference[L"name"] = RawValue(value->Data(), value->Length());
}

string^ JsErrorImpl::Stack::get()
{
    try
    {
        return Reference[L"stack"]().ToString();
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsErrorImpl::Stack::set(string^ value)
{
    Reference[L"stack"] = RawValue(value->Data(), value->Length());
}

#define CREATE_ERROR_WITH_JSSTRING(methodName)                                                    \
IJsError^ JsError::methodName(IJsString^ message)                                                 \
{                                                                                                 \
    return ref new JsErrorImpl(RawValue::methodName(get_ref_or_undefined(message)));              \
}

#define CREATE_ERROR_WITH_STRING(methodName)                                                       \
IJsError^ JsError::methodName(string^ message)                                                     \
{                                                                                                  \
    return ref new JsErrorImpl(RawValue::methodName(RawValue(message->Data(), message->Length())));\
}

CREATE_ERROR_WITH_JSSTRING(CreateError);
CREATE_ERROR_WITH_STRING(CreateError);

CREATE_ERROR_WITH_JSSTRING(CreateRangeError);
CREATE_ERROR_WITH_STRING(CreateRangeError);

CREATE_ERROR_WITH_JSSTRING(CreateReferenceError);
CREATE_ERROR_WITH_STRING(CreateReferenceError);

CREATE_ERROR_WITH_JSSTRING(CreateSyntaxError);
CREATE_ERROR_WITH_STRING(CreateSyntaxError);

CREATE_ERROR_WITH_JSSTRING(CreateTypeError);
CREATE_ERROR_WITH_STRING(CreateTypeError);

CREATE_ERROR_WITH_JSSTRING(CreateUriError);
CREATE_ERROR_WITH_STRING(CreateUriError);
