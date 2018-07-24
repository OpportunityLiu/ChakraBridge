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

std::unordered_map<RawRuntime, JsRuntime^> JsRuntime::RuntimeDictionary;
std::mutex mx;

bool JsRuntime::JsThreadServiceCallbackImpl(const JsBackgroundWorkItemCallback callback, void * const callbackState)
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

JsRuntime::JsRuntime(RawRuntime handle)
    : Handle(std::move(handle))
{
    if (!Handle.IsValid())
        Throw(E_HANDLE, L"handle for JsRuntime is JS_INVALID_RUNTIME_HANDLE");
    std::lock_guard<std::mutex> lock(mx);

    RuntimeDictionary.insert(std::make_pair(Handle, this));
    const auto& rawH = Handle.Ref;
    CHAKRA_CALL(JsSetRuntimeBeforeCollectCallback(rawH, rawH, JsBeforeCollectCallbackImpl));
    CHAKRA_CALL(JsSetRuntimeMemoryAllocationCallback(rawH, rawH, JsMemoryAllocationCallbackImpl));
}

JsRuntime::~JsRuntime()
{
    std::lock_guard<std::mutex> lock(mx);

    const auto cc = RawContext::Current();
    if (cc.IsValid())
    {
        RawRuntime cr = cc.Runtime();
        if (cr == Handle)
            RawContext::Current(RawContext::Invalid());
    }
    CHAKRA_CALL(JsDisposeRuntime(Handle.Ref));
    RuntimeDictionary.erase(Handle);
    std::for_each(this->Contexts.begin(), this->Contexts.end(), [](auto& item) { item.second->Reference = RawContext::Invalid(); });
    this->Contexts.clear();
}

void JsRuntime::CollectGarbage()
{
    Handle.CollectGarbage();
}

JsContext^ JsRuntime::CreateContext()
{
    RawContext ref;
    CHAKRA_CALL(JsCreateContext(Handle.Ref, &ref.Ref));
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

JsRuntime^ JsRuntime::Create(JsRtAttr attributes)
{
    JsRuntimeHandle handle;
    CHAKRA_CALL(JsCreateRuntime(static_cast<::JsRuntimeAttributes>(attributes), JsThreadServiceCallbackImpl, &handle));
    return ref new JsRuntime(handle);
}

uint64 JsRuntime::MemoryUsage::get()
{
    return Handle.MemoryUsage();
}

uint64 JsRuntime::MemoryLimit::get()
{
    return Handle.MemoryLimit();
}

void JsRuntime::MemoryLimit::set(uint64 value)
{
    if (value > std::numeric_limits<size_t>::max())
        Throw(E_INVALIDARG, L"value is too large.");
    Handle.MemoryLimit(static_cast<size_t>(value));
}

bool JsRuntime::IsEnabled::get()
{
    return Handle.Enabled();
}

void JsRuntime::IsEnabled::set(bool value)
{
    Handle.Enabled(value);
}

Windows::Foundation::Collections::IIterable<JsRuntime^>^ JsRuntime::GetRuntimes()
{
    std::vector<JsRuntime^> vec;
    for (auto& v : RuntimeDictionary)
        vec.push_back(v.second);
    return ref new Platform::Collections::VectorView<JsRuntime^>(std::move(vec));
}
