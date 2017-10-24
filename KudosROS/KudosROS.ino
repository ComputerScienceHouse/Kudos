#include <Servo.h>

#include "ros.h"
#include "geometry_msgs/Twist.h"

#define WHEEL_DIST 0.5

ros::NodeHandle nh;

Servo leftMotor;
Servo rightMotor;

const int PIN_MOTOR_LEFT = 5;
const int PIN_MOTOR_RIGHT = 6;

double x_pos = 0;
double theta = 0;

double wheel_goal_left = 0;
double wheel_goal_right = 0;

void velCallback(const geometry_msgs::Twist& vel);

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel" , velCallback);

void setup() {
    Serial.begin(9600);
    Serial.println("Started setup()");
    Serial.println("Attaching Motors");
    leftMotor.attach(PIN_MOTOR_LEFT);
    rightMotor.attach(PIN_MOTOR_RIGHT);
    Serial.println("setting up ROS communications");
    nh.initNode();
    nh.subscribe(sub);
    Serial.println("Finished setup()");
}

void loop() {
    nh.spinOnce();
    delay(10);
}

//Input on a scale of 0-180
void arcadeDrive(int16_t y, int16_t x) {
    Serial.print("Y power: ");
    Serial.print(y);
    Serial.print(", X power: ");
    Serial.print(x);

    //Set left and right to y power
    int leftPower = y;
    int rightPower = y;

    //Balance x around 0 (range -90 to 90).
    x -= 90;
    leftPower -= x;
    rightPower += x;

    //Trim range for left
    if (leftPower > 180) {
        leftPower = 180;
    } else if (leftPower < 0) {
        leftPower = 0;
    }

    //Trim range for right
    if (rightPower > 180) {
        rightPower = 180;
    } else if (rightPower < 0) {
        rightPower = 0;
    }
    rightPower = 180 - rightPower;

    Serial.print(", Left power: ");
    Serial.print(leftPower);
    Serial.print(", Right power: ");
    Serial.print(rightPower);
      
    leftMotor.write(leftPower);
    rightMotor.write(rightPower);
    
}

void velCallback(const geometry_msgs::Twist& vel){
    x_pos = vel.linear.x;
    theta = vel.angular.z;
    
    wheel_goal_right = (theta*WHEEL_DIST)/2 + x_pos;
    wheel_goal_left = (x_pos * 2) - wheel_goal_right;
    
    Serial.println("goal_left");
    Serial.print(wheel_goal_left);
    Serial.println("goal_right");
    Serial.print(wheel_goal_right);
}

