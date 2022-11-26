#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <stdio.h>

// time in [ms] of a simulation step
#define TIME_STEP 64

#define MAX_SPEED 6.28

// entry point of the controller
int main(int argc, char **argv) {
  // initialize the Webots API
  wb_robot_init();


  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  
  
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  
  for(int ind = 0 ;ind < 8; ++ind)
  {
   sprintf(prox_sensor_name, "ps%d", ind);
   prox_sensors[ind] = wb_robot_get_device(prox_sensor_name);
   wb_distance_sensor_enable(prox_sensors[ind], TIME_STEP);
  }
  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;
  // feedback loop: step simulation until an exit event is received
  while (wb_robot_step(TIME_STEP) != -1) {
  
  bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
  bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
  bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;
    
    if(front_wall == true)
    {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
    }
    else
    {
      if(left_wall == true){
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
      }
      else
      {
      left_speed = MAX_SPEED/8;
      right_speed = MAX_SPEED;
      }
      if(left_corner == true){
      left_speed =MAX_SPEED;
      right_speed = MAX_SPEED/8;
      }
    }
    
    

    // write actuators inputs
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  // cleanup the Webots API
  wb_robot_cleanup();
  return 0; //EXIT_SUCCESS
}