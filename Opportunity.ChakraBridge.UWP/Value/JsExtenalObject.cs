using System;
using System.Runtime.InteropServices;
using Windows.Foundation.Metadata;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    ///  A JavaScript object that stores some external data.
    /// </summary>
    public class JsExtenalObject : JsObject
    {
        internal JsExtenalObject(JsValueRef reference) : base(reference) { }

        /// <summary>
        /// Creates a new <see cref="JsExtenalObject"/> that stores some external data.
        /// </summary>
        /// <param name="data">External data that the object will represent. May be null.</param>
        /// <returns>A new <see cref="JsExtenalObject"/> that stores some external data.</returns>
        /// <remarks>Requires an active script context.</remarks>
        public new static JsExtenalObject Create([Variant]object data)
            => new JsExtenalObject(RawObject.CreateExternalObject(data));

        /// <summary>
        /// Gets or sets the data in an external object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        [Variant]
        public object ExternalData
        {
            get { RawObject.GetExternalData(this.Reference, out var value); return value; }
            set => RawObject.SetExternalData(this.Reference, value);
        }
    }
}
