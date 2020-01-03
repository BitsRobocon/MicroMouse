//

// This is the main file for the algorithm.

// Authors - Ashutosh Sharma (ashutoshshrm529)

//           Hardik Jain (nepython)

//

//

// FUNCTIONS

//

//  FLOODFILL

    void initialize_maze(); // initializes the maze to original value(with no knowledge of walls)

    void update_walls(); // checks and updates the walls in the maze array

    void update_maze(); // updates the maze using the floodfill algorithm

    void move_toward_goal(); // move to the adjacent square with minimum number (flood fill)



//  WALL DETECTION

    float Forward_Distance(); // Returns the distance as read by the forward ir using SharpIR Library

    float Left_Distance(); // Returns the distance as read by the left ir using SharpIR Library

    float Right_Distance(); // Returns the distance as read by the right ir using SharpIR Library

    bool check_wall_forward(); // Returns true if there is a wall in front of the mouse else false.

    bool check_wall_left(); // Returns true if there is a wall to left of the mouse else false.

    bool check_wall_right(); // Returns true if there is a wall to right of the mouse else false.



//  MOTOR

    void go_forward(long ); // makes the mouse go certain distance forward

    void go_backward(long ); // makes the mouse go certain distance backward

    void turn_right(); // turns the mouse a quarter circle to right

    void turn_left(); // turns the mouse a quarter circle to left

    void drive_straight(float, float); // makes the mouse move in a straight line using pid



//

// GLOBAL VARIABLES

//

//  FLOODFILL

    #define MAZE_SIZE 16



    int goal_maze[MAZE_SIZE][MAZE_SIZE];

    int wall_maze[MAZE_SIZE][MAZE_SIZE][4]; // up,left,right,down



    int facing = 0; // 0 for up; 1 for left; 2 for right; 3 for down

    int current_row = MAZE_SIZE-1;

    int current_column = 0;

    int GOAL_ROW = 3; // definition of the goal where the mouse wants to go to

    int GOAL_COLUMN = 3;



//  WALL DETECTION

    #define FRONT_IR_PIN A0 // front ir data pin

    #define LEFT_IR_PIN A1 // left ir data pin

    #define RIGHT_IR_PIN A2 // right ir data pin

    #define IR_MODEL 430



    #define THRESHOLD_FORWARD 8 // the threshold to check if wall present forward

    #define THRESHOLD_LEFT 6 // the threshold to check if wall present left

    #define THRESHOLD_RIGHT 6 // the threshold to check if wall present right



    #define FRONT_BACK_BLOCK_DISTANCE 18 // distance to move front and back in cm

    #define LEFT_RIGHT_BLOCK_DISTANCE 10 // distance to move after turning in cm



// MOTOR

    #define RIGHT_MOTOR_1 4 // pin 1 for right motor

    #define RIGHT_MOTOR_2 5 // pin 2 for right motor

    #define LEFT_MOTOR_1 9 // pin 1 for left motor

    #define LEFT_MOTOR_2 17 // pin 2 for left motor



    #define ERROR_THRESHOLD 5 // To define threshold value for error

    #define RIGHT_MOTOR_ENABLE 6 // enable pin for right motor

    #define LEFT_MOTOR_ENABLE 7 // enable pin for left MOTOR

    int LEFT_MOTOR_SPEED=255; // max speed for the motors. reduce if necessary

    int RIGHT_MOTOR_SPEED=255;



// ENCODER

    #include <SharpIR.h>

    #include <Encoder.h>



    #define RIGHT_ENCODER_DISTANCE 19 // pin for checking distance using right encoder

    #define RIGHT_ENCODER_DIRECTION 18 // pin for checking direction of right encoder

    #define LEFT_ENCODER_DISTANCE 21 // pin for checking distance using left encoder

    #define LEFT_ENCODER_DIRECTION 20 // pin for checking direction of left encoder



    Encoder ENCODER_RIGHT(RIGHT_ENCODER_DISTANCE, RIGHT_ENCODER_DIRECTION);

    Encoder ENCODER_LEFT(LEFT_ENCODER_DISTANCE, LEFT_ENCODER_DIRECTION);



    //Global Flags

    int RIGHT_ENCODER_FLAG=0;      //Encoder flags

    int LEFT_ENCODER_FLAG=0;



    #define PI 3.141593



    static int previous_error = 0;    //Error used in PID





