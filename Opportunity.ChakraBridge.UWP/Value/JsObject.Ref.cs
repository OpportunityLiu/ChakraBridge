using System;
using System.ComponentModel;
using System.Threading;

namespace Opportunity.ChakraBridge.UWP
{
    partial class JsObject
    {
        /// <summary>
        /// Adds a reference to a garbage collected object. 
        /// </summary>
        /// <returns>The object's new reference count</returns>
        [EditorBrowsable(EditorBrowsableState.Advanced)]
        public uint AddRef() => this.Reference.AddRef();

        /// <summary>
        /// Releases a reference to a garbage collected object. 
        /// </summary>
        /// <returns>The object's new reference count</returns>
        [EditorBrowsable(EditorBrowsableState.Advanced)]
        public uint Release() => this.Reference.Release();

        /// <summary>
        /// Adds a reference to a garbage collected object. 
        /// </summary>
        /// <returns>A handle to release the reference.</returns>
        public JsObjectAutoReleaseHandle Capture() => new JsObjectAutoReleaseHandle(this);
    }

    /// <summary>
    /// A handle to release the reference when closing.
    /// </summary>
    public sealed class JsObjectAutoReleaseHandle : IDisposable
    {
        /// <summary>
        /// The object that will this handle call <see cref="JsObject.Release()"/> on.
        /// </summary>
        public JsObject Object
            => this.reference ?? throw new ObjectDisposedException(nameof(JsObjectAutoReleaseHandle));

        private JsObject reference;

        internal JsObjectAutoReleaseHandle(JsObject reference)
        {
            reference.AddRef();
            this.reference = reference;
        }

        #region IDisposable Support

        void Dispose(bool disposing)
        {
            var refe = Interlocked.Exchange(ref this.reference, null);
            if (refe != null)
            {
                refe.Release();
            }
        }

        /// <summary>
        /// Releases a reference to a garbage collected object. 
        /// </summary>
        ~JsObjectAutoReleaseHandle()
        {
            Dispose(false);
        }

        /// <summary>
        /// Releases a reference to a garbage collected object. 
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
