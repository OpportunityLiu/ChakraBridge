#include "pch.h"
#include "JsContext.h"
#include "Value\JsError.h"

using namespace Opportunity::ChakraBridge::WinRT;

/// <summary>
/// Get the instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JS_INVALID_REFERENCE"/>. 
/// </summary>
/// <param name="reference">The reference.</param>
/// <returns>The instance of the <see cref="JsContext"/></returns>
JsContext^ JsContext::Get(const RawContext& reference)
{
    if (!reference.IsValid())
        return nullptr;
    const auto rth = reference.Runtime();
    const auto rt = JsRuntime::RuntimeDictionary[rth].Resolve<JsRuntime>();
    _ASSERTE(rt != nullptr);
    const auto ctx = rt->Contexts[reference].Resolve<JsContext>();
    _ASSERTE(ctx != nullptr);
    return ctx;
}

void JsContext::StartDebugging()
{
    RawContext::StartDebugging();
}

uint32 JsContext::Idle()
{
    return static_cast<uint32>(RawContext::Idle());
}

void JsContext::SetException(IJsError^ exception)
{
    NULL_CHECK(exception);
    RawContext::SetException(get_ref(exception));
}

RawValue JsContext::LastJsError = nullptr;

void JsContext::GetAndClearExceptionCore()
{
    if (JsGetAndClearException(&LastJsError.Ref) != ::JsNoError)
        LastJsError = nullptr;
}

IJsError^ JsContext::GetAndClearException()
{
    GetAndClearExceptionCore();
    return LastError;
}

IJsError^ JsContext::LastError::get()
{
    return dynamic_cast<IJsError^>(JsValue::CreateTyped(LastJsError));
}

void JsContext::ProjectWinRTNamespace(string^ namespaceName)
{
    NULL_CHECK(namespaceName);
    RawContext::ProjectWinRTNamespace(namespaceName->Data());
}

JsContext^ JsContext::Current::get()
{
    return Get(RawContext::Current());
}

void JsContext::Current::set(JsContext^ value)
{
    if (value == nullptr)
        RawContext::Current(nullptr);
    else
    {
        value->ThrowIfDestoried();
        const auto ref = value->Reference;
        RawContext::Current(ref);
        RawContext::SetPromiseContinuationCallback<RawContext, JsPromiseContinuationCallbackImpl>(ref);
    }
    LastJsError = nullptr;
}

bool JsContext::HasException::get()
{
    return RawContext::HasException();
}