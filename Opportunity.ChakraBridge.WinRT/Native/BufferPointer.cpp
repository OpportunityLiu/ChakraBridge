#include "pch.h"
#include "BufferPointer.h"
#include <wrl.h>  
#include <robuffer.h>  

using namespace Opportunity::ChakraBridge::WinRT;

using namespace Windows::Storage::Streams;
using namespace Microsoft::WRL;

uint8* Opportunity::ChakraBridge::WinRT::GetPointerOfBuffer(IBuffer^const buffer, unsigned int *const length, unsigned int *const capacity)
{
    NULL_CHECK(buffer);
    if (length != nullptr)
        *length = buffer->Length;
    if (capacity != nullptr)
        *capacity = buffer->Capacity;
    // Query the IBufferByteAccess interface.  
    ComPtr<IBufferByteAccess> bufferByteAccess;
    reinterpret_cast<IInspectable*>(buffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));

    // Retrieve the buffer data.  
    uint8* pixels = nullptr;
    bufferByteAccess->Buffer(&pixels);
    return pixels;
}