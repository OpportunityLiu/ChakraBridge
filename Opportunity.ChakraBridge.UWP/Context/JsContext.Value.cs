namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;
    using Windows.Foundation.Metadata;
    using Windows.Storage.Streams;

    partial class JsContext
    {
        /// <summary>
        /// Gets the runtime that the context belongs to.
        /// </summary>
        public JsRuntime Runtime => JsRuntime.RuntimeDictionary[this.Reference.Runtime];

        /// <summary>
        /// Gets a value indicating whether the context is a valid context or not.
        /// </summary>
        public bool IsValid => this.Reference.IsRuntimeValid;

        /// <inheritdoc/>
        [DefaultOverload]
        [Overload("EqulasTo")]
        public bool Equals(JsContext other) => other is JsContext o && this.Reference == o.Reference;

        /// <inheritdoc/>
        [Overload("Equlas")]
        public override bool Equals(object obj) => obj is JsContext other && Equals(other);

        /// <inheritdoc/>
        public override int GetHashCode() => this.Reference.GetHashCode();

        internal readonly JsContextRef Reference;

        /// <summary>
        /// Get the instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JsContextRef.Invalid"/>. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        /// <returns>The instance of the <see cref="JsContext"/></returns>
        internal static JsContext Get(JsContextRef reference)
        {
            if (!reference.IsValid)
                return null;
            var rt = JsRuntime.RuntimeDictionary[reference.Runtime];
            return rt.Contexts[reference];
        }

        internal JsContext(JsContextRef reference)
        {
            reference.AddRef();
            this.Reference = reference;
        }

        /// <summary>
        /// Releases reference to the script context.
        /// </summary>
        ~JsContext() => this.Reference.Release();

        /// <summary>
        /// The internal data set on <see cref="JsContext"/>. 
        /// </summary>
        [Variant]
        public object Data
        {
            get { this.Reference.GetData(out var value); return value; }
            set => this.Reference.SetData(value);
        }
    }
}
