namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;
    using System.Runtime.CompilerServices;
    using Windows.Foundation.Metadata;
    using Windows.Storage.Streams;

    partial class JsContext
    {
        internal readonly JsContextRef Reference;

        /// <summary>
        /// Create a new instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JsContextRef.Invalid"/>. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        /// <returns>A new instance of the <see cref="JsContext"/></returns>
        internal static JsContext GetOrCreate(JsContextRef reference)
        {
            if (!reference.IsValid)
                return null;
            if (JsRuntime.Contexts.TryGetValue(reference, out var wr))
            {
                if (wr.TryGetTarget(out var r))
                    return r;
                r = new JsContext(reference);
                wr.SetTarget(r);
                return r;
            }
            var cr = new JsContext(reference);
            JsRuntime.Contexts.Add(reference, new WeakReference<JsContext>(cr));
            return cr;
        }

        private JsContext(JsContextRef reference)
        {
            Native.JsContextAddRef(reference, out var count).ThrowIfError();
            this.Reference = reference;
        }

        /// <summary>
        /// Releases reference to the script context.
        /// </summary>
        ~JsContext()
        {
            Native.JsContextRelease(this.Reference, out var count).ThrowIfError();
        }

        /// <summary>
        /// Gets the runtime that the context belongs to.
        /// </summary>
        public JsRuntime Runtime => JsRuntime.RuntimeDictionary[this.Reference.Runtime];

        /// <summary>
        /// Gets a value indicating whether the context is a valid context or not.
        /// </summary>
        public bool IsValid => this.Reference.IsRuntimeValid;

        [DefaultOverload]
        public bool Equals(JsContext other) => other is JsContext o && this.Reference == o.Reference;

        public override bool Equals(object obj) => obj is JsContext other && Equals(other);

        public override int GetHashCode() => this.Reference.GetHashCode();

        //public unsafe void* Data
        //{
        //    get
        //    {
        //        Native.JsGetContextData(this.Reference, out var data).ThrowIfError();
        //        return data.ToPointer();
        //    }
        //    set => Native.JsSetContextData(this.Reference, new IntPtr(value)).ThrowIfError();
        //}
    }
}
