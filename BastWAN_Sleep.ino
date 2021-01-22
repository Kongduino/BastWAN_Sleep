#include <SPI.h>
#include <LoRa.h>
#define REG_OCP 0x0B
#define REG_PA_CONFIG 0x09
#define REG_LNA 0x0c
#define REG_OP_MODE 0x01
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_MODEM_CONFIG_3 0x26
#define REG_PA_DAC 0x4D
#define PA_DAC_HIGH 0x87

#define RFM_TCXO (40u)
#define RFM_SWITCH (41u)

#include <ArduinoLowPower.h>
// Click here to get the library: http://librarymanager/All#Power
const int pin = 8;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("\n\nDeep Sleep");
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pin, INPUT_PULLUP);
  LowPower.attachInterruptWakeup(pin, dummy, CHANGE);
  SerialUSB.print("Starting LoRa");
  pinMode(RFM_TCXO, OUTPUT);
  pinMode(RFM_SWITCH, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  LoRa.setPins(SS, RFM_RST, RFM_DIO0);
  if (!LoRa.begin(863125000)) {
    SerialUSB.println("Starting LoRa failed!\nNow that's disappointing...");
    while (1);
  }
  LoRa.setSpreadingFactor(10);
  LoRa.setSignalBandwidth(250e3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  digitalWrite(RFM_SWITCH, HIGH);
  LoRa.writeRegister(REG_PA_CONFIG, 0b11111111);
  LoRa.writeRegister(REG_PA_DAC, PA_DAC_HIGH);
  LoRa.writeRegister(REG_OCP, 0b00111111); // MAX OCP
  digitalWrite(RFM_SWITCH, LOW);
  digitalWrite(RFM_TCXO, HIGH);
  SerialUSB.println(" done!");
  SerialUSB.print("Sending packet");
  LoRa.beginPacket();
  LoRa.write((const uint8_t*)"Hole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho HoHole Hola Hole Ho Ho", 240);
  LoRa.endPacket();
  SerialUSB.println(" done!");
  digitalWrite(RFM_SWITCH, LOW);
  digitalWrite(RFM_TCXO, LOW);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  LowPower.deepSleep();
}

void dummy() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will be used in the loop)
  // Remember to avoid calling delay() and long running functions since this functions executes in interrupt context
  Serial.println("\n\ndummy");
}
