using System;

namespace Opportunity.ChakraBridge.UWP
{

    partial class JsContext
    {
        /// <summary>
        /// Use the context in the following scope.
        /// </summary>
        /// <returns>A helper class to set the previous context back when invoking <see cref="Scope.Dispose()"/>.</returns>
        /// <remarks>
        /// Usage:
        /// <code>
        /// using(jsContext.Use())
        /// {
        ///     ...
        /// }
        /// </code>
        /// </remarks>
        public JsContextScope Use() => new JsContextScope(this);
    }

    /// <summary>
    /// A helper class to set the previous context back when invoking <see cref="Dispose()"/>.
    /// </summary>
    public sealed class JsContextScope : IDisposable
    {
        private JsContextRef previous;
        private JsContextRef current;

        internal JsContextScope(JsContext jsContext)
        {
            this.previous = JsContextRef.Current;
            this.current = jsContext.Reference;
            JsContextRef.Current = this.current;
            this.previous.AddRef();
            this.current.AddRef();
        }

        #region IDisposable Support

        void Dispose(bool disposing)
        {
            var c = this.current;
            this.current = JsContextRef.Invalid;
            if (c == JsContextRef.Invalid)
                return;
            if (disposing)
                JsContextRef.Current = this.previous;
            c.Release();
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
