#include <Wire.h>
#include <M5AtomS3.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;
const int key0 = 39;
const int key1 = 8;
const int key2 = 7;
const int rotary_A = 5;
const int rotary_B = 6;
const int front_button = 41;

const int8_t ENCODER_TABLE[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

int val_key0,val_key1,val_key2,val_a,val_b,new_val,old_val,front_button_value;
int dif = 0;
int key0_count,key1_count,key2_count,front_button_count,scroll_status;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(key0,INPUT);
  pinMode(key1,INPUT);
  pinMode(key2,INPUT);
  pinMode(rotary_A,INPUT);
  pinMode(rotary_B,INPUT);
  pinMode(front_button,INPUT);

  
  AtomS3.begin();
  AtomS3.Lcd.begin();
  AtomS3.Lcd.setRotation(2);
  Wire.begin(38,39);// SDA,SCL
  AtomS3.Lcd.setBrightness(10);

  Keyboard.begin();
  USB.begin();
  Mouse.begin();

  key0_count = 0;
  key1_count = 0;
  key2_count = 0;

  scroll_status = 3;
  front_button_count = 0;
  AtomS3.Lcd.drawString(String(scroll_status), 0,5, 3);
  //attachInterrupt(key1,key0_changed,FALLING);
  //attachInterrupt(rotary_B,rotary,CHANGE);
}

void key0_changed() {
    Serial.println("The Key0 is down ");
    AtomS3.Lcd.drawString("Key0", 0,0, 3);
    Keyboard.print("he Key0 is down");
}

void loop() {
  // put your main code here, to run repeatedly:

  val_key0 = digitalRead(key0);
  val_key1 = digitalRead(key1);
  val_key2 = digitalRead(key2);
  front_button_value = digitalRead(front_button);

  //Serial.println(val_key0);
  
  if(val_key0 == 0) {

    Serial.println("The Key0 is down ");
    AtomS3.Lcd.drawString("Key0", 0,0, 2);
    //Keyboard.print("0");
    Mouse.press(MOUSE_RIGHT);
    delay(100);

    AtomS3.Lcd.drawString("The Key0 is down", 0,0, 2);
  } else {
    Mouse.release(MOUSE_RIGHT);
  }

  if(val_key1 == 0) {


    Serial.println("The Key1 is down ");
    AtomS3.Lcd.drawString("Key1", 0,0, 2);
    //Keyboard.print("1");
    Mouse.press(MOUSE_LEFT);
    delay(100);

    AtomS3.Lcd.drawString("The Key1 is down", 0,0, 2);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  if(val_key2 == 0) {

    Serial.println("The Key2 is down ");
    AtomS3.Lcd.drawString("Key2", 0,0, 2);
    //Keyboard.print("The Key2 is down");
    Mouse.press(MOUSE_MIDDLE);
    delay(100);

    AtomS3.Lcd.drawString("The Key2 is down", 0,0, 2);
  }else {
     Mouse.release(MOUSE_MIDDLE);
  }

  if(front_button_value == 0) {
    front_button_count++;
    
    delay(500);
    if(front_button_count > 2) {
      front_button_count = 0;
    }
  }

  AtomS3.Lcd.drawString(String(front_button_count),5,0,3);

  if(front_button_count == 0) {
    scroll_status = 1;
  } else if(front_button_count == 1) {
    scroll_status = 3;
  } else if(front_button_count == 2) {
    scroll_status = 5;
  }
  
  AtomS3.Lcd.drawString(String(scroll_status), 0,5, 5);
  rotary(scroll_status);
  delay(1);

  Serial.println(dif);

}

void rotary(int scroll_value) {
    if(digitalRead(rotary_A)== HIGH) {
    val_a = 1;
  }else {
    val_a = 0;
  }

  if (digitalRead(rotary_B) == HIGH) {
    val_b = 1;
  }else {
    val_b = 0;
  }
  
  new_val = val_a *2 + val_b;

  if(((new_val<<2)+old_val) == ((0b11 <<2) + 0b01)) {
    dif++;
    Mouse.move(0, 0, scroll_value);
  }
  if(((new_val<<2)+old_val) == ((0b01 <<2) + 0b11)) {
    dif--;
    Mouse.move(0, 0, scroll_value*-1);
  }
  old_val = new_val;
}
