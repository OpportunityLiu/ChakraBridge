#include "pch.h"
#include "Helper.h"
#include <sstream>

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

void __CHAKRA_CALL_GetChakraError()
{
    JsContext::GetAndClearExceptionCore();
}

string^ __CHAKRA_CALL_MakeMessage(const wchar_t* message, const wchar_t* expr, const int line, const wchar_t* file)
{
    std::wstringstream stream;
    stream << message << std::endl
        << std::endl
        << L"At: " << std::endl
        << expr << std::endl
        << file << L':' << line;
    return ref new string(stream.str().c_str());
}