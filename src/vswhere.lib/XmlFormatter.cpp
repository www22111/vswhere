// <copyright file="XmlFormatter.cpp" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#include "stdafx.h"

#define ESC L"\x1b"
#define COLOR_ATTRIBUTE ESC L"[38;2;156;220;254m"
#define COLOR_BRACKET ESC L"[38;2;128;128;128m"
#define COLOR_ELEMENT ESC L"[38;2;86;156;214m"
#define COLOR_VALUE ESC L"[38;2;206;145;120m"
#define COLOR_RESET ESC L"[0m"

using namespace std;

void XmlFormatter::StartDocument(_In_ Console& console)
{
    const auto ATTRIBUTE = console.IsVT100() ? COLOR_ATTRIBUTE : L"";
    const auto BRACKET = console.IsVT100() ? COLOR_BRACKET : L"";
    const auto ELEMENT = console.IsVT100() ? COLOR_ELEMENT : L"";
    const auto VALUE = console.IsVT100() ? COLOR_VALUE : L"";
    const auto RESET = console.IsVT100() ? COLOR_RESET : L"";

    console.WriteLine(L"%1$ls<?%2$lsxml%3$ls %4$lsversion%3$ls=%5$ls\"1.0\"%1$ls?>%3$ls", BRACKET, ELEMENT, RESET, ATTRIBUTE, VALUE);
}

void XmlFormatter::StartArray(_In_ Console& console, _In_opt_ const wstring& name)
{
    StartScope(console, name, L"instances");
}

void XmlFormatter::StartObject(_In_ Console& console, _In_opt_ const wstring& name)
{
    StartScope(console, name, L"instance");
}

void XmlFormatter::WriteProperty(_In_ Console& console, _In_ const wstring& name, _In_ const wstring& value)
{
    const auto BRACKET = console.IsVT100() ? COLOR_BRACKET : L"";
    const auto ELEMENT = console.IsVT100() ? COLOR_ELEMENT : L"";
    const auto RESET = console.IsVT100() ? COLOR_RESET : L"";

    m_scopes.top().WriteStart(console);

    console.WriteLine(L"%1$ls%4$ls<%5$ls%2$ls%4$ls>%6$ls%3$ls%4$ls</%5$ls%2$ls%4$ls>%6$ls", m_padding.c_str(), name.c_str(), value.c_str(), BRACKET, ELEMENT, RESET);
}

void XmlFormatter::EndObject(_In_ Console& console)
{
    EndScope(console);
}

void XmlFormatter::EndArray(_In_ Console& console)
{
    EndScope(console);
}

wstring XmlFormatter::FormatDate(_In_ const FILETIME& value)
{
    return FormatDateISO8601(value);
}

void XmlFormatter::Push()
{
    m_padding += std::wstring(padding_size, L' ');
}

void XmlFormatter::Pop()
{
    if (m_padding.size() > 0)
    {
        m_padding.resize(m_padding.size() - padding_size);
    }
}

void XmlFormatter::StartScope(_In_ Console& console, _In_opt_ const wstring& name, _In_ std::wstring::const_pointer fallback)
{
    XmlScope* pParent = nullptr;
    if (m_scopes.size())
    {
        pParent = &m_scopes.top();
    }

    if (name.empty())
    {
        m_scopes.push(XmlScope(pParent, m_padding, fallback));
    }
    else
    {
        m_scopes.push(XmlScope(pParent, m_padding, name));
    }

    // Always write the root scope.
    if (m_scopes.size() == 1)
    {
        m_scopes.top().WriteStart(console);
    }

    Push();
}

void XmlFormatter::EndScope(_In_ Console& console)
{
    Pop();

    m_scopes.top().WriteEnd(console);
    m_scopes.pop();
}
