#include "game/layers/board_selection_menu.h"
#include "engine/core/input_manager.h"
#include "engine/ui/text_menu_option.h"
#include "game/components/board.h"
#include "game/file_paths.h"
#include "game/game_application.h"
#include "game/layers/main_menu.h"
#include "game/layers/player_join.h"
#include "game/utils/highscore_utils.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

using game::highscore_utils::HighscoreVec;

BoardSelectionMenuLayer::BoardSelectionMenuLayer() :
    m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2},
           ui::AnchorPoint::TOP_LEFT,
           ui::Alignment::CENTER,
           true,
           10.0f),
    m_leaderboardTitle("Leaderboard",
                       ui::TextStyle{40, ORANGE},
                       {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2},
                       ui::AnchorPoint::TOP_LEFT,
                       true)
{
    SetupMenuOptions();
    UpdateLeaderboard();
}

void BoardSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle boardUnselectedStyle = {30, DARKGRAY};
    TextStyle boardSelectedStyle = {40, ORANGE};

    m_boardPaths.emplace_back("default");
    m_menu.AddOption(
        std::make_unique<TextMenuOption>("default", boardSelectedStyle, boardUnselectedStyle, true, [this]() {
            Board board{};

            TransistionTo(std::make_unique<PlayerJoinLayer>(std::move(board)));
        }));

    const std::filesystem::path& boardDirectory = FilePaths::s_boardsDirectory;
    
    if (!std::filesystem::exists(boardDirectory))
        std::filesystem::create_directories(boardDirectory);
    
    for (const auto& entry : std::filesystem::directory_iterator(boardDirectory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            std::string fullPath = entry.path().string();

            if (filename == "default")
                continue;

            m_boardPaths.emplace_back(fullPath);
            m_menu.AddOption(
                std::make_unique<TextMenuOption>(filename,
                                                 boardSelectedStyle,
                                                 boardUnselectedStyle,
                                                 false,
                                                 [this, fullPath]() {
                                                     Board board(fullPath);
                                                     TransistionTo(std::make_unique<PlayerJoinLayer>(std::move(board)));
                                                 }));
        }
    }

    TextStyle backButtonUnselectedStyle = {25, GRAY};
    TextStyle backButtonSelectedStyle = {30, ORANGE};

    m_boardPaths.emplace_back("back"); // placeholder for back button
    m_menu.AddOption(
        std::make_unique<TextMenuOption>("Back", backButtonSelectedStyle, backButtonUnselectedStyle, false, [this]() {
            TransistionTo(std::make_unique<MainMenuLayer>());
        }));
}

void BoardSelectionMenuLayer::OnUpdate(float ts)
{
    using enum engine::InputState;
    const auto& inputManager = game::GameApplication::GetInputManager();

    if (inputManager.IsAction("move_down", PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager.IsAction("move_up", PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager.IsAction("confirm", PRESSED))
    {
        m_menu.ConfirmSelection();
    }

    if (m_lastSelectedIndex != m_menu.GetSelectedIndex())
    {
        UpdateLeaderboard();
    }

    if (m_menu.IsUIUpdateNeeded())
    {
        PositionUIElements();
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void BoardSelectionMenuLayer::UpdateLeaderboard()
{
    m_lastSelectedIndex = m_menu.GetSelectedIndex();

    Board board;
    if (m_boardPaths[m_lastSelectedIndex] != "default" && m_boardPaths[m_lastSelectedIndex] != "back")
    {
        board = Board::LoadFromFile(m_boardPaths[m_lastSelectedIndex]);
    }

    auto highscores = board.GetHighscores();
    HighscoreVec sortedScores = game::highscore_utils::GetSortedHighscores(highscores);

    m_leaderboardScores.clear();
    m_leaderboardWidth = m_leaderboardTitle.GetDimensions().x;

    for (const auto& [name, score] : sortedScores)
    {
        m_leaderboardScores.emplace_back(std::format("{} {}", name, score),
                                         ui::TextStyle{30, LIGHTGRAY},
                                         Vector2Ex<float>{0, 0},
                                         ui::AnchorPoint::TOP_LEFT,
                                         true);
        if (m_leaderboardScores.back().GetDimensions().x > m_leaderboardWidth)
        {
            m_leaderboardWidth = m_leaderboardScores.back().GetDimensions().x;
        }
    }

    PositionUIElements();
}

void BoardSelectionMenuLayer::PositionUIElements()
{
    float menuWidth = m_menu.GetDimensions().x;
    float menuHeight = m_menu.GetDimensions().y;
    float spacing = 50.0f;

    // Leaderboard dimensions
    float leaderboardHeight = m_leaderboardTitle.GetDimensions().y;
    if (!m_leaderboardScores.empty())
    {
        leaderboardHeight += 10.f; // spacing
        for (const auto& score : m_leaderboardScores)
        {
            leaderboardHeight += 5.f + score.GetDimensions().y;
        }
    }

    Vector2Ex<float> menuPos = {400, ((float)GetScreenHeight() - menuHeight) / 2};

    // Position Menu
    m_menu.SetPosition(menuPos);
    m_menu.SetOrigin(ui::AnchorPoint::TOP_LEFT);

    // Position Leaderboard
    Vector2Ex leaderboardTitlePos = {GetScreenWidth() - m_leaderboardWidth - 300, 100.f};
    m_leaderboardTitle.SetPosition(leaderboardTitlePos);
    m_leaderboardTitle.SetOrigin(ui::AnchorPoint::TOP_LEFT);

    Vector2Ex currentScorePos = m_leaderboardTitle.GetPositionAtAnchor(ui::AnchorPoint::BOTTOM_LEFT);
    currentScorePos.y += 10.f; // spacing between title and scores

    for (auto& score : m_leaderboardScores)
    {
        score.SetPosition(currentScorePos);
        score.SetOrigin(ui::AnchorPoint::TOP_LEFT);
        currentScorePos.y += score.GetDimensions().y + 5.f;
    }
}

void BoardSelectionMenuLayer::OnRender()
{
    m_menu.Render();

    m_leaderboardTitle.Render();
    if (!m_leaderboardScores.empty())
    {
        for (const auto& score : m_leaderboardScores)
        {
            score.Render();
        }
    }
}
