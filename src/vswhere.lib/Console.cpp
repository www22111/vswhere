// <copyright file="Console.cpp" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#include "stdafx.h"
#include "vswhere.lib.Version.h"

using namespace std;

void Console::Initialize() noexcept
{
    if (IsConsole(stdout))
    {
        ::_setmode(_fileno(stdout), _O_WTEXT);
    }
    else
    {
        char sz[10];
        ::sprintf_s(sz, ".%d", ::GetConsoleCP());

        ::setlocale(LC_CTYPE, sz);
    }
}

const wstring Console::get_Version() const noexcept
{
    const auto version = m_module.get_FileVersion();
    const auto nID = version.empty() ? IDS_PROGRAMINFO : IDS_PROGRAMINFOEX;

    return ResourceManager::FormatString(nID, NBGV_INFORMATIONAL_VERSION, version.c_str());
}

const wstring Console::get_Copyright() const noexcept
{
    return ResourceManager::GetString(IDS_COPYRIGHT);
}

void __cdecl Console::Write(_In_ LPCWSTR wzFormat, ...)
{
    va_list args;

    va_start(args, wzFormat);
    Write(wzFormat, args);
    va_end(args);
}

void __cdecl Console::Write(_In_ const std::wstring& value)
{
    Write(value.c_str(), NULL);
}

void __cdecl Console::WriteLine(_In_ LPCWSTR wzFormat, ...)
{
    if (wzFormat)
    {
        va_list args;

        va_start(args, wzFormat);
        Write(wzFormat, args);
        va_end(args);
    }

    Write(L"\n", NULL);
}

void __cdecl Console::WriteLine(_In_ const std::wstring& value)
{
    Write(L"%ls\n", value.c_str());
}

void Console::Write(_In_ LPCWSTR wzFormat, va_list args)
{
    ::_vwprintf_p(wzFormat, args);
}

bool Console::IsConsole(_In_ FILE* f) const noexcept
{
    auto fno = ::_fileno(f);
    auto hFile = (HANDLE)::_get_osfhandle(fno);
    auto dwType = ::GetFileType(hFile);

    dwType &= ~FILE_TYPE_REMOTE;

    if (FILE_TYPE_CHAR != dwType)
    {
        return false;
    }

    DWORD dwMode;
    if (!::GetConsoleMode(hFile, &dwMode))
    {
        return false;
    }

    return true;
}

void __cdecl Console::WriteLogo()
{
    if (get_Logo())
    {
        WriteLine(get_Version());
        WriteLine(get_Copyright());
        WriteLine();
    }
}
