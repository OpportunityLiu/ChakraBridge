#include "pch.h"
#include "JsNumber.h"
#include <sstream>

using namespace Opportunity::ChakraBridge::WinRT;

JsNumberImpl::JsNumberImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

int32 JsNumberImpl::ToInt32()
{
    int32 v;
    CHAKRA_CALL(JsNumberToInt(Reference, &v));
    return v;
}

float64 JsNumberImpl::ToDouble()
{
    float64 v;
    CHAKRA_CALL(JsNumberToDouble(Reference, &v));
    return v;
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
