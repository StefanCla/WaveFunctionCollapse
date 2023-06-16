#include "Precomp.hpp"
#include "WFC.hpp"
#include "Constrain.hpp"
#include <SFML/Graphics.hpp>
#include <Chrono>
#include <bitset>
#include "Image.hpp"
#include "Simple.hpp"

//Rules for which tile can be next to which
const long long* ConstraintSetup()
{
    long long* tileConstraints = new long long[TILEAMOUNT];

    if (SPRITESHEETFILENAME == "Tile_Spritesheet.png")
    {
        //Setting up each tile
        //Each side is 6 aligned
        //0-5 = top | 6-11 = bottom | 12 - 17 = left | 18 - 23 = right
        tileConstraints[0] =
            1 << 1 | 1 << 2 | 1 << 5 |      //Top
            1 << 8 | 1 << 9 | 1 << 10 |     //Bottom
            1 << 14 | 1 << 16 | 1 << 17 |   //Left
            1 << 19 | 1 << 20 | 1 << 21;    //Right

        tileConstraints[1] =
            1 << 1 | 1 << 2 | 1 << 5 |      //Top
            1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
            1 << 12 | 1 << 13 | 1 << 15 |   //Left
            1 << 19 | 1 << 20 | 1 << 21;    //Right

        tileConstraints[2] =
            1 << 0 | 1 << 3 | 1 << 4 |      //Top
            1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
            1 << 12 | 1 << 13 | 1 << 15 |   //Left
            1 << 18 | 1 << 22 | 1 << 23;    //Right

        tileConstraints[3] =
            1 << 0 | 1 << 3 | 1 << 4 |      //Top
            1 << 8 | 1 << 9 | 1 << 10 |     //Bottom
            1 << 12 | 1 << 13 | 1 << 15 |   //Left
            1 << 19 | 1 << 20 | 1 << 21;    //Right

        tileConstraints[4] =
            1 << 0 | 1 << 3 | 1 << 4 |      //Top
            1 << 8 | 1 << 9 | 1 << 10 |     //Bottom
            1 << 14 | 1 << 16 | 1 << 17 |   //Left
            1 << 18 | 1 << 22 | 1 << 23;    //Right

        tileConstraints[5] =
            1 << 1 | 1 << 2 | 1 << 5 |      //Top
            1 << 6 | 1 << 7 | 1 << 11 |     //Bottom
            1 << 14 | 1 << 16 | 1 << 17 |   //Left
            1 << 18 | 1 << 22 | 1 << 23;    //Right

        return tileConstraints;
    }
    else if (SPRITESHEETFILENAME == "urban.png")
    {
        //Setting up each tile
        //Tile order: Top / Bottom / Left / Right
        //If side has brown, flip the bit, else leave it 0
        char tile0 =
            1 << 0 | 1 << 2 | 1 << 3;
        char tile1 =
            1 << 0 | 1 << 2;
        char tile2 =
            1 << 0;
        char tile3 =
            1 << 0 | 1 << 3;
        char tile4 =
            1 << 2 | 1 << 3;
        char tile5 =
            1 << 2;
        char tile6 = 0;
        char tile7 =
            1 << 3;
        char tile8 =
            1 << 1 | 1 << 2 | 1 << 3;
        char tile9 =
            1 << 1 | 1 << 2;
        char tile10 =
            1 << 1;
        char tile11 =
            1 << 1 | 1 << 3;
        char tile12 =
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
        char tile13 =
            1 << 0 | 1 << 1 | 1 << 2;
        char tile14 =
            1 << 0 | 1 << 1;
        char tile15 =
            1 << 0 | 1 << 1 | 1 << 3;

        std::vector<char> tiles;
        tiles.push_back(tile0);
        tiles.push_back(tile1);
        tiles.push_back(tile2);
        tiles.push_back(tile3);
        tiles.push_back(tile4);
        tiles.push_back(tile5);
        tiles.push_back(tile6);
        tiles.push_back(tile7);
        tiles.push_back(tile8);
        tiles.push_back(tile9);
        tiles.push_back(tile10);
        tiles.push_back(tile11);
        tiles.push_back(tile12);
        tiles.push_back(tile13);
        tiles.push_back(tile14);
        tiles.push_back(tile15);

        int bitAlign = 0;
        int reflection = 1;
        bool isBitFlipped;

        //Create the constraints based on which color will match
        for (int i = 0; i < TILEAMOUNT; i++)
        {
            long long combinedConstraints = 0;

            for (int j = 0; j < 4; j++)
            {
                long long tileConstraint = 0;

                if (j == 1)
                {
                    bitAlign = 16;
                    reflection = 0;
                }
                else if (j == 2)
                {
                    bitAlign = 32;
                    reflection = 3;
                }
                else if (j == 3)
                {
                    bitAlign = 48;
                    reflection = 2;
                }
                else
                {
                    bitAlign = 0;
                    reflection = 1;
                }

                if (tiles[i] & 1 << j)
                    isBitFlipped = true;
                else
                    isBitFlipped = false;

                for (int l = 0; l < TILEAMOUNT; l++)
                {
                    std::bitset<8> bits(tiles[l]);

                    if (isBitFlipped && bits.test(reflection))
                        tileConstraint |= 1 << l;
                    else if (!isBitFlipped && !bits.test(reflection))
                        tileConstraint |= 1 << l;
                }

                combinedConstraints |= (tileConstraint << bitAlign);
            }
            tileConstraints[i] = combinedConstraints;
        }

        return tileConstraints;
    }
}

