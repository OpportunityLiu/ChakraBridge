using System;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Storage.Streams;
using System.Reflection;

namespace Opportunity.ChakraBridge.UWP
{
    /// <summary>
    /// WinRT's <code>IBufferByteAccess</code> interface definition.
    /// </summary>
    [ComImport]
    [Guid("905a0fef-bc53-11df-8c49-001e4fc686da")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IBufferByteAccess
    {
        // This needs to be a function - MCG doesn't support properties/events for [ComImport] interface yet
        IntPtr GetBuffer();
    }  // interface IBufferByteAccess

    internal static class BufferByteAccess
    {
        public unsafe static byte* GetPointer(this IBuffer buffer)
        {
            if (buffer is null)
                throw new ArgumentNullException(nameof(buffer));
            var buffPtr = ((IBufferByteAccess)buffer).GetBuffer();
            return (byte*)buffPtr.ToPointer();
        }
    }
}
