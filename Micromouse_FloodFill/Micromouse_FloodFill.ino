//
// This is the main file for the algorithm.
// Author - Ashutosh Sharma (ashutoshshrm529)
//
// Last edit
// 28 Nov, 2019 - changed the algorithm to a better one.
//                maze testing with new algorithm begins.
//
// FUNCTIONS
//
//  FLOODFILL
    void initialize_maze(); // initializes the maze to original value(with no knowledge of walls)
    void update_walls(); // checks and updates the walls in the maze array
    void update_maze(); // updates the maze using the floodfill algorithm
    void move_min(); // move to the adjacent square with minimum number (flood fill)
    void move_max(); // move to the adjacent square with mazimum number (outward flood fill)

//  WALL DETECTION
    bool check_wall_forward(); // Returns true if there is a wall in front of the mouse else false.
    bool check_wall_left(); // Returns true if there is a wall to left of the mouse else false.
    bool check_wall_right(); // Returns true if there is a wall to right of the mouse else false.

//  MOTOR
    void go_forward(int ); // makes the mouse go certain distance forward
    void turn_right(); // turns the mouse a quarter circle to right
    void turn_left(); // turns the mouse a quarter circle to left

//  ENCODER
    int encoder_right();
    int encoder_left();

//
// GLOBAL VARIABLES
//
//  FLOODFILL
    #define MAZE_SIZE 16
    int maze[MAZE_SIZE][MAZE_SIZE][5]; // value,up,left,right,down

    int facing = 0; // 0 for up; 1 for left; 2 for right; 3 for down
    int current_row = 0;
    int current_column = MAZE_SIZE-1;

//  WALL DETECTION
    #define FRONT_IR_PIN A0
    #define LEFT_IR_PIN A1
    #define RIGHT_IR_PIN A2

    #define THRESHOLD_FORWARD 8 // the threshold to check if wall present forward
    #define THRESHOLD_LEFT 6 // the threshold to check if wall present left
    #define THRESHOLD_RIGHT 6 // the threshold to check if wall present right

    static double distance_left=0;  //Converted to global variable as reading required by PID
    static double distance_right=0; //Right IR distance measured

    #define FRONT_BACK_BLOCK_DISTANCE 160 // distance to move front and back in mm
    #define LEFT_RIGHT_BLOCK_DISTANCE 80 // distance to move after turning in mm

// MOTOR
    #define RIGHT_MOTOR_1 4 // pin 1 for right motor
    #define RIGHT_MOTOR_2 5 // pin 2 for right motor
    #define LEFT_MOTOR_1 9 // pin 1 for left motor
    #define LEFT_MOTOR_2 17 // pin 2 for left motor
    #define RIGHT_MOTOR_ENABLE 6 // enable pin for right motor
    #define LEFT_MOTOR_ENABLE 7 // enable pin for left MOTOR
    #define MOTOR_MAX_SPEED 255 // max speed for the motors. reduce if necessary

// ENCODER
    #define RIGHT_ENCODER_DISTANCE 19 // pin for checking distance using right encoder
    #define RIGHT_ENCODER_DIRECTION 18 // pin for checking direction of right encoder
    #define LEFT_ENCODER_DISTANCE 21 // pin for checking distance using left encoder
    #define LEFT_ENCODER_DIRECTION 20 // pin for checking direction of left encoder

    #define Pi 3.14159

    volatile int right_value = 0; // reads right encoder value
    volatile int left_value = 0; // reads left encoder value


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

    // ENCODER
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_DISTANCE),encoder_right,RISING );
    // attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_DIRECTION),inr,RISING );
    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_DISTANCE),encoder_left,RISING );
    // attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_DIRECTION),inr,RISING );

    initialize_maze();
}

void loop()
{
    while(maze[current_row][current_column][0]!=0)
    {
        update_walls();
        update_maze();
        move_min();
    }

    turn_right();
    turn_right();

    while(maze[current_row][current_column][0]!=0)
    {
        update_walls();
        update_maze();
        move_max();
    }

    turn_right();
    turn_right();
}

void move_min()
{
    if(facing == 0) // up
    {
        if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_row--;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 1;
            current_column--;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
    }
    else if(facing == 1) // left
    {
        if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]-1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_column--;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 0;
            current_row--;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
    }
    else if(facing == 2) // right
    {
        if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]-1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_column++;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 0;
            current_row--;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 1;
            current_column--;
        }
    }
    else if(facing == 3) // down
    {
        if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_row++;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_column--;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]-1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 0;
            current_row--;
        }
    }
}

