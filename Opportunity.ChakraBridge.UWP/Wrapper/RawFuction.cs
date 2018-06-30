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
                return func(new JsFunction(callee), isConstructCall, arguments.Select(a => JsValue.CreateTyped(a)).ToList()).Reference;
            }
            catch (Exception ex)
            {
                var error = RawError.CreateError(RawString.FromString(ex.Message ?? ""));
                JsScriptException.SetException(error);
                return RawValue.Undefined;
            }
        }

        private static ushort getArgs(JsValueRef caller, JsValueRef[] arguments, out JsValueRef[] args)
        {
            if (arguments is null || arguments.Length == 0)
            {
                args = new JsValueRef[1];
                args[0] = caller.IsValid ? caller : RawValue.Undefined;
                return 1;
            }

            if (arguments.Length > ushort.MaxValue - 1)
                throw new ArgumentOutOfRangeException("Too many arguments");

            args = new JsValueRef[arguments.Length + 1];
            var undefined = JsValueRef.Invalid;
            if (caller.IsValid)
                args[0] = caller;
            else
            {
                if (!undefined.IsValid)
                    undefined = RawValue.Undefined;
                args[0] = undefined;
            }
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
