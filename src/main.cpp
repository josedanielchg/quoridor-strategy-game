#include "app/Application.hpp"
#include "resources/AssetLoadError.hpp"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        App::Application app;
        app.run();
        return 0;
    }
    catch (const Resources::AssetLoadError &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
