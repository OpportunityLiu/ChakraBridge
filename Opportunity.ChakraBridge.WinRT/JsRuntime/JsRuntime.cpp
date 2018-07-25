#include "pch.h"
#include "JsRuntime.h"
#include <mutex>
#include <limits>
#include "JsContext\JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<RawRuntime, weak_ref> JsRuntime::RuntimeDictionary;
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

void JsRuntime::BeforeCollectCallback(const RWP&callbackState)
{
    const auto rt = callbackState->Runtme.Resolve<JsRuntime>();
    _ASSERTE(rt != nullptr);
    rt->CollectingGarbage(rt, nullptr);
}

bool JsRuntime::MemoryAllocationCallback(const RWP& callbackState, const JsMEType allocationEvent, const size_t allocationSize)
{
    const auto rt = callbackState->Runtme.Resolve<JsRuntime>();
    _ASSERTE(rt != nullptr);
    auto args = ref new JsMemoryEventArgs(allocationEvent, allocationSize);
    rt->AllocatingMemory(rt, args);
    return true;// !args->IsRejected;
}

JsRuntime::JsRuntime(RawRuntime handle)
    : Handle(std::move(handle)), Ptr(std::make_unique<RW>())
{
    _ASSERTE(Handle.IsValid());
    Ptr->Runtme = this;

    {
        std::lock_guard<std::mutex> lock(mx);
        RuntimeDictionary.insert(std::make_pair(Handle, this));
    }

    Handle.BeforeCollectCallback<RWP, BeforeCollectCallback>(Ptr.get());
    Handle.MemoryAllocationCallback<RWP, MemoryAllocationCallback>(Ptr.get());
}

JsRuntime::~JsRuntime()
{
    {
        std::lock_guard<std::mutex> lock(mx);
        RuntimeDictionary.erase(Handle);
    }

    const auto cc = RawContext::Current();
    if (cc.IsValid())
    {
        RawRuntime cr = cc.Runtime();
        if (cr == Handle)
            RawContext::Current(nullptr);
    }
    Handle.Dispose();
    std::for_each(this->Contexts.begin(), this->Contexts.end(), [](auto& item)
    {
        const auto ctx = item.second.Resolve<JsContext>();
        if (ctx)
            ctx->PreDestory();
    });
    this->Contexts.clear();
}

void JsRuntime::CollectGarbage()
{
    Handle.CollectGarbage();
}

JsContext^ JsRuntime::CreateContext()
{
    const auto ref = RawContext(Handle);
    auto context = ref new JsContext(ref, this);
    Contexts[ref] = context;
    return context;
}

JsRuntime^ JsRuntime::Create(JsRA attributes)
{
    return ref new JsRuntime(RawRuntime(attributes, JsThreadServiceCallbackImpl));
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