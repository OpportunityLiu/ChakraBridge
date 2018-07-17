#include "pch.h"
#include "JsContext.Instance.h"
using namespace Opportunity::ChakraBridge::WinRT;

JsContext^ JsContext::Get(JsContextRef reference)
{
    if (reference == JS_INVALID_REFERENCE)
        return nullptr;
    JsRuntimeHandle rth;
    CHAKRA_CALL(JsGetRuntime(reference, &rth));
    auto rt = JsRuntime::RuntimeDictionary[rth];
    return rt->Contexts[reference];
}

void Opportunity::ChakraBridge::WinRT::JsContext::StartDebugging()
{
    CHAKRA_CALL(JsStartDebugging());
}

uint32 Opportunity::ChakraBridge::WinRT::JsContext::Idle()
{
    unsigned int ticks;
    CHAKRA_CALL(JsIdle(&ticks));
    return static_cast<uint32>(ticks);
}

void Opportunity::ChakraBridge::WinRT::JsContext::ProjectWinRTNamespace(Platform::String ^ namespaceName)
{
    if (namespaceName == nullptr || namespaceName->IsEmpty())
        throw ref new Platform::InvalidArgumentException("namespaceName is null or empty");
    CHAKRA_CALL(JsProjectWinRTNamespace(namespaceName->Data()));
}

JsContext^ JsContext::Current::get()
{
    JsContextRef c;
    CHAKRA_CALL(JsGetCurrentContext(&c));
    return Get(c);
}

void JsContext::Current::set(JsContext^ value)
{
    if (value == nullptr || value->Reference == JS_INVALID_REFERENCE)
    {
        CHAKRA_CALL(JsSetCurrentContext(JS_INVALID_REFERENCE));
    }
    else
    {
        CHAKRA_CALL(JsSetCurrentContext(value->Reference));
        CHAKRA_CALL(JsSetPromiseContinuationCallback(JsPromiseContinuationCallbackImpl, value->Reference));
        return;
    }
}

bool JsContext::HasException::get()
{
    bool hasException;
    CHAKRA_CALL(JsHasException(&hasException));
    return hasException;
}