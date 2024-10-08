//
// Created by teekay on 04/01/17.
//

#ifndef COURSEWORK_GHOST_H
#define COURSEWORK_GHOST_H


extern int gameTick;
extern Pacman pacman;
extern bool frighten;
extern int eatenCount;

typedef enum {SCATTER, CHASE, FRIGHTEN, LEAVE, DEATH, PEN} movement;

typedef enum {RED, BLUE, PINK, YELLOW} colour;
direction tempDir = NONE;

class Ghost {
public:
    float ghostX;
    float ghostY;
    direction currentDir;
    colour ghostColour;
    movement moveType;
    float speed;
    bool eaten;

    Ghost(float x, float y, colour ghost) 
    {
        ghostX = x;
        ghostY = y;
        
        switch (ghost) 
        {
            case RED:
                currentDir = LEFT;
                moveType = SCATTER;
                break;
            case PINK:
                currentDir = DOWN;
                moveType = PEN;
                break;
            case BLUE:
                currentDir = UP;
                moveType = PEN;
                break;
            case YELLOW:
                currentDir = UP;
                moveType = PEN;
                break;
        }/*
        
        if(ghost == RED)
        {
        	currentDir = LEFT;
            moveType = SCATTER;
        }
        
        else if(ghost == PINK)
        {
        	currentDir = DOWN;
            moveType = PEN;
        }
        
        else if(ghost == BLUE)
        {
        	currentDir = UP;
            moveType = PEN;
        }
        
        else
        {
        	currentDir = UP;
            moveType = PEN;
        }*/
        
        eaten = false;
        ghostColour = ghost;
        setSpeed();
    }

    /**
     * Draws the score added to the game when pacman eats the ghost. Thie is drawn at the ghosts x and y position at the time of eating
     * and the score also depends on how many ghosts, if any, had been eaten before it
     * @param x the ghosts X position at the time of eating
     * @param y the ghosts y position at the time of eating
     */
    void drawEatScore(float x, float y)
    {
        glPushMatrix();

        unsigned int eat_score_tex;

        switch(eatenCount) 
        { 
        	// Pick which texture to draw depending on which number ghost is beign eaten
        	
            case 1: 
            	eat_score_tex = score_200_tex;
                break;
            case 2: 
            	eat_score_tex = score_400_tex;
                break;
            case 3: 
            	eat_score_tex = score_800_tex;
                break;
            case 4: 
            	eat_score_tex = score_1600_tex;
                break;
                
        }/*
        
        if(eatenCount == 1)
        {
        	eat_score_tex = score_200_tex;
        }
        
        else if(eatenCount == 2)
        {
        	eat_score_tex = score_400_tex;
        }
        
        else if(eatenCount == 3)
        {
        	eat_score_tex = score_800_tex;
        }
        
        else
        {
        	eat_score_tex = score_1600_tex;
        }*/

        translateBottomLeft();
        translateToMazeCoords(x, y); // Translate to correct part of the screen for drawing
        glTranslatef(-2.0f, 0.0f, 0.0f);

        if(eatenCount != 0) 
        { 
        	// Only draw if a ghost has been eaten
            drawTex(eat_score_tex, 10, 8, 0);
        }

        glPopMatrix();
    }

