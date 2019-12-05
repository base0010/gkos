#ifndef LCD_H
#define LCD_H
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

#include <qrcode.h>
#include <string>
#include <sstream>


// #include <U8x8lib.h>
// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/15, /* data=*/4, /* reset=*/16);

// std::string authMsg = "Authed With Hub";

enum Feeling
{
  none = 0,
  wink = 50,
  sad = 20,
  happy = 100
};

struct productDefinition{
  std::string name;
  std::string definition;
  int id;
};



class LCD
{
public:


  static void Print(String msg, Feeling emoji = none);
  static void PrintLogo();
  static void CounterSigning();
  void displayQR(uint8_t x_offset, uint8_t y_offset);
  //Messages
  void RecievedPayment(std::string consumerAddress, uint64_t value, bool isETH = false);



  void DeliveringProduct();

  void ReadingStates(int numStates);
  void VerifiedStates(int numStates);

  // void Authed() = LCD::Print(authMsg.c_str())

  };

#endif //ARDUINO_WEB3_CRYP
