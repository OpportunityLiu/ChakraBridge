#include "pch.h"
#include "JsNumber.h"
#include <sstream>

using namespace Opportunity::ChakraBridge::WinRT;

JsNumberImpl::JsNumberImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

int32 JsNumberImpl::ToInt32()
{
    return static_cast<int32>(RawNumberToInt(Reference));
}

float64 JsNumberImpl::ToDouble()
{
    return static_cast<float64>(RawNumberToDouble(Reference));
}

string^ JsNumberImpl::ToString()
{
    std::wostringstream strs;
    strs << ToDouble();
    std::wstring str = strs.str();
    return ref new string(str.c_str());
}

IJsNumber^ JsNumber::Of(int32 value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsIntToNumber(value, &ref));
    return ref new JsNumberImpl(ref);
}

IJsNumber^ JsNumber::Of(float64 value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsDoubleToNumber(value, &ref));
    return ref new JsNumberImpl(ref);
}
