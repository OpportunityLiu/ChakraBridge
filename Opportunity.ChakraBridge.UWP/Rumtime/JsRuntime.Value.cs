namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using Windows.Foundation.Metadata;

    partial class JsRuntime
    {
        internal static Dictionary<JsRuntimeHandle, JsRuntime> RuntimeDictionary = new Dictionary<JsRuntimeHandle, JsRuntime>();

        /// <summary>
        /// Gets created runtimes.
        /// </summary>
        /// <returns>Collection of runtimes.</returns>
        public static IEnumerable<JsRuntime> GetRuntimes()
            => RuntimeDictionary.Values;

        /// <summary>
        /// Create JsRuntime with initialized handle.
        /// </summary>
        /// <param name="handle">The handle.</param>
        private JsRuntime(JsRuntimeHandle handle)
        {
            if (!handle.IsValid)
                throw new ArgumentException("Invalid handle or runtime", nameof(handle));
            this.Handle = handle;
            lock (RuntimeDictionary)
                RuntimeDictionary.Add(handle, this);
            SetBeforeCollectCallback();
            SetMemoryAllocationCallback();
        }

        /// <summary>
        /// The handle.
        /// </summary>
        internal JsRuntimeHandle Handle;

        /// <inheritdoc/>
        [DefaultOverload]
        [Overload("EqualsTo")]
        public bool Equals(JsRuntime other)
        {
            var h = this.Handle;
            if (!h.IsValid)
                throw new ObjectDisposedException(nameof(JsRuntime));
            return h == other?.Handle;
        }

        /// <inheritdoc/>
        [Overload("Equals")]
        public override bool Equals(object obj) => obj is JsRuntime other && Equals(other);

        /// <inheritdoc/>
        public override int GetHashCode() => this.Handle.GetHashCode();
    }
}
