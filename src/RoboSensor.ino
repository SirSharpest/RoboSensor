#include <Arduino.h>
#include "LedControl.h"
#include <DHT.h>


/*
 *
 * Basic code for handling a little robot of mine
 * this code is under construction and very volitile as I test a lot of things
 * Uses the DHT11 library which is an open license
 *
 */


#define DHTPIN 7     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

//Create object for temp
DHT dht(DHTPIN, DHTTYPE);

//Create object for LedControl
LedControl lc=LedControl(12,11,10,1);


void setup() {
    Serial.begin(9600);
    dht.begin();


    /*
     * Create a new LedControl.
     * We use pins 12,11 and 10 on the Arduino for the SPI interface
     * Pin 12 is connected to the DATA IN-pin of the first MAX7221
     * Pin 11 is connected to the CLK-pin of the first MAX7221
     * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
     * There will only be a single MAX7221 attached to the arduino
     */

    //Wake up
    lc.shutdown(0, false);

    //Set a medium brightness
    lc.setIntensity(0, 7);



}

void loop() {

    readTemp();
    delay(1000);

}

//leftPanel = true if print on the left
//false if you wanna print on the right
void printNumber(int v, bool leftPanel) {
    int ones;
    int tens;
    int hundreds;
    boolean negative = false;

    if(v < -999 || v > 999)
       return;
    if(v<0) {
        negative=true;
        v=v*-1;
    }
    ones=v%10;
    v=v/10;
    tens=v%10;
    v=v/10;
    hundreds=v;

    int pos0, pos1, pos2, pos3;

    pos0 = 0;
    pos1 = 1;
    pos2 = 2;
    pos3 = 3;

    if(leftPanel){
      pos0 += 4;
      pos1 += 4;
      pos2 += 4;
      pos3 += 4;
    }

    if(negative) {
       //print character '-' in the leftmost column
       lc.setChar(0,pos3,'-',false);
    }
    else {
       //print a blank in the sign column
       lc.setChar(0,pos3,' ',false);
    }
    //Now print the number digit by digit
    lc.setDigit(0,pos2,(byte)hundreds,false);
    lc.setDigit(0,pos1,(byte)tens,false);
    lc.setDigit(0,pos0,(byte)ones,false);
}



void readTemp(){

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    printNumber(t, false);
    printNumber(h, true);


}
