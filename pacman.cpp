//#ifdef __APPLE__
#include <GL/glut.h> // on linux
//#endif
#include <math.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <pthread.h>

#include <png.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include<unistd.h>

//header files
#include "png_load.h"
#include "load_and_bind_texture.h"

#include "textures.h"
#include "maze.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Interface.h"


int gameTick = 0;
int deathTick = 0;
int frightenTick = 0;
int timestamp;
int score = 0;
int pills = 244;
int lives = 3;
int eatenCount = 0;
bool frighten;
float eatenX;
float eatenY;
int eatStamp;
Pacman pacman;
bool paused = false;
bool dead = false;

int fruitCounter = 0;
int tempFruitCounter = 0;
int eatFruitStamp = 0;
int eatenFruitX;
int eatenFruitY;
int fruitSpawned = 0;



pthread_mutex_t lock;

pthread_mutex_t lock1;


pthread_mutex_t D_lock;// display lock 

pthread_mutex_t g_lock;// display lock 

pthread_mutex_t detectpill_lock;  //detect pill lock


/**
 * Decleration of each of the ghosts. Each initially given their starting X and Y coordinates and their colour.
 * They are added to a ghost array to enable it easier to change them all at once as the array can be iterated
 * through.
 */
 
 
Ghost ghosts[4] = 
{
        Ghost(13.5f, 19.0f, RED),
        Ghost(11.5f, 16.0f, BLUE),
        Ghost(13.5f, 16.0f, PINK),
        Ghost(15.5f, 16.0f, YELLOW),
};

typedef enum {BEGIN, PLAY, DIE, OVER} gameState; // Enum defining each of the states the game can be in

gameState stateGame = BEGIN; // Initially start the game in begin mode (duh)

/**
 * Every move pacman checks what tile he is on, if it contains a big pill (O) he then runs the following code to set the ghosts into frighten mode
 * and when they enter frighten mode they reverse their direction
 */
void detectPill()
{
	// lock
	pthread_mutex_lock(&detectpill_lock); 
	
    if(pacman.getTile(pacman.pacmanX, pacman.pacmanY) == O) 
    {
        frighten = true; // Set the flag to know that frigthen mode is on
        frightenTick = 0; // Set the counter to 0
        for(int i = 0; i < 4; i++) {
            if(ghosts[i].moveType == CHASE || ghosts[i].moveType == SCATTER) { // Only change the ghosts mode if they are in CHASE or SCATTER mode
                ghosts[i].eaten = false;
                ghosts[i].moveType = FRIGHTEN;
                if(ghosts[i].currentDir == LEFT) { // Reverse their direction
                    ghosts[i].currentDir = RIGHT;
                } else if(ghosts[i].currentDir == UP) {
                    ghosts[i].currentDir = DOWN;
                } else if(ghosts[i].currentDir == RIGHT) {
                    ghosts[i].currentDir = LEFT;
                } else {
                    ghosts[i].currentDir = UP;
                }
            }
        }
    } 
    
    else if(pacman.getTile(pacman.pacmanX, pacman.pacmanY) == F) 
    {
        eatenFruitX = pacman.pacmanX;
        eatenFruitY = pacman.pacmanY;
    }
    // unlock
    
    pthread_mutex_unlock(&detectpill_lock); 
}

/**
 * Every move pacman checks whether he is on the same tile as a ghost and if so, checks what mode they are in and executes the
 * relevant code
 */
void detectGhost()
{
	pthread_mutex_lock(&g_lock);  
	
    for(int i = 0; i < 4; i++) { // Check pacmans position against all of the ghosts
        if(pacman.getPacX() == ghosts[i].getGhostX() && pacman.getPacY() == ghosts[i].getGhostY()) { // If he is on the same tile as any ghost
            if(ghosts[i].moveType != FRIGHTEN && ghosts[i].moveType != DEATH) { // Check if they are in a mode which is damaging to pacman
                stateGame = DIE; // Execute the DIE gameType as the ghost has caught pacman
                timestamp = gameTick;
            } else {
                if(!ghosts[i].eaten) { // Check the ghost hasnt already been eaten
                    eatenCount++;
                    eatStamp = gameTick;
                    switch(eatenCount){ // Check what eaten number the ghost is in order to increment the score correctly
                        case 1: score += 200;
                            break;
                        case 2: score += 400;
                            break;
                        case 3: score += 800;
                            break;
                        case 4: score += 1600;
                            break;
                    }
                    eatenX = ghosts[i].ghostX; // Store the position of the eating in order to draw the eating score tex in the correct place
                    eatenY = ghosts[i].ghostY;
                    ghosts[i].eaten = true; // Set the flag for that specifc ghost to have been eaten so it cannot be eaten again in this frighten cycle
                }
                ghosts[i].moveType = DEATH; // Set the ghosts mode to DEATh as it has been eaten
            }
        }
    }
    if(stateGame == DIE && !dead) { // Check to see if pacman was indeed caught by the ghost and if so take away a life
        lives--;
        dead = true;
    }
    
    pthread_mutex_unlock(&g_lock);  
}

