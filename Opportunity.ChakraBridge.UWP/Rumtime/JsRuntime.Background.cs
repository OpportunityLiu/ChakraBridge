namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Threading.Tasks;

    /// <summary>
    /// A thread service callback.
    /// </summary>
    /// <remarks>
    /// The host can specify a background thread service when creating a runtime. If 
    /// specified, then background work items will be passed to the host using this callback. The
    /// host is expected to either begin executing the background work item immediately and return
    /// true or return false and the runtime will handle the work item in-thread.
    /// </remarks>
    /// <param name="callbackFunction">The callback for the background work item.</param>
    /// <param name="callbackData">The data argument to be passed to the callback.</param>
    /// <returns>Whether the thread service will execute the callback.</returns>
    internal delegate bool JsThreadServiceCallbackPtr(JsBackgroundWorkItemCallbackPtr callbackFunction, IntPtr callbackData);

    /// <summary>
    /// A background work item callback.
    /// </summary>
    /// <remarks>
    /// This is passed to the host's thread service (if provided) to allow the host to 
    /// invoke the work item callback on the background thread of its choice.
    /// </remarks>
    /// <param name="callbackData">Data argument passed to the thread service.</param>
    internal delegate void JsBackgroundWorkItemCallbackPtr(IntPtr callbackData);

    partial class JsRuntime
    {
        private static readonly JsThreadServiceCallbackPtr ThreadServiceCallback = _ThreadServiceCallback;

        private static bool _ThreadServiceCallback(JsBackgroundWorkItemCallbackPtr callbackFunction, IntPtr callbackData)
        {
            Task.Run(() => callbackFunction(callbackData));
            return true;
        }
    }
}