void move_max()
{
    if(facing == 0) // up
    {
        if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_row--;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 1;
            current_column--;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
    }
    else if(facing == 1) // left
    {
        if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]+1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_column--;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 0;
            current_row--;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
    }
    else if(facing == 2) // right
    {
        if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]+1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_column++;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 0;
            current_row--;
        }
        else if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_row++;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 1;
            current_column--;
        }
    }
    else if(facing == 3) // down
    {
        if(maze[current_row+1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            current_row++;
        }
        else if(maze[current_row][current_column+1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 2;
            current_column++;
        }
        else if(maze[current_row][current_column-1][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            facing = 3;
            current_column--;
        }
        else if(maze[current_row-1][current_column][0]==(maze[current_row][current_column][0]+1))
        {
            turn_right();
            turn_right();
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
            facing = 0;
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
            maze[current_row][current_column][1] = 1;
            if((current_row+1)<(MAZE_SIZE-1))
            {
                maze[current_row + 1][current_column][4] = 1;
            }
        }

        if(check_wall_left)
        {
            maze[current_row][current_column][2] = 1;
            if((current_column-1)>0)
            {
                maze[current_row][current_column - 1][3] = 1;
            }
        }

        if(check_wall_right)
        {
            maze[current_row][current_column][3] = 1;
            if((current_column+1)<(MAZE_SIZE-1))
            {
                maze[current_row][current_column + 1][2] = 1;
            }
        }
    }
    else if(facing == 1) // left
    {
        if(check_wall_forward)
        {
            maze[current_row][current_column][2] = 1;
            if((current_column-1)>0)
            {
                maze[current_row][current_column - 1][3] = 1;
            }
        }

        if(check_wall_left)
        {
            maze[current_row][current_column][4] = 1;
            if((current_row-1)<(MAZE_SIZE-1))
            {
                maze[current_row - 1][current_column][1] = 1;
            }
        }

        if(check_wall_right)
        {
            maze[current_row][current_column][1] = 1;
            if((current_row+1)<(MAZE_SIZE-1))
            {
                maze[current_row + 1][current_column][4] = 1;
            }
        }
    }
    else if(facing == 2) // right
    {
        if(check_wall_forward)
        {
            maze[current_row][current_column][3] = 1;
            if((current_column+1)<(MAZE_SIZE-1))
            {
                maze[current_row][current_column + 1][2] = 1;
            }
        }

        if(check_wall_left)
        {
            maze[current_row][current_column][1] = 1;
            if((current_row+1)<(MAZE_SIZE-1))
            {
                maze[current_row + 1][current_column][4] = 1;
            }
        }

        if(check_wall_right)
        {
            maze[current_row][current_column][4] = 1;
            if((current_row-1)<(MAZE_SIZE-1))
            {
                maze[current_row - 1][current_column][1] = 1;
            }
        }
    }
    else if(facing == 3) // down
    {
        if(check_wall_forward)
        {
            maze[current_row][current_column][4] = 1;
            if((current_row-1)<(MAZE_SIZE-1))
            {
                maze[current_row - 1][current_column][1] = 1;
            }
        }

        if(check_wall_left)
        {
            maze[current_row][current_column][3] = 1;
            if((current_column+1)<(MAZE_SIZE-1))
            {
                maze[current_row][current_column + 1][2] = 1;
            }
        }

        if(check_wall_right)
        {
            maze[current_row][current_column][2] = 1;
            if((current_column-1)>0)
            {
                maze[current_row][current_column - 1][3] = 1;
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
            if(!(((i==((MAZE_SIZE/2)-1))&&(j==((MAZE_SIZE/2)-1)))||((i==((MAZE_SIZE/2)-1))&&(j==(MAZE_SIZE/2)))||((i==(MAZE_SIZE/2))&&(j==((MAZE_SIZE/2)-1)))||((i==(MAZE_SIZE/2))&&(j==(MAZE_SIZE/2)))))
            {
                int min = -1;
                if(maze[i][j][1]==0)
                {
                    if(min==-1)
                    {
                        min = maze[i-1][j][0];
                    }
                    else
                    {
                        if(maze[i-1][j][0]<min)
                        {
                            min = maze[i-1][j][0];
                        }
                    }
                }
                if(maze[i][j][2]==0)
                {
                    if(min==-1)
                    {
                        min = maze[i][j-1][0];
                    }
                    else
                    {
                        if(maze[i][j-1][0]<min)
                        {
                            min = maze[i][j-1][0];
                        }
                    }
                }
                if(maze[i][j][3]==0)
                {
                    if(min==-1)
                    {
                        min = maze[i][j+1][0];
                    }
                    else
                    {
                        if(maze[i][j+1][0]<min)
                        {
                            min = maze[i][j+1][0];
                        }
                    }
                }
                if(maze[i][j][4]==0)
                {
                    if(min==-1)
                    {
                        min = maze[i+1][j][0];
                    }
                    else
                    {
                        if(maze[i+1][j][0]<min)
                        {
                            min = maze[i+1][j][0];
                        }
                    }
                }

                if(min!=-1)
                {
                    if(maze[i][j][0] != (min+1))
                    {
                        maze[i][j][0]=(min+1);
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
    for(int i = 0; i < (MAZE_SIZE/2); i++)
    {
        for(int j = 0; j < (MAZE_SIZE/2); j++)
        {
            maze[i][j][0] = (MAZE_SIZE-((i+1) + (j+1)));

            if(i == 0)
            {
                maze[i][j][1] = 1; // for top boundary
            }

            if(j == 0)
            {
                maze[i][j][2] = 1; // for left side boundary
            }
        }
    }

    for(int i = 0; i < (MAZE_SIZE/2); i++)
    {
        for(int j = (MAZE_SIZE/2); j < MAZE_SIZE; j++)
        {
            maze[i][j][0] = (MAZE_SIZE-((i+1) + ((MAZE_SIZE-(j+1))+1)));

            if(i == 0)
            {
                maze[i][j][1] = 1; // for top boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                maze[i][j][3] = 1; // for right side boundary
            }
        }
    }

    for(int i = (MAZE_SIZE/2); i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < (MAZE_SIZE/2); j++)
        {
            maze[i][j][0] = (MAZE_SIZE-(((MAZE_SIZE-(i+1))+1) + (j+1)));

            if(i == (MAZE_SIZE-1))
            {
                maze[i][j][4] = 1; // for bottom boundary
            }

            if(j == 0)
            {
                maze[i][j][2] = 1; // for left side boundary
            }
        }
    }

    for(int i = (MAZE_SIZE/2); i < MAZE_SIZE; i++)
    {
        for(int j = (MAZE_SIZE/2); j < MAZE_SIZE; j++)
        {
            maze[i][j][0] = (MAZE_SIZE-(((MAZE_SIZE-(i+1))+1) + ((MAZE_SIZE-(j+1))+1)));

            if(i == (MAZE_SIZE-1))
            {
                maze[i][j][4] = 1; // for bottom boundary
            }

            if(j == (MAZE_SIZE-1))
            {
                maze[i][j][3] = 1; // for right side boundary
            }
        }
    }
}

bool check_wall_forward()
{
      // check forward using IR
      // return false if wall or -1
      // return true if possible to go there
      double distance = 12.08 * pow(analogRead(FRONT_IR_PIN) , -1.058) * 250; // Specific for the model of IR being used

      if(distance<THRESHOLD_FORWARD) // there is no wall
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
      // check left using IR
      // return false if wall or -1
      // return true if possible to go there
      distance_left = 12.08 * pow(analogRead(LEFT_IR_PIN) , -1.058) * 250; // Specific for the model of IR being used

      if(distance_left<THRESHOLD_LEFT) // there is no wall
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
      distance_right = 12.08 * pow(analogRead(RIGHT_IR_PIN) , -1.058) * 250; // Specific for the model of IR being used

      if(distance_right<THRESHOLD_RIGHT) // there is no wall
      {
          return true;
      }
      else // there is a wall
      {
          return false;
      }
}

void go_forward(int distance)
{
    // go forward by distance
    int right_flag = 0; // Flag to check if Right motor has moved the distance
    int left_flag = 0; // Flag to check if Left motor has moved the Distance
    left_value=0;
    right_value=0;
    while(right_flag+left_flag<2)
    {
        if(right_flag!=1)
        {
            digitalWrite(RIGHT_MOTOR_1,HIGH); // Move forward until distance moved
            digitalWrite(RIGHT_MOTOR_2,LOW);
            analogWrite(RIGHT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
        }
        if(left_flag!=1)
        {
           digitalWrite(LEFT_MOTOR_1,HIGH); // Move forward until distance moved
           digitalWrite(LEFT_MOTOR_2,LOW);
           analogWrite(LEFT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
        }
        if(encoder_right()>=distance) // Flag to check if the mouse has moved given distance
        {
            right_flag=1;
        }
        if(encoder_left()>=distance)
        {
            left_flag=1;
        }
    }

    digitalWrite(RIGHT_MOTOR_1,LOW); // turn off right motor
    digitalWrite(RIGHT_MOTOR_2,LOW);
    analogWrite(RIGHT_MOTOR_ENABLE, 0);

    digitalWrite(LEFT_MOTOR_1,LOW); // turn off left motor
    digitalWrite(LEFT_MOTOR_2,LOW);
    analogWrite(LEFT_MOTOR_ENABLE, 0);
}

void turn_right()
{
    // turn right
    // need to do some axis checking with maze and edit
    //
    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    //
    int flag = 0;
    left_value = 0;
    right_value = 0;
    while(flag<1)
    {
          if(flag!=1)
          {
                digitalWrite(RIGHT_MOTOR_1,LOW);
                digitalWrite(RIGHT_MOTOR_2,HIGH);
                digitalWrite(LEFT_MOTOR_1,HIGH);
                digitalWrite(LEFT_MOTOR_2,LOW);
                analogWrite(RIGHT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
                analogWrite(LEFT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
          }
          if(encoder_right()>=79)
          {
                flag=1;
          }
     }

     digitalWrite(RIGHT_MOTOR_1,LOW); // turn off right motor
     digitalWrite(RIGHT_MOTOR_2,LOW);
     analogWrite(RIGHT_MOTOR_ENABLE, 0);

     digitalWrite(LEFT_MOTOR_1,LOW); // turn off left motor
     digitalWrite(LEFT_MOTOR_2,LOW);
     analogWrite(LEFT_MOTOR_ENABLE, 0);
}

void turn_left()
{
    // turn left
    // need to do some axis checking with maze and edit
    //
    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    //
    int flag = 0;
    left_value = 0;
    right_value = 0;
    while(flag<1)
    {
          if(flag!=1)
          {
                digitalWrite(RIGHT_MOTOR_1,HIGH);
                digitalWrite(RIGHT_MOTOR_2,LOW);
                digitalWrite(LEFT_MOTOR_1,LOW);
                digitalWrite(LEFT_MOTOR_2,HIGH);
                analogWrite(LEFT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
                analogWrite(RIGHT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
          }
          if(encoder_left()>=79)
          {
                flag=1;
          }
     }

     digitalWrite(RIGHT_MOTOR_1,LOW); // turn off right motor
     digitalWrite(RIGHT_MOTOR_2,LOW);
     analogWrite(RIGHT_MOTOR_ENABLE, 0);

     digitalWrite(LEFT_MOTOR_1,LOW); // turn off left motor
     digitalWrite(LEFT_MOTOR_2,LOW);
     analogWrite(LEFT_MOTOR_ENABLE, 0);
}

int encoder_right()
{
      // returns the distance as recorded by the right encoder
      if(digitalRead(RIGHT_ENCODER_DISTANCE) == HIGH) // ISR for right motor
      {
          right_value++; // Only 1 interrupt used as 2nd interrupt was adding to redundancy
      }
      if(digitalRead(RIGHT_ENCODER_DISTANCE) == LOW)
      {
          right_value--;
      }
      return ((right_value/205)*Pi*30); // distance travelled by right motor in mm
}

int encoder_left()
{
      // returns the distance as recorded by the left encoder
      if(digitalRead(LEFT_ENCODER_DISTANCE) == HIGH) // ISR for left motor
      {
          left_value++; // Only 1 interrupt used as 2nd interrupt was adding to redundancy
      }
      if(digitalRead(LEFT_ENCODER_DISTANCE) == LOW)
      {
          left_value--;
      }
      return ((left_value/205)*Pi*30); // distance travelled by left motor in mm
}
