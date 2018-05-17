#include <WiFi.h>
#include <WiFiUdp.h>

/**
   Diese #defines ggf ändern.
*/
#define MAX_GROESSE_UDP   255
#define UDP_Empfangsport  1234
#define UDP_Sendeport     6565

#define ssid              "UDP-ESP32"
#define password          "12345678"
#define BROADCAST         "192.168.4.255"

String recev;
WiFiUDP Udp;

TaskHandle_t udpTaskHandle;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 8);
  Udp.begin(UDP_Empfangsport);

  xTaskCreatePinnedToCore(&udpTask, "udpTask", 4096, NULL, 1, &udpTaskHandle, 0);
}

/**
   Sendet Datenpaket an alle verbundenen Clients an port UDP_Sendeport des AP (Broadcast)
*/
void send(String pNachricht) {
  Udp.beginPacket(BROADCAST, UDP_Sendeport);
  Udp.println(pNachricht);
  Udp.endPacket();
}

/**
   Sendet Datenpaket an alle verbundenen Clients an port pPort des AP (Broadcast)
*/
void send(String pNachricht, uint16_t pPort) {
  Udp.beginPacket(BROADCAST, pPort);
  Udp.println(pNachricht);
  Udp.endPacket();
}

/**
   Sendet Datenpaket an Client der IP pZielAdresse und Port UDP_Sendeport
*/
void send(String pNachricht, IPAddress pZielAdresse) {
  Udp.beginPacket(pZielAdresse, UDP_Sendeport);
  Udp.println(pNachricht);
  Udp.endPacket();
}

/**
   Sendet Datenpaket an Client der IP pZielAdresse und Port pZielPort
*/
void send(String pNachricht, IPAddress pZielAdresse, uint16_t pZielPort) {
  Udp.beginPacket(pZielAdresse, pZielPort);
  Udp.println(pNachricht);
  Udp.endPacket();
}

/**
  Antwortet auf ein empfangenes Datenpaket von einem Client an dessen IP sowie Port.
*/
void antworten(String pNachricht) {
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.println(pNachricht);
  Udp.endPacket();
}

/*
   udpTask lauscht auf zweitem Prozessorkern.
   Reagiert auf eingehenden Datenverkehr.
*/
void udpTask(void *pvParameters) {
  while (1) {
    char incomingPacket[MAX_GROESSE_UDP];
    int packetSize = Udp.parsePacket();
    if (packetSize) {

      int len = Udp.read(incomingPacket, 255);

      if (len > 0)
        incomingPacket[len] = 0;

      recev = String(incomingPacket);
      recev.trim();
      //Ab hier steht recev zur freien Verfügung um analysiert zu werden o.ä..

      //Hier werdne die empfangenen Daten an die Serielle Schnitstelle gesendet.
      Serial.println("Empfang: " + recev);

      //Antworten mit Echo als Präfix
      antworten("Echo " + recev);
    }
    delay(20);
  }
}

void loop() {
  /*
    Wird für UDP nicht benötigt, steht frei zu Verfügung für Sensoren etc.
  */
  delay(1000);
}
