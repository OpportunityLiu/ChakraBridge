namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;

    /// <summary>
    ///     A property identifier.
    /// </summary>
    /// <remarks>
    ///     Property identifiers are used to refer to properties of JavaScript objects instead of using
    ///     strings.
    /// </remarks>
    [DebuggerDisplay(@"{getDisp(),nq}")]
    public readonly struct JsPropertyId : IEquatable<JsPropertyId>
    {
        /// <summary>
        /// The id.
        /// </summary>
        private readonly IntPtr id;

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsPropertyId"/> struct. 
        /// </summary>
        /// <param name="id">The ID.</param>
        private JsPropertyId(IntPtr id)
        {
            this.id = id;
        }

        /// <summary>
        ///     Gets an invalid ID.
        /// </summary>
        public static JsPropertyId Invalid => new JsPropertyId(IntPtr.Zero);

        /// <summary>
        ///     Gets the name associated with the property ID.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        public string Name => getName();

        /// <summary>
        ///     Gets the type of the property ID.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        public JsPropertyIdType Type
        {
            get
            {
                Native.JsGetPropertyIdType(this, out var type);
                return type;
            }
        }

        /// <summary>
        ///     Gets the property ID associated with the name. 
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Property IDs are specific to a context and cannot be used across contexts.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="name">
        ///     The name of the property ID to get or create. The name may consist of only digits.
        /// </param>
        /// <returns>The property ID in this runtime for the given name.</returns>
        public static JsPropertyId FromString(string name)
        {
            Native.JsGetPropertyIdFromName(name, out var id).ThrowIfError();
            return id;
        }

        /// <summary>
        ///     Gets the property ID associated with the name. 
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Property IDs are specific to a context and cannot be used across contexts.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="name">
        ///     The name of the property ID to get or create. The name may consist of only digits.
        /// </param>
        /// <returns>The property ID in this runtime for the given name.</returns>
        public static implicit operator JsPropertyId(string name) => FromString(name);

        /// <summary>
        ///     The equality operator for property IDs.
        /// </summary>
        /// <param name="left">The first property ID to compare.</param>
        /// <param name="right">The second property ID to compare.</param>
        /// <returns>Whether the two property IDs are the same.</returns>
        public static bool operator ==(JsPropertyId left, JsPropertyId right)
        {
            return left.Equals(right);
        }

        /// <summary>
        ///     The inequality operator for property IDs.
        /// </summary>
        /// <param name="left">The first property ID to compare.</param>
        /// <param name="right">The second property ID to compare.</param>
        /// <returns>Whether the two property IDs are not the same.</returns>
        public static bool operator !=(JsPropertyId left, JsPropertyId right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        ///     Checks for equality between property IDs.
        /// </summary>
        /// <param name="other">The other property ID to compare.</param>
        /// <returns>Whether the two property IDs are the same.</returns>
        public bool Equals(JsPropertyId other)
            => this.id == other.id;

        /// <summary>
        ///     Checks for equality between property IDs.
        /// </summary>
        /// <param name="obj">The other property ID to compare.</param>
        /// <returns>Whether the two property IDs are the same.</returns>
        public override bool Equals(object obj)
            => obj is JsPropertyId other && Equals(other);

        /// <summary>
        ///     The hash code.
        /// </summary>
        /// <returns>The hash code of the property ID.</returns>
        public override int GetHashCode()
            => this.id.ToInt32();

        /// <summary>
        ///     Converts the property ID to a string.
        /// </summary>
        /// <returns>The name of the property ID.</returns>
        public override string ToString()
        {
            if (this.id == default)
                return "JS_INVALID_PROPERTYID";
            return Name;
        }

        private unsafe string getName()
        {
            Native.JsGetPropertyNameFromId(this, out var name).ThrowIfError();
            return new string(name);
        }

        private string getDisp()
        {
            if (this.id == default)
                return "JS_INVALID_PROPERTYID";
            return $"[{Name}]";
        }
    }
}