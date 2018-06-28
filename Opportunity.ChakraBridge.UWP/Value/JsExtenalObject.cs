using System;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///      A JavaScript object that stores some external data.
    /// </summary>
    public class JsExtenalObject : JsObject
    {
        internal JsExtenalObject(JsValueRef reference) : base(reference)
        {
        }

        /// <summary>
        ///     Creates a new <c>Object</c> that stores some external data.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="data">External data that the object will represent. May be null.</param>
        /// <param name="finalizer">
        ///     A callback for when the object is finalized. May be null.
        /// </param>
        public JsExtenalObject(IntPtr data, JsObjectFinalizeCallback finalizer)
            : this(CreateExternalObject(data, finalizer))
        {
        }

        private static JsValueRef CreateExternalObject(IntPtr data, JsObjectFinalizeCallback finalizer)
        {
            Native.JsCreateExternalObject(data, finalizer, out var reference).ThrowIfError();
            return reference;
        }

        /// <summary>
        ///     Gets or sets the data in an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        public IntPtr ExternalData
        {
            get
            {
                Native.JsGetExternalData(this.Reference, out var data).ThrowIfError();
                return data;
            }
            set => Native.JsSetExternalData(this.Reference, value).ThrowIfError();
        }
    }

    /// <summary>
    ///     A finalization callback.
    /// </summary>
    /// <param name="data">
    ///     The external data that was passed in when creating the object being finalized.
    /// </param>
    public delegate void JsObjectFinalizeCallback(IntPtr data);
}