    /**
     * Method to draw the ghost with the correct texture and the eyes facing the correct direction.
     */
    void draw() 
    {
        glPushMatrix();

        unsigned int ghost_tex;
        unsigned int ghost_eye_tex;

        if (moveType != FRIGHTEN && moveType != DEATH) { // As long as the ghost is not in death or frighten mode, assign its relevant texture
            int tickDiv = gameTick % 20;
            if (tickDiv % 20 < 10) 
            {
                switch (ghostColour) 
                { 
                	// Check ghost colour in order to determine which tex file to use
                    case RED:
                        ghost_tex = ghost_r_0_tex;
                        break;
                    case BLUE:
                        ghost_tex = ghost_b_0_tex;
                        break;
                    case PINK:
                        ghost_tex = ghost_p_0_tex;
                        break;
                    case YELLOW:
                        ghost_tex = ghost_y_0_tex;
                        break;
                }/*
                
                if (ghostColour == RED) 
                {
					ghost_tex = ghost_r_0_tex;
				} 
				else if (ghostColour == BLUE) 
				{
					ghost_tex = ghost_b_0_tex;
				} 
				else if (ghostColour == PINK) 
				{
					ghost_tex = ghost_p_0_tex;
				} 
				else if (ghostColour == YELLOW) 
				{
					ghost_tex = ghost_y_0_tex;
				}*/

            } 
            else 
            {
                /*switch (ghostColour) 
                {
                    case RED:
                        ghost_tex = ghost_r_1_tex;
                        break;
                    case BLUE:
                        ghost_tex = ghost_b_1_tex;
                        break;
                    case PINK:
                        ghost_tex = ghost_p_1_tex;
                        break;
                    case YELLOW:
                        ghost_tex = ghost_y_1_tex;
                        break;
                }*/
                
                if (ghostColour == RED) 
                {
					ghost_tex = ghost_r_1_tex;
				} 
				else if (ghostColour == BLUE) 
				{
					ghost_tex = ghost_b_1_tex;
				} 
				else if (ghostColour == PINK) 
				{
					ghost_tex = ghost_p_1_tex;
				} 
				else if (ghostColour == YELLOW) 
				{
					ghost_tex = ghost_y_1_tex;
				}

            }

            switch (currentDir) 
            { 
            	// Eye position/tex file is dependent on direction the ghost is moving
                case LEFT:
                    ghost_eye_tex = eye_left_tex;
                    break;
                case UP:
                    ghost_eye_tex = eye_up_tex;
                    break;
                case RIGHT:
                    ghost_eye_tex = eye_right_tex;
                    break;
                case DOWN:
                    ghost_eye_tex = eye_down_tex;
                    break;
            }/*
            
            if (currentDir == LEFT) 
            {
				ghost_eye_tex = eye_left_tex;
			} 
			else if (currentDir == UP) 
			{
				ghost_eye_tex = eye_up_tex;
			} 
			else if (currentDir == RIGHT) 
			{
				ghost_eye_tex = eye_right_tex;
			} 
			else if (currentDir == DOWN) 
			{
				ghost_eye_tex = eye_down_tex;
			}*/

        } 
        else if(moveType == FRIGHTEN)
        { 
        // If the ghost is frightened assign its relevant texture
            int tickDiv = gameTick % 40;
            if (tickDiv % 40 < 10) 
            {
                ghost_tex = ghost_scared_0_tex;
            } 
            else if (tickDiv % 40 < 20) 
            {
                ghost_tex = ghost_scared_1_tex;
            } 
            else if (tickDiv % 40 < 30) 
            {
                ghost_tex = ghost_scared_2_tex;
            } 
            else 
            {
                ghost_tex = ghost_scared_3_tex;
            }
        } 
        else  if(moveType == DEATH) 
        {
            switch (currentDir) 
            { // Eye position/tex file is dependent on direction the ghost is moving
                case LEFT:
                    ghost_eye_tex = eye_left_tex;
                    break;
                case UP:
                    ghost_eye_tex = eye_up_tex;
                    break;
                case RIGHT:
                    ghost_eye_tex = eye_right_tex;
                    break;
                case DOWN:
                    ghost_eye_tex = eye_down_tex;
                    break;
            }/*
            
            if (currentDir == LEFT) 
            {
				ghost_eye_tex = eye_left_tex;
			} 
			else if (currentDir == UP) 
			{
				ghost_eye_tex = eye_up_tex;
			} 
			else if (currentDir == RIGHT) 
			{
				ghost_eye_tex = eye_right_tex;
			} else if (currentDir == DOWN) 
			{
				ghost_eye_tex = eye_down_tex;
			}*/

        }


        translateBottomLeft(); // Translate to bottom left of screen
        translateToMazeCoords(ghostX, ghostY);

        glTranslatef(-3.0f, -3.0f, 0.0f);

        if(moveType != DEATH) { // If the ghost is not dead then draw its body texture
            drawTex(ghost_tex, 14, 14, 0);
        }

        if (moveType != FRIGHTEN) { // If the ghost is not frightened then draw the eye texture
            drawTex(ghost_eye_tex, 14, 14, 0);
        }

        glPopMatrix();

    }

