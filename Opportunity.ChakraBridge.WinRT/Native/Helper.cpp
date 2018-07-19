#include "pch.h"
#include "Helper.h"

using namespace Opportunity::ChakraBridge::WinRT;

string^ GetChakraError()
{
    JsValueRef error;
    CHAKRA_CALL(JsGetAndClearException(&error));
    JsPropertyIdRef messageP;
    CHAKRA_CALL(JsGetPropertyIdFromName(L"message", &messageP));
    JsValueRef message;
    CHAKRA_CALL(JsGetProperty(error, messageP, &message));
    const wchar_t * msg;
    size_t len;
    CHAKRA_CALL(JsStringToPointer(message, &msg, &len));
    return ref new string(msg, static_cast<uint32>(len));
}
