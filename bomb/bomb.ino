#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <Keypad.h>

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3' ,'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'#','0','*', 'D'}
};
byte pin_rows[4]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[4] = {16, 4, 2,15};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, rows, cols );
hw_timer_t * timer = NULL;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int minutes = 1;
int seconds = 0 ;

int challange = 1;

void IRAM_ATTR onTimer(){
  if(minutes != 0 || seconds != 0 ){
  if(seconds == 0){
    seconds = 60;
    minutes --;
  }
  seconds -- ;
  }


}
const int TOTAL_WIRES = 3;

const int CUTTABLE_WIRES[TOTAL_WIRES] = {33 ,25 ,32};
bool wireStates[TOTAL_WIRES];


void setup()
{
  Serial.begin(19200);

  for (int i=0; i<TOTAL_WIRES; i++) {
    pinMode(CUTTABLE_WIRES[i], INPUT_PULLUP);
    wireStates[i] = LOW; // wires are initially tied to GND
  }

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

  PrintTimer();

  if (challange == 1){
  game1();
  }


  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }



}

void game1(){

display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,20);
display.println("Cut the Right Wire ");


  if (digitalRead(CUTTABLE_WIRES[1]) == 1) {
    // Win
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(30,50);
    display.println("Great!!");
    display.display();
    delay(2000);
    challange++;
  }else if(digitalRead(CUTTABLE_WIRES[0]) == 1 || digitalRead(CUTTABLE_WIRES[2]) == 1) {
lose();
}

}


void lose(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("Dunya Dies!!");
  display.display();
  challange = 0 ;
  while (true){}
}




void PrintTimer(){

  if(minutes != 0 || seconds != 0 ){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55,0);
  display.println(minutes);
  display.setCursor(60,0);
  display.println(":");
  display.setCursor(65,0);
  display.println(seconds);


  display.display();
  display.clearDisplay();
  }else{

   lose();

  }


}