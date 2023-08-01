#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

hw_timer_t * timer = NULL;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int minutes = 20;
int seconds = 0 ;

void IRAM_ATTR onTimer(){
  if(seconds == 0){
    seconds = 60;
    minutes --;
  }

  seconds -- ;
}


void setup()
{
  Serial.begin(19200);
  
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  timer = timerBegin(0 , 80 , true);
  timerAttachInterrupt(timer , &onTimer , true);
  timerAlarmWrite(timer , 1000000 , true);
  timerAlarmEnable(timer);

}

void loop() {

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println(minutes);
  display.setCursor(15,28);
  display.println(":");
  display.setCursor(30,28);
  display.println(seconds);

  display.display();
  display.clearDisplay();



}