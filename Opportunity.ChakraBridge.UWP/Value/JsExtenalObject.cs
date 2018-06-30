using System;
using System.Runtime.InteropServices;

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
        public static JsExtenalObject Create(object data) => new JsExtenalObject(RawObject.CreateExternalObject(data));

        /// <summary>
        /// Gets or sets the data in an external object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public object ExternalData
        {
            get => RawObject.GetExternalData(this.Reference);
            set => RawObject.GetExternalData(this.Reference) = value;
        }
    }
}
