//
// This is the main file for the algorithm.
// Author - Ashutosh Sharma (ashutoshshrm529)
// Co Author - Hardik Jain (nepython)
//
// Last edit
// 18 Nov, 2019 - Added enables which I had forgotten.
//                Will add PID if needed after testing.

//
// FUNCTIONS
//
//  FLOODFILL
    void next_square(); // finds the next square using floodfill and takes the mouse there.
    void update_path(int ); // updates the path as necessary by the floodfill algorithm

//  WALL DETECTION
    bool check_wall_forward(); // checks if mouse can go forward. Returns TRUE if it can else false.
    bool check_wall_left(); // checks if mouse can go left. Returns TRUE if it can else false.
    bool check_wall_right(); // checks if mouse can go right. Returns TRUE if it can else false.

//  MOTOR
    void go_forward(int ); // makes the mouse go one block forward
    void turn_right(); // turns the mouse a quarter circle to right
    void turn_left(); // turns the mouse and goes to the block left of current position
    void go_backward(int ); // makes the mouse reverse the previous step

//  ENCODER
    int encoder_right();
    int encoder_left();

//
// GLOBAL VARIABLES
//
//  FLOODFILL
    int maze[16][16] = {{14,13,12,11,10, 9, 8, 7, 7, 8, 9,10,11,12,13,14},
                        {13,12,11,10, 9, 8, 7, 6, 6, 7, 8, 9,10,11,12,13},
                        {12,11,10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9,10,11,12},
                        {11,10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9,10,11},
                        {10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9,10},
                        { 9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9},
                        { 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8},
                        { 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7},
                        { 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7},
                        { 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8},
                        { 9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9},
                        {10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9,10},
                        {11,10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9,10,11},
                        {12,11,10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9,10,11,12},
                        {13,12,11,10, 9, 8, 7, 6, 6, 7, 8, 9,10,11,12,13},
                        {14,13,12,11,10, 9, 8, 7, 7, 8, 9,10,11,12,13,14}};

    String path= "";
    int facing = 0; // 0 for up; 1 for left; 2 for right; 3 for down
    int current_row = 0;
    int current_column = 15;

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
    #define RIGHT_MOTOR_1 5 // pin 1 for right motor
    #define RIGHT_MOTOR_2 4 // pin 2 for right motor
    #define LEFT_MOTOR_1 9 // pin 1 for left motor
    #define LEFT_MOTOR_2 52 // pin 2 for left motor
    #define RIGHT_MOTOR_ENABLE 6 // enable pin for right motor
    #define LEFT_MOTOR_ENABLE 7 // enable pin for left MOTOR
    #define MOTOR_MAX_SPEED 255 // max speed for the motors. reduce if necessary

// ENCODER
    #define RIGHT_ENCODER_DISTANCE 2 // pin for checking distance using right encoder
    #define RIGHT_ENCODER_DIRECTION 3 // pin for checking direction of right encoder
    #define LEFT_ENCODER_DISTANCE 18 // pin for checking distance using left encoder
    #define LEFT_ENCODER_DIRECTION 19 // pin for checking direction of left encoder

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
}

void loop()
{
    // keep going to next block using floodfill until maze center
    while(maze[current_row][current_column]!=0)
    {
        next_square();
    }

    // turn 180 degrees
    turn_right();
    turn_right();

    // follow path backwards
    for(int i = path.length()-1; i>=0; i--)
    {
        if(path.charAt(i)=='F')
        {
            go_forward(FRONT_BACK_BLOCK_DISTANCE);
        }
        else if(path.charAt(i)=='R')
        {
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            turn_left();
        }
        else if(path.charAt(i)=='L')
        {
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            turn_right();
        }
    }

    // turn 180 degrees
    turn_right();
    turn_right();
}

