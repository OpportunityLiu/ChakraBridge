#include "pch.h"
#include "Native\BufferPointer.h"
#include "JsContext.h"
#include "Value\Declare.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsSourceContext JsContext::SourceContext = 0;

void JsContext::JsPromiseContinuationCallbackImpl(const JsValueRef task, void*const callbackState)
{
    const auto current = Get(callbackState);
    const auto tt = RawValue(task);
    tt.AddRef();
    current->PromiseContinuationQueue.push(tt);
}

void JsContext::HandlePromiseContinuation()
{
    const auto current = Current;
    const RawValue global = RawValue::GlobalObject();
    while (!current->PromiseContinuationQueue.empty())
    {
        auto task = current->PromiseContinuationQueue.front();
        current->PromiseContinuationQueue.pop();
        void(task.Invoke(global));
        task.Release();
    }
}

IBuffer^ JsContext::SerializeScript(string^ script)
{
    NULL_CHECK(script);
    const auto bufferSize = RawContext::SerializeScript(script->Data());
    const auto buf = ref new Windows::Storage::Streams::Buffer(bufferSize);
    buf->Length = RawContext::SerializeScript(script->Data(), GetPointerOfBuffer(buf));
    return buf;
}

IJsFunction^ JsContext::ParseScript(string^ script, IBuffer^ buffer, string^ sourceName)
{
    const auto pointer = GetPointerOfBuffer(buffer);
    const auto r = RawContext::ParseScript(script->Data(), pointer, SourceContext++, sourceName->Data());
    return ref new JsFunctionImpl(r);
}

IJsValue^ JsContext::RunScript(string^ script, IBuffer^ buffer, string^ sourceName)
{
    const auto pointer = GetPointerOfBuffer(buffer);
    const auto r = RawContext::RunScript(script->Data(), pointer, SourceContext++, sourceName->Data());
    HandlePromiseContinuation();
    return JsValue::CreateTyped(r);
}

IJsFunction^ JsContext::ParseScript(string^ script, string^ sourceName)
{
    NULL_CHECK(script);
    const auto r = RawContext::ParseScript(script->Data(), SourceContext++, sourceName->Data());
    return ref new JsFunctionImpl(r);
}

IJsValue^ JsContext::RunScript(string^ script, string^ sourceName)
{
    NULL_CHECK(script);
    const auto r = RawContext::RunScript(script->Data(), SourceContext++, sourceName->Data());
    HandlePromiseContinuation();
    return JsValue::CreateTyped(r);
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
    const auto pointer = GetPointerOfBuffer(buffer);
    LoadSource[SourceContext] = scriptLoadCallback;
    const auto r = RawContext::ParseScript(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data());
    return ref new JsFunctionImpl(r);
}

IJsValue^ JsContext::RunScript(Opportunity::ChakraBridge::WinRT::JsSerializedScriptLoadSourceCallback^ scriptLoadCallback, IBuffer^ buffer, string^ sourceUrl)
{
    NULL_CHECK(scriptLoadCallback);
    const auto pointer = GetPointerOfBuffer(buffer);
    LoadSource[SourceContext] = scriptLoadCallback;
    const auto r = RawContext::RunScript(JsSerializedScriptLoadSourceCallbackImpl, JsSerializedScriptUnloadCallbackImpl, pointer, SourceContext++, sourceUrl->Data());
    HandlePromiseContinuation();
    return JsValue::CreateTyped(r);
}