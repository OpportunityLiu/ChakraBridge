namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using Windows.Storage.Streams;

    partial class JsContext
    {
        private static JsFunction PromiseContinuation;

        internal static void HandlePromiseContinuation()
        {
            while (PromiseContinuation != null)
            {
                var task = PromiseContinuation;
                PromiseContinuation = null;
                task.Invoke(null);
            }
        }

        private readonly static JsPromiseContinuationCallbackPtr OnPromiseContinuationCallback = _OnPromiseContinuationCallback;

        private static void _OnPromiseContinuationCallback(JsValueRef task, IntPtr callbackState)
        {
            PromiseContinuation = new JsFunction(task);
        }
    }

    /// <summary>s
    /// A promise continuation callback.
    /// </summary>
    /// <remarks>
    /// The host can specify a promise continuation callback in <c>JsSetPromiseContinuationCallback</c>. If
    /// a script creates a task to be run later, then the promise continuation callback will be called with
    /// the task and the task should be put in a FIFO queue, to be run when the current script is
    /// done executing.
    /// </remarks>
    /// <param name="task">The task, represented as a JavaScript function.</param>
    /// <param name="callbackState">The data argument to be passed to the callback.</param>
    internal delegate void JsPromiseContinuationCallbackPtr(JsValueRef task, IntPtr callbackState);
}
