// <copyright file="PropertySelector.h" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#pragma once

class PropertySelector
{
public:
    PropertySelector()
    {
    }

    PropertySelector(_In_ const PropertySelector& obj) :
        m_properties(obj.m_properties)
    {
    }

    size_t get_Length() const
    {
        return m_properties.size();
    }

    bool IsEmpty() const
    {
        return m_properties.empty();
    }

    operator const std::wstring&() const
    {
        if (m_properties.size())
        {
            return m_properties.front();
        }

        return empty;
    }

private:
    friend class CommandArgs;

    static const std::wstring empty;
    std::vector<std::wstring> m_properties;
};