void next_square()
{
  int forward = -1;
  int left = -1;
  int right = -1;

  // find the values at front, left and right
  if(facing==0) // up
  {
    if(((current_row-1)>=0)&&(check_wall_forward())) // checking if maze value exists
    {
      forward = maze[current_row-1][current_column];
    }
    if(((current_column+1)<=15)&&(check_wall_right())) // checking if maze value exists
    {
      right = maze[current_row][current_column+1];
    }
    if(((current_column-1)>=0)&&(check_wall_left())) // checking if maze value exists
    {
      left = maze[current_row][current_column-1];
    }
  }
  else if(facing==1) //left
  {
    if(((current_column-1)>=0)&&(check_wall_forward())) // checking if maze value exists
    {
      forward = maze[current_row][current_column-1];
    }
    if(((current_row-1)>=0)&&(check_wall_right())) // checking if maze value exists
    {
      right = maze[current_row-1][current_column];
    }
    if(((current_row+1)<=15)&&(check_wall_left())) // checking if maze value exists
    {
      left = maze[current_row+1][current_column];
    }
  }
  else if(facing==2) //right
  {
    if(((current_column+1)<=15)&&(check_wall_forward())) // checking if maze value exists
    {
      forward = maze[current_row][current_column+1];
    }
    if(((current_row+1)<=15)&&(check_wall_right())) // checking if maze value exists
    {
      right = maze[current_row+1][current_column];
    }
    if(((current_row-1)>=0)&&(check_wall_left())) // checking if maze value exists
    {
      left = maze[current_row-1][current_column];
    }
  }
  else if(facing==3) //down
  {
    if(((current_row+1)<=15)&&(check_wall_forward())) // checking if maze value exists
    {
      forward = maze[current_row+1][current_column];
    }
    if(((current_column-1)>=0)&&(check_wall_right())) // checking if maze value exists
    {
      right = maze[current_row][current_column-1];
    }
    if(((current_column+1)<=15)&&(check_wall_left())) // checking if maze value exists
    {
      left = maze[current_row][current_column+1];
    }
  }

  //check values of available sides
  if(forward!=-1)
  {
    if(left!=-1)
    {
      if(right!=-1)
      {
        // check smallest value from forward, left and right
        if(((forward<right)&&(forward<left))||((forward==left)&&(forward<right))||((forward==right)&&(forward<left))||((right==left)&&(left==forward)))
        {
          go_forward(FRONT_BACK_BLOCK_DISTANCE);

          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=forward)||((maze[current_row][current_column]-forward)>1))
          {
            update_path(forward);
          }

          if(facing==0) //up
          {
            current_row--;
          }
          else if(facing==1) //left
          {
            current_column--;
          }
          else if(facing==2) //right
          {
            current_column++;
          }
          else if(facing==3) //down
          {
            current_row++;
          }

          path += "F"
        }
        else if((right<forward)&&(right<left))
        {
          turn_right();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=right)||((maze[current_row][current_column]-right)>1))
          {
            update_path(right);
          }

          if(facing==0) //up
          {
            current_column++;
            facing=2; //right
          }
          else if(facing==1) //left
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==2) //right
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==3) //down
          {
            current_column--;
            facing=1; //left
          }

          path += "R"
        }
        else if((left<forward)&&(left<right))
        {
          turn_left();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
          {
            update_path(left);
          }

          if(facing==0) //up
          {
            current_column--;
            facing=1; //left
          }
          else if(facing==1) //left
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==2) //right
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==3) //down
          {
            current_column++;
            facing=2; //right
          }

          path += "L"
        }
        else if((right==left)&&(left<forward))
        {
          // go random in left or right
          if(random(2)==0)
          {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            // go to smallest and update value if smallest is greater than current
            if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
            {
              update_path(left);
            }

            if(facing==0) //up
            {
              current_column--;
              facing=1; //left
            }
            else if(facing==1) //left
            {
              current_row++;
              facing=3; //down
            }
            else if(facing==2) //right
            {
              current_row--;
              facing=0; //up
            }
            else if(facing==3) //down
            {
              current_column++;
              facing=2; //right
            }

            path += "L"
          }
          else
          {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            // go to smallest and update value if smallest is greater than current
            if((maze[current_row][current_column]<=right)||((maze[current_row][current_column]-right)>1))
            {
              update_path(right);
            }

            if(facing==0) //up
            {
              current_column++;
              facing=2; //right
            }
            else if(facing==1) //left
            {
              current_row--;
              facing=0; //up
            }
            else if(facing==2) //right
            {
              current_row++;
              facing=3; //down
            }
            else if(facing==3) //down
            {
              current_column--;
              facing=1; //left
            }

            path += "R"
          }
        }
      }
      else
      {
        // check smallest value from forward and left
        if((forward<left)||(forward==left))
        {
          go_forward(FRONT_BACK_BLOCK_DISTANCE);

          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=forward)||((maze[current_row][current_column]-forward)>1))
          {
            update_path(forward);
          }

          if(facing==0) //up
          {
            current_row--;
          }
          else if(facing==1) //left
          {
            current_column--;
          }
          else if(facing==2) //right
          {
            current_column++;
          }
          else if(facing==3) //down
          {
            current_row++;
          }

          path += "F"
        }
        else
        {
          turn_left();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
          {
            update_path(left);
          }

          if(facing==0) //up
          {
            current_column--;
            facing=1; //left
          }
          else if(facing==1) //left
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==2) //right
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==3) //down
          {
            current_column++;
            facing=2; //right
          }

          path += "L"
        }
      }
    }
    else
    {
      if(right!=-1)
      {
        // check smallest value from forward and right
        if((forward<right)||(forward==right))
        {
          go_forward(FRONT_BACK_BLOCK_DISTANCE);

          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=forward)||((maze[current_row][current_column]-forward)>1))
          {
            update_path(forward);
          }

          if(facing==0) //up
          {
            current_row--;
          }
          else if(facing==1) //left
          {
            current_column--;
          }
          else if(facing==2) //right
          {
            current_column++;
          }
          else if(facing==3) //down
          {
            current_row++;
          }

          path += "F"
        }
        else
        {
          turn_right();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=right)||((maze[current_row][current_column]-right)>1))
          {
            update_path(right);
          }

          if(facing==0) //up
          {
            current_column++;
            facing=2; //right
          }
          else if(facing==1) //left
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==2) //right
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==3) //down
          {
            current_column--;
            facing=1; //left
          }

          path += "R"
        }
      }
      else
      {
        go_forward(FRONT_BACK_BLOCK_DISTANCE);

        // go to smallest and update value if smallest is greater than current
        if((maze[current_row][current_column]<=forward)||((maze[current_row][current_column]-forward)>1))
        {
          update_path(forward);
        }

        if(facing==0) //up
        {
          current_row--;
        }
        else if(facing==1) //left
        {
          current_column--;
        }
        else if(facing==2) //right
        {
          current_column++;
        }
        else if(facing==3) //down
        {
          current_row++;
        }

        path += "F"
      }
    }
  }
  else
  {
    if(left!=-1)
    {
      if(right!=-1)
      {
        // check smallest value from right and left
        if(right<left)
        {
          turn_right();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=right)||((maze[current_row][current_column]-right)>1))
          {
            update_path(right);
          }

          if(facing==0) //up
          {
            current_column++;
            facing=2; //right
          }
          else if(facing==1) //left
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==2) //right
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==3) //down
          {
            current_column--;
            facing=1; //left
          }

          path += "R"
        }
        else if(left<right)
        {
          turn_left();
          go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
          // go to smallest and update value if smallest is greater than current
          if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
          {
            update_path(left);
          }

          if(facing==0) //up
          {
            current_column--;
            facing=1; //left
          }
          else if(facing==1) //left
          {
            current_row++;
            facing=3; //down
          }
          else if(facing==2) //right
          {
            current_row--;
            facing=0; //up
          }
          else if(facing==3) //down
          {
            current_column++;
            facing=2; //right
          }

          path += "L"
        }
        else
        {
          // go random in left or right
          if(random(2)==0)
          {
            turn_left();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            // go to smallest and update value if smallest is greater than current
            if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
            {
              update_path(left);
            }

            if(facing==0) //up
            {
              current_column--;
              facing=1; //left
            }
            else if(facing==1) //left
            {
              current_row++;
              facing=3; //down
            }
            else if(facing==2) //right
            {
              current_row--;
              facing=0; //up
            }
            else if(facing==3) //down
            {
              current_column++;
              facing=2; //right
            }

            path += "L"
          }
          else
          {
            turn_right();
            go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
            // go to smallest and update value if smallest is greater than current
            if((maze[current_row][current_column]<=right)||((maze[current_row][current_column])-right>1))
            {
              update_path(right);
            }

            if(facing==0) //up
            {
              current_column++;
              facing=2; //right
            }
            else if(facing==1) //left
            {
              current_row--;
              facing=0; //up
            }
            else if(facing==2) //right
            {
              current_row++;
              facing=3; //down
            }
            else if(facing==3) //down
            {
              current_column--;
              facing=1; //left
            }

            path += "R"
          }
        }
      }
      else
      {
        turn_left();
        go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
        // go to smallest and update value if smallest is greater than current
        if((maze[current_row][current_column]<=left)||((maze[current_row][current_column]-left)>1))
        {
          update_path(left);
        }

        if(facing==0) //up
        {
          current_column--;
          facing=1; //left
        }
        else if(facing==1) //left
        {
          current_row++;
          facing=3; //down
        }
        else if(facing==2) //right
        {
          current_row--;
          facing=0; //up
        }
        else if(facing==3) //down
        {
          current_column++;
          facing=2; //right
        }

        path += "L"
      }
    }
    else
    {
      if(right!=-1)
      {
        turn_right();
        go_forward(LEFT_RIGHT_BLOCK_DISTANCE);
        // go to smallest and update value if smallest is greater than current
        if((maze[current_row][current_column]<=right)||((maze[current_row][current_column]-right)>1))
        {
          update_path(right);
        }

        if(facing==0) //up
        {
          current_column++;
          facing=2; //right
        }
        else if(facing==1) //left
        {
          current_row--;
          facing=0; //left
        }
        else if(facing==2) //right
        {
          current_row++;
          facing=3; //down
        }
        else if(facing==3) //down
        {
          current_column--;
          facing=1; //left
        }

        path += "R"
      }
      else
      {
        // reverse previous step
        if(path.charAt(path.length()-1)=='F')
        {
            go_backward(FRONT_BACK_BLOCK_DISTANCE);
        }
        else if(path.charAt(path.length()-1)=='L')
        {
            go_backward(LEFT_RIGHT_BLOCK_DISTANCE);
            turn_right();
        }
        else if(path.charAt(path.length()-1)=='R')
        {
            go_backward(LEFT_RIGHT_BLOCK_DISTANCE);
            turn_left();
        }

        // delete path since going back
        path = path.substring(0, path.length());

        // curretn position of maze = -1
        maze[current_row][current_column] = -1;

        if(facing==0) //up
        {
          current_row++;
        }
        else if(facing==1) //left
        {
          current_column++;
        }
        else if(facing==2) //right
        {
          current_column--;
        }
        else if(facing==3) //down
        {
          current_row--;
        }
      }
    }
  }
}