    /**
     * Method to reset the ghosts to their initial settings/ positions when the game is restarted
     */
    void reset() 
    {
        switch (ghostColour) { // Different settings depending on the colour of the ghost
            case RED:
                ghostX = 13.5f;
                ghostY = 19.0f;
                currentDir = LEFT;
                moveType = SCATTER;
                break;
            case BLUE:
                ghostX = 11.5f;
                ghostY = 16.0f;
                currentDir = UP;
                if (pills <= 163) {
                    moveType = LEAVE;
                } else {
                    moveType = PEN;
                }
                break;
            case YELLOW:
                ghostX = 15.5f;
                ghostY = 16.0f;
                currentDir = UP;
                if (pills <= 214) {
                    moveType = LEAVE;
                } else {
                    moveType = PEN;
                }
                break;
            case PINK:
                ghostX = 13.5f;
                ghostY = 16.0f;
                currentDir = DOWN;
                moveType = LEAVE;
                break;
        }/*
        
        if (ghostColour == RED) 
        {
			ghostX = 13.5f;
			ghostY = 19.0f;
			currentDir = LEFT;
			moveType = SCATTER;
		} 
		
		else if (ghostColour == BLUE) 
		{
			ghostX = 11.5f;
			ghostY = 16.0f;
			currentDir = UP;
			if (pills <= 170) 
			{
				moveType = LEAVE;
			} 
			
			else 
			{
				moveType = PEN;
			}
		} 
		
		else if (ghostColour == YELLOW) 
		{
			ghostX = 15.5f;
			ghostY = 16.0f;
			currentDir = UP;
			if (pills <= 214) 
			{
				moveType = LEAVE;
			} 
			
			else 
			{
				moveType = PEN;
			}
		} 
		
		else if (ghostColour == PINK) 
		{
			ghostX = 13.5f;
			ghostY = 16.0f;
			currentDir = DOWN;
			moveType = LEAVE;
		}*/

        eaten = false; // Upon reset no ghost has been eaten
    }

    /**
     * Method determines what mode ghosts are in, and sets the speed accordingly
     */
    void setSpeed() {
        if (moveType == FRIGHTEN) 
        { 
        	// Move slower if frightened
            speed = 0.05f;
        } 
        else 
        {
            speed = 0.1f;
        }
    }

    /**
     * Method determines the colour of the ghost and assigns the relevant movement technique to each ghost
     * ghost Variable used if a ghost needs to potentially use another ghosts position in its movement tehcnique
     */
    void move(Ghost ghost) 
    {
        setSpeed();
        
        switch (ghostColour) {
            case RED:
                redGhostMove(ghost);
                break;
            case BLUE:
                blueGhostMove(ghost);
                break;
            case PINK:
                pinkGhostMove(ghost);
                break;
            case YELLOW:
                yellowGhostMove(ghost);
                break;
        }/*
        
        if (ghostColour == RED) 
        {
			redGhostMove(ghost);
		} 
		
		else if (ghostColour == BLUE) 
		{
			blueGhostMove(ghost);
		} 
		
		else if (ghostColour == PINK) 
		{
			pinkGhostMove(ghost);
		} 
		
		else if (ghostColour == YELLOW) 
		{
			yellowGhostMove(ghost);
		}*/

    }

    /**
     * Returns the ghosts current X position
     *  ghosts X position
     */
    int getGhostX() 
    {
        return (int) floor(ghostX);
    }

    /**
     * Returns the ghosts current X position
     * ghosts Y position
     */
    int getGhostY() 
    {
        return (int) floor(ghostY);
    }

    /**
     * Returns the number of possible exits from the specified position
     * x the X position to check
     * y the Y position to check
     * the number of exits from the current position that arent walls
     */
    int checkJuncs(float x, float y) 
    {
        int exit = 0;
        if (!isWall(LEFT)) 
        { 
        	// Check all directions and if its not a wall in that direction, increment the counter
            exit++;
        }
        
        if (!isWall(UP)) 
        {
            exit++;
        }
        
        if (!isWall(RIGHT)) 
        {
            exit++;
        }
        
        if (!isWall(DOWN)) 
        {
            exit++;
        }

        return exit;
    }

    /**
     * Returns the tile type at the specified coordinates
     * x X position of the tile to check
     * y Y position of the tile to check
     * The type of the tile at the specified coordinates
     */
    tile getTile(float x, float y) {
        return maze[(int) floor(x)][(int) floor(y)];
    }

