namespace Opportunity.ChakraBridge.UWP
{
    using System;

    internal readonly struct JsValueRef
    {
        /// <summary>
        /// The reference.
        /// </summary>
        internal readonly IntPtr Value;

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsValueRef"/> struct.
        /// </summary>
        /// <param name="value">The reference.</param>
        private JsValueRef(IntPtr value)
        {
            this.Value = value;
        }

        /// <summary>
        ///     Gets an invalid value.
        /// </summary>
        public static JsValueRef Invalid { get; } = new JsValueRef(IntPtr.Zero);

        /// <summary>
        ///     Gets a value indicating whether the value is valid.
        /// </summary>
        public bool IsValid => this.Value != IntPtr.Zero;
    }
}