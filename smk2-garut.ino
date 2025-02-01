#include <WiFi.h>
#include <MQTT.h>
#include <NusabotSimpleTimer.h>
#include <DHTesp.h>

WiFiClient net;
MQTTClient client;
NusabotSimpleTimer timer;
DHTesp dhtsensor;

const char ssid[] = "Wokwi-GUEST";
const char pass[] ="";

const int pin_red = 27;
const int pin_green = 26;
const int pin_blue = 25;
const int pin_relay_dapur = 16;
const int pin_dht = 18;

void connect(){
  rgb(1,0,0);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  rgb(0,1,0);

  while(!client.connect("g0p4p[w9e]")){ // tidak boleh sama
    delay(500);
  }
  rgb(0,0,1);
  client.subscribe("smkn2garut/#");
}

void subscribe(String &topic, String &data){
  Serial.println(topic);
  if(topic == "smkn2garut/dapur"){
    if(data == "Nyala"){
        digitalWrite(pin_relay_dapur, 1);
    } else {
        digitalWrite(pin_relay_dapur, 0);
    }
  }
}

void setup() {
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
  pinMode(pin_relay_dapur, OUTPUT);

  WiFi.begin(ssid, pass);
  client.begin("broker.emqx.io", net);
  client.onMessage(subscribe);
  timer.setInterval(1000, publish);
  dhtsensor.setup(pin_dht, DHTesp:DHT22);
  
  connect();
}

void publish(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  client.publish("smkn2garut/suhurumah", String(data.temperature), true, 1);
}

void loop() {
  timer.run();
  client.loop();     //untuk mengecek apakah ada data yang masuk
  delay(10);
}

void rgb(bool red, bool green, bool blue){
  digitalWrite(pin_red, red);
  digitalWrite(pin_green, green);
  digitalWrite(pin_blue, blue);
}