## Main Steps to Follow to Make the Snake Game

1. **Creating a Blank Canvas and Game Loop** 
   - This step involves setting up the initial environment for the game, including creating a blank canvas where the game will be displayed and setting up a game loop to handle game logic and rendering.

2. **Creating the Food**
   - **Food Class:**
     - Involves defining a class to represent the food object in the game.
     - The position of the food is determined using a Vector2 struct from the Raylib library, which simplifies handling 2D coordinates.
     - A method is created to draw the food on the screen.

   - **Taking This to the Next Level:**
     - Plans to enhance the food by using an image instead of simple shapes.
     - Describes the process of loading an image file for the food object.
     - After loading the image, it's converted into a texture to render it on the screen.
     - Explains the importance of unloading the image to free up memory.


3. **Creating the Snake (Collection of Cells)**
   - Explains the process of creating the snake's body, which consists of a collection of cells.
   - Discusses using an array to store the positions of the cells, simplifying the representation of the snake's body.
   - Considers using a deque (Double-ended Queue) data structure for more efficient manipulation of the snake's body.
   - Describes using the DrawRectangleRounded function to give the snake's edges a rounded appearance.

  

4. **Moving the Snake**
   - Explains the logic behind moving the snake by adding a cell in front of the snake and removing a cell from the tail.
   - Describes the different movements of the snake (right, up, down) and how they are implemented.

5. **Game Class as Container**
   - Suggests creating a Game class to organize and manage the snake and food objects.
   - Highlights the benefits of encapsulating game elements within a class for better organization and maintainability of the code.

6. **Making the Snake Eat the Food**
   - Describes the process of making the snake consume the food.
   - Explains that when the snake's head position matches the food position, a new set of coordinates is generated for the food.
   - Addresses the issue of the food spawning on top of the snake's body and proposes a solution by checking for collision with the snake's body.

7. **Making the Snake Grow Longer**
   - Details the implementation of making the snake grow longer each time it consumes food.
   - Explains the use of a boolean variable to control whether the snake grows or not after eating food.

8. **Check for Collision**
   - Discusses checking for collision with the edge of the map and the snake's own body to determine game over conditions.
   - Provides the logic for detecting collisions and handling them appropriately.

9. **Adding Title and Frame**
   - Describes adding visual elements to enhance the appearance of the game.
   - Explains the creation of a frame using an offset variable and drawing a rectangle outline.
   - Details adding a title to the game screen using text rendering functions provided by the Raylib library.

10. **Keeping Score**
    - Mentions the addition of a scoring system to track the player's progress in the game.

11. **(Optional) Adding Sounds**
    - Considers the possibility of adding sound effects to enhance the gaming experience.
    - Describes how sound effects could be implemented but leaves it as an optional feature.

