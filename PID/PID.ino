/*PID.ino
 *Last updated:10/11/19
 *Author: Hardik
 *CoAuthor: Ashutosh
 */

# define error_threshold                   //To be defined error of threshold
void drive_straight( double distance_left, double distance_right) //To be included in Forward
{
  static int previous_error = 0;
  static int Kp = 16, Ki = 1, Kd = 4;      // constants for scaling P I D effects (will need adjusting)
  static int error, P, I = 0,  D;          // error variables
  int total;

  error = (int)(distance_right - distance_left);
  if(error<error_threshold)
    {
      P = error * Kp;

      I = (I + error)*Ki;

      D = (error - previous_error) * Kd;                   // may take out
      previous_error = error;

      total = (P+I+D);

      {
        L_enable_val -= (total);
        L_enable_val = constrain(L_enable_val, 120, 255)   // may need to adjust

        R_enable_val += (total);
        R_enable_val = constrain(R_enable_val, 120, 255);

        analogWrite(left_enable, L_enable_val);            // enable pins and values
                                                           // must be global
        analogWrite(right_enable, R_enable_val);
    }
  }
}
// Reference : MattFach's work
