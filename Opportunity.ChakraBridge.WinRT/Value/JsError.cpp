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