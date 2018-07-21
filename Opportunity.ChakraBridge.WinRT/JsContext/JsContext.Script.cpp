#include "pch.h"
#include "JsContext.h"
#include "Value\Declare.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsSourceContext JsContext::SourceContext = 0;

void CALLBACK JsContext::JsPromiseContinuationCallbackImpl(_In_ JsValueRef task, _In_opt_ void *callbackState)
{
    auto current = Get(reinterpret_cast<JsContextRef>(callbackState));
    CHAKRA_CALL(JsAddRef(task, nullptr));
    current->PromiseContinuationQueue.push(task);
}

void JsContext::HandlePromiseContinuation()
{
    auto current = Current;
    JsValueRef global = RawGlobalObject();
    while (!current->PromiseContinuationQueue.empty())
    {
        auto task = current->PromiseContinuationQueue.front();
        current->PromiseContinuationQueue.pop();
        RawCallFunction(task, global);
        CHAKRA_CALL(JsRelease(task, nullptr));
    }
}

JsContext::IBuffer^ JsContext::SerializeScript(string^ script)
{
    NULL_CHECK(script);
    unsigned long bufferSize = 0;
    CHAKRA_CALL(JsSerializeScript(script->Data(), nullptr, &bufferSize));
    auto buf = ref new Windows::Storage::Streams::Buffer(bufferSize);
    CHAKRA_CALL(JsSerializeScript(script->Data(), GetPointerOfBuffer(buf, nullptr), &bufferSize));
    buf->Length = bufferSize;
    return buf;
}

IJsFunction^ JsContext::ParseScript(string^ script, IBuffer^ buffer, string^ sourceName)
{
    unsigned int buflen;
    auto pointer = GetPointerOfBuffer(buffer, &buflen);
    JsValueRef result;
    CHAKRA_CALL(JsParseSerializedScript(script->Data(), pointer, SourceContext++, sourceName->Data(), &result));
    return ref new JsFunctionImpl(result);
}

IJsValue^ JsContext::RunScript(string^ script, IBuffer^ buffer, string^ sourceName)
{
    unsigned int buflen;
    auto pointer = GetPointerOfBuffer(buffer, &buflen);
    JsValueRef result;
    CHAKRA_CALL(JsRunSerializedScript(script->Data(), pointer, SourceContext++, sourceName->Data(), &result));
    HandlePromiseContinuation();
    return JsValue::CreateTyped(result);
}

IJsFunction^ JsContext::ParseScript(string^ script, string^ sourceName)
{
    NULL_CHECK(script);
    JsValueRef result;
    CHAKRA_CALL(JsParseScript(script->Data(), SourceContext++, sourceName->Data(), &result));
    return ref new JsFunctionImpl(result);
}

IJsValue^ JsContext::RunScript(string^ script, string^ sourceName)
{
    NULL_CHECK(script);
    JsValueRef result;
    CHAKRA_CALL(JsRunScript(script->Data(), SourceContext++, sourceName->Data(), &result));
    HandlePromiseContinuation();
    return JsValue::CreateTyped(result);
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
        string^ s;
        if (!cb(&s))
            return false;
        auto buflen = s->Length() + 1;
        auto buf = std::unique_ptr<wchar_t[]>(new wchar_t[buflen]);
        wcscpy_s(buf.get(), buflen, s->Data());
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

IJsFunction^ JsContext::ParseScript(Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, string^ sourceUrl)
{
    NULL_CHECK(scriptLoadCallback);
    auto pointer = GetPointerOfBuffer(buffer, nullptr);
    LoadSource[SourceContext] = scriptLoadCallback;
    JsValueRef result;
    CHAKRA_CALL(JsParseSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data(), &result));
    return ref new JsFunctionImpl(result);
}

IJsValue^ JsContext::RunScript(Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, string^ sourceUrl)
{
    NULL_CHECK(scriptLoadCallback);
    auto pointer = GetPointerOfBuffer(buffer, nullptr);
    LoadSource[SourceContext] = scriptLoadCallback;
    JsValueRef result;
    CHAKRA_CALL(JsRunSerializedScriptWithCallback(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data(), &result));
    HandlePromiseContinuation();
    return JsValue::CreateTyped(result);
}