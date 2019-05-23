// <copyright file="Console.h" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#pragma once

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x4
#endif

class Console
{
public:
    Console(_In_ const CommandArgs& args) :
        m_args(args),
        m_fInitialized(false),
        m_fvt100(false)
    {
    }

    Console(_In_ const Console& obj) :
        m_args(obj.m_args),
        m_fInitialized(obj.m_fInitialized),
        m_fvt100(obj.m_fvt100)
    {
    }

    bool IsVT100() noexcept;

    void __cdecl Write(_In_ LPCWSTR wzFormat, ...);
    void __cdecl Write(_In_ const std::wstring& value);
    void __cdecl WriteLine(_In_ LPCWSTR wzFormat = NULL, ...);
    void __cdecl WriteLine(_In_ const std::wstring& value);

protected:
    virtual void Initialize() noexcept;
    virtual void Write(_In_ LPCWSTR wzFormat, va_list args);

private:
    bool IsConsole(_In_ FILE* f, _Outptr_ HANDLE* hStdOut) const noexcept;

    const CommandArgs& m_args;
    bool m_fInitialized;
    bool m_fvt100;
};
