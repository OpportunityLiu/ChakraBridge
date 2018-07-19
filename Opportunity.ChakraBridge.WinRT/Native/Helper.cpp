#include "pch.h"
#include "Helper.h"

using namespace Opportunity::ChakraBridge::WinRT;

string^ GetChakraError()
{
    JsValueRef error;
    CHAKRA_CALL(JsGetAndClearException(&error));
    auto message = RawGetProperty(error, L"message");
    try
    {
        return RawStringToPointer(message);
    }
    catch (...)
    {
        return JsValue::CreateTyped(message)->ToString();
    }
}
