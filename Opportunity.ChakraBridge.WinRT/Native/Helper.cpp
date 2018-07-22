#include "pch.h"
#include "Helper.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsValueRef LastJsError;

void GetChakraError()
{
    CHAKRA_CALL(JsGetAndClearException(&LastJsError));
}

string^ Opportunity::ChakraBridge::WinRT::CHAKRA_LAST_ERROR()
{
    auto message = RawGetProperty(LastJsError, L"message");
    try
    {
        return RawStringToPointer(message);
    }
    catch (...)
    {
        return JsValue::CreateTyped(message)->ToString();
    }
}
