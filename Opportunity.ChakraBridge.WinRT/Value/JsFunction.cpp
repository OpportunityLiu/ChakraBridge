#include "pch.h"
#include "JsFunction.h"

using namespace Opportunity::ChakraBridge::WinRT;

Platform::String^ JsFunctionImpl::ToString()
{
    throw ref new Platform::NotImplementedException();
    // TODO: �ڴ˴����� return ���
}

IJsValue^ JsFunctionImpl::Invoke(IJsValue ^ caller, IJsValueVectorView^ arguments)
{
    throw ref new Platform::NotImplementedException();
    // TODO: �ڴ˴����� return ���
}

IJsObject^ JsFunctionImpl::New(IJsValueVectorView^ arguments)
{
    throw ref new Platform::NotImplementedException();
    // TODO: �ڴ˴����� return ���
}
