#include <iostream>
#include <raylib.h> // raylib library 
#include <deque>

using namespace std;


// color struct takes 4 component ( red,green, blue, alpha) alpha means the transparancy of the color
// Color struct: 
Color darkGreen = {0, 117, 44, 255 }; // colors values for dark green, values: 0 to 255
Color green = {173, 204, 96, 255};


int cellSize = 30; 
int cellCount = 25; //rows and column

class Snake 
{
    public:
    deque <Vector2> SnakeBody = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};

    void Draw()
    {
        for (int i = 0; i < SnakeBody.size(); i++)
        {
            int x = SnakeBody[i].x; // accessing x coordinate from Vector2 struct in body
            int y = SnakeBody[i].y; // accessing y coordinate from Vector2 struct in body

            DrawRectangle(x*cellSize, y*cellSize, cellSize, cellSize, darkGreen);
        }
    }

};

// Food class
class Food{

    public: 

    Vector2 position; // position of food object
    Texture2D texture; // holds the returned texture data
    

    // constructor

    Food()
    {
        Image image = LoadImage("pics/food.png"); // loading the image 
        // using this image to render pixels using raylib
        // requires gpu access
        texture = LoadTextureFromImage(image); // loads texture from the image
        UnloadImage(image); // unloading to free up the memory after texture is loaded

        position = GenerateRandomPosition();
    }

    //Destructor
    ~Food()
    {
        UnloadTexture(texture);
    }

    // ---> All the methods of the food class are below!! <---

    // method to generate random position for the food
    Vector2 GenerateRandomPosition()
    {
        float x = GetRandomValue(0, cellCount - 1); // number of rows and colum is 25, need 0 to 24
        float y = GetRandomValue(0, cellCount - 1);

        return Vector2{x,y};
    }


    // method to draw the food 
    void Draw()
    {  
        // takes 5 arguement ( x,y, width, height, color)
        // DrawRectangle(position.x*cellSize, position.y*cellSize, cellSize, cellSize, darkGreen); 
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); // white is default value which means no color tint/filter is applied


    }

};

int main () {

    


    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake");
    SetTargetFPS(60);

    Food food = Food(); // Food() invokes the default constructor of the food class
    Snake snake = Snake();

    // game loop
    while (WindowShouldClose() == false){
        BeginDrawing();
        // painting the screen green
        ClearBackground(green);

        food.Draw();
        snake.Draw();


        EndDrawing();
    }

    CloseWindow();

    return 0;
}