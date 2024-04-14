#include <iostream>
#include <raylib.h> // raylib library 
#include <deque>
#include <raymath.h>

using namespace std;


// color struct takes 4 component ( red,green, blue, alpha) alpha means the transparancy of the color
// Color struct: 
Color darkGreen = {0, 117, 44, 255 }; // colors values for dark green, values: 0 to 255
Color green = {173, 204, 96, 255};


int cellSize = 30; 
int cellCount = 25; //rows and column

double lastUpdateTime = 0; // counter to keep track of time at which last update of snake movement occured


// since the game is running in 60 fps, movement happens 60 times per second 
// in that case the sanke moves very fast (doesnt make sense, it will be impossible to have good game experience)

// this bool function is used to slow down the snake movement 
// takes interval parameter which will be 2 milisecond 
bool eventHappened (double interval) 
{
    double currentTime = GetTime(); // raylib function to get the time since the program started running
    if (currentTime - lastUpdateTime >= interval) // if condition is true it means that the interval has passed since the last update
    {
        lastUpdateTime = currentTime; // updating the time of last snake movement occured
        return true;
    }
    return false;
}


// checks if generated position is on top of existing snake
bool ElementsInDeque(Vector2 element, deque<Vector2> deque)// checks if the randomly generated position is same to the previous one
{   
    for (unsigned i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
        return false;
    }
}

class Snake 
{
    public:
    deque <Vector2> SnakeBody = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}}; // initatio body of the sanke
    // P.S: origin for the grid is located in the top left cornor 
    Vector2 direction = {1,0}; // move to right


    void Draw()
    {
        for (unsigned int  i = 0; i < SnakeBody.size(); i++)
        {
            float x = SnakeBody[i].x; // accessing x coordinate from Vector2 struct in body
            float y = SnakeBody[i].y; // accessing y coordinate from Vector2 struct in body

            // recrangle struct to draw rounded edge rectangle
            Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.8, 6, darkGreen );
        }
    }

    void Move()
    {
        SnakeBody.pop_back(); // removing last cell
        SnakeBody.push_front(Vector2Add(SnakeBody[0], direction));
    }

};

// Food class
class Food{

    public: 

    Vector2 position; // position of food object
    Texture2D texture; // holds the returned texture data
    

    // constructor

    Food(deque<Vector2>snakeBody)
    {
        Image image = LoadImage("pics/food.png"); // loading the image 
        // using this image to render pixels using raylib
        // requires gpu access
        texture = LoadTextureFromImage(image); // loads texture from the image
        UnloadImage(image); // unloading to free up the memory after texture is loaded

        position = GenerateRandomPosition(snakeBody);
    }

    //Destructor
    ~Food()
    {
        UnloadTexture(texture);
    }

    // ---> All the methods of the food class are below!! <---

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1); // number of rows and colum is 25, need 0 to 24
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }
    // method to generate random position for the food
    Vector2 GenerateRandomPosition(deque<Vector2>snakeBody) 
    {
        
        Vector2 position = GenerateRandomCell();
        while(ElementsInDeque(position, snakeBody)) // checks if generated position is on top of existing snake
        {
            position = GenerateRandomCell();
        }
        return position;
    }


    // method to draw the food 
    void Draw()
    {  
        // takes 5 arguement ( x,y, width, height, color)
        // DrawRectangle(position.x*cellSize, position.y*cellSize, cellSize, cellSize, darkGreen); 
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); // white is default value which means no color tint/filter is applied


    }

};

// Container for food and snake objects 
class Game // will hold the snake and food class
{
    public:
    Snake snake = Snake();
    Food food = Food(snake.SnakeBody);

    // methods 

    void Draw()
    { // food and snake needs to be rendered, it does so by calling these function
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        snake.Move(); // this function updates/ moves the snake as the user pressed keys
        checkColliosionWithFood();
    }

    void checkColliosionWithFood()
    {
        // check the head of the snake is the same as the coordinate of food
        if (Vector2Equals(snake.SnakeBody[0],food.position)) // Vector2Equals is raylib function that compares two vector2 
        {
            // generate  food at random position after head of the snake touches the food
            food.position = food.GenerateRandomPosition(snake.SnakeBody); // gives random new position of the food
            // same food renders at different position becasue we are just updating the position of the food 
            //so we dont need to create another food

            // add a cell to the body of the snake
            
        }
    }


};

int main () {
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake");
    SetTargetFPS(60);

    // Doesnt need to create food and snake objects becasue the container does it.
    // Food food = Food(); // Food() invokes the default constructor of the food class
    // Snake snake = Snake();

    // instead we create game object

    Game game = Game();


    // game loop
    while (WindowShouldClose() == false){
        BeginDrawing();

        // updating the movement of the snake every ---> 0.2 sec <---

        if(eventHappened(0.2)) // if the 0.2 interval is passed update the movement
        { 
        game.Update(); // movement function
        }

        // ---> adding keyboard controls <---
        
        // snake cannot go in the oposite direction if oposite directin arrow is pressed
        // for example: if the snake is moving downward, and UP key is pressed, the snake cant change 180 degree direction 
        // to avoid this, we add a contition
        // snake can only go up if it is not moving in downward direction 

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) 
        {
            // move the snake upwards; we can do that by decreasing y coordinate
            game.snake.direction = {0,-1}; 
        }
        // DOWN
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) 
        {
            game.snake.direction = {0,1};
        }
        //LEFT
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1,0};
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1,0};
        }



        // painting the screen green
        ClearBackground(green);
        
        game.Draw();
        


        EndDrawing();
    }

    CloseWindow();

    return 0;
}