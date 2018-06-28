namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;

    /// <summary>
    ///     A function callback.
    /// </summary>
    /// <param name="callee">
    ///     A <c>Function</c> object that represents the function being invoked.
    /// </param>
    /// <param name="isConstructCall">Indicates whether this is a regular call or a 'new' call.</param>
    /// <param name="arguments">The arguments to the call.</param>
    /// <param name="argumentCount">The number of arguments.</param>
    /// <param name="callbackData">Callback data, if any.</param>
    /// <returns>The result of the call, if any.</returns>
    internal delegate JsValueRef JsNativeFunctionPtr(JsValueRef callee, [MarshalAs(UnmanagedType.U1)] bool isConstructCall, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] JsValueRef[] arguments, ushort argumentCount, IntPtr callbackData);

    /// <summary>
    ///     A function callback.
    /// </summary>
    /// <param name="callee">
    ///     A <c>Function</c> object that represents the function being invoked.
    /// </param>
    /// <param name="isConstructCall">Indicates whether this is a regular call or a 'new' call.</param>
    /// <param name="arguments">The arguments to the call.</param>
    /// <returns>The result of the call, if any.</returns>
    public delegate JsValue JsNativeFunction(JsFunction callee, bool isConstructCall, IList<JsValue> arguments);

    public class JsFunction : JsObject
    {
        internal JsFunction(JsValueRef reference)
            : base(reference)
        {
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="function">The method to call when the function is invoked.</param>
        public JsFunction(JsNativeFunction function)
            : base(CreateFunction(function))
        { }

        private static JsValueRef CreateFunction(JsNativeFunction function)
        {
            Native.JsCreateFunction(JsNativeFunctionCallback, (IntPtr)FuncTable.Count, out var reference).ThrowIfError();
            FuncTable.Add(function);
            return reference;
        }

        internal static JsValueRef JsNativeFunctionCallback(JsValueRef callee, [MarshalAs(UnmanagedType.U1)] bool isConstructCall, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] JsValueRef[] arguments, ushort argumentCount, IntPtr callbackData)
        {
            try
            {
                var func = FuncTable[callbackData.ToInt32()];
                return func.Invoke(new JsFunction(callee), isConstructCall, arguments.Select(a => CreateTyped(a)).ToList()).Reference;
            }
            catch (Exception)
            {
                //TODO: Set Context error
                throw;
            }
        }

        private static readonly List<JsNativeFunction> FuncTable = new List<JsNativeFunction>(32);

        private JsValueRef[] getArgs(JsValue[] arguments)
        {
            arguments = arguments ?? Array.Empty<JsValue>();
            if (arguments.Length > ushort.MaxValue)
                throw new ArgumentOutOfRangeException("Too many arguments");
            var args = arguments.Length == 0 ? Array.Empty<JsValueRef>() : new JsValueRef[arguments.Length];
            for (var i = 0; i < arguments.Length; i++)
            {
                args[i] = arguments[i].Reference;
            }
            return args;
        }

        /// <summary>
        ///     Invokes a function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation, if any.</returns>
        public JsValue Invoke(params JsValue[] arguments)
        {
            var args = getArgs(arguments);
            Native.JsCallFunction(this.Reference, args, (ushort)args.Length, out var returnReference).ThrowIfError();
            return CreateTyped(returnReference);
        }

        /// <summary>
        ///     Invokes a function as a constructor.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation.</returns>
        public JsValue ConstructObject(params JsValue[] arguments)
        {
            var args = getArgs(arguments);
            Native.JsConstructObject(this.Reference, args, (ushort)args.Length, out var returnReference).ThrowIfError();
            return CreateTyped(returnReference);
        }
    }
}