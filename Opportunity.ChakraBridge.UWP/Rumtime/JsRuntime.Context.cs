namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

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
            return JsContext.GetOrCreate(reference);
        }

        internal static readonly Dictionary<JsContextRef, WeakReference<JsContext>> Contexts
            = new Dictionary<JsContextRef, WeakReference<JsContext>>();

        internal static void CleanUpContexts()
        {
            var cleanList = new List<JsContextRef>();
            foreach (var item in Contexts)
            {
                if (!item.Value.TryGetTarget(out _))
                    cleanList.Add(item.Key);
            }
            if (cleanList.Count == 0)
                return;
            lock (Contexts)
            {
                foreach (var item in cleanList)
                {
                    Contexts.Remove(item);
                }
            }
        }

        /// <summary>
        /// Gets created contexts.
        /// </summary>
        /// <returns>Collection of contexts.</returns>
        public static IEnumerable<JsContext> GetContexts()
        {
            var contexts = Contexts.Values.Select(v =>
            {
                if (v.TryGetTarget(out var r))
                    return r;
                return null;
            }).Where(r => r != null).ToList();

            if (contexts.Count != Contexts.Count)
                CleanUpContexts();

            return contexts;
        }
    }
}
