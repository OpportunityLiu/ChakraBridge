using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using static Opportunity.ChakraBridge.UWP.Native;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class Helpers
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

        public static string GetDebugDisp(this IntPtr value, IntPtr nullptrValue, string nullptrRepresent)
        {
            if (value == nullptrValue)
                return nullptrRepresent;
            return value.ToString("X" + IntPtr.Size * 2);
        }

        public static JsTypedArrayType GetArrayType<T>()
            where T : struct, IEquatable<T>, IComparable<T>, IConvertible, IFormattable
        {
            switch (default(T).GetTypeCode())
            {
            case TypeCode.Byte:
                return JsTypedArrayType.Uint8;
            case TypeCode.Double:
                return JsTypedArrayType.Float64;
            case TypeCode.Int16:
                return JsTypedArrayType.Int16;
            case TypeCode.Int32:
                return JsTypedArrayType.Int32;
            case TypeCode.SByte:
                return JsTypedArrayType.Int8;
            case TypeCode.Single:
                return JsTypedArrayType.Float32;
            case TypeCode.UInt16:
                return JsTypedArrayType.Uint16;
            case TypeCode.UInt32:
                return JsTypedArrayType.Uint32;
            }
            throw new InvalidOperationException("Unsupported type.");
        }

        public static string GetPropertyString(this JsObject obj, JsPropertyId id)
        {
            var s = RawProperty.GetProperty(obj.Reference, id);
            try
            {
                return RawString.ToString(s);
            }
            catch
            {
                var s2 = RawOperator.ToJsString(s);
                return RawString.ToString(s2);
            }
        }

        public static void SetPropertyString(this JsObject obj, JsPropertyId id, string value)
        {
            var pvalue = value is null ? RawValue.Undefined : RawString.FromString(value);
            RawProperty.SetProperty(obj.Reference, id, pvalue, true);
        }

        public static bool GetPropertyBoolean(this JsObject obj, JsPropertyId id)
        {
            var s = RawProperty.GetProperty(obj.Reference, id);
            try
            {
                return RawBoolean.ToBoolean(s);
            }
            catch
            {
                var s2 = RawOperator.ToJsBoolean(s);
                return RawBoolean.ToBoolean(s2);
            }
        }

        public static void SetPropertyBoolean(this JsObject obj, JsPropertyId id, bool value)
        {
            var pvalue = RawBoolean.FromBoolean(value);
            RawProperty.SetProperty(obj.Reference, id, pvalue, true);
        }

        public static int GetPropertyInt32(this JsObject obj, JsPropertyId id)
        {
            var s = RawProperty.GetProperty(obj.Reference, id);
            try
            {
                return RawNumber.ToInt32(s);
            }
            catch
            {
                var s2 = RawOperator.ToJsNumber(s);
                return RawNumber.ToInt32(s2);
            }
        }

        public static void SetPropertyInt32(this JsObject obj, JsPropertyId id, int value)
        {
            var pvalue = RawNumber.FromInt32(value);
            RawProperty.SetProperty(obj.Reference, id, pvalue, true);
        }

        public static double GetPropertyDouble(this JsObject obj, JsPropertyId id)
        {
            var s = RawProperty.GetProperty(obj.Reference, id);
            try
            {
                return RawNumber.ToDouble(s);
            }
            catch
            {
                var s2 = RawOperator.ToJsNumber(s);
                return RawNumber.ToDouble(s2);
            }
        }

        public static void SetPropertyDouble(this JsObject obj, JsPropertyId id, double value)
        {
            var pvalue = RawNumber.FromDouble(value);
            RawProperty.SetProperty(obj.Reference, id, pvalue, true);
        }
    }
}
