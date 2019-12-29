/*PID.ino
 *Last updated:29/12/19
 *Author: Hardik
 *CoAuthor: Ashutosh
 */

# define error_threshold 5                  //Beyond this error, PID won't work
void drive_straight( double distance_left, double distance_right) //To be included in Forward
{
  static int previous_error = 0;
  static int Kp = 25, Ki = 1, Kd = 4;      // constants for scaling P I D effects (will need adjusting)
  static int error, P, I = 0,  D;          // error variables
  int total;

  error = (int)(distance_right - distance_left);
  if(error<error_threshold)
    {
      P = error * Kp;

      //I = (I + error)*Ki;

      D = (error - previous_error) * Kd;                   // may take out
      previous_error = error;

      total = (P+I+D);

      {
        L_MOTOR_val -= (total);
        L_MOTOR_val = constrain(L_enable_val, 120, 255)   // may need to adjust

        R_MOTOR_val += (total);
        R_MOTOR_val = constrain(R_MOTOR_val, 120, 255);

        analogWrite(LEFT_MOTOR_1, L_MOTOR_val);            // MOTOR pins and values
                                                           // must be global
        analogWrite(RIGHT_MOTOR_1, R_MOTOR_val);
    }
  }
}
// Reference : MattFach's work