void update_path(int value)
{
  int row = current_row;
  int column = current_column;
  int face = facing; // 0 for up; 1 for left; 2 for right; 3 for down

  for(int i = (path.length()-1); i>=0; i--)
  {
    value++;

    maze[row][column] = value;

    if(path.charAt(i)=='F')
    {
      if(face==0) //up
      {
        row++;
      }
      else if(face==1) //left
      {
        column++;
      }
      else if(face==2) //right
      {
        column--;
      }
      else if(face==3) //down
      {
        row--;
      }
    }
    else if(path.charAt(i)=='R')
    {
      if(face==0) //up
      {
        row++;
        face=1;
      }
      else if(face==1) //left
      {
        column++;
        face=3;
      }
      else if(face==2) //right
      {
        column--;
        face=0;
      }
      else if(face==3) //down
      {
        row--;
        face=2;
      }
    }
    else if(path.charAt(i)=='L')
    {
      if(face==0)
      {
        row++;
        face=2;
      }
      else if(face==1)
      {
        column++;
        face=0;
      }
      else if(face==2)
      {
        column--;
        face=3;
      }
      else if(face==3)
      {
        row--;
        face=1;
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
    if(facing==0) // up
    {
      if(maze[current_row-1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==1) // left
    {
      if(maze[current_row][current_column-1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==2) // right
    {
      if(maze[current_row][current_column+1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==3) // down
    {
      if(maze[current_row+1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
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
    if(facing==0) // up
    {
      if(maze[current_row][current_column-1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==1) // left
    {
      if(maze[current_row+1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==2) // right
    {
      if(maze[current_row-1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==3) // down
    {
      if(maze[current_row][current_column+1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
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
    if(facing==0) // up
    {
      if(maze[current_row][current_column+1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==1) // left
    {
      if(maze[current_row-1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==2) // right
    {
      if(maze[current_row+1][current_column]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
    else if(facing==3) // down
    {
      if(maze[current_row][current_column-1]==-1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
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
    left_value = 0;
    right_value = 0;
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
}

void turn_right()
{
    // turn right and go one block ahead
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
}

void turn_left()
{
    // turn left and go one block ahead
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
}

void go_backward(int distance)
{
    // go backward by distance
    int right_flag; // Flag to check if Right motor has moved the distance
    int left_flag; // Flag to check if Left motor has moved the distance
    left_value = 0;
    right_value = 0;
    while(right_flag+left_flag<2)
    {
        if(right_flag!=1)
        {
            digitalWrite(RIGHT_MOTOR_1,LOW); // Move backward until distance moved
            digitalWrite(RIGHT_MOTOR_2,HIGH);
            analogWrite(RIGHT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
        }
        if(left_flag!=1)
        {
           digitalWrite(LEFT_MOTOR_1,LOW); // Move backward until distance moved
           digitalWrite(LEFT_MOTOR_2,HIGH);
           analogWrite(LEFT_MOTOR_ENABLE, MOTOR_MAX_SPEED);
        }
        if(encoder_right()==distance) // Flag to check if the mouse has moved given distance
        {
            right_flag=1;
        }
        if(encoder_left()==distance)
        {
            left_flag=1;
        }
    }
}

int encoder_left()
{
      // returns the distance as recorded by the left encoder
      if(digitalRead(LEFT_ENCODER_DISTANCE) == HIGH) // ISR for left motor
      {
          left_value++;
      }
      if (digitalRead(LEFT_ENCODER_DISTANCE) == LOW)
      {
          left_value--; // Only 1 interrupt used as 2nd interrupt was adding to redundancy
      }
      return ((left_value/205)*Pi*30); // distance travelled by left motor in mm
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
          right_value--; // a keeps track of the rotary motion of the right encoder
      }
      return ((right_value/205)*Pi*30); // distance travelled by right motor in mm
}
