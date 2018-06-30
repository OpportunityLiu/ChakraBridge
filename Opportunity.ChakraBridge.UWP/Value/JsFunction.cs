namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;

    /// <summary>
    /// A function callback.
    /// </summary>
    /// <param name="callee">
    /// A <c>Function</c> object that represents the function being invoked.
    /// </param>
    /// <param name="isConstructCall">Indicates whether this is a regular call or a 'new' call.</param>
    /// <param name="arguments">The arguments to the call.</param>
    /// <param name="argumentCount">The number of arguments.</param>
    /// <param name="callbackData">Callback data, if any.</param>
    /// <returns>The result of the call, if any.</returns>
    internal delegate JsValueRef JsNativeFunctionPtr(JsValueRef callee, [MarshalAs(UnmanagedType.U1)] bool isConstructCall, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] JsValueRef[] arguments, ushort argumentCount, IntPtr callbackData);

    /// <summary>
    /// A function callback.
    /// </summary>
    /// <param name="callee">
    /// A <c>Function</c> object that represents the function being invoked.
    /// </param>
    /// <param name="isConstructCall">Indicates whether this is a regular call or a 'new' call.</param>
    /// <param name="arguments">The arguments to the call.</param>
    /// <returns>The result of the call, if any.</returns>
    public delegate JsValue JsNativeFunction(JsFunction callee, bool isConstructCall, IList<JsValue> arguments);

    /// <summary>
    /// A JavaScript function object.
    /// </summary>
    public class JsFunction : JsObject
    {
        internal JsFunction(JsValueRef reference)
            : base(reference)
        {
        }

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        /// <exception cref="ArgumentNullException"><paramref name="function"/> is <see langword="null"/>.</exception>
        public static JsFunction Create(JsNativeFunction function) => new JsFunction(RawFuction.Create(function));

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="name">The name of this function that will be used for diagnostics and stringification purposes. </param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        /// <exception cref="ArgumentNullException"><paramref name="function"/> is <see langword="null"/>.</exception>
        public static JsFunction Create(JsNativeFunction function, JsString name)
            => name is null ? Create(function) : new JsFunction(RawFuction.Create(function, name.Reference));

        private JsValueRef[] getArgs(JsValue[] arguments)
        {
            if (arguments is null || arguments.Length == 0)
                return Array.Empty<JsValueRef>();
            if (arguments.Length > ushort.MaxValue - 1)
                throw new ArgumentOutOfRangeException("Too many arguments");
            var args = new JsValueRef[arguments.Length];
            for (var i = 0; i < arguments.Length; i++)
            {
                args[i] = arguments[i]?.Reference ?? JsValueRef.Invalid;
            }
            return args;
        }

        /// <summary>
        /// Invokes a function.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="caller">The caller of function.</param>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation, if any.</returns>
        public JsValue Invoke(JsValue caller, params JsValue[] arguments)
            => CreateTyped(RawFuction.Invoke(this.Reference, caller?.Reference ?? JsValueRef.Invalid, getArgs(arguments)));

        /// <summary>
        /// Invokes a function as a constructor.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation.</returns>
        public JsValue New(params JsValue[] arguments)
            => CreateTyped(RawFuction.New(this.Reference, getArgs(arguments)));
    }
}