using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    partial class JsRuntime
    {
        /// <summary>
        /// Performs a full garbage collection.
        /// </summary>
        public void CollectGarbage()
        {
            Native.JsCollectGarbage(this.Handle).ThrowIfError();
        }

        private void SetBeforeCollectCallback()
        {
            Native.JsSetRuntimeBeforeCollectCallback(this.Handle, this.Handle.Value, OnCollectingGarbage).ThrowIfError();
        }

        private static JsBeforeCollectCallbackPtr OnCollectingGarbage = _OnCollectingGarbage;

        private static void _OnCollectingGarbage(IntPtr callbackState)
        {
            // release js handles on .net runtime.
            GC.Collect(0, GCCollectionMode.Forced, true);
            var rth = new JsRuntimeHandle(callbackState);
            if (!RuntimeDictionary.TryGetValue(rth, out var runtime))
                return;
            var rte = runtime.CollectingGarbage;
            if (rte is null)
                return;
            rte(runtime, default);
        }

        public event JsCollectingGarbageEventHandler CollectingGarbage;
    }

    /// <summary>
    /// A callback called before collection.
    /// </summary>
    public delegate void JsCollectingGarbageEventHandler(JsRuntime sender, object args);

    /// <summary>
    /// A callback called before collection.
    /// </summary>
    /// <param name="callbackState">The state passed to SetBeforeCollectCallback.</param>
    internal delegate void JsBeforeCollectCallbackPtr(IntPtr callbackState);
}
