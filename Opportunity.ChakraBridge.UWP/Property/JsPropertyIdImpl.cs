namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.ComponentModel;
    using System.Diagnostics;

    /// <summary>
    /// A property identifier.
    /// </summary>
    /// <remarks>
    /// Property identifiers are used to refer to properties of JavaScript objects instead of using
    /// strings.
    /// </remarks>
    [DebuggerDisplay(@"[{Disp,nq}]")]
    internal readonly struct JsPropertyId
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        internal string Disp => this.Reference.IsValid ? ToString() : "(INVALID_PROPERTYID)";

        /// <summary>
        /// The id.
        /// </summary>
        public readonly JsPropertyIdRef Reference;

        /// <summary>
        /// Wheteher the value is vaild reference.
        /// </summary>
        public bool IsValid => this.Reference.IsValid;

        /// <summary>
        /// Initializes a new instance of the <see cref="JsPropertyId"/> struct. 
        /// </summary>
        /// <param name="id">The ID.</param>
        internal JsPropertyId(JsPropertyIdRef id)
        {
            this.Reference = id;
        }

        /// <summary>
        /// Gets an invalid ID.
        /// </summary>
        public static JsPropertyId Invalid => new JsPropertyId(JsPropertyIdRef.Invalid);

        /// <summary>
        /// Gets the name associated with the property ID.
        /// </summary>
        /// <remarks>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        public string Name => getName();

        /// <summary>
        /// Gets the symbol associated with the property ID. 
        /// </summary>
        /// <remarks>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        public JsSymbol Symbol => getSymbol();

        /// <summary>
        /// Gets the type of the property ID.
        /// </summary>
        /// <remarks>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        public JsPropertyIdType IdType
        {
            get
            {
                Native.JsGetPropertyIdType(this, out var type);
                return type;
            }
        }

        /// <summary>
        /// Gets the property ID associated with the name. 
        /// </summary>
        /// <remarks>
        /// <para>
        /// Property IDs are specific to a context and cannot be used across contexts.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="name">
        /// The name of the property ID to get or create. The name may consist of only digits.
        /// </param>
        /// <returns>The property ID in this runtime for the given name.</returns>
        public static JsPropertyId FromString(string name)
        {
            Native.JsGetPropertyIdFromName(name, out var id).ThrowIfError();
            return id;
        }

        /// <summary>
        /// Gets the property ID associated with the name. 
        /// </summary>
        /// <remarks>
        /// <para>
        /// Property IDs are specific to a context and cannot be used across contexts.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="name">
        /// The name of the property ID to get or create. The name may consist of only digits.
        /// </param>
        /// <returns>The property ID in this runtime for the given name.</returns>
        public static implicit operator JsPropertyId(string name) => FromString(name);

        /// <summary>
        /// Gets the property ID associated with the symbol. 
        /// </summary>
        /// <remarks>
        /// <para>
        /// Property IDs are specific to a context and cannot be used across contexts.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="symbol">The symbol whose property ID is being retrieved. </param>
        /// <returns>The property ID for the given symbol. </returns>
        public static JsPropertyId FromSymbol(JsSymbol symbol)
        {
            if (symbol is null)
                throw new ArgumentNullException(nameof(symbol));
            Native.JsGetPropertyIdFromSymbol(symbol.Reference, out var id).ThrowIfError();
            return id;
        }

        /// <summary>
        /// Gets the property ID associated with the symbol. 
        /// </summary>
        /// <remarks>
        /// <para>
        /// Property IDs are specific to a context and cannot be used across contexts.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="symbol">The symbol whose property ID is being retrieved. </param>
        /// <returns>The property ID for the given symbol. </returns>
        public static implicit operator JsPropertyId(JsSymbol symbol) => FromSymbol(symbol);

        /// <summary>
        /// The equality operator for property IDs.
        /// </summary>
        /// <param name="left">The first property ID to compare.</param>
        /// <param name="right">The second property ID to compare.</param>
        /// <returns>Whether the two property IDs are the same.</returns>
        public static bool operator ==(JsPropertyId left, JsPropertyId right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// The inequality operator for property IDs.
        /// </summary>
        /// <param name="left">The first property ID to compare.</param>
        /// <param name="right">The second property ID to compare.</param>
        /// <returns>Whether the two property IDs are not the same.</returns>
        public static bool operator !=(JsPropertyId left, JsPropertyId right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Checks for equality between property IDs.
        /// </summary>
        /// <param name="other">The other property ID to compare.</param>
        /// <returns>Whether the two property IDs are the same.</returns>
        public bool Equals(JsPropertyId other)
            => this.Reference.Value == other.Reference.Value;

        /// <summary>
        /// Invailid operation.
        /// </summary>
        /// <exception cref="InvalidOperationException"></exception>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public override bool Equals(object obj) => throw new InvalidOperationException();

        /// <summary>
        /// The hash code.
        /// </summary>
        /// <returns>The hash code of the property ID.</returns>
        public override int GetHashCode() => this.Reference.Value.GetHashCode();

        /// <summary>
        /// Converts the property ID to a string.
        /// </summary>
        /// <returns>The name or symbol of the property ID.</returns>
        public override string ToString()
        {
            if (!this.Reference.IsValid)
                return "";
            if (IdType == JsPropertyIdType.String)
                return Name;
            else
                return Symbol.ToString();
        }

        private unsafe string getName()
        {
            Native.JsGetPropertyNameFromId(this, out var name).ThrowIfError();
            return new string(name);
        }

        private JsSymbol getSymbol()
        {
            Native.JsGetSymbolFromPropertyId(this, out var sym).ThrowIfError();
            return new JsSymbol(sym);
        }
    }
}