/**
 * Sets the ghosts movement technique depending on what the gameTick is. Ghosts attack in waves of CHASE and SCATTER
 */
 
void setMode()
{
    if(gameTick >= 4450) 
    {
        for (int i = 0; i < 4; i++) 
        {
            if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            { // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
                ghosts[i].moveType = CHASE; // Set the correct move type for all ghosts
            }
        }
    } 
    
    else if(gameTick >= 4200) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = SCATTER;
            }
        }
    } 
    
    else if(gameTick >= 3200) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = CHASE;               
            }
        }
    } 
    
    else if(gameTick >= 2950) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = SCATTER;
            }
        }
    } 
    
    else if(gameTick >= 1950) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = CHASE;
            }
        }
    } 
    
    else if (gameTick >= 1600) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = SCATTER;
            }
        }
    } 
    
    else if(gameTick >= 600) 
    {
        for(int i = 0; i < 4; i++) 
        {
            if(ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH) 
            {
                ghosts[i].moveType = CHASE;
            }
        }
    }
}

/**
 * Method for all options that need to be checked/ altered at every tick in the game
 */
 
void idle()
{

	pthread_mutex_lock(&lock1);  // Lock before input 
    if(!paused) 
    { // If the game is not paused
        switch (stateGame) 
        { // Check what state the game is in
            case BEGIN:
                if (gameTick >= 250) 
                { // After a certain amount of time switch the game into PLAY mode
                    stateGame = PLAY;
                }
                break;
            case PLAY:
                setMode(); // Set the movement mode for Ghosts
                detectPill(); // Check if pacman has eaten a super pill
                pacman.checkTile(); // Check if pacman has eaten a regular pill to increase score if he is in a portal
                detectGhost(); // Check if pacman has hit a ghost
                pacman.move(); // Move pacman in the maze
                detectGhost();
                for (int i = 0; i < 4; i++) 
                { // Move the ghosts in the maze
                    ghosts[i].move(ghosts[0]);
                }
                
                detectGhost();
                if (frightenTick <= 450 && frighten) 
                { // If frighten mode is on and not expired, increment the counter
                    frightenTick++;
                } 
                
                else if (frighten) 
                { // If frighten mode is on but needs to expire, run correct expirary code
                    frighten = false;
                    eatenCount = 0;
                    frightenTick = 0;
                    
                    for (int i = 0; i < 4; i++) 
                    {
                        if (ghosts[i].moveType == FRIGHTEN) 
                        {
                            ghosts[i].eaten = false;
                        }
                    }
                    
                    setMode(); // Set the ghosts back into the correct mode after frighten mode has finished
                }
                
                if (pills <= 0) 
                { // If all the pills have been eaten reset the level so pacman can continue
                    fruitReset();
                    resetMaze();
                    pacman.reset();
                    pills = 244;
                    fruitSpawned = 0;
                    fruitCounter = 0;
                    tempFruitCounter = 0;
                    
                    for (int i = 0; i < 4; i++) 
                    {
                        ghosts[i].reset();
                    }
                    gameTick = 0;
                    stateGame = BEGIN;
                }
                
                break;
                
            case DIE:
            
                if (gameTick - timestamp >= 150) 
                { // Play pacmans death animation
                    deathTick++;
                }
                
                break;

        }
        
        gameTick++;

        glutPostRedisplay(); // Force a redraw at the end
    }
    
    pthread_mutex_unlock(&lock1);  // Lock before input 
}





void* threadFunction(void* arg) 
{ 
        glutIdleFunc(idle);        
        return NULL;
}





/**
 * Method for handling special keys on the keyboard
 * @param key the key that has been pressed
 */
 
void special(int key, int, int)
{

	pthread_mutex_lock(&lock);  // Lock before input 
    // Handle special keys
    switch (key)
    {
        case GLUT_KEY_LEFT: pacman.setDirStore(LEFT); // If left key press set pacmans next direction to left
            break;
        case GLUT_KEY_UP: pacman.setDirStore(UP); // If up key press set pacmans next direction to up
            break;
        case GLUT_KEY_RIGHT: pacman.setDirStore(RIGHT); // If right key press set pacmans next direction to right
            break;
        case GLUT_KEY_DOWN: pacman.setDirStore(DOWN); // If down key press set pacmans next direction to down
            break;
    }
    
     pthread_mutex_unlock(&lock);  // Unlock the mutex
}

/**
 * Method run at the very beginning to initiale certain aspects of the game
 */
 
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    loadAndBindTextures();
}

/**
 * Method which handles thr drawing of objects to the screen
 */
 