void setup()

{

    // WALL DETECTION

    pinMode(FRONT_IR_PIN,INPUT);

    pinMode(LEFT_IR_PIN,INPUT);

    pinMode(RIGHT_IR_PIN,INPUT);



    // MOTOR

    pinMode(RIGHT_MOTOR_1, OUTPUT);

    pinMode(LEFT_MOTOR_1, OUTPUT);

    pinMode(RIGHT_MOTOR_2, OUTPUT);

    pinMode(LEFT_MOTOR_2, OUTPUT);



    initialize_maze();
}



void loop()
{
    GOAL_ROW=(MAZE_SIZE/2);
    GOAL_COLUMN=(MAZE_SIZE/2);

    while(!((current_row==GOAL_ROW)&&(current_column==GOAL_COLUMN)))
    {
        update_walls();
        update_maze();
        move_toward_goal();
        delay(500);
    }

    GOAL_ROW=(MAZE_SIZE-1);
    GOAL_COLUMN=0;

    while(!((current_row==GOAL_ROW)&&(current_column==GOAL_COLUMN)))
    {
        update_walls();
        update_maze();
        move_toward_goal();
        delay(500);
    }
}



void move_toward_goal()

{

    if(facing == 0) // up

    {

        if(goal_maze[current_row-1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_row--;

        }

        else if(goal_maze[current_row][current_column-1]==(goal_maze[current_row][current_column]-1))

        {

            turn_left();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 1;

            current_column--;

        }

        else if(goal_maze[current_row][current_column+1]==(goal_maze[current_row][current_column]-1))

        {

            turn_right();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 2;

            current_column++;

        }

        else if(goal_maze[current_row+1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            go_backward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_row++;

        }

    }

    else if(facing == 1) // left

    {

        if(goal_maze[current_row][current_column-1]==(goal_maze[current_row][current_column]-1))

        {

            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_column--;

        }

        else if(goal_maze[current_row+1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            turn_left();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 3;

            current_row++;

        }

        else if(goal_maze[current_row-1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            turn_right();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 0;

            current_row--;

        }

        else if(goal_maze[current_row][current_column+1]==(goal_maze[current_row][current_column]-1))

        {

            go_backward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_column++;

        }

    }

    else if(facing == 2) // right

    {

        if(goal_maze[current_row][current_column+1]==(goal_maze[current_row][current_column]-1))

        {

            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_column++;

        }

        else if(goal_maze[current_row-1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            turn_left();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 0;

            current_row--;

        }

        else if(goal_maze[current_row+1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            turn_right();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 3;

            current_row++;

        }

        else if(goal_maze[current_row][current_column-1]==(goal_maze[current_row][current_column]-1))

        {

            go_backward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_column--;

        }

    }

    else if(facing == 3) // down

    {

        if(goal_maze[current_row+1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_row++;

        }

        else if(goal_maze[current_row][current_column+1]==(goal_maze[current_row][current_column]-1))

        {

            turn_left();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 2;

            current_column++;

        }

        else if(goal_maze[current_row][current_column-1]==(goal_maze[current_row][current_column]-1))

        {

            turn_right();
            delay(500);
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            delay(500);
            facing = 3;

            current_column--;

        }

        else if(goal_maze[current_row-1][current_column]==(goal_maze[current_row][current_column]-1))

        {

            go_backward(FRONT_BACK_BLOCK_DISTANCE);
            delay(500);
            current_row--;

        }

    }

}



void update_walls()

{

    if(facing == 0) // up

    {

        if(check_wall_forward)

        {

            wall_maze[current_row][current_column][0] = 1;

            if((current_row+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row + 1][current_column][3] = 1;

            }

        }



        if(check_wall_left)

        {

            wall_maze[current_row][current_column][1] = 1;

            if((current_column-1)>0)

            {

                wall_maze[current_row][current_column - 1][2] = 1;

            }

        }



        if(check_wall_right)

        {

            wall_maze[current_row][current_column][2] = 1;

            if((current_column+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row][current_column + 1][1] = 1;

            }

        }

    }

    else if(facing == 1) // left

    {

        if(check_wall_forward)

        {

            wall_maze[current_row][current_column][1] = 1;

            if((current_column-1)>0)

            {

                wall_maze[current_row][current_column - 1][2] = 1;

            }

        }



        if(check_wall_left)

        {

            wall_maze[current_row][current_column][3] = 1;

            if((current_row-1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row - 1][current_column][0] = 1;

            }

        }



        if(check_wall_right)

        {

            wall_maze[current_row][current_column][0] = 1;

            if((current_row+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row + 1][current_column][3] = 1;

            }

        }

    }

    else if(facing == 2) // right

    {

        if(check_wall_forward)

        {

            wall_maze[current_row][current_column][2] = 1;

            if((current_column+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row][current_column + 1][1] = 1;

            }

        }



        if(check_wall_left)

        {

            wall_maze[current_row][current_column][0] = 1;

            if((current_row+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row + 1][current_column][3] = 1;

            }

        }



        if(check_wall_right)

        {

            wall_maze[current_row][current_column][3] = 1;

            if((current_row-1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row - 1][current_column][0] = 1;

            }

        }

    }

    else if(facing == 3) // down

    {

        if(check_wall_forward)

        {

            wall_maze[current_row][current_column][3] = 1;

            if((current_row-1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row - 1][current_column][0] = 1;

            }

        }



        if(check_wall_left)

        {

            wall_maze[current_row][current_column][2] = 1;

            if((current_column+1)<(MAZE_SIZE-1))

            {

                wall_maze[current_row][current_column + 1][1] = 1;

            }

        }



        if(check_wall_right)

        {

            wall_maze[current_row][current_column][1] = 1;

            if((current_column-1)>0)

            {

                wall_maze[current_row][current_column - 1][2] = 1;

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



bool check_wall_forward()

{

      // check forward using IR

      // return false if wall or -1

      // return true if possible to go there

      if(Forward_Distance()<THRESHOLD_FORWARD) // there is no wall

      {

          return true;

      }

      else // there is a wall

      {

          return false;

      }

}



bool check_wall_left()

{

      if(Left_Distance()<THRESHOLD_LEFT) // there is no wall

      {

          return true;

      }

      else // there is a wall

      {

          return false;

      }

}



bool check_wall_right()

{

      // check right using IR

      // return false if wall or -1

      // return true if possible to go there

      if(Right_Distance()<THRESHOLD_RIGHT) // there is no wall

      {

          return true;

      }

      else // there is a wall

      {

          return false;

      }

}



void go_forward(long distance)

{

    RIGHT_ENCODER_FLAG=0;

    LEFT_ENCODER_FLAG=0;

    distance=((distance*820)/(3*PI))*0.8;

    long distance_right=abs(ENCODER_RIGHT.read());

    long distance_left=abs(ENCODER_LEFT.read());



    long OldPositionRight=(-999);

    long OldPositionLeft=(-999);

    long NewPositionRight=0;

    long NewPositionLeft=0;

    while((RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG)!=2)

    {

        if (Forward_Distance<THRESHOLD_FORWARD)

        {

            analogWrite(LEFT_MOTOR_1,0);

            analogWrite(LEFT_MOTOR_2,0);

            analogWrite(RIGHT_MOTOR_1,0);

            analogWrite(RIGHT_MOTOR_2,0);

            return;

        }

        drive_straight(Right_Distance(),Left_Distance());

        if(NewPositionRight<distance*0.5+distance_right)

        {

            NewPositionRight = abs(ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

            analogWrite(RIGHT_MOTOR_1,222);

            analogWrite(RIGHT_MOTOR_2,0);

        }

        else if(NewPositionRight>(distance*0.5+distance_right)&&NewPositionRight<(distance*0.7+distance_right))

        {

            analogWrite(RIGHT_MOTOR_1,132);

            analogWrite(RIGHT_MOTOR_2,0);

            NewPositionRight = abs(ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else if((NewPositionRight>(distance*0.7+distance_right)&&NewPositionRight<(distance+distance_right))&&!(LEFT_ENCODER_FLAG==1&&previous_error<0.2))

        {

            analogWrite(RIGHT_MOTOR_1,88);

            analogWrite(RIGHT_MOTOR_2,0);

            NewPositionRight = abs(ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else

        {

            analogWrite(RIGHT_MOTOR_1,0);

            analogWrite(RIGHT_MOTOR_2,0);

            RIGHT_ENCODER_FLAG=1;

        }



        if(NewPositionLeft<distance*0.5+distance_left)

        {

            NewPositionLeft = abs(ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

            analogWrite(LEFT_MOTOR_1,255);

            analogWrite(LEFT_MOTOR_2,0);

        }

        else if(NewPositionLeft>(distance*0.5+distance_left)&&NewPositionLeft<(distance*0.7+distance_left))

        {

            analogWrite(LEFT_MOTOR_1,150);

            analogWrite(LEFT_MOTOR_2,0);

            NewPositionLeft = abs(ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

        }

        else if((NewPositionLeft>(distance*0.7+distance_left)&&NewPositionLeft<(distance+distance_left))&&!(RIGHT_ENCODER_FLAG==1&&previous_error<0.2))

        {

            analogWrite(LEFT_MOTOR_1,98);

            analogWrite(LEFT_MOTOR_2,0);

            NewPositionLeft = abs(ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

        }

        else

        {

            analogWrite(LEFT_MOTOR_1,0);

            analogWrite(LEFT_MOTOR_2,0);

            LEFT_ENCODER_FLAG=1;

        }

        if((RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG)==2)

        {

            RIGHT_ENCODER_FLAG=0;

            LEFT_ENCODER_FLAG=0;

            LEFT_MOTOR_SPEED=255;

            RIGHT_MOTOR_SPEED=255;

            return;

        }

    }

}



void go_backward(long distance)

{

    RIGHT_ENCODER_FLAG=0;

    LEFT_ENCODER_FLAG=0;

    distance=((distance*820)/(3*PI));

    long distance_right=(ENCODER_RIGHT.read());

    long distance_left=(ENCODER_LEFT.read());

    long OldPositionRight=(-999);

    long OldPositionLeft=(-999);

    long NewPositionRight=(ENCODER_RIGHT.read());

    long NewPositionLeft=(ENCODER_LEFT.read());

    while((RIGHT_ENCODER_FLAG!=1)&&(LEFT_ENCODER_FLAG!=1))                //Some shortcoming is not allowing left motor to ro rotate fully

    {

        drive_straight(Right_Distance(),Left_Distance());

        if(NewPositionRight>-distance*0.5+distance_right)

        {

            NewPositionRight = (ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

            OldPositionRight = NewPositionRight;

            }

            analogWrite(RIGHT_MOTOR_2,255);

            analogWrite(RIGHT_MOTOR_1,0);

        }

        else if(NewPositionRight<(-distance*0.5+distance_right)&&NewPositionRight>(-distance*0.7+distance_right))

        {

            analogWrite(RIGHT_MOTOR_2,150);

            analogWrite(RIGHT_MOTOR_1,0);

            NewPositionRight = (ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else if((NewPositionRight<(-distance*0.7+distance_right)&&NewPositionRight>(-distance+distance_right)))

        {

            analogWrite(RIGHT_MOTOR_2,135);

            analogWrite(RIGHT_MOTOR_1,0);

            NewPositionRight = abs(ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else

        {

            analogWrite(RIGHT_MOTOR_2,0);

            analogWrite(RIGHT_MOTOR_1,0);

            RIGHT_ENCODER_FLAG=1;

        }



        if(NewPositionLeft>-distance*0.5+distance_left)

        {

            NewPositionLeft = (ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

            analogWrite(LEFT_MOTOR_2,200);

            analogWrite(LEFT_MOTOR_1,0);

        }

        else if(NewPositionLeft<(-distance*0.5+distance_left)&&NewPositionLeft>(-distance*0.7+distance_left))

        {

            analogWrite(LEFT_MOTOR_2,137);

            analogWrite(LEFT_MOTOR_1,0);

            NewPositionLeft = (ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

        }

        else if((NewPositionLeft<(-distance*0.7+distance_left)&&NewPositionLeft>(-distance*0.92+distance_left)))

        {

            analogWrite(LEFT_MOTOR_2,120);

            analogWrite(LEFT_MOTOR_1,0);

            NewPositionLeft = abs(ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

        }

        else

        {

            analogWrite(LEFT_MOTOR_1,0);

            analogWrite(LEFT_MOTOR_2,0);

            LEFT_ENCODER_FLAG=1;

        }

        if((RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG)==2)

        {

            RIGHT_ENCODER_FLAG=0;

            LEFT_ENCODER_FLAG=0;

            LEFT_MOTOR_SPEED=255;

            RIGHT_MOTOR_SPEED=255;

            return;

        }

    }

    analogWrite(RIGHT_MOTOR_1,0);

    analogWrite(RIGHT_MOTOR_2,0);

    analogWrite(LEFT_MOTOR_1,0);

    analogWrite(LEFT_MOTOR_2,0);

}



void turn_right()

{

    // turn right and go one block ahead

    // need to do some axis checking with maze and edit



    // Distance between centres of the 2 wheels~~101mm

    // Quarter circle traversed by each wheel=pi*50.5/2=79.32mm



    RIGHT_ENCODER_FLAG=0;

    LEFT_ENCODER_FLAG=0;

    long distance=486+abs(ENCODER_LEFT.read());

    long OldPositionRight  = -999;

    long OldPositionLeft  = -999;

    long NewPositionRight=0;

    long NewPositionLeft=0;



    if (NewPositionRight != OldPositionRight)

    {

        OldPositionRight = NewPositionRight;

    }

    if (NewPositionLeft != OldPositionLeft)

    {

        OldPositionLeft = NewPositionLeft;

    }

    while(RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG<2)

    {

        if(RIGHT_ENCODER_FLAG!=1)

        {

            analogWrite(RIGHT_MOTOR_2,255);

            analogWrite(RIGHT_MOTOR_1,0);

        }

        if(LEFT_ENCODER_FLAG!=1)

        {

            analogWrite(LEFT_MOTOR_2,0);

            analogWrite(LEFT_MOTOR_1,255);

        }

        if(NewPositionLeft<distance)

        {

            NewPositionLeft = abs(ENCODER_LEFT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

                OldPositionLeft = NewPositionLeft;

            }

        }

        else

        {

            LEFT_ENCODER_FLAG=1;

            analogWrite(LEFT_MOTOR_2,0);

            analogWrite(LEFT_MOTOR_1,0);

        }

        if(NewPositionRight<distance)

        {

            NewPositionRight = abs(ENCODER_LEFT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else

        {

            RIGHT_ENCODER_FLAG=1;

            analogWrite(RIGHT_MOTOR_2,0);

            analogWrite(RIGHT_MOTOR_1,0);

        }

    }

}



void turn_left()

{

    // turn left and go one block ahead

    // need to do some axis checking with maze and edit



    //   Distance between centres of the 2 wheels~~101mm

    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm



    RIGHT_ENCODER_FLAG=0;

    LEFT_ENCODER_FLAG=0;

    long distance=490+abs(ENCODER_RIGHT.read());    //Though actual value is 345, I had to compensate for slipping

    long OldPositionRight  = -999;

    long OldPositionLeft  = -999;

    long NewPositionRight=0;

    long NewPositionLeft=0;



    if (NewPositionRight != OldPositionRight)

    {

      OldPositionRight = NewPositionRight;

    }

    if (NewPositionLeft != OldPositionLeft)

    {

      OldPositionLeft = NewPositionLeft;

    }



    while(RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG<2)

    {

        if(RIGHT_ENCODER_FLAG!=1)

        {

            analogWrite(RIGHT_MOTOR_2,0);

            analogWrite(RIGHT_MOTOR_1,255);

        }

        if(LEFT_ENCODER_FLAG!=1)

        {

            analogWrite(LEFT_MOTOR_2,255);

            analogWrite(LEFT_MOTOR_1,0);

        }

        if(NewPositionLeft<distance)

        {

            NewPositionLeft = abs(ENCODER_RIGHT.read());

            if (NewPositionLeft != OldPositionLeft)

            {

              OldPositionLeft = NewPositionLeft;

            }

        }

        else

        {

            LEFT_ENCODER_FLAG=1;

            analogWrite(LEFT_MOTOR_2,0);

            analogWrite(LEFT_MOTOR_1,0);

        }

        if(NewPositionRight<distance)

        {

            NewPositionRight = abs(ENCODER_RIGHT.read());

            if (NewPositionRight != OldPositionRight)

            {

                OldPositionRight = NewPositionRight;

            }

        }

        else

        {

            RIGHT_ENCODER_FLAG=1;

            analogWrite(RIGHT_MOTOR_2,0);

            analogWrite(RIGHT_MOTOR_1,0);

        }

    }

}

void drive_straight( float distance_right, float distance_left) //To be included in Forward

{

    static int Kp = 32, Ki = 1, Kd = 4;      // constants for scaling P I D effects (will need adjusting)

    static int error, P, I = 0,  D;          // error variables

    int total;



    error = distance_left - distance_right;

    if((abs(error)<ERROR_THRESHOLD)&&abs(error)>0.2)

    {

        P = error * Kp;



        //I = (I + error)*Ki;



        D = (error - previous_error) * Kd;                   // may take out

        previous_error = error;



        total = (P+I+D);



        {

            LEFT_MOTOR_SPEED -= (total);

            LEFT_MOTOR_SPEED = constrain(LEFT_MOTOR_SPEED, 120, 255);   // may need to adjust



            RIGHT_MOTOR_SPEED += (total);

            RIGHT_MOTOR_SPEED = constrain(RIGHT_MOTOR_SPEED, 120, 255);



            analogWrite(LEFT_MOTOR_ENABLE, LEFT_MOTOR_SPEED);      // enable pins and values

                                                               // must be global

            analogWrite(RIGHT_MOTOR_ENABLE, RIGHT_MOTOR_SPEED);

        }

    }

    LEFT_MOTOR_SPEED=255;

    RIGHT_MOTOR_SPEED=255;

}

float Forward_Distance()

{

    SharpIR SharpIR(FRONT_IR_PIN, IR_MODEL);

    return SharpIR.distance();

}



float Left_Distance()

{

    SharpIR SharpIR(LEFT_IR_PIN, IR_MODEL);

    return (SharpIR.distance() + 0.45); //accounting for difference in values

}



float Right_Distance()

{

    SharpIR SharpIR(RIGHT_IR_PIN, IR_MODEL);

    return SharpIR.distance();

}