    /**
     * Looks ahead one tile in the specified direction and returns that type of tile
     * dir The direction to look ahead in
     * The type of the tile at the specified position
     */
    tile getFollowingTile(direction dir) 
    {
        switch (dir) 
        {
            case LEFT:
                return getTile(getGhostX() - 1, getGhostY());
            case UP:
                return getTile(getGhostX(), getGhostY() + 1);
            case RIGHT:
                return getTile(getGhostX() + 1, getGhostY());
            case DOWN:
                return getTile(getGhostX(), getGhostY() - 1);
            default:
                return getTile(getGhostX(), getGhostY());
        }/*
        
        if (dir == LEFT) 
        {
			return getTile(getGhostX() - 1, getGhostY());
		} 
		
		else if (dir == UP) 
		{
			return getTile(getGhostX(), getGhostY() + 1);
		} 
		
		else if (dir == RIGHT) 
		{
			return getTile(getGhostX() + 1, getGhostY());
		} 
		
		else if (dir == DOWN) 
		{
			return getTile(getGhostX(), getGhostY() - 1);
		} 
		
		else 
		{
			return getTile(getGhostX(), getGhostY());
		}*/

    }

    /**
     * Returns a boolean on whether the ghost is currently at the centre of its tile
     * Whether it is at the centre of the tile
     */
    bool isAtCenter() 
    {
        return (int) (ghostY * 10.0f) % 10 == 0 && (int) (ghostX * 10.0f) % 10 == 0;
    }

    /**
     *
     * x the given x position of a ghost
     * y the given y position of a ghost
     * ghost Col the ghost colour of the current ghost
     * ghost another ghost object as some ghosts need to use other ghosts in order to target their movement
     * returns the distance from the point given to the target tile
     */
    float getDistance(int x, int y, colour ghostCol, Ghost ghost) 
    {
        std::vector<int> v = getTargetTile(ghostCol);
        int targetX = v.at(0);
        int targetY = v.at(1);

        if (ghostCol == BLUE && moveType == CHASE) {
            float tempX = ghost.getGhostX() - targetX;
            float tempY = ghost.getGhostY() - targetY;
            targetX = ghost.getGhostX() + 2 * tempX;
            targetY = ghost.getGhostY() + 2 * tempY;
        }

        return (sqrt(((targetX - x) * (targetX - x)) + ((targetY - y) * (targetY - y))));
    }

    /**
     * Method that handles when a ghost reaches a corner, which way it should turn
     */
    void cornerHandler() 
    {
        if (isAtCenter()) { // Checks it is at the centre of its tile
            if (isWall(currentDir)) { // Checks whether if it were to keep moving in it current direction if it would hit a wall
                if (!isWall(LEFT) && currentDir != RIGHT) 
                {
                 	// Checks its new direction wouldnt make it hit a wall AND that its not reversing direction
                    currentDir = LEFT;
                } 
                
                else if (!isWall(UP) && currentDir != DOWN) 
                {
                    currentDir = UP;
                } 
                
                else if (!isWall(RIGHT) && currentDir != LEFT) 
                {
                    currentDir = RIGHT;
                } 
                
                else 
                {
                    currentDir = DOWN;
                }
            }
        }
    }

    /**
     * Used to move ghosts back into the pen for the final move during the DEATH moveType.
     * Due to the fact that they see the ghost gate as a wall so the code moves them down until their X and Y coordinates satisfy
     * certain conditions and then their moveType is changed back to CHASE or SCATTER depending on the gameTick.
     */
    void moveToPen()
    {
        if (ghostX <= 13.55 && ghostX >= 13.45) 
        { 
        	// Initially check they have the correct X coordinates
            if (ghostY >= 18.95f) { // If so move them up and out of the gate
                currentDir = DOWN;
            } 
            
            else if (ghostY <= 17.0f)
            {
                moveType = LEAVE;
            }
        } 
        
        else 
        { 
        	// If they dont have the correct x coordinates
            if (ghostX < 13.5f) 
            {
            	// Determine if they need to move left or right
                currentDir = RIGHT;
            } 
            
            else if (ghostX > 13.5f) 
            {
                currentDir = LEFT;
            }
        }
        // As on the final call to this method the moveType is changed they could still move 0.1f in a direciton when not needed, so necessary check for moveType
        switch (currentDir) 
        {
            case LEFT:
                ghostX -= speed;
                break;
            case UP:
                ghostY += speed;
                break;
            case RIGHT:
                ghostX += speed;
                break;
            case DOWN:
                ghostY -= speed;
                break;
        }/*
        
        if (currentDir == LEFT) 
        {
			ghostX -= speed;
		} 
		
		else if (currentDir == UP) 
		{
			ghostY += speed;
		} 
		
		else if (currentDir == RIGHT) 
		{
			ghostX += speed;
		} 
		
		else if (currentDir == DOWN) 
		{
			ghostY -= speed;
		}*/

    }

