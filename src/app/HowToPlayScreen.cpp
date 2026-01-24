#include "app/HowToPlayScreen.hpp"
#include "audio/SfxManager.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <sstream>

#include <fstream>

namespace App
{
    static constexpr unsigned int DIALOGUE_FONT_SIZE = 48;
    static constexpr unsigned int NAMEPLATE_FONT_SIZE = 32;
    static constexpr float DIALOGUE_PADDING = 24.f;
    static constexpr float NAMEPLATE_PADDING = 10.f;
    static constexpr float INDICATOR_SIZE = 14.f;
    static constexpr float INDICATOR_PADDING = 18.f;
    static constexpr float INDICATOR_BLINK_SPEED = 4.2f;

    static const char *KNIGHT_BACKGROUND_PATH = "assets/textures/knight_whiteboard.png";
    static const char *WIZARD_BACKGROUND_PATH = "assets/textures/wizard_whiteboard.png";
    static const char *FONT_PATH = "assets/fonts/pixelon.ttf";
    static const char *SCRIPT_PATH = "assets/tutorial/how-to-play-tutorial.txt";

    // Build sprites and texts bound to their textures/fonts. #
    HowToPlayScreen::HowToPlayScreen()
        : m_backgroundSprite(m_knightBackgroundTexture),
          m_boardSprite(m_boardTexture),
          m_dialogueText(m_font),
          m_speakerNameText(m_font),
          m_speakerNameShadow(m_font)
    {
    }

    // Load background assets, font, and tutorial steps. #
    bool HowToPlayScreen::init()
    {
        if (!m_knightBackgroundTexture.loadFromFile(KNIGHT_BACKGROUND_PATH))
        {
            std::cerr << "Failed to load knight how-to background\n";
            return false;
        }

        if (!m_wizardBackgroundTexture.loadFromFile(WIZARD_BACKGROUND_PATH))
        {
            std::cerr << "Failed to load wizard how-to background\n";
            return false;
        }

        m_backgroundSprite.setTexture(m_knightBackgroundTexture, true);
        const sf::Vector2u bgSize = m_knightBackgroundTexture.getSize();
        m_backgroundSprite.setOrigin({static_cast<float>(bgSize.x) / 2.f,
                                      static_cast<float>(bgSize.y) / 2.f});

        if (m_font.openFromFile(FONT_PATH))
        {
            m_hasFont = true;
            m_dialogueText.setFillColor(sf::Color::White);
            m_speakerNameText.setFillColor(sf::Color::White);
            m_speakerNameShadow.setFillColor(sf::Color(0, 0, 0, 180));
        }
        else
        {
            std::cerr << "Failed to load font for how-to screen\n";
        }

        m_steps = loadTutorialStepsFromFile(SCRIPT_PATH);
        if (m_steps.empty())
        {
            TutorialStep fallback;
            fallback.speaker = TutorialStep::Speaker::Wizard;
            fallback.text = "Tutorial data missing.\nAdd assets/tutorial/how-to-play-tutorial.txt.";
            fallback.boardImagePath.clear();
            m_steps.push_back(std::move(fallback));
        }

        m_continueIndicator.setPointCount(3);
        m_continueIndicator.setPoint(0, {0.f, 0.f});
        m_continueIndicator.setPoint(1, {INDICATOR_SIZE, 0.f});
        m_continueIndicator.setPoint(2, {INDICATOR_SIZE * 0.5f, INDICATOR_SIZE});
        m_continueIndicator.setFillColor(sf::Color::White);

        applyStep(0);
        return true;
    }

    // Register callback to return to the menu. #
    void HowToPlayScreen::setOnBack(std::function<void()> onBack)
    {
        m_onBack = std::move(onBack);
    }

    // Handle advance input (enter/space/click). #
    void HowToPlayScreen::handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        (void)window;
        if (event.is<sf::Event::Resized>())
            return;

