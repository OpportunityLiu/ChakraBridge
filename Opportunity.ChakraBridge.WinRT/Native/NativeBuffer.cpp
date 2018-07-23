#include "pch.h"
#include "NativeBuffer.h"

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<size_t, std::wstring> __NativeBufferStaticStorage::RuntimeClassname;

void find_and_replace(std::string& source, std::string const& find, std::string const& replace)
{
    for (std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}
const wchar_t* __NativeBufferStaticStorage::GetRuntimeName(const std::type_info& typeInfo)
{
    const auto typeHash = typeInfo.hash_code();
    auto ex = RuntimeClassname.find(typeHash);
    if (ex != RuntimeClassname.end())
        return ex->second.c_str();
    std::wstringstream str;
    std::string typeName = typeInfo.name();
    find_and_replace(typeName, "::", ".");
    find_and_replace(typeName, "class ", "");
    find_and_replace(typeName, "interface ", "");
    str << L"Opportunity.ChakraBridge.WinRTNativeBuffer.NativeBuffer`1<" << typeName.c_str() << L">";
    RuntimeClassname[typeHash] = str.str();
    return RuntimeClassname[typeHash].c_str();
}
