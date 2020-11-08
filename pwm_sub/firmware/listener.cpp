#include <ros.h>
#include <std_msgs/UInt16.h>

#include <Arduino.h>

ros::NodeHandle nh;

uint32_t timeCheck = 0;

void pwmCb( const std_msgs::UInt16& pwm_value){
  OCR1A = pwm_value.data;
  char buffer[100];
  sprintf(buffer, "PWM signal: %u", pwm_value.data);
  nh.loginfo(buffer);
}

ros::Subscriber<std_msgs::UInt16> sub("pwm_signal", &pwmCb );

void setup()
{
  nh.getHardware()->setBaud(115200);
  // Set PB5/6 as outputs. [**Digital pin 11,12 on Arduino Mega!**]
  DDRB |= (1 << DDB5) | (1 << DDB6);

  TCCR1A =
      (1 << COM1A1) | (1 << COM1B1) |
      // Fast PWM mode.
      (1 << WGM11);
  TCCR1B =
      // Fast PWM mode.
      (1 << WGM12) | (1 << WGM13) |
      // No clock prescaling (fastest possible
      // freq).
      (1 << CS10);
  OCR1A = 0;
  // Set the counter value that corresponds to
  // full duty cycle. For 15-bit PWM use
  // 0x7fff, etc. A lower value for ICR1 will
  // allow a faster PWM frequency.
  ICR1 = 0xffff;

  nh.initNode();
  nh.subscribe(sub);
  nh.loginfo("Node booted...");
}

void loop()
{
  if (micros() - timeCheck > 50) {
    nh.spinOnce();
    timeCheck = millis();
  }
}