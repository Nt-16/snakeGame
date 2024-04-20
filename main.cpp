#include <iostream>
#include <raylib.h> // raylib library 
#include <deque>
#include <raymath.h>

using namespace std;


// color struct takes 4 component ( red,green, blue, alpha) alpha means the transparancy of the color
// Color struct: 
Color darkGreen = {0, 117, 44, 255 }; // colors values for dark green, values: 0 to 255
Color green = {173, 204, 96, 255};

int scoreCount = 0; 
int cellSize = 30; 
int cellCount = 25; //rows and column

int offsetvalue = 70; // width of the border

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


// checks if generated position is on top of existing snake or
// checks weather a vector2 coordinate is in the deque or not
bool ElementsInDeque(Vector2 element, deque<Vector2> deque)// checks if the randomly generated position is same to the previous one
{   
    for (unsigned i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element)) // raylib fucntion to compare Vector2 
        {
            return true;
        }
        
    }
    return false;
}

class Snake 
{
    public:
    deque <Vector2> SnakeBody = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}}; // initatio body of the sanke
    // P.S: origin for the grid is located in the top left cornor 
    Vector2 direction = {1,0}; // move to right

    bool growCell = false; // if true we add a sgment in the snake


    void Draw()
    {
        for (unsigned int  i = 0; i < SnakeBody.size(); i++)
        {
            float x = SnakeBody[i].x; // accessing x coordinate from Vector2 struct in body
            float y = SnakeBody[i].y; // accessing y coordinate from Vector2 struct in body

            // recrangle struct to draw rounded edge rectangle
            Rectangle segment = Rectangle{offsetvalue + x * cellSize, offsetvalue + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.8, 6, darkGreen );
        }
    }

    void Move()
    {
        //we take this put from the if statement becasue it was both inside if and else so we could take it out 
        // no duplication
        SnakeBody.push_front(Vector2Add(SnakeBody[0], direction)); 
        
        // if this is if statement is true we dont pop last element in deque thats why snake grows 
        //when it collides with the food 
        if (growCell == true)
        {   
            // to add a segment, a newelement have to be pushed in front of the deque
            

            // after adding set the grow cell to false
            growCell = false;
        }else
        {
            SnakeBody.pop_back(); // removing last cell
            
        }
 
    }
    
    void Reset() // resetting the snake position if the game ends
    {
        // initial snake position 
        SnakeBody = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0}; 
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
        
        Vector2 position = GenerateRandomCell(); // generating random points for food
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
        DrawTexture(texture, offsetvalue + position.x * cellSize, offsetvalue + position.y * cellSize, WHITE); // white is default value which means no color tint/filter is applied


    }

};

// Container for food and snake objects 
class Game // will hold the snake and food class
{
    public:
    Snake snake = Snake();
    Food food = Food(snake.SnakeBody);

    // we use this boolean to end the game if the snake collides with the edges or its body instead.
    bool gameRunning = true; // if the game is running its true and if the game ends it is false.


    // methods 

    void Draw()
    { // food and snake needs to be rendered, it does so by calling these function
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (gameRunning == true) // gameRunning has to be true for the game to run
        {
            snake.Move(); // this function updates/ moves the snake as the user pressed keys
            checkColliosionWithFood(); //  snake when it eats the food
            collisionWithEdges(); // snake when it goes out of the map
            collisioWithBody();
            scoreTracker();
        }
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
            snake.growCell = true; // if there is colliosion with food we add a new segment so the snake grows

        }
    }

    // method check for the collion w the edges 
    void collisionWithEdges()
    {
        // if the head of the snake is equal to the edge coordinate of the map
        // cell count = 25 (not inculusive so 0 - 24)
        // if the x ccoordinat of snake head is 25 or -1, it means that the head hit the either edge of the 
        //screen so we end the game
        if(snake.SnakeBody[0].x == cellCount || snake.SnakeBody[0].x == -1) 
        {
            GameFinished();// TODO: make this method 

        }

        if(snake.SnakeBody[0].y == cellCount || snake.SnakeBody[0].y == -1) 
        {
            GameFinished();
           
        }
    }

    void collisioWithBody()
    {
        // create a copy of the snake body

        deque<Vector2> headCollied = snake.SnakeBody;
        
        // pop the head from the copy and check that that heaad lies in the original
        headCollied.pop_front();

        if (ElementsInDeque(snake.SnakeBody[0], headCollied))
        {
            GameFinished();
        }

    }

    void scoreTracker()
    {
        if (snake.growCell)
        {
            scoreCount ++;
        }
    }

    // game over method
    void GameFinished()
    {
        snake.Reset();
         // need to change the position of the food 
         //// we have to call this function instaed of generatRandomPosition because we dont want the 
         // generated position for food be in top of the existing snake position

        food.position = food.GenerateRandomPosition(snake.SnakeBody); //  takes the deque as the parameter
        gameRunning = false; // if the game ends we update this bool variable
    }


};

int main () {
    InitWindow(2 * offsetvalue + cellCount * cellSize, 2 * offsetvalue + cellSize*cellCount, "Snake");
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
            game.gameRunning = true; // if any arrow is pressed then the game restarts
        }
        // DOWN
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) 
        {
            game.snake.direction = {0,1};
            game.gameRunning = true; // if any arrow is pressed then the game restarts
        }
        //LEFT
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1,0};
            game.gameRunning = true; // if any arrow is pressed then the game restarts
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1,0};
            game.gameRunning = true; // if any arrow is pressed then the game restarts
        }



        // painting the screen green
        ClearBackground(green);
        
        DrawRectangleLinesEx(Rectangle{(float)offsetvalue-5, (float)offsetvalue -5, (float)cellCount*cellSize + 10, (float)cellCount*cellSize+10}, 5, darkGreen); // draws outline of a rectangle. takes rect. thickness (pixels), color
        DrawText("Snake", offsetvalue - 5, 20, 42, darkGreen);// default font takes 5 parameters: text, posX, posY, fontSize, color
        DrawText("Score: ", offsetvalue + 5, 730, 42, darkGreen);
        

        game.Draw();

        


        EndDrawing();
    }

    CloseWindow();

    return 0;
}