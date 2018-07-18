#include "pch.h"
#include "JsRuntime.h"
#include <mutex>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <concrt.h>
#include <limits>
#include "JsContext\JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<JsRuntimeHandle, JsRuntime^> JsRuntime::RuntimeDictionary;
std::mutex mx;

bool CALLBACK JsThreadServiceCallbackImpl(_In_ JsBackgroundWorkItemCallback callback, _In_opt_ void *callbackState)
{
    using namespace Windows::System::Threading;
    auto wih = ref new WorkItemHandler([callback, callbackState](Windows::Foundation::IAsyncAction^ workItem)
    {
        callback(callbackState);
    });
    ThreadPool::RunAsync(wih);
    return true;
}

void CALLBACK JsRuntime::JsBeforeCollectCallbackImpl(_In_opt_ void *callbackState)
{
    JsRuntimeHandle rth = static_cast<JsRuntimeHandle>(callbackState);
    auto rt = JsRuntime::RuntimeDictionary[rth];
    rt->CollectingGarbage(rt, nullptr);
}

bool CALLBACK JsRuntime::JsMemoryAllocationCallbackImpl(_In_opt_ void *callbackState, _In_::JsMemoryEventType allocationEvent, _In_ size_t allocationSize)
{
    JsRuntimeHandle rth = static_cast<JsRuntimeHandle>(callbackState);
    auto rt = JsRuntime::RuntimeDictionary[rth];
    auto args = ref new JsMemoryEventArgs(allocationEvent, allocationSize);
    rt->AllocatingMemory(rt, args);
    return true;// !args->IsRejected;
}

JsRuntime::JsRuntime(JsRuntimeHandle handle)
    : Handle(handle)
{
    std::lock_guard<std::mutex> lock(mx);
    RuntimeDictionary.insert(std::make_pair(handle, this));
    CHAKRA_CALL(JsSetRuntimeBeforeCollectCallback(handle, handle, JsBeforeCollectCallbackImpl));
    CHAKRA_CALL(JsSetRuntimeMemoryAllocationCallback(handle, handle, JsMemoryAllocationCallbackImpl));
}

JsContext^ JsRuntime::CreateContext()
{
    JsContextRef ref;
    CHAKRA_CALL(JsCreateContext(this->Handle, &ref));
    auto context = ref new JsContext(ref);
    this->Contexts.insert(std::make_pair(ref, context));
    return context;
}

Windows::Foundation::Collections::IIterable<JsContext^>^ JsRuntime::GetContexts()
{
    std::vector<JsContext^> vec;
    for (auto& v : Contexts)
        vec.push_back(v.second);
    return ref new Platform::Collections::VectorView<JsContext^>(std::move(vec));
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

uint64 JsRuntime::MemoryUsage::get()
{
    size_t memoryUsage;
    CHAKRA_CALL(JsGetRuntimeMemoryUsage(this->Handle, &memoryUsage));
    return memoryUsage;
}

uint64 JsRuntime::MemoryLimit::get()
{
    size_t memoryLimit;
    CHAKRA_CALL(JsGetRuntimeMemoryLimit(this->Handle, &memoryLimit));
    return memoryLimit;
}

void JsRuntime::MemoryLimit::set(uint64 value)
{
    if (value > std::numeric_limits<size_t>::max())
        throw ref new Platform::InvalidArgumentException();
    CHAKRA_CALL(JsSetRuntimeMemoryLimit(this->Handle, static_cast<size_t>(value)));
}

bool JsRuntime::IsEnabled::get()
{
    bool isDisabled;
    CHAKRA_CALL(JsIsRuntimeExecutionDisabled(this->Handle, &isDisabled));
    return !isDisabled;
}

void JsRuntime::IsEnabled::set(bool value)
{
    if (value)
        CHAKRA_CALL(JsEnableRuntimeExecution(this->Handle));
    else
        CHAKRA_CALL(JsDisableRuntimeExecution(this->Handle));
}

Windows::Foundation::Collections::IIterable<JsRuntime^>^ JsRuntime::GetRuntimes()
{
    std::vector<JsRuntime^> vec;
    for (auto& v : RuntimeDictionary)
        vec.push_back(v.second);
    return ref new Platform::Collections::VectorView<JsRuntime^>(std::move(vec));
}
