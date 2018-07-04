using System;
using System.Threading;

namespace Opportunity.ChakraBridge.UWP
{

    partial class JsContext
    {
        /// <summary>
        /// Use the context in the following scope.
        /// </summary>
        /// <param name="disposeContext">
        /// Whether <see cref="Dispose()"/> need to be invoke when invoking <see cref="JsContextScope.Dispose()"/>.
        /// </param>
        /// <returns>A helper class to set the previous context back when invoking <see cref="JsContextScope.Dispose()"/>.</returns>
        /// <remarks>
        /// Usage:
        /// <code>
        /// using(jsContext.Use())
        /// {
        ///     ...
        /// }
        /// </code>
        /// </remarks>
        public JsContextScope Use(bool disposeContext) => new JsContextScope(this, disposeContext);
    }

    /// <summary>
    /// A helper class to set the previous context back when invoking <see cref="Dispose()"/>.
    /// </summary>
    public sealed class JsContextScope : IDisposable
    {
        private JsContextRef previous;
        private JsContext current;
        private readonly bool disposeContext;

        internal JsContextScope(JsContext jsContext, bool disposeContext)
        {
            this.previous = JsContextRef.Current;
            this.current = jsContext;
            JsContextRef.Current = this.current.Reference;
            if (this.previous != JsContextRef.Invalid)
                this.previous.AddRef();
            this.disposeContext = disposeContext;
        }

        #region IDisposable Support

        void Dispose(bool disposing)
        {
            var c = Interlocked.Exchange(ref this.current, null);
            if (c is null)
                return;
            if (disposing)
            {
                JsContextRef.Current = this.previous;
                if (this.disposeContext)
                    c.Dispose();
            }
            if (this.previous != JsContextRef.Invalid)
                this.previous.Release();
            this.previous = JsContextRef.Invalid;
        }

        /// <summary>
        /// Release references to contexts.
        /// </summary>
        ~JsContextScope()
        {
            Dispose(false);
        }

        /// <summary>
        /// Restore the previous context.
        /// </summary>。
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
