using System;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Windows.Foundation.Metadata;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// A JavaScript object.
    /// </summary>
    public partial class JsObject : JsValue
    {
        internal JsObject(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Creates a new <see cref="JsObject"/>.
        /// </summary>
        /// <returns>A new <see cref="JsObject"/>.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public static JsObject Create() => new JsObject(RawObject.Create());

        /// <summary>
        /// Gets or sets the prototype of an object, use <see langword="null"/> instead of <see cref="JsNull"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public JsObject Prototype
        {
            get => CreateTyped(RawObject.GetPrototype(this.Reference)) as JsObject;
            set => RawObject.SetPrototype(this.Reference, value?.Reference ?? RawValue.Null);
        }

        /// <summary>
        /// Sets an object to not be extensible.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public void PreventExtension() => RawObject.PreventExtension(this.Reference);

        /// <summary>
        /// Gets a value indicating whether an object is extensible or not.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public bool IsExtensionAllowed => RawObject.IsExtensionAllowed(this.Reference);

        /// <summary>
        /// Performs JavaScript "instanceof" operator test. 
        /// </summary>
        /// <param name="constructor">The constructor function to test against. </param>
        /// <returns>Whether the "object instanceof constructor" is <see langword="true"/>. </returns>
        /// <exception cref="ArgumentNullException"><paramref name="constructor"/> is <see langword="null"/>.</exception>
        public bool InstanceOf(JsFunction constructor)
            => RawObject.InstanceOf(this.Reference, (constructor ?? throw new ArgumentNullException(nameof(constructor))).Reference);

        /// <summary>
        /// A callback function that is called by the runtime before garbage collection of the object. 
        /// </summary>
        public JsObjectBeforeCollectCallback ObjectCollectingCallback
        {
            get
            {
                var runtime = JsRuntime.RuntimeDictionary[this.Reference.Context.Runtime];
                var callbackDic = runtime.ObjectCollectingCallbacks;
                if (callbackDic.TryGetValue(this.Reference, out var cb))
                    return cb;
                return null;
            }
            set
            {
                var runtime = JsRuntime.RuntimeDictionary[this.Reference.Context.Runtime];
                var callbackDic = runtime.ObjectCollectingCallbacks;
                if (value is null)
                {
                    Native.JsSetObjectBeforeCollectCallback(this.Reference, runtime.Handle.Value, null).ThrowIfError();
                    callbackDic.Remove(this.Reference);
                }
                else
                {
                    Native.JsSetObjectBeforeCollectCallback(this.Reference, runtime.Handle.Value, OnObjectCollecting).ThrowIfError();
                    callbackDic[this.Reference] = value;
                }
            }
        }

        private static JsObjectBeforeCollectCallbackPtr OnObjectCollecting = _OnObjectCollecting;

        private static void _OnObjectCollecting(JsValueRef reference, IntPtr callbackState)
        {
            var runtime = JsRuntime.RuntimeDictionary[new JsRuntimeHandle(callbackState)];
            var callbackDic = runtime.ObjectCollectingCallbacks;
            var callback = callbackDic[reference];
            try
            {
                callback((JsObject)CreateTyped(reference));
            }
            finally
            {
                callbackDic.Remove(reference);
            }
        }

        /// <summary>
        /// Converts the value to string using standard JavaScript semantics. 
        /// </summary>
        /// <remarks>Requires an active script context. </remarks>
        /// <returns>The string.</returns>
        public override sealed string ToString()
        {
            try
            {
                var s = RawOperator.ToJsString(this.Reference);
                return RawString.ToString(s);
            }
            catch (JsScriptException)
            {
                // no toString method.
                return "[object Object]";
            }
        }
    }

    internal delegate void JsObjectBeforeCollectCallbackPtr(JsValueRef reference, IntPtr callbackState);

    /// <summary>
    /// A callback called before collecting an object.
    /// </summary>
    /// <remarks>
    /// Use <c>JsSetObjectBeforeCollectCallback</c> to register this callback.
    /// </remarks>
    /// <param name="obj">The object to be collected.</param>
    public delegate void JsObjectBeforeCollectCallback(JsObject obj);
}
