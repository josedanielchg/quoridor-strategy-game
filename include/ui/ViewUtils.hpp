#pragma once
#include <SFML/Graphics.hpp>
#include "ui/UiConstants.hpp"

namespace UI
{
    inline sf::View makeLetterboxView(sf::Vector2u windowSize,
                                      sf::Vector2f designSize = UI_DESIGN_SIZE)
    {
        sf::View view;
        view.setSize(designSize);
        view.setCenter({designSize.x * 0.5f, designSize.y * 0.5f});

        if (windowSize.x == 0 || windowSize.y == 0)
        {
            view.setViewport({{0.f, 0.f}, {1.f, 1.f}});
            return view;
        }

        const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
        const float designRatio = designSize.x / designSize.y;

        sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
        if (windowRatio > designRatio)
        {
            const float scale = designRatio / windowRatio;
            viewport.size.x = scale;
            viewport.position.x = (1.f - scale) * 0.5f;
        }
        else
        {
            const float scale = windowRatio / designRatio;
            viewport.size.y = scale;
            viewport.position.y = (1.f - scale) * 0.5f;
        }

        view.setViewport(viewport);
        return view;
    }
}
