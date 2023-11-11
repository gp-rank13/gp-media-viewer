#include "LibMain.h"
#include <array>

namespace gigperformer
{
namespace sdk
{

GigPerformerAPI *CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}

} // namespace sdk
} // namespace gigperformer

// List of menu items
std::vector<std::string> menuNames = {
    "Show",
    "Hide",
};

int LibMain::GetMenuCount()
{
    return static_cast<int>(menuNames.size());
}

std::string LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        text = menuNames[index];
    }

    return text;
}

void LibMain::InvokeMenu(int index)
{
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        switch (index)
        {
        case 0:
            MediaViewer::showWindow();
            break;
        case 1:
            MediaViewer::hideWindow();
            break;
        default:
            break;
        }
    }
}

extern "C" void ShowMediaViewer(GPRuntimeEngine *)
{
    juce::MessageManager::getInstance()->callAsync([]() { MediaViewer::showWindow(); });
}

extern "C" void HideMediaViewer(GPRuntimeEngine *)
{
    juce::MessageManager::getInstance()->callAsync([]() { MediaViewer::hideWindow(); });
}

extern "C" void DisplayMedia(GPRuntimeEngine *vm)
{
    char buffer[100];
    GP_VM_PopString(vm, buffer, 100);
    std::string s = buffer;
    juce::MessageManager::getInstance()->callAsync([s]() { MediaViewer::displayMedia(s); });
}

ExternalAPI_GPScriptFunctionDefinition functionList[] = {
    {"Show", "", "", "Show the media viewer", ShowMediaViewer},
    {"Hide", "", "", "Hide the media viewer", HideMediaViewer},
    {"DisplayMedia", "path : String", "", "Specify the file path of the image/video to be displayed", DisplayMedia},
};

int LibMain::RequestGPScriptFunctionSignatureList(GPScript_AllowedLocations, // these are allowed in any script
                                                  ExternalAPI_GPScriptFunctionDefinition **list)
{
    *list = functionList;
    int count = sizeof(functionList) / sizeof(ExternalAPI_GPScriptFunctionDefinition);
    return count;
}
