#include "pch.h"
#include "JsNumber.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsNumberImpl::JsNumberImpl(RawValue ref)
    :JsValueImpl(std::move(ref)) {}

int32 JsNumberImpl::ToInt32()
{
    return static_cast<int32>(Reference.ToInt());
}

float64 JsNumberImpl::ToDouble()
{
    return static_cast<float64>(Reference.ToDouble());
}

string^ JsNumberImpl::ToString()
{
    return Reference.ToJsString().ToString();
}

IJsNumber^ JsNumber::Create(int32 value)
{
    return ref new JsNumberImpl(RawValue(static_cast<int>(value)));
}

IJsNumber^ JsNumber::Create(float64 value)
{
    return ref new JsNumberImpl(RawValue(static_cast<double>(value)));
}
