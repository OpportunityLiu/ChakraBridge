#include "pch.h"
#include "JsRuntime.h"
#include <mutex>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <concrt.h>
#include "JsContext\JsContext.Instance.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<JsRuntimeHandle, JsRuntime^> JsRuntime::RuntimeDictionary;
std::mutex mx;

bool CALLBACK JsThreadServiceCallbackImpl(_In_ JsBackgroundWorkItemCallback callback, _In_opt_ void *callbackState)
{
    concurrency::create_task([callback, callbackState]()
    {
        callback(callbackState);
    });

    return true;
}

void CALLBACK JsRuntime::JsBeforeCollectCallbackImpl(_In_opt_ void *callbackState)
{
    JsRuntimeHandle rth = static_cast<JsRuntimeHandle>(callbackState);
    auto rt = JsRuntime::RuntimeDictionary[rth];
    rt->CollectingGarbage(rt, nullptr);
}

JsRuntime::JsRuntime(JsRuntimeHandle handle)
    : Handle(handle)
{
    std::lock_guard<std::mutex> lock(mx);
    RuntimeDictionary.insert(std::make_pair(handle, this));
    CHAKRA_CALL(JsSetRuntimeBeforeCollectCallback(handle, handle, JsBeforeCollectCallbackImpl));
}

JsRuntime::~JsRuntime()
{
    if (this->Handle == JS_INVALID_RUNTIME_HANDLE)
        return;
    std::lock_guard<std::mutex> lock(mx);
    if (this->Handle == JS_INVALID_RUNTIME_HANDLE)
        return;
    JsContextRef cc;
    CHAKRA_CALL(JsGetCurrentContext(&cc));
    if (cc != JS_INVALID_REFERENCE)
    {
        JsRuntimeHandle cr;
        CHAKRA_CALL(JsGetRuntime(cc, &cr));
        if (cr == this->Handle)
            CHAKRA_CALL(JsSetCurrentContext(JS_INVALID_REFERENCE));
    }
    CHAKRA_CALL(JsDisposeRuntime(this->Handle));
    RuntimeDictionary.erase(this->Handle);
    std::for_each(this->Contexts.begin(), this->Contexts.end(), [](auto& item) { item.second->Reference = JS_INVALID_REFERENCE; });
    this->Contexts.clear();
    this->Handle = JS_INVALID_RUNTIME_HANDLE;
}

JsRuntime^ JsRuntime::Create(Opportunity::ChakraBridge::WinRT::JsRuntimeAttributes attributes)
{
    JsRuntimeHandle handle;
    CHAKRA_CALL(JsCreateRuntime(static_cast<::JsRuntimeAttributes>(attributes), JsThreadServiceCallbackImpl, &handle));
    return ref new JsRuntime(handle);
}
