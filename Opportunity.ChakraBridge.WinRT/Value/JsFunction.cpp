#include "pch.h"
#include "JsFunction.h"

using namespace Opportunity::ChakraBridge::WinRT;

Platform::String^ JsFunctionImpl::ToString()
{
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}

IJsValue^ JsFunctionImpl::Invoke(IJsValue ^ caller, IJsValueVectorView^ arguments)
{
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}

IJsObject^ JsFunctionImpl::New(IJsValueVectorView^ arguments)
{
    throw ref new Platform::NotImplementedException();
    // TODO: 在此处插入 return 语句
}
