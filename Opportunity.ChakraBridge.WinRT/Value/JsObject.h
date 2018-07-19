#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "JsSymbol.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsObject : IJsValue,
        Windows::Foundation::Collections::IMap<string^, IJsValue^>, Windows::Foundation::Collections::IMapView<string^, IJsValue^>,
        Windows::Foundation::Collections::IMap<IJsSymbol^, IJsValue^>, Windows::Foundation::Collections::IMapView<IJsSymbol^, IJsValue^>
    {
    };

    template<typename K, typename V>
    ref class KVIteratorImpl : Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IKeyValuePair<K, V>^>
    {
    private:
        using IKVP = Windows::Foundation::Collections::IKeyValuePair<K, V>;
        using KVP = std::pair<K, V>;
        typename std::vector<KVP>::const_iterator Iter;
    internal:
        std::vector<KVP> Data;
        void Init()
        {
            Iter = Data.cbegin();
        }
    public:
        virtual property bool HasCurrent
        {
            bool get()
            {
                return Iter < Data.cend();
            }
        }

        virtual property IKVP^ Current
        {
            IKVP^ get()
            {
                using namespace Platform::Collections::Details;
                if (!HasCurrent)
                    throw ref new Platform::OutOfBoundsException(L"Iterate has ended.");
                return ref new KeyValuePair<K, V>(MakeWrap(Iter->first), MakeWrap(Iter->second));
            }
        }

        virtual bool MoveNext()
        {
            if (Iter < Data.cend())
                Iter++;
            return Iter < Data.cend();
        }

        virtual uint32 GetMany(Platform::WriteOnlyArray<IKVP^>^ items)
        {
            using namespace Platform::Collections::Details;
            if (items == nullptr)
                throw ref new Platform::InvalidArgumentException();
            auto capacity = items->Length;
            uint32 copied = 0;
            for (; Iter != Data.cend() && copied < capacity; Iter++, copied++)
            {
                items[copied] = ref new KeyValuePair<K, V>(MakeWrap(Iter->first), MakeWrap(Iter->second));
            }
            return copied;
        }
    };

    ref class JsObjectImpl : JsValueImpl, IJsObject
    {
    internal:
        using IStrMap = Windows::Foundation::Collections::IMap<string^, IJsValue^>;
        using ISymMap = Windows::Foundation::Collections::IMap<IJsSymbol^, IJsValue^>;
        using IStrMapView = Windows::Foundation::Collections::IMapView<string^, IJsValue^>;
        using ISymMapView = Windows::Foundation::Collections::IMapView<IJsSymbol^, IJsValue^>;
        using IStrKVP = Windows::Foundation::Collections::IKeyValuePair<string^, IJsValue^>;
        using ISymKVP = Windows::Foundation::Collections::IKeyValuePair<IJsSymbol^, IJsValue^>;
        using IStrIterator = Windows::Foundation::Collections::IIterator<IStrKVP^>;
        using ISymIterator = Windows::Foundation::Collections::IIterator<ISymKVP^>;
        using IStrIterable = Windows::Foundation::Collections::IIterable<IStrKVP^>;
        using ISymIterable = Windows::Foundation::Collections::IIterable<ISymKVP^>;

        JsObjectImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual ~JsObjectImpl();
        virtual Platform::String^ ToString() override;

        virtual void Clear() {};

        virtual IJsValue^ Lookup(string^ key);
        virtual void Remove(string^ key);
        virtual bool HasKey(string^ key);
        virtual bool Insert(string^ key, IJsValue^ value);
        virtual IStrMapView^ GetStrView() = IStrMap::GetView{ return this; };
        virtual property uint32 StrSize { uint32 get() = IStrMap::Size::get, IStrMapView::Size::get; };
        virtual IStrIterator^ StrFirst() = IStrIterable::First;
        virtual void Split(IStrMapView^*, IStrMapView^*) {}

        virtual IJsValue^ Lookup(IJsSymbol^ key);
        virtual void Remove(IJsSymbol^ key);
        virtual bool HasKey(IJsSymbol^ key);
        virtual bool Insert(IJsSymbol^ key, IJsValue^ value);
        virtual ISymMapView^ GetSymView() = ISymMap::GetView{ return this; };
        virtual property uint32 SymSize { uint32 get() = ISymMap::Size::get, ISymMapView::Size::get; };
        virtual ISymIterator^ SymFirst() = ISymIterable::First;
        virtual void Split(ISymMapView^*, ISymMapView^*) {}
    };

    /// <summary>
    /// Static methods of <see cref="IJsObject"/>.
    /// </summary>
    public ref class JsObject sealed
    {
    private:
        JsObject() {}

    };
}
