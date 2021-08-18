
//for i2c
#include <Wire.h>
#include <XInput.h>
const int ADC_Max = 90;  // 10 bit
const int ADC_Max_Trigger = 1023;

int gyro_x, gyro_y, gyro_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;

long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;


long loop_timer;
int temp;


// Trigger Pins
const int Pin_TriggerL = A0;
const int Pin_TriggerR = A1;

// Button Pins
const int Pin_ButtonA = 4;
const int Pin_ButtonB = 5;
const int Pin_ButtonX = 6;
const int Pin_ButtonY = 7;

// Directional Pad Pins
const int Pin_DpadUp    = 8;
const int Pin_DpadDown  = 9;
const int Pin_DpadLeft  = 10;
const int Pin_DpadRight = 11;

void setup() {

  // Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_TriggerL, INPUT_PULLUP);
  pinMode(Pin_TriggerR, INPUT_PULLUP);

  // Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_ButtonA, INPUT_PULLUP);
  pinMode(Pin_ButtonB, INPUT_PULLUP);
  pinMode(Pin_ButtonX, INPUT_PULLUP);
  pinMode(Pin_ButtonY, INPUT_PULLUP);

  pinMode(Pin_DpadUp, INPUT_PULLUP);
  pinMode(Pin_DpadDown, INPUT_PULLUP);
  pinMode(Pin_DpadLeft, INPUT_PULLUP);
  pinMode(Pin_DpadRight, INPUT_PULLUP);

  
  XInput.setJoystickRange(-ADC_Max, ADC_Max);  // Set joystick range to the ADC
  XInput.setAutoSend(false);  // Wait for all controls before sending
  XInput.setTriggerRange(0, ADC_Max_Trigger);
  XInput.begin();
  
  Wire.begin();
                                                       
  setup_mpu_6050_registers(); 
                                            
  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  
    read_mpu_6050_data();                                            
        gyro_x_cal += gyro_x;                                             
    gyro_y_cal += gyro_y;                                           
    gyro_z_cal += gyro_z;                                          
    delay(3);                                                          
  }

  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;
  
  loop_timer = micros();                                               
}

void loop(){

  read_mpu_6050_data();
     
  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                

  angle_pitch += gyro_x * 0.0000611;  
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z)); 
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296; 
  angle_pitch_acc -= 0.0;
  if(set_gyro_angles){                  
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;       
  }
  else{                                                          
    angle_pitch = angle_pitch_acc;                                      
    set_gyro_angles = true;                                            
  }
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1; 

  int leftJoyX = constrain(angle_pitch_output, -ADC_Max, ADC_Max);
  XInput.setJoystick(JOY_LEFT, -leftJoyX, 0);
  int triggerLeft  = 1023 - analogRead(Pin_TriggerL);
  int triggerRight = 1023 - analogRead(Pin_TriggerR);
  XInput.setTrigger(TRIGGER_LEFT, triggerLeft);
  XInput.setTrigger(TRIGGER_RIGHT, triggerRight);
  boolean buttonA = !digitalRead(Pin_ButtonA);
  boolean buttonB = !digitalRead(Pin_ButtonB);
  boolean buttonX = !digitalRead(Pin_ButtonX);
  boolean buttonY = !digitalRead(Pin_ButtonY);
  boolean dpadUp    = !digitalRead(Pin_DpadUp);
  boolean dpadDown  = !digitalRead(Pin_DpadDown);
  boolean dpadLeft  = !digitalRead(Pin_DpadLeft);
  boolean dpadRight = !digitalRead(Pin_DpadRight);
  XInput.setButton(BUTTON_A, buttonA);
  XInput.setButton(BUTTON_B, buttonB);
  XInput.setButton(BUTTON_X, buttonX);
  XInput.setButton(BUTTON_Y, buttonY);
  XInput.setDpad(dpadUp, dpadDown, dpadLeft, dpadRight);
  XInput.send();

 while(micros() - loop_timer < 4000);                                
 loop_timer = micros();
  
}

void setup_mpu_6050_registers(){
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x6B);                                                   
  Wire.write(0x00);                                                
  Wire.endTransmission(); 
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x1C);                                                   
  Wire.write(0x10);                                                   
  Wire.endTransmission(); 
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x1B);                                                    
  Wire.write(0x08);                                                 
  Wire.endTransmission(); 
                                              
}


void read_mpu_6050_data(){                                        
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x3B);                                                
  Wire.endTransmission();                                 
  Wire.requestFrom(0x68,14);                                        
  while(Wire.available() < 14);                                    
  acc_x = Wire.read()<<8|Wire.read();                                  
  acc_y = Wire.read()<<8|Wire.read();                                  
  acc_z = Wire.read()<<8|Wire.read();                                  
  temp = Wire.read()<<8|Wire.read();                                   
  gyro_x = Wire.read()<<8|Wire.read();                                 
  gyro_y = Wire.read()<<8|Wire.read();                                 
  gyro_z = Wire.read()<<8|Wire.read();                                 
}