    /**
     * Sets the ghosts movement, dependent on colour, to take a route back to the ghost pen so that they can respawn and join the game again
     *  colGhost The specified ghost colour
     *  ghost Pass in a ghost object in case the chosen move technique must use another ghost to calculate where to move
     */
    void setDeath(colour colGhost, Ghost ghost)
    {
        float distance = 999.0f;
        bool move = true;
        if(ghostX <= 13.55 && ghostX >= 13.45 && ghostY >= 15.5f && ghostY <= 20.0f) {
            moveToPen();
        } else {
            if (checkJuncs(ghostX, ghostY) >= 3 && isAtCenter()) {
                if (currentDir != DOWN && !isOnlyWall(UP)) {
                    float d = getDistance(getGhostX(), getGhostY() + 1, colGhost, ghost);
                    if (d < distance) {
                        tempDir = UP;
                        distance = d;
                    }
                }
                if (currentDir != LEFT && !isOnlyWall(RIGHT)) {
                    float d = getDistance(getGhostX() + 1, getGhostY(), colGhost, ghost);
                    if (d < distance) {
                        tempDir = RIGHT;
                        distance = d;
                    }
                }
                if (currentDir != RIGHT && !isOnlyWall(LEFT)) {
                    float d = getDistance(getGhostX() - 1, getGhostY(), colGhost, ghost);
                    if (d < distance) {
                        tempDir = LEFT;
                        distance = d;
                    }
                }
                if (currentDir != UP && !isOnlyWall(DOWN)) {
                    float d = getDistance(getGhostX(), getGhostY() - 1, colGhost, ghost);
                    if (d < distance) {
                        tempDir = DOWN;
                        distance = d;
                    }
                }
                currentDir = tempDir;
            } else {
                cornerHandler();
            }

			if (move) 
			{
				if (currentDir == LEFT) 
				{
					ghostX -= speed;
					ghostY = round(ghostY);
				} 
				
				else if (currentDir == UP) 
				{
					ghostY += speed;
					ghostX = round(ghostX);
				} 
				
				else if (currentDir == RIGHT) 
				{
					ghostX += speed;
					ghostY = round(ghostY);
				} 
				
				else if (currentDir == DOWN) 
				{
					ghostY -= speed;
					ghostX = round(ghostX);
				}
			}

        }


    }

    /**
     * Determines which direction to move by checking all potential moves around itself and comparing how close each
     * potential move will put it to the target tile. It then selects the move which puts it the closest to the
     * target tile
     *  colGhost The colour of the current ghost
     *  ghost Pass in a ghost object in case the chosen move technique must use another ghost to calculate where to move
     */
    void moveTargetTile(colour colGhost, Ghost ghost)
    {
        float distance = 999.0f;
        if (checkJuncs(ghostX, ghostY) >= 3 && isAtCenter()) 
        {
            if (currentDir != DOWN && !isWall(UP)) 
            {
                float d = getDistance(getGhostX(), getGhostY()+1, colGhost, ghost);
                if(d < distance)
                {
                    tempDir = UP;
                    distance = d;
                }
            }
            
            else if (currentDir != LEFT && !isWall(RIGHT)) 
            {
                float d = getDistance(getGhostX()+1, getGhostY(), colGhost, ghost);
                if(d < distance)
                {
                    tempDir = RIGHT;
                    distance = d;
                }
            }
            
            else if (currentDir != RIGHT && !isWall(LEFT)) 
            {
                float d = getDistance(getGhostX()-1, getGhostY(), colGhost, ghost);
                if(d < distance)
                {
                    tempDir = LEFT;
                    distance = d;
                }
            }
            else if (currentDir != UP && !isWall(DOWN)) 
            {
                float d = getDistance(getGhostX(), getGhostY()-1, colGhost, ghost);
                if(d < distance)
                {
                    tempDir = DOWN;
                    distance = d;
                }
            }
            currentDir = tempDir;
        } 
        
        else 
        {
            cornerHandler();
        }

        
        if (currentDir == LEFT) 
        {
			ghostX -= speed;
			ghostY = round(ghostY);
		} 
		
		else if (currentDir == UP) 
		{
			ghostY += speed;
			ghostX = round(ghostX);
		} 
		
		else if (currentDir == RIGHT) 
		{
			ghostX += speed;
			ghostY = round(ghostY);
		} 
		
		else if (currentDir == DOWN) 
		{
			ghostY -= speed;
			ghostX = round(ghostX);
		}

    }

