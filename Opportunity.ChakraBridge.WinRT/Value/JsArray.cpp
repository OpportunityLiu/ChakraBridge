#include "pch.h"
#include "JsArray.h"
#include <limits>
#include <vector>

using namespace Opportunity::ChakraBridge::WinRT;

#define ARRAY_INDEX_CHECK(prop) \
do{\
    if ((prop) > static_cast<decltype(prop)>(std::numeric_limits<int>::max()))\
        Throw(E_INVALIDARG, _CRT_WIDE(_CRT_STRINGIZE(prop)) L" is too large.");\
}while (false)

uint32 JsArrayImpl::ArraySize::get()
{
    return Reference[L"length"]().ToInt();
}

void JsArrayImpl::Append(T^ value)
{
    void(Reference[L"push"]().Invoke(Reference, get_ref_or_undefined(value)));
}

void JsArrayImpl::ArrayClear()
{
    Reference[L"length"] = RawValue(0);
}

JsArrayImpl::T^ JsArrayImpl::GetAt(uint32 index)
{
    ARRAY_INDEX_CHECK(index);
    return JsValue::CreateTyped(Reference[RawValue(static_cast<int>(index))]);
}

uint32 JsArrayImpl::GetMany(uint32 startIndex, write_only_array<T>^ items)
{
    NULL_CHECK(items);
    ARRAY_INDEX_CHECK(startIndex);
    auto end = static_cast<uint32>(std::min(static_cast<uint64>(startIndex) + items->Length, static_cast<uint64>(ArraySize)));
    for (uint32 i = startIndex; i < end; i++)
    {
        items[i - startIndex] = GetAt(i);
    }
    return end - startIndex;
}

vector_view<JsArrayImpl::T>^ JsArrayImpl::ArrayGetView()
{
    const auto size = ArraySize;
    auto r = std::vector<T^>(size);
    for (uint32 i = 0; i < size; i++)
    {
        r[i] = GetAt(i);
    }
    return ref new Platform::Collections::VectorView<T^>(std::move(r));
}

bool JsArrayImpl::IndexOf(T^ value, uint32* index)
{
    NULL_CHECK(index);
    auto rindex = Reference[L"indexOf"]().Invoke(Reference, get_ref_or_undefined(value)).ToInt();
    if (rindex < 0)
        return false;
    *index = rindex;
    return true;
}

void JsArrayImpl::InsertAt(uint32 index, T^ value)
{
    ARRAY_INDEX_CHECK(index);
    void(Reference[L"splice"]().Invoke(Reference, RawValue(static_cast<int>(index)), RawValue(0), get_ref_or_undefined(value)));
}

void JsArrayImpl::RemoveAt(uint32 index)
{
    ARRAY_INDEX_CHECK(index);
    void(Reference[L"splice"]().Invoke(Reference, RawValue(static_cast<int>(index)), RawValue(1)));
}

void JsArrayImpl::RemoveAtEnd()
{
    void(Reference[L"pop"]().Invoke(Reference));
}

void JsArrayImpl::ReplaceAll(const array<T>^ items)
{
    if (items == nullptr || items->Length == 0)
    {
        ArrayClear();
        return;
    }
    ARRAY_INDEX_CHECK(items->Length);
    const auto length = static_cast<int>(items->Length);
    for (int i = 0; i < length; i++)
    {
        SetAt(i, items[i]);
    }
}

void JsArrayImpl::SetAt(uint32 index, T^ value)
{
    ARRAY_INDEX_CHECK(index);
    Reference[RawValue(static_cast<int>(index))] = get_ref_or_undefined(value);
}

iterator<JsArrayImpl::T>^ JsArrayImpl::ArrayFirst()
{
    return ArrayGetView()->First();
}

template<JsType TExpacted>
RawValue GetArrayProperty(const wchar_t* name)
{
    try
    {
        RawValue value = RawValue::GlobalObject()[L"Array"][name];
        auto valueType = value.Type();
        if (valueType != TExpacted)
            goto GET_FALLBACK;
        return value;
    }
    catch (...)
    {
        goto GET_FALLBACK;
    }
GET_FALLBACK:
    const auto arr = RawValue::CreateArray(0);
    return arr[L"constructor"][name];
}

IJsArray^ JsArray::Create(uint32 length)
{
    return ref new JsArrayImpl(RawValue::CreateArray(length));
}

IJsArray^ JsArray::Create(vector_view<IJsValue>^ items)
{
    NULL_CHECK(items);
    auto ofFunc = ref new JsFunctionImpl(GetArrayProperty<JsType::Function>(L"of"));
    return safe_cast<IJsArray^>(ofFunc->Invoke(nullptr, items));
}

IJsArray^ JsArray::Create(IJsValue^ arrayLike)
{
    NULL_CHECK(arrayLike);
    const auto fromFunc = GetArrayProperty<JsType::Function>(L"from");
    return safe_cast<IJsArray^>(JsValue::CreateTyped(fromFunc.Invoke(RawValue::Invalid(), get_ref(arrayLike))));
}
