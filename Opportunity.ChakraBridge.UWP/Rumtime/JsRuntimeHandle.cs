namespace Opportunity.ChakraBridge.UWP
{
    using System;

    internal readonly struct JsRuntimeHandle : IEquatable<JsRuntimeHandle>
    {
        internal readonly IntPtr Value;

        public bool Equals(JsRuntimeHandle other) => this.Value == other.Value;

        public override bool Equals(object obj) => obj is JsRuntimeHandle other && Equals(other);

        public override int GetHashCode() => this.Value.GetHashCode();

        private JsRuntimeHandle(IntPtr value) => this.Value = value;

        public bool IsValid => this != Invalid;

        public static JsRuntimeHandle Invalid { get; } = new JsRuntimeHandle(IntPtr.Zero);

        public static bool operator ==(JsRuntimeHandle h1, JsRuntimeHandle h2) => h1.Value == h2.Value;
        public static bool operator !=(JsRuntimeHandle h1, JsRuntimeHandle h2) => h1.Value != h2.Value;

    }
}
