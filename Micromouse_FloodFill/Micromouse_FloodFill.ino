/*
*  This is the main file for the algorithm.
*  Author - Ashutosh Sharma
*
*  Last edit
*  7 Nov, 2019 - The algorithm has been checked and wall detection functions added(need checking)
*                Just need to add the movement functions now.
*/

//
// FUNCTIONS
//
//  FLOODFILL
    void next_square(); // finds the next square using floodfill and takes the mouse there.
    void update_path(int ); // updates the path as necessary by the floodfill algorithm
    bool check_wall_forward(); // checks if mouse can go forward. Returns TRUE if it can else false.
    bool check_wall_left(); // checks if mouse can go left. Returns TRUE if it can else false.
    bool check_wall_right(); // checks if mouse can go right. Returns TRUE if it can else false.
    void go_forward(); // makes the mouse go one block forward
    void go_right(); // turns the mouse and goes to the block right of current position
    void go_left(); // turns the mouse and goes to the block left of current position
    void go_back(); // makes the mouse reverse the previous step

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

void setup()
{
  // WALL DETECTION
    pinMode(FRONT_IR_PIN,INPUT);
    pinMode(LEFT_IR_PIN,INPUT);
    pinMode(RIGHT_IR_PIN,INPUT);
}

void loop()
{
  while(maze[current_row][current_column]!=0)
  {
    next_square();
  }

  // turn 180 degrees

  // follow path backwards

  // turn 180 degrees
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
          go_forward();

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
          go_right();
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
          go_left();
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
            go_left();
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
            go_right();
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
          go_forward();

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
          go_left();
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
          go_forward();

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
          go_right();
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
        go_forward();

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
          go_right();
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
          go_left();
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
            go_left();
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
            go_right();
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
        go_left();
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
        go_right();
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
        go_back();

        // no need to update path since going back
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

        path = path.substring(0, path.length());
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
  double distance = 12.08 * pow(analogRead(LEFT_IR_PIN) , -1.058) * 250; // Specific for the model of IR being used

  if(distance<THRESHOLD_LEFT) // there is no wall
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
  double distance = 12.08 * pow(analogRead(RIGHT_IR_PIN) , -1.058) * 250; // Specific for the model of IR being used

  if(distance<THRESHOLD_RIGHT) // there is no wall
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

void go_forward()
{
    //go forward one block
    int a;                                     //Flag to check if Rmotor has moved by 16cm
    int b;                                     //Flag to check if Lmotor has moved by 16cm
    while(a+b<2)
    {
        if(a!=1)
        {
            digitalWrite(rm1,HIGH); //Move forward until 16cm moved 
            digitalWrite(rm2,LOW);
        }
        if(b!=1)
        {
           digitalWrite(lm1,HIGH); //Move fwd until 16cm moved
           digitalWrite(lm2,LOW);
        }
        if(encoder_right()%160==0) // Flag to check if the mouse has moved 16 cm
        {
            a=1;
        }
        if(encoder_left()%160==0)
        {
            b=1;
        }
    }
}

void go_right()
{
    // turn right and go one block ahead
    // need to do some axis checking with maze and edit
    //
    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    //
    int a;
    while(a<1)
    {
          if(a!=1)
          {
                digitalWrite(rm1,LOW);
                digitalWrite(rm2,HIGH);
                digitalWrite(lm1,HIGH);
                digitalWrite(lm2,LOW);
          }
          if(encoder_right()%79==0)
          {
                a=1;
          }
     }
}

void go_left()
{
    // turn left and go one block ahead
    // need to do some axis checking with maze and edit
    //
    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    //
    int a;
    while(a<1)
    {
          if(a!=1)
          {
                digitalWrite(rm1,HIGH);
                digitalWrite(rm2,LOW);
                digitalWrite(lm1,LOW);
                digitalWrite(lm2,HIGH);
          }
          if(encoder_left()%79==0)
          {
                a=1;
          }
     }
}

void go_back()
{
  // reverse the previous step
}

int encoder_left()
{
      // returns the distance as recorded by the left encoder
      if(digitalRead(le1) == HIGH)
      {
        b++;
      }
      if (digitalRead(le1) == LOW)
      {
        b--;
      }
      return ((b/205)*pi*30);
}

int encoder_right()
{
      // returns the distance as recorded by the right encoder
      if(digitalRead(re1) == HIGH)                   //ISR for right motor
      {
          a++;                                            //Only 1 interrupt used as 2nd interrupt was adding to redundacy
      }
      if(digitalRead(re1) == LOW)
      {
          a--;                                            //a keeps track of the rotary motion of the right encoder
      }
      return ((a/205)*pi*30);                              //distance travelled by right motor in mm
}