void display()
{
	pthread_mutex_lock(&D_lock);

    glClear (GL_COLOR_BUFFER_BIT);
    switch(stateGame)
    { // Check what state the game is in
        case BEGIN:
            drawMaze();
            pacman.draw();
            
            for(int i = 0; i < 4; i++)
            {            
                ghosts[i].draw();
            }
            
            drawInterface();
            drawReady();
            break;
            
        case PLAY:
            drawMaze();
            pacman.draw();
            
            if(pills <= 174 && fruitSpawned == 0) 
            {
                locationFruit();
            }
            
            if(pills <= 74 && fruitSpawned == 1) 
            {
                locationFruit();
            }
            
            if(fruitCounter > tempFruitCounter) 
            {
                eatFruitStamp = gameTick;
            }
            
            if(gameTick - eatFruitStamp <= 200) 
            {
                eatFruitScore(eatenFruitX, eatenFruitY);
            }
            
            for(int i = 0; i < 4; i++)
            {
                ghosts[i].draw();
                
                if(frighten && gameTick - eatStamp <= 200) 
                {
                    ghosts[i].drawEatScore(eatenX, eatenY);
                }
            }
            
            drawInterface();
            tempFruitCounter = fruitCounter;
            break;
            
        case DIE:
            fruitReset();
            drawMaze();
            
            if(gameTick - timestamp < 150) 
            { // Freeze pacman momentarily after he is hit by a ghost
                pacman.draw();
                
                for(int i = 0; i < 4; i++)
                {
                    ghosts[i].draw();
                }
            }
            
            if(gameTick - timestamp >= 150  && deathTick < 110) 
            { // After that time has passed play pacmans death animation
                pacman.death();
            }
            
            if(gameTick - timestamp >= 310) 
            { // After the death animation, reset pacman
                pacman.reset();
                
                for(int i = 0; i < 4; i++)
                {
                    ghosts[i].reset(); // Reset the ghosts also
                }
                gameTick = 0;
                
                eatFruitStamp = -999999999;
                deathTick = 0;
                
                if(lives > 0) 
                { // If pacman has lives left begin the game again
                    stateGame = BEGIN;
                }
                
                else 
                { // Otherwise show the gameover screen
                    setHighScore(score); // If the game is finished check if the high score needs to be updated
                    stateGame = OVER;
                }
                
            }
            
            drawInterface();

            break;
        case OVER:
            drawMaze();
            drawInterface();
            drawGameOver();
    }

    glutSwapBuffers(); // Force a redraw at the end
    
    pthread_mutex_unlock(&D_lock);
}

/**
 * Method to reset all aspects of the game mechanics if the player wishes to reset a new game once their previous one
 * is over.
 */
 
void resetGame()
{
    gameTick = 0;
    deathTick = 0;
    score = 0;
    lives = 3;
    pills = 244;
    fruitCounter = 0;
    fruitSpawned = 0;
    tempFruitCounter = 0;
    stateGame = BEGIN;
}

/**
 * Method to handle when a keyboard key has been pressed
 * @param key the key that has been pressed
 */
 
void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'q': exit(1); // quit!
        case 'p':// Key to pause the game
        
            if(paused) 
            {
                paused = false;
            }
            
            else 
            {
                paused = true;
            }
            break;
            
        case 'r':
        
            if(stateGame == OVER) 
            {
                resetGame();
                pacman.reset();
                fruitReset();
                resetMaze();
                
                for(int i = 0; i < 4; i++) 
                {
                    ghosts[i].reset(); // Reset the ghosts also
                }
            }
            break;
            
    }

    glutPostRedisplay(); // force a redraw
}

void* threadFunction1(void* arg) 
{
   
       glutSpecialFunc(special);
        
        return NULL;


}

void* threadFunction2(void* arg) 
{
   
     glutKeyboardFunc(keyboard);
    
        
        return NULL;


}

void* DisplayFunction(void* arg) 
{
   
      glutDisplayFunc(display);
    
        
        return NULL;


}



void* ghostThreadsFunctions(void * arg)
{


	detectGhost();
	
	return NULL;


}


int main(int argc, char* argv[])
{

	pthread_t gameThread;
	pthread_t gameThread1;
	pthread_t keyBoardThread;
	pthread_t DisplayThread;
	
	
	pthread_t ghostThreads[4];
	
	
	pthread_mutex_init(&lock, NULL);
	
	pthread_mutex_init(&lock1, NULL);
	
	pthread_mutex_init(&D_lock, NULL);
	
	pthread_mutex_init(&g_lock, NULL);

	pthread_mutex_init(&detectpill_lock, NULL);

    glutInit(&argc, argv); // we can pass certain X windows attributes to GLUT

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

    glutInitWindowSize(512,512);
    glutCreateWindow("Pacman"); // a named window of default size and position
    
    init();
  

   	
    
   	pthread_create(&DisplayThread, NULL, DisplayFunction, NULL);
	pthread_create(&keyBoardThread, NULL, threadFunction2, NULL);
	pthread_create(&gameThread1, NULL, threadFunction1, NULL);
	
	for (int i = 0; i < 4; i++)
	{
		pthread_create(&ghostThreads[i], NULL, ghostThreadsFunctions, (void*)&ghosts[i]);
    }
    
    
    int result = pthread_create(&gameThread, NULL, threadFunction, NULL);
    
    if (result != 0) 
    {
        std::cerr << "Failed to create thread" << std::endl;
        return 1;
    }
    glutMainLoop(); // go into the main loop and wait for window events...
    
    
    
  

    return 0; // safe to return 0 to shell unless error

}
