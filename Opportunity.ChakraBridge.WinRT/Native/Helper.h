#pragma once
#include "alias.h"
#include <jsrt.h>
#include <limits>
#include <vector>

namespace Opportunity::ChakraBridge::WinRT
{
    string_ref CHAKRA_LAST_ERROR();

    inline void __CHAKRA_CALL(const JsErrorCode result, const wchar_t* expr, const int line, const wchar_t* file)
    {
        void __CHAKRA_CALL_GetChakraError();
        string^ __CHAKRA_CALL_MakeMessage(const wchar_t* message, const wchar_t* expr, const int line, const wchar_t* file);
#define THROW(errtype, message) Throw(errtype, __CHAKRA_CALL_MakeMessage(message, expr, line, file))
        if (result == JsErrorCode::JsNoError)
            return;
        switch (result)
        {
        case JsErrorCode::JsErrorInvalidArgument:
            THROW(E_INVALIDARG, L"An argument to a hosting API was invalid.");
        case JsErrorCode::JsErrorNullArgument:
            THROW(E_INVALIDARG, L"An argument to a hosting API was null in a context where null is not allowed.");
        case JsErrorCode::JsErrorNoCurrentContext:
            THROW(E_NOT_VALID_STATE, L"The hosting API requires that a context be current, but there is no current context.");
        case JsErrorCode::JsErrorInExceptionState:
            THROW(E_ILLEGAL_METHOD_CALL, L"The engine is in an exception state and no APIs can be called until the exception is cleared.");
        case JsErrorCode::JsErrorNotImplemented:
            THROW(E_NOTIMPL, L"A hosting API is not yet implemented.");
        case JsErrorCode::JsErrorWrongThread:
            THROW(RPC_E_WRONG_THREAD, L"A hosting API was called on the wrong thread.");
        case JsErrorCode::JsErrorRuntimeInUse:
            THROW(E_ILLEGAL_METHOD_CALL, L"A runtime that is still in use cannot be disposed.");
        case JsErrorCode::JsErrorBadSerializedScript:
            THROW(E_INVALIDARG, L"A bad serialized script was used, or the serialized script was serialized by a different version of the Chakra engine.");
        case JsErrorCode::JsErrorInDisabledState:
            THROW(E_ILLEGAL_METHOD_CALL, L"The runtime is in a disabled state.");
        case JsErrorCode::JsErrorCannotDisableExecution:
            THROW(E_ILLEGAL_STATE_CHANGE, L"Runtime does not support reliable script interruption.");
        case JsErrorCode::JsErrorAlreadyDebuggingContext:
            THROW(E_ILLEGAL_METHOD_CALL, L"The context cannot be put into a debug state because it is already in a debug state.");
        case JsErrorCode::JsErrorHeapEnumInProgress:
            THROW(E_CHANGED_STATE, L"A heap enumeration is currently underway in the script context.");
        case JsErrorCode::JsErrorArgumentNotObject:
            THROW(E_INVALIDARG, L"A hosting API that operates on object values was called with a non-object value.");
        case JsErrorCode::JsErrorInProfileCallback:
            THROW(E_FAIL, L"A script context is in the middle of a profile callback.");
        case JsErrorCode::JsErrorInThreadServiceCallback:
            THROW(E_CHANGED_STATE, L"A thread service callback is currently underway.");
        case JsErrorCode::JsErrorCannotSerializeDebugScript:
            THROW(E_ILLEGAL_METHOD_CALL, L"Scripts cannot be serialized in debug contexts.");
        case JsErrorCode::JsErrorAlreadyProfilingContext:
            THROW(E_ILLEGAL_METHOD_CALL, L"The context cannot start profiling because it is already profiling.");
        case JsErrorCode::JsErrorIdleNotEnabled:
            THROW(E_ILLEGAL_METHOD_CALL, L"Idle notification given when the host did not enable idle processing.");
        case JsErrorCode::JsErrorOutOfMemory:
            THROW(E_OUTOFMEMORY, L"The Chakra engine has run out of memory.");
        case JsErrorCode::JsErrorScriptException:
        {
            string^ error;
            try
            {
                __CHAKRA_CALL_GetChakraError();
                error = CHAKRA_LAST_ERROR();
            }
            catch (...)
            {
                THROW(E_FAIL, L"A JavaScript exception occurred while running a script.");
            }
            THROW(E_FAIL, error->Data());
        }

        case JsErrorCode::JsErrorScriptCompile:
        {
            string^ error;
            try
            {
                __CHAKRA_CALL_GetChakraError();
                error = CHAKRA_LAST_ERROR();
            }
            catch (...)
            {
                THROW(E_FAIL, L"JavaScript failed to compile.");
            }
            THROW(E_FAIL, error->Data());
        }

        case JsErrorCode::JsErrorScriptTerminated:
            THROW(E_ABORT, L"A script was terminated due to a request to suspend a runtime.");
        case JsErrorCode::JsErrorScriptEvalDisabled:
            THROW(E_NOT_VALID_STATE, L"A script was terminated because it tried to use 'eval' or 'function' and eval was disabled.");
        case JsErrorCode::JsErrorFatal:
            THROW(E_FAIL, L"A fatal error in the engine has occurred.");
        case JsErrorCode::JsCannotSetProjectionEnqueueCallback:
            THROW(E_ILLEGAL_DELEGATE_ASSIGNMENT, L"The context did not accept the enqueue callback.");
        case JsErrorCode::JsErrorCannotStartProjection:
            THROW(E_ILLEGAL_METHOD_CALL, L"Failed to start projection.");
        case JsErrorCode::JsErrorInObjectBeforeCollectCallback:
            THROW(E_ILLEGAL_METHOD_CALL, L"The operation is not supported in an object before collect callback.");
        case JsErrorCode::JsErrorObjectNotInspectable:
            THROW(E_INVALIDARG, L"Object cannot be unwrapped to 'IInspectable' pointer.");
        case JsErrorCode::JsErrorPropertyNotSymbol:
            THROW(E_INVALIDARG, L"A hosting API that operates on symbol property ids but was called with a non-symbol property id.");
        case JsErrorCode::JsErrorPropertyNotString:
            THROW(E_INVALIDARG, L"A hosting API that operates on string property ids but was called with a non-string property id.");
        case JsErrorCode::JsErrorWrongRuntime:
            THROW(RPC_E_WRONG_THREAD, L"A hosting API was called with object created on different javascript runtime.");

        default:
            THROW(E_FAIL, L"Unknown error.");
        }
#undef THROW
    }

#define CHAKRA_CALL(expr) __CHAKRA_CALL((expr), _CRT_WIDE(_CRT_STRINGIZE(expr)), __LINE__, _CRT_WIDE(__FILE__))
}