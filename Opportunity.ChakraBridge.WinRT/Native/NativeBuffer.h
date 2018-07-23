#pragma once

#include <wrl.h>
#include <wrl/implements.h>
#include <windows.storage.streams.h>
#include <robuffer.h>
#include <vector>

namespace Opportunity::ChakraBridge::WinRT
{
    template<typename TData>
    class NativeBuffer :
        public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
        ABI::Windows::Storage::Streams::IBuffer,
        Windows::Storage::Streams::IBufferByteAccess>
    {
        InspectableClass(L"NativeBuffer", BaseTrust);
    public:
        using My = NativeBuffer<TData>;

        virtual ~NativeBuffer()
        {
            
        }

        STDMETHODIMP RuntimeClassInitialize(TData^ data)
        {
            m_data = data;
            m_length = data->BufferLen;

            return S_OK;
        }

        STDMETHODIMP Buffer(uint8 **value)
        {
            auto data = m_data.Resolve<TData>();
            if (data == nullptr)
                return RPC_E_DISCONNECTED;

            *value = data->BufferPtr;
            return S_OK;
        }

        STDMETHODIMP get_Capacity(uint32 *value)
        {
            auto data = m_data.Resolve<TData>();
            if (data == nullptr)
                return RPC_E_DISCONNECTED;

            *value = data->BufferLen;
            return S_OK;
        }

        STDMETHODIMP get_Length(uint32 *value)
        {
            auto data = m_data.Resolve<TData>();
            if (data == nullptr)
                return RPC_E_DISCONNECTED;

            *value = m_length;
            return S_OK;
        }

        STDMETHODIMP put_Length(uint32 value)
        {
            auto data = m_data.Resolve<TData>();
            if (data == nullptr)
                return RPC_E_DISCONNECTED;

            if (data->BufferLen < value)
                return E_INVALIDARG;

            m_length = value;
            return S_OK;
        }

    private:
        uint32 m_length;
        weak_ref m_data; 
    };

    template<typename TData>
    static Windows::Storage::Streams::IBuffer^ CreateNativeBuffer(TData^ data)
    {
        using My = NativeBuffer<TData>;
        Microsoft::WRL::ComPtr<My> nativeBuffer;
        Microsoft::WRL::Details::MakeAndInitialize<My>(&nativeBuffer, data);
        auto iinspectable = reinterpret_cast<IInspectable *>(nativeBuffer.Get());
        auto buffer = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(iinspectable);
        return buffer;
    }
};