    /**
     * Sets the ghosts movement during FRIGHTEN. When initially called the ghost reverses direction and then at every junction
     * the ghost will pick a random direction to move in aslong as that chosen direction does not cause it to hit a wall or reverse.
     * It then calls its relevant corner handler to handle corners correctly.
     */
    void setFrighten()
    {
        direction chosenDir = NONE;
        if (checkJuncs(ghostX, ghostY) >= 3) 
        { // If at a junction
            do 
            {
                int randNum = rand() % checkJuncs(ghostX, ghostY) - 1; // Select a random number between 0 and 3
                if(randNum == 0 && currentDir != RIGHT) 
                {
                    chosenDir = LEFT;
                } 
                
                else if(randNum == 1 && currentDir != DOWN) 
                {
                    chosenDir = UP;
                } 
                
                else if(randNum == 2 && currentDir != LEFT) 
                {
                    chosenDir = RIGHT;
                } 
                
                else if(currentDir != UP)
                {
                    chosenDir = DOWN;
                }
            } 
            
            while (isWall(chosenDir) && currentDir != NONE); // Only select the direction if it is not in a wall and the ghost is currently moving
        } 
        
        else 
        {
            cornerHandler();
        }

        if(isAtCenter() && chosenDir != NONE) 
        {
            if(!isWall(chosenDir))
            {
                currentDir = chosenDir; // Only apply chosen direction if the ghost is at its centre, the chosen direction is NONE and the chosen direciton doesnt take it into a wal
            }
        }

        
        if (currentDir == LEFT) 
        {
			ghostX -= speed;
			ghostY = round(ghostY);
		} 
		
		else if (currentDir == UP) 
		{
			ghostY += speed;
			ghostX = round(ghostX);
		} 
		
		else if (currentDir == RIGHT) 
		{
			ghostX += speed;
			ghostY = round(ghostY);
		} 
		
		else if (currentDir == DOWN) 
		{
			ghostY -= speed;
			ghostX = round(ghostX);
		}

    }

    /**
     * Determines what movement technique each ghost should take depending on its colour
     *  col the colour of the ghost
     *  ghost pass in a ghost object in case the chosen move technique must use another ghost to calculate where to move
     */
    void setMove(colour col, Ghost ghost)
    {
        float distance = getDistance(ghostX, ghostY, RED, ghost); // Get the distance between the ghost and pacman
        
        
        if (col == RED) 
        {
			moveTargetTile(RED, ghost);
		} 
				
		else if (col == PINK) 
		{
			moveTargetTile(RED, ghost);
		} 
		
		else if (col == YELLOW) 
		{
			if (distance >= 8.0f) 
			{
				moveTargetTile(RED, ghost);
			} 
			
			else 
			{
				moveTargetTile(YELLOW, ghost);
			}
		}
		else if (col == BLUE) 
		{
			moveTargetTile(BLUE, ghost);
		}

    }

    /**
     * Calls the correct method for movement depending on what the current MoveType of the ghost is.
     *  ghost used for certain movement techniques for certain ghosts
     */
    void redGhostMove(Ghost ghost)
    {
        
        
        
        if (moveType == SCATTER) 
        {
			portalCheck();
			setMove(RED, ghost);
		} 
		
		else if (moveType == CHASE) 
		{
			portalCheck();
			setMove(RED, ghost);
		} 
		
		else if (moveType == FRIGHTEN) 
		{
			portalCheck();
			setFrighten();
		} 
		
		else if (moveType == DEATH) 
		{
			setDeath(RED, ghost);
		} 
		
		else if (moveType == LEAVE) 
		{
			leaveMove();
		}

    }

