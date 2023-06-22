#include "Precomp.hpp"
#include "WFC.hpp"
#include "Constrain.hpp"
#include <SFML/Graphics.hpp>
#include <Chrono>
#include <bitset>
#include "Image.hpp"
#include "Simple.hpp"

int main()
{
    //Important and sample from image
    std::string path = "../Assets/Custom.png";
    Image* image = new Image(&path, 16, false, false);

    //Build constrains using the new pixel comparison
    Constrain* constrain = new Constrain(*image);
    constrain->BuildWithImage();
    
    //Load constrains in using a cvs file
    //std::string filePath = "../Assets/Example.csv";
    //constrain->BuildWithCSV(filePath);

    //Setup window
    sf::RenderWindow window(sf::VideoMode(GRIDSIZEX * TILESIZE, GRIDSIZEY * TILESIZE), "WFC second iteration");

    //Import texture
    sf::Texture texture;
    if (!texture.loadFromFile("../Assets/" + static_cast<std::string>(SPRITESHEETFILENAME)))
    {
        printf("Unable to load image file\n");
    }

    //Sample the sprites from the texture
    sf::Sprite sprite[TILEAMOUNT];
    for (int y = 0; y < SPRITESHEETY; y++)
    {
        for (int x = 0; x < SPRITESHEETX; x++)
        {
            sprite[x + (SPRITESHEETX * y)].setTexture(texture);
            sprite[x + (SPRITESHEETX * y)].setTextureRect(sf::IntRect(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE));
        }
    }

    Simple* simple = new Simple(*constrain);
    simple->Initialization();
    simple->StartWFC();

    //Main loop
    while (window.isOpen())
    {
        //Poll input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            simple->RestartWFC();

        window.clear();

        for (int i = 0; i < GRIDSIZE; i++)
        {
            sprite[simple->GetCellGrid()[i][0]].setPosition(sf::Vector2f((i % GRIDSIZEX) * TILESIZE, (i / GRIDSIZEX) * TILESIZE));
            window.draw(sprite[simple->GetCellGrid()[i][0]]);
        }

        window.display();
    }

    delete simple;

    return 0;
}