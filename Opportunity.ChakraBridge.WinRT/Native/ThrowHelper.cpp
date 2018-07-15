#include "pch.h"
#include "ThrowHelper.h"

Platform::String^ GetCharkaError()
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
    return ref new Platform::String(msg, static_cast<uint32>(len));
}
