namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;

    partial class JsRuntime
    {
        /// <summary>
        /// Creates a script context for running scripts.
        /// </summary>
        /// <remarks>
        /// Each script context has its own global object that is isolated from all other script 
        /// contexts.
        /// </remarks>
        /// <returns>The created script context.</returns>
        public JsContext CreateContext()
        {
            Native.JsCreateContext(this.Handle, out var reference).ThrowIfError();
            var context = new JsContext(reference);
            this.Contexts.Add(reference, context);
            return context;
        }

        /// <summary>
        /// Clear <see cref="Contexts"/> and set it to <see langword="null"/>.
        /// </summary>
        private void ClearContexts()
        {
            var c = Interlocked.Exchange(ref this.Contexts, null);
            if (c is null)
                return;
            foreach (var item in c.Values)
            {
                GC.SuppressFinalize(item);
            }
        }

        internal Dictionary<JsContextRef, JsContext> Contexts
            = new Dictionary<JsContextRef, JsContext>();

        /// <summary>
        /// Gets created contexts.
        /// </summary>
        /// <returns>Collection of contexts.</returns>
        public IEnumerable<JsContext> GetContexts() => this.Contexts.Values;
    }
}
