#include "engine/ui/text_menu_option.h"
#include "game/components/profile.h"
#include "game/file_paths.h"
#include "game/game_application.h"
#include "game/layers/create_profile.h"
#include "game/layers/main_menu.h"
#include "game/layers/profile_selection_menu_layer.h"
#include "game/utils/file_utils.h"
#include "game/serialization/json_converters.hpp"
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

ProfileSelectionMenuLayer::ProfileSelectionMenuLayer() :
    BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}
void ProfileSelectionMenuLayer::OnRender()
{
    BaseMenuLayer::OnRender();
    m_title.Render();
}

void ProfileSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle profileUnselectedStyle = {30, DARKGRAY};
    TextStyle profileSelectedStyle = {40, ORANGE};

    TextStyle buttonUnselectedStyle = {25, GRAY}; // Use for "Create Profile" and "Back"
    TextStyle buttonSelectedStyle = {30, ORANGE}; // Use for "Create Profile" and "Back"

    const std::filesystem::path& path = FilePaths::s_profilesDirectory;
    std::vector<nlohmann::json> profile_jsons = game::file_utils::ReadJsonsFromDirectory(path);

    bool isFirstOption = true;
    for (const auto& data : profile_jsons)
    {
        try
        {
            auto profile = std::make_shared<Profile>(data.get<Profile>());

            m_menu.AddOption(std::make_unique<TextMenuOption>(profile->GetUsername(),
                                                              profileSelectedStyle,
                                                              profileUnselectedStyle,
                                                              isFirstOption,
                                                              [this, profile]() {
                                                                  game::GameApplication::Get().SetProfile(profile);
                                                                  TransistionTo(std::make_unique<MainMenuLayer>());
                                                              }));
            isFirstOption = false;
        }
        catch (const nlohmann::json::exception& e)
        {
            std::cerr << "Failed to load profile: " << e.what() << std::endl;
            continue;
        }
    }

    m_menu.AddOption(
        std::make_unique<TextMenuOption>("Create Profile",
                                         buttonSelectedStyle,
                                         buttonUnselectedStyle,
                                         isFirstOption,
                                         [this]() { TransistionTo(std::make_unique<CreateProfileLayer>()); }));

    if (isFirstOption)
        isFirstOption = false; // "Create Profile" took the first slot if no profiles existed.

    m_menu.AddOption(
        std::make_unique<TextMenuOption>("Exit", buttonSelectedStyle, buttonUnselectedStyle, false, [this]() {
            game::GameApplication::Get().Stop();
        }));
}