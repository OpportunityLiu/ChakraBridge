#include "pch.h"
#include "Helper.h"

using namespace Opportunity::ChakraBridge::WinRT;

string^ Opportunity::ChakraBridge::WinRT::CHAKRA_LAST_ERROR()
{
    const auto error = JsContext::LastJsError;
    if (error == JS_INVALID_REFERENCE)
        return nullptr;
    try
    {
        JsValueRef str;
        CHAKRA_CALL(JsConvertValueToString(error, &str));
        return RawStringToPointer(str);
    }
    catch (...)
    {
        return nullptr;
    }
}

void GetChakraError()
{
    JsContext::GetAndClearExceptionCore();
}