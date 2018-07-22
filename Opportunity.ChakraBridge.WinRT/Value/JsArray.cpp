#include "pch.h"
#include "JsArray.h"
#include <limits>
#include <vector>

using namespace Opportunity::ChakraBridge::WinRT;

#define ARRAY_INDEX_CHECK(prop) \
do{\
    if ((prop) > static_cast<decltype(prop)>(std::numeric_limits<int>::max()))\
        throw ref new ::Platform::InvalidArgumentException(_CRT_WIDE(_CRT_STRINGIZE(prop)) L" too large.");\
}while (false)

uint32 JsArrayImpl::ArraySize::get()
{
    return RawNumberToInt(RawGetProperty(Reference, L"length"));
}

void JsArrayImpl::Append(T value)
{
    NULL_CHECK(value);
    RawCallFunction(RawGetProperty(Reference, L"push"), Reference, to_impl(value)->Reference);
}

void JsArrayImpl::ArrayClear()
{
    RawSetProperty(Reference, L"length", RawIntToNumber(0));
}

JsArrayImpl::T JsArrayImpl::GetAt(uint32 index)
{
    ARRAY_INDEX_CHECK(index);
    return JsValue::CreateTyped(RawGetProperty(Reference, RawIntToNumber(static_cast<int>(index))));
}

uint32 JsArrayImpl::GetMany(uint32 startIndex, Platform::WriteOnlyArray<T>^ items)
{
    NULL_CHECK(items);
    ARRAY_INDEX_CHECK(startIndex);
    ARRAY_INDEX_CHECK(items->Length);
    ARRAY_INDEX_CHECK(startIndex + items->Length);
    uint32 end = startIndex + items->Length;
    for (uint32 i = startIndex; i < end; i++)
    {
        items[i - startIndex] = GetAt(i);
    }
    return end - startIndex;
}

JsArrayImpl::TVectorView^ JsArrayImpl::ArrayGetView()
{
    const auto size = ArraySize;
    auto r = std::vector<T>(size);
    for (uint32 i = 0; i < size; i++)
    {
        r[i] = GetAt(i);
    }
    return ref new Platform::Collections::VectorView<T>(std::move(r));
}

bool JsArrayImpl::IndexOf(T value, uint32* index)
{
    NULL_CHECK(value);
    NULL_CHECK(index);
    auto indexOf = RawGetProperty(Reference, L"indexOf");
    auto rindex = RawNumberToInt(RawCallFunction(indexOf, Reference, to_impl(value)->Reference));
    if (rindex < 0)
        return false;
    *index = rindex;
    return true;
}

void JsArrayImpl::InsertAt(uint32 index, T value)
{
    NULL_CHECK(value);
    ARRAY_INDEX_CHECK(index);
    auto splice = RawGetProperty(Reference, L"splice");
    RawCallFunction(splice, Reference, RawIntToNumber(static_cast<int>(index)), RawIntToNumber(0), to_impl(value)->Reference);
}

void JsArrayImpl::RemoveAt(uint32 index)
{
    ARRAY_INDEX_CHECK(index);
    auto splice = RawGetProperty(Reference, L"splice");
    RawCallFunction(splice, Reference, RawIntToNumber(static_cast<int>(index)), RawIntToNumber(1));
}

void JsArrayImpl::RemoveAtEnd()
{
    auto pop = RawGetProperty(Reference, L"pop");
    RawCallFunction(pop, Reference);
}

void JsArrayImpl::ReplaceAll(const Platform::Array<T>^ items)
{
    if (items == nullptr || items->Length == 0)
    {
        ArrayClear();
        return;
    }
    ARRAY_INDEX_CHECK(items->Length);
    if (std::find(items->begin(), items->end(), nullptr) != items->end())
        throw ref new Platform::InvalidArgumentException(L"items contains null items.");
    const auto length = static_cast<int>(items->Length);
    for (int i = 0; i < length; i++)
    {
        SetAt(i, items[i]);
    }
}

void JsArrayImpl::SetAt(uint32 index, T value)
{
    NULL_CHECK(value);
    ARRAY_INDEX_CHECK(index);
    RawSetProperty(Reference, RawIntToNumber(static_cast<int>(index)), to_impl(value)->Reference);
}

JsArrayImpl::TIterator^ JsArrayImpl::ArrayFirst()
{
    return ArrayGetView()->First();
}

IJsArray^ JsArray::Create(uint32 length)
{
    JsValueRef r;
    CHAKRA_CALL(JsCreateArray(length, &r));
    return ref new JsArrayImpl(r);
}
