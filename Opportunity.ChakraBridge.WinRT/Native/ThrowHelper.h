#pragma once
#include "alias.h"
#include <jsrt.h>

namespace Opportunity::ChakraBridge::WinRT
{
    inline void CHAKRA_CALL(JsErrorCode result)
    {
        string^ GetChakraError();
        using namespace Platform;
        if (result == JsErrorCode::JsNoError)
            return;
        switch (result)
        {
        case JsErrorCode::JsErrorInvalidArgument:
            throw ref new InvalidArgumentException("An argument to a hosting API was invalid.");
        case JsErrorCode::JsErrorNullArgument:
            throw ref new InvalidArgumentException("An argument to a hosting API was null in a context where null is not allowed.");
        case JsErrorCode::JsErrorNoCurrentContext:
            throw ref new FailureException("The hosting API requires that a context be current, but there is no current context.");
        case JsErrorCode::JsErrorInExceptionState:
            throw ref new FailureException("The engine is in an exception state and no APIs can be called until the exception is cleared.");
        case JsErrorCode::JsErrorNotImplemented:
            throw ref new NotImplementedException("A hosting API is not yet implemented.");
        case JsErrorCode::JsErrorWrongThread:
            throw ref new WrongThreadException("A hosting API was called on the wrong thread.");
        case JsErrorCode::JsErrorRuntimeInUse:
            throw ref new FailureException("A runtime that is still in use cannot be disposed.");
        case JsErrorCode::JsErrorBadSerializedScript:
            throw ref new InvalidArgumentException("A bad serialized script was used, or the serialized script was serialized by a different version of the Chakra engine.");
        case JsErrorCode::JsErrorInDisabledState:
            throw ref new FailureException("The runtime is in a disabled state.");
        case JsErrorCode::JsErrorCannotDisableExecution:
            throw ref new FailureException("Runtime does not support reliable script interruption.");
        case JsErrorCode::JsErrorAlreadyDebuggingContext:
            throw ref new FailureException("The context cannot be put into a debug state because it is already in a debug state.");
        case JsErrorCode::JsErrorHeapEnumInProgress:
            throw ref new FailureException("A heap enumeration is currently underway in the script context.");
        case JsErrorCode::JsErrorArgumentNotObject:
            throw ref new InvalidArgumentException("A hosting API that operates on object values was called with a non-object value.");
        case JsErrorCode::JsErrorInProfileCallback:
            throw ref new FailureException("A script context is in the middle of a profile callback.");
        case JsErrorCode::JsErrorInThreadServiceCallback:
            throw ref new FailureException("A thread service callback is currently underway.");
        case JsErrorCode::JsErrorCannotSerializeDebugScript:
            throw ref new FailureException("Scripts cannot be serialized in debug contexts.");
        case JsErrorCode::JsErrorAlreadyProfilingContext:
            throw ref new FailureException("The context cannot start profiling because it is already profiling.");
        case JsErrorCode::JsErrorIdleNotEnabled:
            throw ref new FailureException("Idle notification given when the host did not enable idle processing.");
        case JsErrorCode::JsErrorOutOfMemory:
            throw ref new OutOfMemoryException("The Chakra engine has run out of memory.");
        case JsErrorCode::JsErrorScriptException:
        {
            string^ error;
            try
            {
                error = GetChakraError();
            }
            catch (...)
            {
                throw ref new FailureException("A JavaScript exception occurred while running a script.");
            }
            throw ref new FailureException(error);
        }

        case JsErrorCode::JsErrorScriptCompile:
        {
            string^ error;
            try
            {
                error = GetChakraError();
            }
            catch (...)
            {
                throw ref new FailureException("JavaScript failed to compile.");
            }
            throw ref new FailureException(error);
        }

        case JsErrorCode::JsErrorScriptTerminated:
            throw ref new OperationCanceledException("A script was terminated due to a request to suspend a runtime.");
        case JsErrorCode::JsErrorScriptEvalDisabled:
            throw ref new FailureException("A script was terminated because it tried to use 'eval' or 'function' and eval was disabled.");
        case JsErrorCode::JsErrorFatal:
            throw ref new FailureException("A fatal error in the engine has occurred.");
        case JsErrorCode::JsCannotSetProjectionEnqueueCallback:
            throw ref new FailureException("The context did not accept the enqueue callback.");
        case JsErrorCode::JsErrorCannotStartProjection:
            throw ref new FailureException("Failed to start projection.");
        case JsErrorCode::JsErrorInObjectBeforeCollectCallback:
            throw ref new FailureException("The operation is not supported in an object before collect callback.");
        case JsErrorCode::JsErrorObjectNotInspectable:
            throw ref new FailureException("Object cannot be unwrapped to 'IInspectable' pointer.");
        case JsErrorCode::JsErrorPropertyNotSymbol:
            throw ref new FailureException("A hosting API that operates on symbol property ids but was called with a non-symbol property id.");
        case JsErrorCode::JsErrorPropertyNotString:
            throw ref new FailureException("A hosting API that operates on string property ids but was called with a non-string property id.");
        case JsErrorCode::JsErrorWrongRuntime:
            throw ref new FailureException("A hosting API was called with object created on different javascript runtime.");

        default:
            throw ref new FailureException("Unknown error.");
        }
    }
}