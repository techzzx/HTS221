/*
  A simple example to read temperature and humidity value 
  from HTS221 sensor on X-NUCLEO-IKS01A1 board.

  Author:zhixinzhao
  Email:techzzx@foxmail.com
*/
#include <HTS221_IKS01A1.h>

HTS221 hts221;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  if (true == hts221.begin())
  {
    Serial.println("start...");
  }
  else {
    Serial.println("error...Please check device and wiring...");
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  double data;
  data = hts221.readTemperature();
  Serial.print("Temp: ");
  Serial.print(data);
  Serial.println(" C");

  data = hts221.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(data);
  Serial.println(" %");

  delay(1000);
}
