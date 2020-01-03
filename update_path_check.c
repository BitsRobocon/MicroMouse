#include<stdio.h>

void print_maze();
void update_maze();
void initialize_maze();

int MAZE_SIZE = 8;

int goal_maze[8][8];
int wall_maze[8][8][4]; // up,left,right,down

int GOAL_ROW = 3; // definition of the goal where the mouse wants to go to
int GOAL_COLUMN = 3;

int main()
{
    initialize_maze();

    wall_maze[3][3][0] = 1;
    wall_maze[2][3][3] = 1;

    update_maze();
    print_maze();

    return 0;
}
void print_maze()
{
    for(int i = 0; i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            if(wall_maze[i][j][0]==1)
            {
                Serial.println("   --   ");
            }
            else
            {
                Serial.println("        ");
            }
        }
        Serial.println("\n");
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            if(wall_maze[i][j][1]==1)
            {
                Serial.println(" | ");
            }
            else
            {
                Serial.println("   ");
            }
            if(goal_maze[i][j]<10)
            {
                Serial.println(" %d", goal_maze[i][j]);
            }
            else
            {
                Serial.println("%d", goal_maze[i][j]);
            }
            if(wall_maze[i][j][2]==1)
            {
                Serial.println(" | ");
            }
            else
            {
                Serial.println("   ");
            }
        }
        Serial.println("\n");
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            if(wall_maze[i][j][3]==1)
            {
                Serial.println("   --   ");
            }
            else
            {
                Serial.println("        ");
            }
        }
        Serial.println("\n");
    }
}
void initialize_maze()
{
    for(int i = 0; i < (GOAL_ROW); i++)
    {
        for(int j = 0; j < (GOAL_COLUMN); j++)
        {
            goal_maze[i][j] = ((GOAL_ROW+GOAL_COLUMN)-(i + j));

            if(i == 0)
            {
                wall_maze[i][j][0] = 1; // for top boundary
            }

            if(i == (MAZE_SIZE-1))
            {
                wall_maze[i][j][3] = 1; // for bottom boundary
            }

            if(j == 0)
            {
                wall_maze[i][j][1] = 1; // for left side boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                wall_maze[i][j][2] = 1; // for right side boundary
            }
        }
    }

    for(int i = 0; i < (GOAL_ROW); i++)
    {
        for(int j = (GOAL_COLUMN); j < MAZE_SIZE; j++)
        {
            goal_maze[i][j] = ((GOAL_ROW+GOAL_COLUMN)-((i+1) + (((GOAL_ROW+GOAL_COLUMN)-(j+1)))));

            if(i == 0)
            {
                wall_maze[i][j][0] = 1; // for top boundary
            }

            if(i == (MAZE_SIZE-1))
            {
                wall_maze[i][j][3] = 1; // for bottom boundary
            }

            if(j == 0)
            {
                wall_maze[i][j][1] = 1; // for left side boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                wall_maze[i][j][2] = 1; // for right side boundary
            }
        }
    }

    for(int i = (GOAL_ROW); i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < (GOAL_COLUMN); j++)
        {
            goal_maze[i][j] = ((GOAL_ROW+GOAL_COLUMN)-((((GOAL_ROW+GOAL_COLUMN)-(i+1))) + (j+1)));

            if(i == 0)
            {
                wall_maze[i][j][0] = 1; // for top boundary
            }

            if(i == (MAZE_SIZE-1))
            {
                wall_maze[i][j][3] = 1; // for bottom boundary
            }

            if(j == 0)
            {
                wall_maze[i][j][1] = 1; // for left side boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                wall_maze[i][j][2] = 1; // for right side boundary
            }
        }
    }

    for(int i = (GOAL_ROW); i < MAZE_SIZE; i++)
    {
        for(int j = (GOAL_COLUMN); j < MAZE_SIZE; j++)
        {
            goal_maze[i][j] = ((GOAL_ROW+GOAL_COLUMN)-((((GOAL_ROW+GOAL_COLUMN)-(i+1))+1) + (((GOAL_ROW+GOAL_COLUMN)-(j+1))+1)));

            if(i == 0)
            {
                wall_maze[i][j][0] = 1; // for top boundary
            }

            if(i == (MAZE_SIZE-1))
            {
                wall_maze[i][j][3] = 1; // for bottom boundary
            }

            if(j == 0)
            {
                wall_maze[i][j][1] = 1; // for left side boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                wall_maze[i][j][2] = 1; // for right side boundary
            }
        }
    }
}
void update_maze()
{
    int temp = 0;
    for(int i = 0; i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            if(!((i==GOAL_ROW)&&(j==GOAL_COLUMN)))
            {
                int min = -1;
                if(wall_maze[i][j][0]==0)
                {
                    if(min==-1)
                    {
                        min = goal_maze[i-1][j];
                    }
                    else
                    {
                        if(goal_maze[i-1][j]<min)
                        {
                            min = goal_maze[i-1][j];
                        }
                    }
                }
                if(wall_maze[i][j][1]==0)
                {
                    if(min==-1)
                    {
                        min = goal_maze[i][j-1];
                    }
                    else
                    {
                        if(goal_maze[i][j-1]<min)
                        {
                            min = goal_maze[i][j-1];
                        }
                    }
                }
                if(wall_maze[i][j][2]==0)
                {
                    if(min==-1)
                    {
                        min = goal_maze[i][j+1];
                    }
                    else
                    {
                        if(goal_maze[i][j+1]<min)
                        {
                            min = goal_maze[i][j+1];
                        }
                    }
                }
                if(wall_maze[i][j][3]==0)
                {
                    if(min==-1)
                    {
                        min = goal_maze[i+1][j];
                    }
                    else
                    {
                        if(goal_maze[i+1][j]<min)
                        {
                            min = goal_maze[i+1][j];
                        }
                    }
                }

                if(min!=-1)
                {
                    if(goal_maze[i][j] != (min+1))
                    {
                        goal_maze[i][j]=(min+1);
                        temp = 1;
                    }
                }
            }
        }
        if(temp == 1)
        {
            break;
        }
    }

    if(temp == 1)
    {
        update_maze();
    }
}
