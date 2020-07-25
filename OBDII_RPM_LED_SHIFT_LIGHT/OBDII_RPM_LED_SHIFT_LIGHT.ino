/*************************************************************************************************
4-digit 7-segment display for RPM

You can pass it different PIDs


***************************************************************************************************/
#include <SPI.h>
#include "mcp_can.h"
#include "SevSeg.h"
SevSeg sevseg;
#include <FastLED.h>

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    8
#define DATA_PIN    A5
#define BRIGHTNESS  200

CRGB leds[NUM_LEDS];

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

#define PID_ENGIN_PRM       0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_COOLANT_TEMP    0x05

#define CAN_ID_PID          0x7DF

unsigned char PID_INPUT;
unsigned char getPid    = 0;

void set_mask_filt() {
    /*
        set mask, set both the mask to 0x3ff
    */
    CAN.init_Mask(0, 0, 0x7FC);
    CAN.init_Mask(1, 0, 0x7FC);

    /*
        set filter, we can receive id from 0x04 ~ 0x09
    */
    CAN.init_Filt(0, 0, 0x7E8);
    CAN.init_Filt(1, 0, 0x7E8);

    CAN.init_Filt(2, 0, 0x7E8);
    CAN.init_Filt(3, 0, 0x7E8);
    CAN.init_Filt(4, 0, 0x7E8);
    CAN.init_Filt(5, 0, 0x7E8);
}

void sendPid(unsigned char __pid) {
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
//    SERIAL.print("SEND PID: 0x");
//    SERIAL.println(__pid, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

void setup() {

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

    byte numDigits = 4;  
    byte digitPins[] = {2, 3, 4, 5};
    byte segmentPins[] = {6, 7, 8, A4, A3, A0, A1, A2};
    bool resistorsOnSegments = 0; 
    // variable above indicates that 4 resistors were placed on the digit pins.
    // set variable to 1 if you want to use 8 resistors on the segment pins.
    sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
    sevseg.setBrightness(90);
  
    SERIAL.begin(115200);
    while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
    set_mask_filt();
    
}

//-------------------------------------MAIN LOOP--------------------------------------/

void loop() {
  
    taskCanRecv();
    taskDbg(); 
    sendPid(PID_ENGIN_PRM);
//    delay(10);


//    if (getPid) {       // GET A PID - checks if there was input then calls sendPID
//        getPid = 0;
//        sendPid(PID_INPUT);
//        PID_INPUT = 0;
//    }
}

void taskCanRecv() {
  
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {                // check if get data
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int rpm =  (uint16_t(buf[3] * 256) + buf[4])/4;
        SERIAL.print("RPM: ");
        SERIAL.print(rpm);


//start setting LEDs ------- 
//OFF: 0-1500
//BLUE: 1500-3000
//GREEN: 3001-4000
//YELLOW: 4001-5000
//RED: 5001-5500
//BLINKING: OVER 5500


        
           if (rpm <= 1500) {                    //sets all LEDs off under 1500 RPM
            leds[0] = CRGB::Black;
            leds[1] = CRGB::Black;
            leds[2] = CRGB::Black;
            leds[3] = CRGB::Black;
            leds[4] = CRGB::Black;
            leds[5] = CRGB::Black;
            leds[6] = CRGB::Black;
            leds[7] = CRGB::Black;
        } else if (rpm <= 2000) {
            leds[0] = CRGB(0, 0, 50);
            leds[1] = CRGB::Black;
        } else if (rpm <= 2500) {
            leds[0, 1] = CRGB(0, 0, 50);
            leds[2] = CRGB::Black;
        } else if (rpm <= 3000) {
            leds[0, 1, 2] = CRGB(0, 0, 50);
            leds[3] = CRGB::Black;
        } else if (rpm <= 3500) {
           leds[0, 1, 2] = CRGB(0, 0, 50);
           leds[3] = CRGB(0, 50, 0);
           leds[4] = CRGB::Black;
        } else if (rpm <= 4000) {
            leds[0, 1, 2] = CRGB(0, 0, 50);
            leds[3, 4] = CRGB(0, 50, 0);
            leds[5] = CRGB::Black;
        } else if (rpm <= 4500) {
            leds[0, 1, 2] = CRGB(0, 0, 50);
            leds[3, 4] = CRGB(0, 50, 0);
            leds[5] = CRGB(25, 25, 0);
            leds[6] = CRGB::Black;
        } else if (rpm <= 5000) {
            leds[0, 1, 2] = CRGB(0, 0, 50);
            leds[3, 4] = CRGB(0, 50, 0);
            leds[5, 6] = CRGB(25, 25, 0);
            leds[7] = CRGB::Black;
        } else if (rpm <= 5500) {
            leds[0, 1, 2] = CRGB(0, 0, 50);
            leds[3, 4] = CRGB(0, 50, 0);
            leds[5, 6] = CRGB(25, 25, 0);
            leds[7] = CRGB(50, 0, 0);
        } else if (rpm >= 5501) {
            for(int dot = 0; dot < NUM_LEDS; dot++) { 
                leds[dot] = CRGB::Red;
                FastLED.show();
                // clear this led for the next time around the loop
                leds[dot] = CRGB::Black;
                delay(30);
               }
                FastLED.show();
            }
        FastLED.show();
        
        sevseg.setNumber(rpm, 0); //displays rpm on 4-digit seven segment
        sevseg.refreshDisplay();
        
//        SERIAL.println("\r\n------------------------------------------------------------------");
//        SERIAL.print("Get Data From id: 0x");
//        SERIAL.println(CAN.getCanId(), HEX);
//        for (int i = 0; i < len; i++) { // print the data
//            SERIAL.print("0x");
//            SERIAL.print(buf[i], HEX);
//            SERIAL.print("\t");
//        }
        SERIAL.println();
    }
}

void taskDbg() {
    while (SERIAL.available()) {
        char c = Serial.read();

        if (c >= '0' && c <= '9') {
            PID_INPUT *= 0x10;
            PID_INPUT += c - '0';

        } else if (c >= 'A' && c <= 'F') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'A';
        } else if (c >= 'a' && c <= 'f') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'a';
        } else if (c == '\n') { // END
            getPid = 1;
        }
    }
}
// END FILE
