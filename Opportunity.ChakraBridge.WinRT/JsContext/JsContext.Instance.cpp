#include "pch.h"
#include "JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContext::JsContext(JsContextRef ref)
    :Reference(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

/// <summary>
/// Releases reference to the script context.
/// </summary>
JsContext::~JsContext()
{
    if (this->Reference == JS_INVALID_REFERENCE)
        return;
    JsContextRef ct;
    CHAKRA_CALL(JsGetCurrentContext(&ct));
    if (this->Reference == ct)
        Throw(E_ILLEGAL_METHOD_CALL, L"Can not dispose JsContexts in using.");
    this->Runtime->Contexts.erase(this->Reference);
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
    this->Reference = JS_INVALID_REFERENCE;
}

JsRuntime^ JsContext::Runtime::get()
{
    JsRuntimeHandle rth;
    CHAKRA_CALL(JsGetRuntime(this->Reference, &rth));
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