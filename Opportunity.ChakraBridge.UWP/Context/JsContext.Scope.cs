namespace Opportunity.ChakraBridge.UWP
{
    using System;

    public partial struct JsContext
    {
        /// <summary>
        ///     A scope automatically sets a context to current and resets the original context
        ///     when disposed.
        /// </summary>
        public struct Scope : IDisposable
        {
            /// <summary>
            ///     The previous context.
            /// </summary>
            private readonly JsContext previousContext;

            /// <summary>
            ///     Whether the structure has been disposed.
            /// </summary>
            private bool disposed;

            /// <summary>
            ///     Initializes a new instance of the <see cref="Scope"/> struct. 
            /// </summary>
            /// <param name="context">The context to create the scope for.</param>
            public Scope(JsContext context)
            {
                this.disposed = false;
                this.previousContext = Current;
                Current = context;
            }

            /// <summary>
            ///     Disposes the scope and sets the previous context to current.
            /// </summary>
            public void Dispose()
            {
                if (this.disposed)
                {
                    return;
                }

                Current = this.previousContext;
                this.disposed = true;
            }
        }
    }
}
