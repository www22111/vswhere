// <copyright file="XmlScope.h" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#pragma once

#define ESC L"\x1b"
#define COLOR_BRACKET ESC L"[38;2;128;128;128m"
#define COLOR_ELEMENT ESC L"[38;2;86;156;214m"
#define COLOR_RESET ESC L"[0m"

class XmlScope :
    public Scope<XmlScope>
{
public:
    XmlScope(_In_opt_ XmlScope* pParent, _In_ const std::wstring& padding, _In_ const std::wstring& name) :
        Scope(pParent, padding, name)
    {
    }

    XmlScope(_In_opt_ XmlScope* pParent, _In_ std::wstring& padding, _In_ std::wstring::const_pointer name) :
        Scope(pParent, padding, name)
    {
    }

    XmlScope(_In_ const XmlScope& obj) :
        Scope(obj)
    {
    }

protected:
    void WriteStartImpl(_In_ Console& console) override
    {
        const auto BRACKET = console.IsVT100() ? COLOR_BRACKET : L"";
        const auto ELEMENT = console.IsVT100() ? COLOR_ELEMENT : L"";
        const auto RESET = console.IsVT100() ? COLOR_RESET : L"";

        console.WriteLine(L"%1$ls%3$ls<%4$ls%2$ls%3$ls>%5$ls", Padding().c_str(), Name().c_str(), BRACKET, ELEMENT, RESET);
    }

    void WriteEndImpl(_In_ Console& console) override
    {
        const auto BRACKET = console.IsVT100() ? COLOR_BRACKET : L"";
        const auto ELEMENT = console.IsVT100() ? COLOR_ELEMENT : L"";
        const auto RESET = console.IsVT100() ? COLOR_RESET : L"";

        console.WriteLine(L"%1$ls%3$ls</%4$ls%2$ls%3$ls>%5$ls", Padding().c_str(), Name().c_str(), BRACKET, ELEMENT, RESET);
    }
};
