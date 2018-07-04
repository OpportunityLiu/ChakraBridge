using System;
using System.Linq;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawFuction
    {
        public static JsValueRef Create(JsNativeFunction function, JsValueRef nameStr)
        {
            if (!nameStr.IsValid)
                throw new ArgumentException("Name is invalid reference.", nameof(nameStr));
            if (function is null)
                throw new ArgumentNullException(nameof(function));
            var runtime = JsRuntime.RuntimeDictionary[JsContextRef.Current.Runtime];
            var dic = runtime.NativeFunctions;
            Native.JsCreateNamedFunction(nameStr, OnFunctionInvoke, runtime.Handle.Value, out var reference).ThrowIfError();
            dic.Add(reference, function);
            return reference;
        }

        public static JsValueRef Create(JsNativeFunction function)
        {
            if (function is null)
                throw new ArgumentNullException(nameof(function));
            var runtime = JsRuntime.RuntimeDictionary[JsContextRef.Current.Runtime];
            var dic = runtime.NativeFunctions;
            Native.JsCreateFunction(OnFunctionInvoke, runtime.Handle.Value, out var reference).ThrowIfError();
            dic.Add(reference, function);
            return reference;
        }

        private static readonly JsNativeFunctionPtr OnFunctionInvoke = _OnFunctionInvoke;

        private static JsValueRef _OnFunctionInvoke(JsValueRef callee, bool isConstructCall, JsValueRef[] arguments, ushort argumentCount, IntPtr callbackData)
        {
            var runtime = JsRuntime.RuntimeDictionary[new JsRuntimeHandle(callbackData)];
            var func = runtime.NativeFunctions[callee];
            try
            {
                var caller = JsValue.CreateTyped(arguments[0]);
                var callObj = caller as JsObject;
                if (callObj is null)
                {
                    if (caller.ValueType == JsValueType.Undefined || caller.ValueType == JsValueType.Null)
                        callObj = JsValue.GlobalObject;
                    else
                        callObj = caller.ToJsObject();
                }
                var args = arguments.Length > 1 ? new JsValue[arguments.Length - 1] : Array.Empty<JsValue>();
                for (var i = 0; i < args.Length; i++)
                {
                    args[i] = JsValue.CreateTyped(arguments[i + 1]);
                }
                var result = func(new JsFunction(callee), callObj, isConstructCall, args);
                return (result?.Reference).GetValueOrDefault();
            }
            catch (Exception ex)
            {
                var error = RawError.CreateError(RawString.FromString(ex.Message ?? ""));
                JsScriptException.SetException(error);
                return JsValueRef.Invalid;
            }
        }

        private static ushort getArgs(JsValueRef caller, JsValueRef[] arguments, out JsValueRef[] args)
        {
            if (arguments is null || arguments.Length == 0)
            {
                args = new JsValueRef[1];
                args[0] = caller.IsValid ? caller : RawValue.GlobalObject;
                return 1;
            }

            if (arguments.Length > ushort.MaxValue - 1)
                throw new ArgumentOutOfRangeException("Too many arguments");

            args = new JsValueRef[arguments.Length + 1];
            args[0] = caller.IsValid ? caller : RawValue.GlobalObject;
            var undefined = JsValueRef.Invalid;
            for (var i = 0; i < arguments.Length; i++)
            {
                if (arguments[i].IsValid)
                    args[i + 1] = arguments[i];
                else
                {
                    if (!undefined.IsValid)
                        undefined = RawValue.Undefined;
                    args[i + 1] = undefined;
                }
            }
            return (ushort)args.Length;
        }

        /// <summary>
        /// Invokes a function.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="callee">The function.</param>
        /// <param name="caller">The caller of function.</param>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation, if any.</returns>
        public static JsValueRef Invoke(JsValueRef callee, JsValueRef caller, params JsValueRef[] arguments)
        {
            var narg = getArgs(caller, arguments, out var args);
            Native.JsCallFunction(callee, args, narg, out var returnReference).ThrowIfError();
            return returnReference;
        }

        /// <summary>
        /// Invokes a function as a constructor.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="callee">The function.</param>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation.</returns>
        public static JsValueRef New(JsValueRef callee, params JsValueRef[] arguments)
        {
            var narg = getArgs(JsValueRef.Invalid, arguments, out var args);
            Native.JsConstructObject(callee, args, narg, out var returnReference).ThrowIfError();
            return returnReference;
        }
    }
}