    /**
     * Calls the correct method for movement depending on what the current MoveType of the ghost is.
     * ghost used for certain movement techniques for certain ghosts
     */
    void blueGhostMove(Ghost ghost)
    {
        if(pills == 200)
        {
            moveType = LEAVE;
        }
        
        
        if (moveType == SCATTER) 
        {
			portalCheck();
			setMove(BLUE, ghost);
		} 
		
		else if (moveType == CHASE) 
		{
			portalCheck();
			setMove(BLUE, ghost);
		} 
		
		else if (moveType == FRIGHTEN) 
		{
			portalCheck();
			setFrighten();
		} 
		
		else if (moveType == DEATH) 
		{
			setDeath(BLUE, ghost);
		} 
		
		else if (moveType == LEAVE) 
		{
			leaveMove();
		} 
		
		else if (moveType == PEN) {
			penMove();
		}

    }

    /**
     * Calls the correct method for movement depending on what the current MoveType of the ghost is.
     *  ghost used for certain movement techniques for certain ghosts
     */
    void yellowGhostMove(Ghost ghost)
    {
        if(pills == 214){
            moveType = LEAVE;
        }
        
        
        if (moveType == SCATTER) 
        {
			portalCheck();
			setMove(YELLOW, ghost);
		} 
		
		else if (moveType == CHASE) 
		{
			portalCheck();
			setMove(YELLOW, ghost);
		} 
		
		else if (moveType == FRIGHTEN) 
		{
			portalCheck();
			setFrighten();
		} 
		
		else if (moveType == DEATH) 
		{
			setDeath(YELLOW, ghost);
		} 
		
		else if (moveType == LEAVE) 
		{
			leaveMove();
		} 
		
		else if (moveType == PEN) 
		{
			penMove();
		}

    }

    /**
     * Calls the correct method for movement depending on what the current MoveType of the ghost is.
     * ghost used for certain movement techniques for certain ghosts
     */
    void pinkGhostMove(Ghost ghost)
    {
        
        if (moveType == SCATTER) 
        {
			portalCheck();
			setMove(PINK, ghost);
		} 
		
		else if (moveType == CHASE) 
		{
			portalCheck();
			setMove(PINK, ghost);
		} 
		
		else if (moveType == FRIGHTEN) 
		{
			portalCheck();
			setFrighten();
		} 
		
		else if (moveType == DEATH) 
		{
			setDeath(PINK, ghost);
		} 
		
		else if (moveType == LEAVE) 
		{
			leaveMove();
		} 
		
		else if (moveType == PEN) 
		{
			penMove();
		}

    }

    /**
     * Boolean function which returns if the next tile in the ghosts direction is a wall
     *  dir the ghosts current direction
     *  boolean value which is true if it is a wall in front of the ghost
     */
    bool isWall(direction dir)
    {
        if (getFollowingTile(dir) == W || getFollowingTile(dir) == G) 
        {
            return true;
        }

        return false;
    }

    /**
     * Boolean function which returns if the next tile in the ghosts direction is a wall (not including ghost gate)
     * @param dir the ghosts current direction
     * @return boolean value which is true if it is a wall in front of the ghost
     */
    bool isOnlyWall(direction dir)
    {
        if (getFollowingTile(dir) == W) 
        {
            return true;
        }

        return false;
    }

    /**
     * Moves the ghosts up and down inside the ghost pen until they are told to leave
     */
    void penMove()
    {
        if(ghostY <= 15.5f && currentDir == DOWN) 
        {
            if(ghostColour == PINK && moveType == PEN) 
            {
                moveType = LEAVE;
            } 
            
            else 
            {
                currentDir = UP;
            }
        } 
        
        else if(ghostY >= 16.5f && currentDir == UP) 
        {
            currentDir = DOWN;
        }

        
        if (currentDir == UP) 
        {
			ghostY += speed;
		} 
		
		else if (currentDir == DOWN) 
		{
			ghostY -= speed;
		}

    }

