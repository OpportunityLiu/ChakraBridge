namespace Opportunity.ChakraBridge.UWP
{
    using System;

    /// <summary>
    ///     A thread service callback.
    /// </summary>
    /// <remarks>
    ///     The host can specify a background thread service when creating a runtime. If 
    ///     specified, then background work items will be passed to the host using this callback. The
    ///     host is expected to either begin executing the background work item immediately and return
    ///     true or return false and the runtime will handle the work item in-thread.
    /// </remarks>
    /// <param name="callbackFunction">The callback for the background work item.</param>
    /// <param name="callbackData">The data argument to be passed to the callback.</param>
    /// <returns>Whether the thread service will execute the callback.</returns>
    public delegate bool JsThreadServiceCallback(JsBackgroundWorkItemCallback callbackFunction, IntPtr callbackData);

    /// <summary>
    ///     A background work item callback.
    /// </summary>
    /// <remarks>
    ///     This is passed to the host's thread service (if provided) to allow the host to 
    ///     invoke the work item callback on the background thread of its choice.
    /// </remarks>
    /// <param name="callbackData">Data argument passed to the thread service.</param>
    public delegate void JsBackgroundWorkItemCallback(IntPtr callbackData);

    /// <summary>
    ///     A callback called before collection.
    /// </summary>
    /// <param name="callbackState">The state passed to SetBeforeCollectCallback.</param>
    public delegate void JsBeforeCollectCallback(IntPtr callbackState);

    /// <summary>
    ///     User implemented callback routine for memory allocation events
    /// </summary>
    /// <param name="callbackState">The state passed to SetRuntimeMemoryAllocationCallback.</param>
    /// <param name="allocationEvent">The type of type allocation event.</param>
    /// <param name="allocationSize">The size of the allocation.</param>
    /// <returns>
    ///     For the Allocate event, returning true allows the runtime to continue with 
    ///     allocation. Returning false indicates the allocation request is rejected. The return value
    ///     is ignored for other allocation events.
    /// </returns>
    public delegate bool JsMemoryAllocationCallback(IntPtr callbackState, JsMemoryEventType allocationEvent, UIntPtr allocationSize);
}
