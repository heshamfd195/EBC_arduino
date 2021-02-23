#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ________________   LCD Setup ___________________
LiquidCrystal_I2C lcd(0x27, 16, 2);

int hall_pin = 11; 



float hall_thresh = 6.0;  // Set the value for RPM Accuracy
float rpm_val;           
float a; 
int spd = 0;              // Speed
int actual_rpm = 0;       


//_____________________ Sensor Declaration _________________
int SensorValue_actual = 0;
int SensorValue_old = 0;
int SensorValue_new = 0;
int SensorVal_step = 10;
int init_speed = 225;
int PWMpin=9;
int pot=A1;
double init_val = 420;
bool run_status = false;

void setup() { 


// Timer for Arduino Mega pin 9
TCCR2B = TCCR2B & B11111000 | B00000010;
  pinMode(PWMpin,OUTPUT);
  pinMode(pot,INPUT);
  Serial.begin(115200); 

// Setup for Hall sensor

  pinMode(hall_pin, INPUT); 
   
  // Sensor requires pull up resistor
  digitalWrite(hall_pin, HIGH);
 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Tachometer V1");

 

    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
   
  lcd.print(spd);
  lcd.print(" km/h ");
   lcd.setCursor(0, 1);
  lcd.print(actual_rpm);
  lcd.print ("          ");
}


 


void loop() { 

  //Serial.println(SensorValue_new);
  SensorValue_new = analogRead(pot);
  //Serial.println(SensorValue_new);
  while (true) { 
              SensorValue_new = analogRead(pot);
              if (SensorValue_new > 470) {run_status = true; break;}
              run_status = false; 
              SensorValue_actual = 0; 
              SensorValue_old = 0; 
              SensorValue_new = 0; 
              analogWrite(PWMpin,SensorValue_actual);
              Serial.println("Infinite loop");
              }
  
  
   

if (run_status==true)  {

//Serial.println(spd);
if (spd == 0) 
{ 
  SensorValue_actual = init_speed; 
SensorValue_old = init_speed; 
SensorValue_new = init_speed; 
analogWrite(PWMpin,SensorValue_actual);   
delay(3000); 
Serial.println("Done first time"); 
}

else {
  
  if (SensorValue_new <= 634 && SensorValue_new >= 470)
  { 
    SensorValue_new = 0.4573*SensorValue_new - 35;
    }
  else if (SensorValue_new < 470) 
  {
    SensorValue_new = 0; 
    }
  else if (SensorValue_new > 634) 
  {
    SensorValue_new = 255;  
    }

      Serial.println(SensorValue_old);
      Serial.println(SensorValue_new);
    
  if ((SensorValue_new - SensorValue_old) >= SensorVal_step) 
    { 
      if (SensorValue_old + SensorVal_step >= 255) 
      { 
        SensorValue_actual = 255; 
        SensorValue_new = SensorValue_actual; 
        SensorValue_old = SensorValue_new; 
        analogWrite(PWMpin,SensorValue_actual);
        }
      else  {  
        SensorValue_actual = SensorValue_old + SensorVal_step; 
        SensorValue_new = SensorValue_actual;
        SensorValue_old = SensorValue_new; 
        analogWrite(PWMpin,SensorValue_actual); 
        }  
                                                              
      }
  else if ((SensorValue_old - SensorValue_new) >= SensorVal_step) { 
            if (SensorValue_old - SensorVal_step <= 180) 
            { 
              SensorValue_actual = 180; SensorValue_new = SensorValue_actual; 
              SensorValue_old = SensorValue_new; analogWrite(PWMpin,SensorValue_actual);
              }
            else { SensorValue_actual = SensorValue_old - SensorVal_step; 
            SensorValue_new = SensorValue_actual; 
            SensorValue_old = SensorValue_new; 
            analogWrite(PWMpin,SensorValue_actual); 
            Serial.println(SensorValue_actual); 
            }
          }
                                                                    
    else {
    SensorValue_actual = SensorValue_new; 
    SensorValue_new = SensorValue_actual; 
    SensorValue_old = SensorValue_new; 
    analogWrite(PWMpin,SensorValue_actual);
    }


}  

//Serial.println(SensorValue_new);

}
  
   
  // preallocate values for tach 

  float hall_count = 0.0; 
  float start = micros(); 
  bool on_state = false; 
 a = 0; 

  // counting number of times the hall sensor is tripped 
  // but without double counting during the same trip 
  while(true){ 
    a = a+1; 
    if (digitalRead(hall_pin)==0){ 
      if (on_state==false){ 
        on_state = true; 
        hall_count+=1.0; 
          } 

    } else { 
      on_state = false; 
    } 

    if (a>=300000){ 
      //Serial.println(hall_count);
      //Serial.println("Not counting");  
      rpm_val = 0; 
      hall_count = 0;
       
      break;     } 

    if (hall_count>=hall_thresh){ 
      break;     } 

  } 
    // print information about Time and RPM 

  float end_time = micros(); 
  float time_passed = ((end_time-start)/1000000.0); 


  rpm_val = (hall_count/time_passed)*60.0;


  spd = int((((rpm_val)/3.0)*2*3.14*0.26*60.0/1000.0));
  actual_rpm = (rpm_val)/3.0;


  // Display the result  
  lcd.setCursor(0, 0);
   
  lcd.print(spd);
  lcd.print(" km/h ");
   lcd.setCursor(0, 1);
  lcd.print(actual_rpm);
  lcd.print ("          ");
  
} 
