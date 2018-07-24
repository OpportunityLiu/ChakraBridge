#include "pch.h"
#include "JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContext::JsContext(const RawContext ref)
    :Reference(std::move(ref))
{
    if(!Reference.IsValid())
        Throw(E_HANDLE, L"ref for JsContext is JS_INVALID_REFERENCE");
    Reference.AddRef();
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
        RawContext::Current(RawContext::Invalid());
    this->Runtime->Contexts.erase(Reference);
    Reference.Release();
}

JsRuntime^ JsContext::Runtime::get()
{
    const auto rth = Reference.Runtime();
    return JsRuntime::RuntimeDictionary[rth];
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
    return ref new JsContextScope(this, disposeContext);
}