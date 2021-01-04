/*Example sketch to control a stepper motor with A4988 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:
//#define dir_pin_1 2
//#define step_pin_1 3
//#define dir_pin_2 4
//#define step_pin_2 5
#define stepsPerRevolution 200

#define max_motor_speed 100.0
#define min_delay 500

int motor_speed = 50; // lower than 2 not acceptable - in that case motor starts turning at max speed
int delay_for_speed = max_motor_speed * min_delay / motor_speed;  //60000

void setup_motor(int, int);

class StepperMotor {
  public:
    int dir_pin;
    int step_pin;
    bool dir_clockwise, next_step_high_or_low = true;
    int motor_speed;
    unsigned long last_step_time;
    int delay_for_speed = 60000;

    StepperMotor(int dir_pin_l, int step_pin_l)
    {
      dir_pin = dir_pin_l;
      step_pin = step_pin_l;
    }

    void setup()
    {
      pinMode(dir_pin, OUTPUT);
      pinMode(step_pin, OUTPUT);
    }

    void set_speed(int new_speed)
    { // abs() math function could be used, but below way is more efficient
      if (new_speed >= 0)
      {
        dir_clockwise = true; //let's say forward
        motor_speed = new_speed;
      }
      else
      {
        dir_clockwise = false; //~ backward
        motor_speed = (-1) * new_speed;
      }
      digitalWrite(dir_pin, dir_clockwise);
      set_delay_for_speed(motor_speed);
    }

    void set_delay_for_speed(int motor_speed)
    {
      delay_for_speed = max_motor_speed * min_delay / motor_speed;
    }

    //    void make_a_step()
    //    { // this is not efficient, bcs if we do the same thing with two motors, another one is going to wait for this one to finish the delay
    //
    //      digitalWrite(step_pin, HIGH);
    //      delayMicroseconds(delay_for_speed);
    //      digitalWrite(step_pin, LOW);
    //      delayMicroseconds(delay_for_speed);
    //    }

    void make_a_step()
    {
      unsigned long current_time = micros();
      if (current_time - last_step_time >= delay_for_speed)
      {
        if(next_step_high_or_low)
        {
          digitalWrite(step_pin, HIGH);
          if(dir_pin == 2)
            Serial.println("dMaking a  + step");
        }
        else
        {
          digitalWrite(step_pin, LOW);
          if(dir_pin == 2)
            Serial.println("dMaking a  - step");
        }
        next_step_high_or_low = !next_step_high_or_low;
        last_step_time = micros(); // Caution: does not account for costs in step()
      }
    }

};

StepperMotor left_motor(2, 3);
StepperMotor right_motor(4, 5);

void setup() {
  Serial.begin(115200);
  // Declare pins as output:
  left_motor.setup();
  right_motor.setup();
  left_motor.set_speed(50);
  right_motor.set_speed(50);
}

void loop() {
  
 left_motor.make_a_step();
 right_motor.make_a_step();


}
