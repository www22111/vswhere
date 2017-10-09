// <copyright file="Program.cpp" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#include "stdafx.h"

using namespace std;

void GetEnumerator(_In_ const CommandArgs& args, _In_ ISetupConfigurationPtr& query, _In_ IEnumSetupInstancesPtr& e);

int wmain(_In_ int argc, _In_ LPCWSTR argv[])
{
    CommandArgs args;
    Module queryModule;
    Console console(args, queryModule);

    console.Initialize();
    try
    {
        CoInitializer init;

        // Create the query object early to print version in logo.
        ISetupConfigurationPtr query;
        auto hr = query.CreateInstance(__uuidof(SetupConfiguration));
        if (FAILED(hr))
        {
            if (REGDB_E_CLASSNOTREG != hr)
            {
                throw win32_error(hr);
            }
        }

        // Try to get information about the query module for later.
        queryModule.FromIUnknown(static_cast<IUnknown*>(query));

        args.Parse(argc, argv);
        if (args.get_Help())
        {
            console.WriteLogo();
            args.Usage(console);

            return ERROR_SUCCESS;
        }

        // Attempt to get the ISetupHelper.
        ISetupHelperPtr helper;
        if (query)
        {
            query->QueryInterface(&helper);
        }

        if (!args.get_Version().empty() && !query)
        {
            auto message = ResourceManager::GetString(IDS_E_VERSION);
            throw win32_error(ERROR_INVALID_PARAMETER, message);
        }

        IEnumSetupInstancesPtr e;
        GetEnumerator(args, query, e);

        InstanceSelector selector(args, helper);
        auto instances = selector.Select(e);

        auto formatter = Formatter::Create(args.get_Format());
        formatter->Write(args, console, instances);

        return ERROR_SUCCESS;
    }
    catch (const system_error& ex)
    {
        const auto code = ex.code().value();
        if (ERROR_INVALID_PARAMETER == code)
        {
            console.WriteLogo();
        }

        console.Write(L"%ls 0x%x: ", ResourceManager::GetString(IDS_ERROR).c_str(), code);

        const auto* err = dynamic_cast<const win32_error*>(&ex);
        if (err)
        {
            console.WriteLine(err->wwhat());
        }
        else
        {
            console.WriteLine(L"%hs", ex.what());
        }

        return ex.code().value();
    }
    catch (const exception& ex)
    {
        console.WriteLine(L"%ls: %hs", ResourceManager::GetString(IDS_ERROR).c_str(), ex.what());
    }
    catch (...)
    {
        console.WriteLine(L"%ls: %ls", ResourceManager::GetString(IDS_ERROR).c_str(), ResourceManager::GetString(IDS_E_UNKNOWN).c_str());
    }

    return E_FAIL;
}

void GetEnumerator(_In_ const CommandArgs& args, _In_ ISetupConfigurationPtr& query, _In_ IEnumSetupInstancesPtr& e)
{
    if (!query)
    {
        return;
    }

    // If all instances are requested, try to get the proper enumerator; otherwise, fall back to original enumerator.
    if (args.get_All())
    {
        ISetupConfiguration2Ptr query2;

        auto hr = query->QueryInterface(&query2);
        if (SUCCEEDED(hr))
        {
            hr = query2->EnumAllInstances(&e);
            if (FAILED(hr))
            {
                throw win32_error(hr);
            }
        }
    }

    if (!e)
    {
        auto hr = query->EnumInstances(&e);
        if (FAILED(hr))
        {
            throw win32_error(hr);
        }
    }
}
