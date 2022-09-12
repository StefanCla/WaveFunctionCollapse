#include "Precomp.hpp"
#include "WFC.hpp"
#include <SFML/Graphics.hpp>
#include <Chrono>

//Rules for which tile can be next to which
std::vector<int> ConstraintSetup()
{
    //Setting up each tile
    //Each side is 6 aligned
    //0-5 = top | 6-11 = bottom | 12 - 17 = left | 18 - 23 = right
    const int tile0 =
        1 << 1 | 1 << 2 | 1 << 5 |      //Top
        1 << 6 | 1 << 9 | 1 << 10 |     //Bottom
        1 << 14 | 1 << 16 | 1 << 17 |   //Left
        1 << 19 | 1 << 20 | 1 << 21;    //Right

    const int tile1 =
        1 << 1 | 1 << 2 | 1 << 5 |      //Top
        1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
        1 << 12 | 1 << 13 | 1 << 15 |   //Left
        1 << 19 | 1 << 20 | 1 << 21;    //Right

    const int tile2 =
        1 << 0 | 1 << 4 | 1 << 5 |      //Top
        1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
        1 << 12 | 1 << 13 | 1 << 15 |   //Left
        1 << 18 | 1 << 22 | 1 << 23;    //Right

    const int tile3 =
        1 << 0 | 1 << 3 | 1 << 4 |      //Top
        1 << 8 | 1 << 9 | 1 << 10 |     //Bottom
        1 << 12 | 1 << 13 | 1 << 15 |   //Left
        1 << 19 | 1 << 20 | 1 << 21;    //Right

    const int tile4 =
        1 << 0 | 1 << 3 | 1 << 4 |      //Top
        1 << 8 | 1 << 9 | 1 << 10 |     //Bottom
        1 << 14 | 1 << 16 | 1 << 17 |   //Left
        1 << 18 | 1 << 22 | 1 << 23;    //Right

    const int tile5 =
        1 << 1 | 1 << 2 | 1 << 5 |      //Top
        1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
        1 << 14 | 1 << 16 | 1 << 17 |   //Left
        1 << 18 | 1 << 22 | 1 << 23;    //Right


    std::vector<int> constraints;
    constraints.push_back(tile0);
    constraints.push_back(tile1);
    constraints.push_back(tile2);
    constraints.push_back(tile3);
    constraints.push_back(tile4);
    constraints.push_back(tile5);

    return constraints;
}

int main()
{
    //Setup window
    sf::RenderWindow window(sf::VideoMode(192, 192), "WFC test");

    //Import texture
    sf::Texture texture;
    if (!texture.loadFromFile("../Assets/Tile_Spritesheet.png"))
    {
        printf("Unable to load image file\n");
    }

    //Sample the sprites from the texture
    sf::Sprite sprite[6];
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            sprite[x + (3 * y)].setTexture(texture);
            sprite[x + (3 * y)].setTextureRect(sf::IntRect(x * 64, y * 64, 64, 64));
        }
    }

    //Create waveFunction and generate the map
    WFC* waveFunction = new WFC();
    waveFunction->Init(TILEAMOUNT, ConstraintSetup());
    waveFunction->ChooseRandomCell();

    //Function to collapse the wave

    //Expect WFC to return a vector, this is temporarely
    //std::vector<int> emptyvec;

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

        window.clear();

        for (int i = 0; i < GRIDSIZE; i++)
        {
            //Set postion of the sprite
            //sprite[emptyvec[i]].setPosition(sf::Vector2f((i % GRIDSIZEX) * 64.0f, (i / GRIDSIZEX) * 64.0f));
            //window.draw(sprite[emptyvec[i]]);
        }

        window.display();
    }

    delete waveFunction;

    return 0;
}