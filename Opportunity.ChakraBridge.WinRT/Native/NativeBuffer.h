#pragma once

#include <wrl.h>
#include <wrl/implements.h>
#include <windows.storage.streams.h>
#include <robuffer.h>
#include <vector>

namespace Opportunity::ChakraBridge::WinRT
{

    class NativeBuffer :
        public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
        ABI::Windows::Storage::Streams::IBuffer,
        Windows::Storage::Streams::IBufferByteAccess>
    {
    public:
        static Windows::Storage::Streams::IBuffer^ CreateNativeBuffer(byte* buf, int32 size, int32 length)
        {
            Microsoft::WRL::ComPtr<NativeBuffer> nativeBuffer;
            Microsoft::WRL::Details::MakeAndInitialize<NativeBuffer>(&nativeBuffer, buf, size, length);
            auto iinspectable = reinterpret_cast<IInspectable *>(nativeBuffer.Get());
            auto buffer = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(iinspectable);
            return buffer;
        }

        virtual ~NativeBuffer()
        {
        }

        STDMETHODIMP RuntimeClassInitialize(byte *buffer, UINT32 size, UINT32 length)
        {
            m_length = length;
            m_size = size;
            m_buffer = buffer;

            return S_OK;
        }

        STDMETHODIMP Buffer(byte **value)
        {
            *value = m_buffer;

            return S_OK;
        }

        STDMETHODIMP get_Capacity(UINT32 *value)
        {
            *value = m_length;

            return S_OK;
        }

        STDMETHODIMP get_Length(UINT32 *value)
        {
            *value = m_length;

            return S_OK;
        }

        STDMETHODIMP put_Length(UINT32 value)
        {
            if (m_size < value)
                return E_INVALIDARG;
            m_length = value;

            return S_OK;
        }

    private:
        UINT32 m_length;
        UINT32 m_size;
        byte *m_buffer;
    };
};