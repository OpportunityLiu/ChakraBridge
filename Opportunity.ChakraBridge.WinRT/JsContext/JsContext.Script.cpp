#include "pch.h"
#include "JsContext.Instance.h"
using namespace Opportunity::ChakraBridge::WinRT;

JsSourceContext JsContext::SourceContext = 0;
JsValueRef JsContext::PromiseContinuation = JS_INVALID_REFERENCE;

void CALLBACK JsContext::JsPromiseContinuationCallbackImpl(_In_ JsValueRef task, _In_opt_ void *callbackState)
{
    JsContext::PromiseContinuation = task;
}

void JsContext::HandlePromiseContinuation()
{
    JsValueRef caller = JS_INVALID_REFERENCE;
    CHAKRA_CALL(JsGetGlobalObject(&caller));
    while (PromiseContinuation != JS_INVALID_REFERENCE)
    {
        JsValueRef result;
        auto task = PromiseContinuation;
        PromiseContinuation = JS_INVALID_REFERENCE;
        CHAKRA_CALL(JsCallFunction(task, &caller, 1, &result));
    }
}

JsContext::IBuffer^ JsContext::SerializeScript(Platform::String^ script)
{
    if (script->IsEmpty())
        throw ref new Platform::InvalidArgumentException("script is null or empty.");
    unsigned long bufferSize = 0;
    CHAKRA_CALL(JsSerializeScript(script->Data(), nullptr, &bufferSize));
    auto buf = ref new Windows::Storage::Streams::Buffer(bufferSize);
    CHAKRA_CALL(JsSerializeScript(script->Data(), GetPointerOfBuffer(buf, nullptr), &bufferSize));
    buf->Length = bufferSize;
    return buf;
}

JsValue^ JsContext::ParseScript(Platform::String^ script, IBuffer^ buffer, Platform::String^ sourceName)
{
    unsigned int buflen;
    auto pointer = GetPointerOfBuffer(buffer, &buflen);
    JsValueRef result;
    CHAKRA_CALL(JsParseSerializedScript(script->Data(), pointer, SourceContext++, sourceName->Data(), &result));
    return ref new JsValue(result);
}

JsValue^ JsContext::RunScript(Platform::String^ script, IBuffer^ buffer, Platform::String^ sourceName)
{
    unsigned int buflen;
    auto pointer = GetPointerOfBuffer(buffer, &buflen);
    JsValueRef result;
    CHAKRA_CALL(JsRunSerializedScript(script->Data(), pointer, SourceContext++, sourceName->Data(), &result));
    return ref new JsValue(result);
}

JsValue^ JsContext::ParseScript(Platform::String^ script, Platform::String^ sourceName)
{
    if (script->IsEmpty())
        throw ref new Platform::InvalidArgumentException("script is null or empty.");
    JsValueRef result;
    CHAKRA_CALL(JsParseScript(script->Data(), SourceContext++, sourceName->Data(), &result));
    return ref new JsValue(result);
}

JsValue^ JsContext::RunScript(Platform::String^ script, Platform::String^ sourceName)
{
    if (script->IsEmpty())
        throw ref new Platform::InvalidArgumentException("script is null or empty.");
    JsValueRef result;
    CHAKRA_CALL(JsRunScript(script->Data(), SourceContext++, sourceName->Data(), &result));
    HandlePromiseContinuation();
    return ref new JsValue(result);
}

std::unordered_map<JsSourceContext, Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^> LoadSource;
std::unordered_map<JsSourceContext, std::unique_ptr<wchar_t[]>> UnloadSource;

bool CALLBACK JsSerializedScriptLoadSourceCallbackImpl(_In_ JsSourceContext sourceContext, _Outptr_result_z_ const wchar_t** scriptBuffer)
{
    auto cbp = LoadSource.find(sourceContext);
    if (cbp == LoadSource.end())
        return false;
    auto cb = cbp->second;
    if (cb == nullptr)
        return false;
    try
    {
        Platform::String^ s;
        if (!cb(&s))
            return false;
        auto buf = std::unique_ptr<wchar_t[]>(new wchar_t[s->Length() + 1]);
        wcscpy_s(buf.get(), s->Length() + 1, s->Data());
        *scriptBuffer = buf.get();
        UnloadSource[sourceContext] = std::move(buf);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void CALLBACK JsSerializedScriptUnloadCallbackImpl(_In_ JsSourceContext sourceContext)
{
    LoadSource.erase(sourceContext);
    UnloadSource.erase(sourceContext);
}

JsValue^ JsContext::ParseScript(Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, Platform::String^ sourceUrl)
{
    if (scriptLoadCallback == nullptr)
        throw ref new Platform::InvalidArgumentException("scriptLoadCallback is null.");
    auto pointer = GetPointerOfBuffer(buffer, nullptr);
    LoadSource[SourceContext] = scriptLoadCallback;
    JsValueRef result;
    CHAKRA_CALL(JsParseSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data(), &result));
    return ref new JsValue(result);
}

JsValue^ JsContext::RunScript(Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, Platform::String^ sourceUrl)
{
    if (scriptLoadCallback == nullptr)
        throw ref new Platform::InvalidArgumentException("scriptLoadCallback is null.");
    auto pointer = GetPointerOfBuffer(buffer, nullptr);
    LoadSource[SourceContext] = scriptLoadCallback;
    JsValueRef result;
    CHAKRA_CALL(JsRunSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data(), &result));
    HandlePromiseContinuation();
    return ref new JsValue(result);
}