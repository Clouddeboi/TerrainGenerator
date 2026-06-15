#include "../headers/Application.hpp"

int main()
{
    Application app(
        1280,
        720,
        "Terrain Generator");

    app.Run();

    return 0;
}