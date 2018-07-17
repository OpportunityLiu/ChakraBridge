#include "pch.h"
#include <wrl.h>  
#include <robuffer.h>  

using namespace Windows::Storage::Streams;
using namespace Microsoft::WRL;
typedef uint8 byte;
// Retrieves the raw pixel data from the provided IBuffer object.  
// Warning: The lifetime of the returned buffer is controlled by  
// the lifetime of the buffer object that's passed to this method.  
// When the buffer has been released, the pointer becomes invalid  
// and must not be used.  
byte* GetPointerOfBuffer(IBuffer^ buffer, unsigned int *length)
{
    if (buffer == nullptr)
        throw ref new Platform::InvalidArgumentException("buffer is null.");
    if (length != nullptr)
        *length = buffer->Length;
    // Query the IBufferByteAccess interface.  
    ComPtr<IBufferByteAccess> bufferByteAccess;
    reinterpret_cast<IInspectable*>(buffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));

    // Retrieve the buffer data.  
    byte* pixels = nullptr;
    bufferByteAccess->Buffer(&pixels);
    return pixels;
}