    /**
     * Checks whether the ghost is currently on a portal tile. If they are, translate them to the other side of the maze respectively
     */
    void portalCheck()
    {
        
        
        if (getTile(ghostX, ghostY) == P) 
        {
			if (getGhostX() >= 26.5) 
			{
				ghostX = 1.0f;
			} 
			
			else 
			{
				ghostX = 26.5;
			}
		}

    }

    /**
     * Method that allows the ghosts to move from the ghost pen and enter the maze, no matter where in the pen they are
     */
    void leaveMove()
    {
        if (ghostX <= 13.55 && ghostX >= 13.45) 
        { 
        	// Initially check they have the correct X coordinates
            if (ghostY < 19.0f) 
            { 
            	// If so move them up and out of the gate
                currentDir = UP;
            }
        } 
        
        else 
        { // If they dont have the correct x coordinates
            if (ghostX < 13.5f) 
            { 
            	// Determine if they need to move left or right
                currentDir = RIGHT;
            } 
            
            else if (ghostX > 13.5f) 
            {
                currentDir = LEFT;
            }
        }

        if(ghostY >= 19.0f) 
        {
            currentDir = LEFT;
            
            if(isAtCenter())
            {
                moveType = SCATTER;
            }
        }

        if(moveType == LEAVE) 
        { 
        	// As on the final call to this method the moveType is changed they could still move 0.1f in a direciton when not needed, so necessary check for moveType
            
            
            if (currentDir == LEFT) 
            {
				ghostX -= speed;
			} 
			
			else if (currentDir == UP) 
			{
				ghostY += speed;
			} 
			
			else if (currentDir == RIGHT) 
			{
				ghostX += speed;
			} 
			
			else if (currentDir == DOWN) 
			{
				ghostY -= speed;
			}

        }
    }

    /**
     * Creates the vectors for the relevant target tiles depending on which mode the ghost is in and also what colour the ghost is
     *  ghostCol the colour of the ghost
     *  vector containing the x and y coordinate of the target tile
     */
    std::vector<int> getTargetTile(colour ghostCol)
    {
        std::vector<int> v;
        
        if (moveType == CHASE) 
        {
			if (ghostCol == RED) 
			{
				v.push_back(pacman.getPacX());
				v.push_back(pacman.getPacY());
			} 
			
			else if (ghostCol == BLUE) 
			{
				if (pacman.currentDir == LEFT) 
				{
					v.push_back(pacman.getPacX() - 2);
					v.push_back(pacman.getPacY());
				} 
				
				else if (pacman.currentDir == UP) 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY() + 2);
				} 
				
				else if (pacman.currentDir == RIGHT) 
				{
					v.push_back(pacman.getPacX() + 2);
					v.push_back(pacman.getPacY());
				} 
				
				else if (pacman.currentDir == DOWN) 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY() - 2);
				} 
				
				else 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY());
				}


			} 
			
			else if (ghostCol == YELLOW) 
			{
				v.push_back(0);
				v.push_back(0);
			} 
			
			else if (ghostCol == PINK) 
			{
				if (pacman.currentDir == LEFT) 
				{
					v.push_back(pacman.getPacX() - 4);
					v.push_back(pacman.getPacY());
				} 
				
				else if (pacman.currentDir == UP) 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY() + 4);
				} 
				
				else if (pacman.currentDir == RIGHT) 
				{
					v.push_back(pacman.getPacX() + 4);
					v.push_back(pacman.getPacY());
				} 
				
				else if (pacman.currentDir == DOWN) 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY() - 4);
				} 
				
				else 
				{
					v.push_back(pacman.getPacX());
					v.push_back(pacman.getPacY());
				}

			}
		} 
		
		else if (moveType == SCATTER) 
		{
			if (ghostColour == RED) 
			{
				v.push_back(100);
				v.push_back(100);
			} 
			
			else if (ghostColour == BLUE) 
			{
				v.push_back(20);
				v.push_back(0);
			} 
			
			else if (ghostColour == YELLOW) 
			{
				v.push_back(0);
				v.push_back(0);
			} 
			
			else if (ghostColour == PINK) 
			{
				v.push_back(0);
				v.push_back(100);
			}

		} 
		
		else if (moveType == DEATH) 
		{
			v.push_back(13.5);
			v.push_back(19);
		}


        return v;
    }

};

#endif 