        if (const auto *key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Enter ||
                key->scancode == sf::Keyboard::Scancode::Space)
            {
                advanceStep();
            }
            return;
        }

        if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button == sf::Mouse::Button::Left)
                advanceStep();
        }
    }

    // Reflow layout on resize. #
    void HowToPlayScreen::handleResize(sf::RenderWindow &window, sf::Vector2u /*size*/)
    {
        updateLayout(UI::makeLetterboxView(window.getSize()));
    }

    // Reset tutorial to the first step when entering. #
    void HowToPlayScreen::onEnter()
    {
        Screen::onEnter();
        applyStep(0);
    }

    // Animate the continue indicator blink. #
    void HowToPlayScreen::update(float dt)
    {
        m_blinkTimer += dt;
        const float pulse = (std::sin(m_blinkTimer * INDICATOR_BLINK_SPEED) + 1.f) * 0.5f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(80 + pulse * 175);
        sf::Color color = m_continueIndicator.getFillColor();
        color.a = alpha;
        m_continueIndicator.setFillColor(color);
    }

    // Draw background, board image, dialogue, and indicator. #
    void HowToPlayScreen::render(sf::RenderWindow &window)
    {
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        updateLayout(uiView);

        sf::View oldView = window.getView();
        window.setView(uiView);

        window.draw(m_backgroundSprite);

        if (m_hasBoardImage)
        {
            sf::View boardView;
            boardView.setSize({m_boardRect.size.x, m_boardRect.size.y});
            boardView.setCenter({m_boardRect.position.x + m_boardRect.size.x / 2.f,
                                 m_boardRect.position.y + m_boardRect.size.y / 2.f});

            const sf::FloatRect uiViewport = uiView.getViewport();
            const sf::Vector2f viewSize = uiView.getSize();
            const float normLeft = m_boardRect.position.x / viewSize.x;
            const float normTop = m_boardRect.position.y / viewSize.y;
            const float normWidth = m_boardRect.size.x / viewSize.x;
            const float normHeight = m_boardRect.size.y / viewSize.y;

            sf::FloatRect viewport;
            viewport.position.x = uiViewport.position.x + normLeft * uiViewport.size.x;
            viewport.position.y = uiViewport.position.y + normTop * uiViewport.size.y;
            viewport.size.x = normWidth * uiViewport.size.x;
            viewport.size.y = normHeight * uiViewport.size.y;
            boardView.setViewport(viewport);

            window.setView(boardView);
            window.draw(m_boardSprite);
            window.setView(uiView);
        }

        if (m_hasFont)
        {
            window.draw(m_dialogueText);
            window.draw(m_speakerNameShadow);
            window.draw(m_speakerNameText);
            window.draw(m_continueIndicator);
        }

        window.setView(oldView);
    }

    // Compute rectangles and update text/sprite layout. #
    void HowToPlayScreen::updateLayout(const sf::View &view)
    {
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();

        const sf::Texture &bgTexture = m_backgroundSprite.getTexture();
        const sf::Vector2u texSize = bgTexture.getSize();
        if (texSize.x > 0 && texSize.y > 0)
        {
            const float scaleX = viewSize.x / static_cast<float>(texSize.x);
            const float scaleY = viewSize.y / static_cast<float>(texSize.y);
            const float scale = std::max(scaleX, scaleY);
            m_backgroundSprite.setScale({scale, scale});
            m_backgroundSprite.setPosition(viewCenter);
        }

        m_boardRect = computeBoardRect(view);
        m_dialogueRect = computeDialogueRect(view);
        m_nameplateRect = computeNameplateRect(view);

        if (m_hasBoardImage)
        {
            const sf::Vector2u texSize = m_boardTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                const float scaleX = m_boardRect.size.x / static_cast<float>(texSize.x);
                const float scaleY = m_boardRect.size.y / static_cast<float>(texSize.y);
                const float scale = std::min(scaleX, scaleY);
                m_boardSprite.setScale({scale, scale});
                m_boardSprite.setPosition({m_boardRect.position.x + m_boardRect.size.x / 2.f,
                                           m_boardRect.position.y + m_boardRect.size.y / 2.f});
            }
        }

        if (m_hasFont)
        {
            const float textScale = std::min(viewSize.x / UI::UI_DESIGN_SIZE.x,
                                             viewSize.y / UI::UI_DESIGN_SIZE.y);
            const unsigned int textSize = std::max(1u, static_cast<unsigned int>(
                                                           std::round(DIALOGUE_FONT_SIZE * textScale)));
            m_dialogueText.setCharacterSize(textSize);
            const unsigned int nameSize = std::max(1u, static_cast<unsigned int>(
                                                           std::round(NAMEPLATE_FONT_SIZE * textScale)));
            m_speakerNameText.setCharacterSize(nameSize);
            m_speakerNameShadow.setCharacterSize(nameSize);
            updateDialogueText();

            const float padding = DIALOGUE_PADDING * textScale;
            m_dialogueText.setPosition({m_dialogueRect.position.x + padding,
                                        m_dialogueRect.position.y + padding});

            const float namePadding = NAMEPLATE_PADDING * textScale;
            const sf::Vector2f namePos{
                m_nameplateRect.position.x + namePadding,
                m_nameplateRect.position.y + m_nameplateRect.size.y / 2.f};
            const sf::FloatRect nameBounds = m_speakerNameText.getLocalBounds();
            m_speakerNameText.setOrigin({nameBounds.position.x,
                                         nameBounds.position.y + nameBounds.size.y / 2.f});
            m_speakerNameText.setPosition(namePos);
            m_speakerNameShadow.setOrigin(m_speakerNameText.getOrigin());
            m_speakerNameShadow.setPosition({namePos.x + 2.f * textScale,
                                             namePos.y + 2.f * textScale});

            const float indicatorSize = INDICATOR_SIZE * textScale;
            m_continueIndicator.setPoint(0, {0.f, 0.f});
            m_continueIndicator.setPoint(1, {indicatorSize, 0.f});
            m_continueIndicator.setPoint(2, {indicatorSize * 0.5f, indicatorSize});
            const float indicatorPadding = INDICATOR_PADDING * textScale;
            m_continueIndicator.setPosition({m_dialogueRect.position.x + m_dialogueRect.size.x - indicatorSize - indicatorPadding,
                                             m_dialogueRect.position.y + m_dialogueRect.size.y - indicatorSize - indicatorPadding});
        }
    }

    // Apply the requested tutorial step and update assets. #
    void HowToPlayScreen::applyStep(std::size_t index)
    {
        if (index >= m_steps.size())
            return;

        m_currentStep = index;
        const TutorialStep &step = m_steps[m_currentStep];

        if (step.speaker == TutorialStep::Speaker::Wizard)
            m_backgroundSprite.setTexture(m_wizardBackgroundTexture, true);
        else
            m_backgroundSprite.setTexture(m_knightBackgroundTexture, true);

        if (m_hasFont)
        {
            const char *label = (step.speaker == TutorialStep::Speaker::Wizard) ? "WIZARD" : "KNIGHT";
            m_speakerNameText.setString(label);
            m_speakerNameShadow.setString(label);
        }

        const sf::Texture &bgTexture = m_backgroundSprite.getTexture();
        const sf::Vector2u size = bgTexture.getSize();
        m_backgroundSprite.setOrigin({static_cast<float>(size.x) / 2.f,
                                      static_cast<float>(size.y) / 2.f});

        if (!step.boardImagePath.empty())
            loadBoardImage(step.boardImagePath);
        else
            m_hasBoardImage = false;

        updateDialogueText();
    }

    // Advance to the next tutorial step or exit. #
    void HowToPlayScreen::advanceStep()
    {
        if (m_steps.empty())
            return;

        if (m_currentStep + 1 >= m_steps.size())
        {
            if (m_onBack)
                m_onBack();
            return;
        }

        const std::size_t previousStep = m_currentStep;
        applyStep(m_currentStep + 1);
        if (m_currentStep != previousStep)
            Audio::SfxManager::instance().play(Audio::SfxId::TutorialNextDialog);
    }

    // Load the board screenshot for the current step. #
    bool HowToPlayScreen::loadBoardImage(const std::string &path)
    {
        if (path == m_loadedBoardPath && m_hasBoardImage)
            return true;

        if (!m_boardTexture.loadFromFile(path))
        {
            std::cerr << "Failed to load tutorial board image: " << path << "\n";
            m_hasBoardImage = false;
            m_loadedBoardPath.clear();
            return false;
        }

        m_boardSprite.setTexture(m_boardTexture, true);
        const sf::Vector2u size = m_boardTexture.getSize();
        m_boardSprite.setOrigin({static_cast<float>(size.x) / 2.f,
                                 static_cast<float>(size.y) / 2.f});
        m_loadedBoardPath = path;
        m_hasBoardImage = true;
        return true;
    }

    // Rewrap and update dialogue text for the layout. #
    void HowToPlayScreen::updateDialogueText()
    {
        if (!m_hasFont || m_steps.empty())
            return;

        const float maxWidth = std::max(0.f, m_dialogueRect.size.x - DIALOGUE_PADDING * 2.f);
        const std::string wrapped = wrapText(m_steps[m_currentStep].text,
                                             m_font,
                                             m_dialogueText.getCharacterSize(),
                                             maxWidth);
        m_dialogueText.setString(wrapped);
    }

    // Compute the board image rectangle in view space. #
    sf::FloatRect HowToPlayScreen::computeBoardRect(const sf::View &view) const
    {
        const sf::Vector2f size = view.getSize();
        const float left = size.x * UI::HOWTO_BOARD_LEFT_RATIO;
        const float top = size.y * UI::HOWTO_BOARD_TOP_RATIO;
        const float width = size.x * UI::HOWTO_BOARD_WIDTH_RATIO;
        const float height = size.y * UI::HOWTO_BOARD_HEIGHT_RATIO;
        return sf::FloatRect({left, top}, {width, height});
    }

    // Compute the dialogue text rectangle in view space. #
    sf::FloatRect HowToPlayScreen::computeDialogueRect(const sf::View &view) const
    {
        const sf::Vector2f size = view.getSize();
        const float left = size.x * UI::HOWTO_DIALOGUE_LEFT_RATIO;
        const float top = size.y * UI::HOWTO_DIALOGUE_TOP_RATIO;
        const float width = size.x * UI::HOWTO_DIALOGUE_WIDTH_RATIO;
        const float height = size.y * UI::HOWTO_DIALOGUE_HEIGHT_RATIO;
        return sf::FloatRect({left, top}, {width, height});
    }

    // Compute the speaker nameplate rectangle in view space. #
    sf::FloatRect HowToPlayScreen::computeNameplateRect(const sf::View &view) const
    {
        const sf::Vector2f size = view.getSize();
        const float left = size.x * UI::HOWTO_NAMEPLATE_LEFT_RATIO;
        const float top = size.y * UI::HOWTO_NAMEPLATE_TOP_RATIO;
        const float width = size.x * UI::HOWTO_NAMEPLATE_WIDTH_RATIO;
        const float height = size.y * UI::HOWTO_NAMEPLATE_HEIGHT_RATIO;
        return sf::FloatRect({left, top}, {width, height});
    }

    // Wrap text to fit within a maximum width. #
    std::string HowToPlayScreen::wrapText(const std::string &input,
                                          const sf::Font &font,
                                          unsigned int characterSize,
                                          float maxWidth)
    {
        if (maxWidth <= 0.f)
            return input;

        std::istringstream lines(input);
        std::string line;
        std::string output;
        bool firstLine = true;

        while (std::getline(lines, line))
        {
            std::istringstream words(line);
            std::string word;
            std::string currentLine;

            sf::Text text(font);
            text.setCharacterSize(characterSize);

            while (words >> word)
            {
                std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
                text.setString(testLine);
                if (text.getLocalBounds().size.x > maxWidth && !currentLine.empty())
                {
                    if (!firstLine)
                        output += "\n";
                    output += currentLine;
                    firstLine = false;
                    currentLine = word;
                }
                else
                {
                    currentLine = testLine;
                }
            }

            if (!currentLine.empty())
            {
                if (!firstLine)
                    output += "\n";
                output += currentLine;
                firstLine = false;
            }
        }

        return output;
    }

    // Load tutorial steps from a delimited text file. #
    std::vector<HowToPlayScreen::TutorialStep> HowToPlayScreen::loadTutorialStepsFromFile(const std::string &path)
    {
        std::vector<TutorialStep> steps;
        std::ifstream file(path);
        if (!file)
        {
            std::cerr << "Failed to open tutorial script: " << path << "\n";
            return steps;
        }

        auto trim = [](const std::string &value)
        {
            size_t start = 0;
            while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
                ++start;
            size_t end = value.size();
            while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
                --end;
            return value.substr(start, end - start);
        };

        auto unescape = [](const std::string &value)
        {
            std::string out;
            out.reserve(value.size());
            for (size_t i = 0; i < value.size(); ++i)
            {
                if (value[i] == '\\' && i + 1 < value.size())
                {
                    const char next = value[i + 1];
                    if (next == 'n')
                        out.push_back('\n');
                    else if (next == 't')
                        out.push_back('\t');
                    else if (next == '|')
                        out.push_back('|');
                    else if (next == '\\')
                        out.push_back('\\');
                    else
                        out.push_back(next);
                    ++i;
                }
                else
                {
                    out.push_back(value[i]);
                }
            }
            return out;
        };

        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            const std::string trimmed = trim(line);
            if (trimmed.empty() || trimmed[0] == '#')
                continue;

            std::string speakerPart;
            std::string boardPart;
            std::string textPart;
            std::string current;
            int part = 0;
            bool escape = false;

            for (char c : line)
            {
                if (escape)
                {
                    current.push_back(c);
                    escape = false;
                    continue;
                }
                if (c == '\\')
                {
                    current.push_back(c);
                    escape = true;
                    continue;
                }
                if (c == '|' && part < 2)
                {
                    if (part == 0)
                        speakerPart = current;
                    else if (part == 1)
                        boardPart = current;
                    current.clear();
                    ++part;
                    continue;
                }
                current.push_back(c);
            }

            if (part < 2)
                continue;

            textPart = current;

            speakerPart = trim(speakerPart);
            boardPart = trim(boardPart);
            textPart = trim(textPart);

            if (speakerPart.empty() || textPart.empty())
                continue;

            TutorialStep step;
            std::string speakerLower;
            speakerLower.reserve(speakerPart.size());
            for (char c : speakerPart)
                speakerLower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

            if (speakerLower == "wizard")
                step.speaker = TutorialStep::Speaker::Wizard;
            else
                step.speaker = TutorialStep::Speaker::Knight;

            step.boardImagePath = unescape(boardPart);
            step.text = unescape(textPart);
            steps.push_back(std::move(step));
        }

        return steps;
    }
}
