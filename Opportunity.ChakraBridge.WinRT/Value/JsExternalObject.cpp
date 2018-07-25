#include "pch.h"
#include "JsExternalObject.h"
#include <algorithm>
#include <memory>

using namespace Opportunity::ChakraBridge::WinRT;

using EOP = struct EO
{
    object^ Object;
}*;

object^ JsExternalObjectImpl::ExternalData::get()
{
    EOP ptr = Reference.ExtObjExternalData<EOP>();
    return ptr->Object;
}

void JsExternalObjectImpl::ExternalData::set(object^ value)
{
    EOP ptr = Reference.ExtObjExternalData<EOP>();
    ptr->Object = value;
}

IJsExternalObject^ JsExternalObject::Create(object^ data)
{
    auto ptr = std::unique_ptr<EO>(new EO());
    ptr->Object = data;
    const auto r = RawValue::CreateExternalObject<EOP, [](const EOP& data) { 
        delete data; 
    }>(ptr.get());
    ptr.release();
    return ref new JsExternalObjectImpl(r);
}