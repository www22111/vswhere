// <copyright file="JsonScope.cpp" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#include "stdafx.h"

#define ESC L"\x1b"
#define COLOR_PROPERTY ESC L"[38;2;156;220;254m"
#define COLOR_RESET ESC L"[0m"

using namespace std;

void JsonScope::StartScope(_In_ Console& console)
{
    WriteSeparator(console);
}

void JsonScope::StartProperty(_In_ Console& console)
{
    // Delay writing the parent scope until we write a property.
    WriteStart(console);

    WriteSeparator(console);
    RequireSeparator();
}

void JsonScope::WriteStartImpl(_In_ Console& console)
{
    WriteSeparator(console);

    // Write new line if not the root scope.
    if (Parent())
    {
        console.WriteLine();
    }

    if (m_type == Type::array || Name().empty())
    {
        console.Write(L"%ls%lc", Padding().c_str(), StartChar());
    }
    else if (console.IsVT100())
    {
        console.Write(L"%ls" COLOR_PROPERTY L"\"%ls\"" COLOR_RESET L": %lc", Padding().c_str(), Name().c_str(), StartChar());
    }
    else
    {
        console.Write(L"%ls\"%ls\": %lc", Padding().c_str(), Name().c_str(), StartChar());
    }
}

void JsonScope::WriteEndImpl(_In_ Console& console)
{
    if (m_requireSep)
    {
        // Write new line and padding only if elements were written.
        // This keeps empty arrays and objects looking like [] and {}.
        console.Write(L"\n%ls", Padding().c_str());
    }

    console.Write(L"%lc", EndChar());
}

void JsonScope::RequireSeparator() noexcept
{
    m_requireSep = true;

    if (auto p = Parent())
    {
        p->RequireSeparator();
    }
}

void JsonScope::WriteSeparator(_In_ Console& console)
{
    if (m_requireSep)
    {
        console.Write(L",");
    }
}
