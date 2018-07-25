#include "pch.h"
#include "JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContext::JsContext(const RawContext ref, JsRuntime^const runtime)
    :Reference(std::move(ref)), Rt(runtime)
{
    _ASSERTE(runtime != nullptr);
    _ASSERTE(Reference.IsValid());
    _ASSERTE(Reference.Runtime() == runtime->Handle);

    Reference.AddRef();
}

void JsContext::ThrowIfDestoried()
{
    if (!Reference.IsValid())
        Throw(RPC_E_DISCONNECTED, L"The context has been disposed.");
}

void JsContext::PreDestory()
{
    if (!Reference.IsValid())
        return;
    Reference = nullptr;
    Rt = nullptr;
}

/// <summary>
/// Releases reference to the script context.
/// </summary>
JsContext::~JsContext()
{
    if (!Reference.IsValid())
        // has been set to invalid by JsRuntime::~JsRuntime
        return;
    if (RawContext::Current() == Reference)
        RawContext::Current(nullptr);
    Rt->Contexts.erase(Reference);
    Reference.Release();
    PreDestory();
}

JsRuntime^ JsContext::Runtime::get()
{
    ThrowIfDestoried();
    _ASSERTE(Rt != nullptr);
    return Rt;
}

/// <summary>
/// Use the context in the following scope.
/// </summary>
/// <param name="disposeContext">
/// Whether <see cref="~JsContext()"/> need to be invoke when invoking <see cref="JsContextScope::~JsContextScope()"/>.
/// </param>
/// <returns>A helper class to set the previous context back when invoking <see cref="JsContextScope::~JsContextScope()"/>.</returns>
/// <remarks>
/// Usage:
/// <code>
/// using(jsContext.Use())
/// {
///     ...
/// }
/// </code>
/// </remarks>
JsContextScope^ JsContext::Use(bool disposeContext)
{
    ThrowIfDestoried();
    return ref new JsContextScope(this, disposeContext);
}