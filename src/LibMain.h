#pragma once

#include <cstddef>
#include <gigperformer/sdk/GPMidiMessages.h>
#include <gigperformer/sdk/GPUtils.h>
#include <gigperformer/sdk/GigPerformerAPI.h>
#include "MediaViewer.h"

const std::string XMLProductDescription =
    R"xml(<Library>
    <Product Name="GP Media Viewer" Version="1.0" BuildDate="13/8/2023"></Product>
    <Description>Display images/videos in a dedicated window</Description>
    "</Library>)xml";

class LibMain : public gigperformer::sdk::GigPerformerAPI
{
  protected:
    int GetMenuCount() override;
    std::string GetMenuName(int index) override;
    void InvokeMenu(int itemIndex) override;

    int RequestGPScriptFunctionSignatureList(GPScript_AllowedLocations location,
                                             ExternalAPI_GPScriptFunctionDefinition **list) override;

  public:
    LibMain(LibraryHandle handle) : GigPerformerAPI(handle)
    {
    }
    virtual ~LibMain()
    {
    }

    void OnOpen() override
    {
        MediaViewer::initialize();
    }

    void OnClose() override
    {
        MediaViewer::finalize();
    }

    void Initialization() override
    {
        registerCallback("OnOpen");
        registerCallback("OnClose");
    }

    std::string GetProductDescription() override
    {
        return XMLProductDescription;
    }
};
