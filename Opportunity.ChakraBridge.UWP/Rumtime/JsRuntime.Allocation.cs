using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    partial class JsRuntime
    {
        private static readonly JsMemoryAllocationCallbackPtr OnMemoryEvent = _OnMemoryEvent;

        private void SetMemoryAllocationCallback()
        {
            Native.JsSetRuntimeMemoryAllocationCallback(this.Handle, this.Handle.Value, OnMemoryEvent).ThrowIfError();
        }

        private static bool _OnMemoryEvent(IntPtr callbackState, JsMemoryEventType allocationEvent, UIntPtr allocationSize)
        {
            var rth = new JsRuntimeHandle(callbackState);
            if (!RuntimeDictionary.TryGetValue(rth, out var runtime))
                return true;
            var rte = runtime.MemoryEvent;
            if (rte is null)
                return true;
            var args = new JsMemoryEventArgs
            {
                AllocationSize = allocationSize.ToUInt64(),
                EventType = allocationEvent,
                IsRejected = false,
            };
            rte(runtime, args);
            return !args.IsRejected;
        }

        /// <summary>
        /// Raises when the charka engine allocating or freeing memories.
        /// </summary>
        public event JsMemoryEventHandler MemoryEvent;
    }

    /// <summary>
    /// User implemented callback routine for memory allocation events
    /// </summary>
    /// <param name="callbackState">The state passed to SetRuntimeMemoryAllocationCallback.</param>
    /// <param name="allocationEvent">The type of type allocation event.</param>
    /// <param name="allocationSize">The size of the allocation.</param>
    /// <returns>
    /// For the Allocate event, returning true allows the runtime to continue with 
    /// allocation. Returning false indicates the allocation request is rejected. The return value
    /// is ignored for other allocation events.
    /// </returns>
    internal delegate bool JsMemoryAllocationCallbackPtr(IntPtr callbackState, JsMemoryEventType allocationEvent, UIntPtr allocationSize);

    /// <summary>
    /// Event handler for <see cref="JsRuntime.MemoryEvent"/>.
    /// </summary>
    /// <param name="sender">The <see cref="JsRuntime"/> raises the event.</param>
    /// <param name="args">Args of the event.</param>
    public delegate void JsMemoryEventHandler(JsRuntime sender, JsMemoryEventArgs args);

    /// <summary>
    /// Allocation callback event type.
    /// </summary>
    public enum JsMemoryEventType
    {
        /// <summary>
        /// Indicates a request for memory allocation.
        /// </summary>
        Allocate = 0,

        /// <summary>
        /// Indicates a memory freeing event.
        /// </summary>
        Free = 1,

        /// <summary>
        /// Indicates a failed allocation event.
        /// </summary>
        Failure = 2
    }

    /// <summary>
    /// Event args of <see cref="JsRuntime.MemoryEvent"/>.
    /// </summary>
    public sealed class JsMemoryEventArgs
    {
        /// <summary>
        /// The type of type allocation event.
        /// </summary>
        public JsMemoryEventType EventType { get; internal set; }

        /// <summary>
        /// The size of the allocation.
        /// </summary>
        public ulong AllocationSize { get; internal set; }

        /// <summary>
        /// For the Allocate event, <see langword="false"/> allows the runtime to continue with allocation. 
        /// <see langword="true"/> indicates the allocation request is rejected. 
        /// The value is ignored for other allocation events.
        /// </summary>
        public bool IsRejected { get; set; }
    }
}
