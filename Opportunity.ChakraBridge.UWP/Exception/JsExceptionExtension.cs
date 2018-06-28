using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using static Opportunity.ChakraBridge.UWP.Native;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class JsExceptionExtension
    {
#if DEBUG
        /// <summary>
        /// Throws if a native method returns an error code.
        /// </summary>
        /// <param name="error">The error.</param>
        /// <param name="callerFilePath"></param>
        /// <param name="callerLineNumber"></param>
        /// <param name="callerMemberName"></param>
        public static void ThrowIfError(this JsErrorCode error,
            [CallerFilePath] string callerFilePath = "",
            [CallerLineNumber] int callerLineNumber = 0,
            [CallerMemberName] string callerMemberName = ""
            )
#else
        /// <summary>
        /// Throws if a native method returns an error code.
        /// </summary>
        /// <param name="error">The error.</param>
        public static void ThrowIfError(this JsErrorCode error)
#endif
        {
            if (error is JsErrorCode.NoError)
                return;
            try
            {
                switch (error)
                {
                case JsErrorCode.InvalidArgument:
                    throw new JsUsageException(error, "Invalid argument.");

                case JsErrorCode.NullArgument:
                    throw new JsUsageException(error, "Null argument.");

                case JsErrorCode.NoCurrentContext:
                    throw new JsUsageException(error, "No current context.");

                case JsErrorCode.InExceptionState:
                    throw new JsUsageException(error, "Runtime is in exception state.");

                case JsErrorCode.NotImplemented:
                    throw new JsUsageException(error, "Method is not implemented.");

                case JsErrorCode.WrongThread:
                    throw new JsUsageException(error, "Runtime is active on another thread.");

                case JsErrorCode.RuntimeInUse:
                    throw new JsUsageException(error, "Runtime is in use.");

                case JsErrorCode.BadSerializedScript:
                    throw new JsUsageException(error, "Bad serialized script.");

                case JsErrorCode.InDisabledState:
                    throw new JsUsageException(error, "Runtime is disabled.");

                case JsErrorCode.CannotDisableExecution:
                    throw new JsUsageException(error, "Cannot disable execution.");

                case JsErrorCode.AlreadyDebuggingContext:
                    throw new JsUsageException(error, "Context is already in debug mode.");

                case JsErrorCode.HeapEnumInProgress:
                    throw new JsUsageException(error, "Heap enumeration is in progress.");

                case JsErrorCode.ArgumentNotObject:
                    throw new JsUsageException(error, "Argument is not an object.");

                case JsErrorCode.InProfileCallback:
                    throw new JsUsageException(error, "In a profile callback.");

                case JsErrorCode.InThreadServiceCallback:
                    throw new JsUsageException(error, "In a thread service callback.");

                case JsErrorCode.CannotSerializeDebugScript:
                    throw new JsUsageException(error, "Cannot serialize a debug script.");

                case JsErrorCode.AlreadyProfilingContext:
                    throw new JsUsageException(error, "Already profiling this context.");

                case JsErrorCode.IdleNotEnabled:
                    throw new JsUsageException(error, "Idle is not enabled.");

                case JsErrorCode.OutOfMemory:
                    throw new JsEngineException(error, "Out of memory.");

                case JsErrorCode.ScriptException:
                {
                    try
                    {
                        throw JsScriptException.CreateFromContext(error, "Script threw an exception.");
                    }
                    catch (JsScriptException)
                    {
                        throw;
                    }
                    catch (Exception ex)
                    {
                        throw new JsScriptException(error, "Script threw an exception.", ex);
                    }
                }

                case JsErrorCode.ScriptCompile:
                {
                    try
                    {
                        throw JsScriptException.CreateFromContext(error, "Compile error.");
                    }
                    catch (JsScriptException)
                    {
                        throw;
                    }
                    catch (Exception ex)
                    {
                        throw new JsScriptException(error, "Compile error.", ex);
                    }
                }

                case JsErrorCode.ScriptTerminated:
                    throw new JsScriptException(error, null, "Script was terminated.");

                case JsErrorCode.ScriptEvalDisabled:
                    throw new JsScriptException(error, null, "Eval of strings is disabled in this runtime.");

                case JsErrorCode.Fatal:
                    throw new JsFatalException(error);

                default:
                    throw new JsFatalException(error);
                }
            }
            catch (JsException ex)
            {
#if DEBUG
                ex.Data["ErrorCallerFilePath"] = callerFilePath;
                ex.Data["ErrorCallerLineNumber"] = callerLineNumber;
                ex.Data["ErrorCallerMemberName"] = callerMemberName;
#endif
                throw;
            }
        }

    }
}
