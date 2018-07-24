#pragma once

namespace Opportunity::ChakraBridge::WinRT
{
    using IBuffer = ::Windows::Storage::Streams::IBuffer;

    uint8* GetPointerOfBuffer(IBuffer^ const buffer, unsigned int* const length = nullptr, unsigned int* const capacity = nullptr);
};