using System;
using System.Runtime.InteropServices;

namespace Opportunity.ChakraBridge.UWP
{
    internal static class RawObject
    {
        public static JsValueRef CreateExternalObject(object data)
        {
            var container = new ExternalDataContainer(data);
            var handle = GCHandle.Alloc(container);
            try
            {
                Native.JsCreateExternalObject(GCHandle.ToIntPtr(handle), OnObjectFinalize, out var reference).ThrowIfError();
                return reference;
            }
            catch
            {
                handle.Free();
                throw;
            }
        }

        public static bool HasExternalData(JsValueRef obj)
        {
            Native.JsHasExternalData(obj, out var hasExternalData).ThrowIfError();
            return hasExternalData;
        }

        private static readonly JsObjectFinalizeCallbackPtr OnObjectFinalize = _OnObjectFinalize;

        private static void _OnObjectFinalize(IntPtr data)
        {
            GCHandle.FromIntPtr(data).Free();
        }

        public static ref object GetExternalData(JsValueRef extenalObject)
        {
            Native.JsGetExternalData(extenalObject, out var data).ThrowIfError();
            var handle = GCHandle.FromIntPtr(data);
            return ref ((ExternalDataContainer)handle.Target).Value;
        }

        private sealed class ExternalDataContainer
        {
            public object Value;

            public ExternalDataContainer(object data) => this.Value = data;
        }

        public static JsValueRef Create()
        {
            Native.JsCreateObject(out var reference).ThrowIfError();
            return reference;
        }

        public static JsValueRef GetPrototype(JsValueRef objRef)
        {
            Native.JsGetPrototype(objRef, out var prototypeReference).ThrowIfError();
            return prototypeReference;
        }

        public static void SetPrototype(JsValueRef objRef, JsValueRef prototypeReference)
        {
            if (!prototypeReference.IsValid)
                prototypeReference = RawValue.Null;
            Native.JsSetPrototype(objRef, prototypeReference).ThrowIfError();
        }

        /// <summary>
        /// Sets an object to not be extensible.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static void PreventExtension(JsValueRef objRef)
        {
            Native.JsPreventExtension(objRef).ThrowIfError();
        }

        /// <summary>
        /// Gets a value indicating whether an object is extensible or not.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static bool IsExtensionAllowed(JsValueRef objRef)
        {
            Native.JsGetExtensionAllowed(objRef, out var allowed).ThrowIfError();
            return allowed;
        }

        /// <summary>
        /// Performs JavaScript "instanceof" operator test. 
        /// </summary>
        /// <param name="objRef">The object.</param>
        /// <param name="constructorRef">The constructor function to test against. </param>
        /// <returns>Whether the "objRef instanceof constructorRef" is <see langword="true"/>. </returns>
        public static bool InstanceOf(JsValueRef objRef, JsValueRef constructorRef)
        {
            Native.JsInstanceOf(objRef, constructorRef, out var r).ThrowIfError();
            return r;
        }
    }

    /// <summary>
    /// A finalization callback.
    /// </summary>
    /// <param name="data">
    /// The external data that was passed in when creating the object being finalized.
    /// </param>
    internal delegate void JsObjectFinalizeCallbackPtr(IntPtr data);
}