//Rules for which tile can be next to which
const long long* ConstraintSetup2()
{
    //Setting up each tile
    //Tile order: Top / Bottom / Left / Right
    //If side has brown, flip the bit, else leave it 0
    char tile0 =
        1 << 0 | 1 << 2 | 1 << 3;
    char tile1 =
        1 << 0 | 1 << 2;
    char tile2 =
        1 << 0;
    char tile3 =
        1 << 0 | 1 << 3;
    char tile4 =
        1 << 2 | 1 << 3;
    char tile5 =
        1 << 2;
    char tile6 = 0;
    char tile7 =
        1 << 3;
    char tile8 =
        1 << 1 | 1 << 2 | 1 << 3;
    char tile9 =
        1 << 1 | 1 << 2;
    char tile10 =
        1 << 1;
    char tile11 =
        1 << 1 | 1 << 3;
    char tile12 =
        1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
    char tile13 =
        1 << 0 | 1 << 1 | 1 << 2;
    char tile14 =
        1 << 0 | 1 << 1;
    char tile15 =
        1 << 0 | 1 << 1 | 1 << 3;

    std::vector<char> tiles;
    tiles.push_back(tile0);
    tiles.push_back(tile1);
    tiles.push_back(tile2);
    tiles.push_back(tile3);
    tiles.push_back(tile4);
    tiles.push_back(tile5);
    tiles.push_back(tile6);
    tiles.push_back(tile7);
    tiles.push_back(tile8);
    tiles.push_back(tile9);
    tiles.push_back(tile10);
    tiles.push_back(tile11);
    tiles.push_back(tile12);
    tiles.push_back(tile13);
    tiles.push_back(tile14);
    tiles.push_back(tile15);

    int bitAlign = 0;
    int reflection = 1;
    bool isBitFlipped;

    //Create the constraints based on which color will match
    long long* tileConstraints = new long long[TILEAMOUNT];
    for (int i = 0; i < TILEAMOUNT; i++)
    {
        long long combinedConstraints = 0;

        for (int j = 0; j < 4; j++)
        {
            long long tileConstraint = 0;

            if (j == 1)
            {
                bitAlign = 16;
                reflection = 0;
            }
            else if (j == 2)
            {
                bitAlign = 32;
                reflection = 3;
            }
            else if (j == 3)
            {
                bitAlign = 48;
                reflection = 2;
            }
            else
            {
                bitAlign = 0;
                reflection = 1;
            }

            if (tiles[i] & 1 << j)
                isBitFlipped = true;
            else
                isBitFlipped = false;

            for (int l = 0; l < TILEAMOUNT; l++)
            {
                std::bitset<8> bits(tiles[l]);

                if (isBitFlipped && bits.test(reflection))
                    tileConstraint |= 1 << l;
                else if (!isBitFlipped && !bits.test(reflection))
                    tileConstraint |= 1 << l;
            }

            combinedConstraints |= (tileConstraint << bitAlign);
        }
        tileConstraints[i] = combinedConstraints;
    }

    return tileConstraints;
}

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


    //Previews version of the WFC
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

    //Create waveFunction and generate the map
    //WFC* waveFunction = new WFC(ConstraintSetup2());
    //waveFunction->Initialization();
    //waveFunction->StartWFC();

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
            //Set postion of the sprite
            //sprite[waveFunction->GetMap()[i].second].setPosition(sf::Vector2f((i % GRIDSIZEX) * TILESIZE, (i / GRIDSIZEX) * TILESIZE));
            //window.draw(sprite[waveFunction->GetMap()[i].second]);

            sprite[simple->GetCellGrid()[i][0]].setPosition(sf::Vector2f((i % GRIDSIZEX) * TILESIZE, (i / GRIDSIZEX) * TILESIZE));
            window.draw(sprite[simple->GetCellGrid()[i][0]]);
        }

        window.display();
    }

    //delete waveFunction;
    delete simple;

    return 0;
}