#include <WiFi.h>

const char* ssid = "Vectra-WiFi-52B010";    // Twoje SSID
const char* password = "34379-0001";      // Twoje hasło
WiFiServer server(1234);  // Dowolny port

void setup() {
  Serial.begin(115200);
  Serial.println("Łączenie z WiFi...");
  
  WiFi.begin(ssid, password);  // Połączenie z siecią WiFi

  int retries = 0;  // Zmienna do kontrolowania liczby prób połączenia
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    
    if (retries > 20) {  // Po 10 sekundach zakończ próbę
      Serial.println("Nie udało się połączyć z WiFi!");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nPołączono z WiFi!");
    Serial.print("Adres IP ESP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Brak połączenia z WiFi.");
  }

  server.begin();  // Rozpoczynanie serwera
}

void loop() {
  WiFiClient client = server.available(); // Sprawdzanie, czy klient się połączył
  if (client) {
    Serial.println("Klient połączony");
    int time = 0;
    while (client.connected()) {
      
      
      bool tof1 = random(0, 2);
      bool tof2 = random(0, 2);
      bool tof3 = random(0, 2);
      bool tof4 = random(0, 2);
      bool lS1 = random(0, 2);
      bool lS2 = random(0, 2);
      bool lS3 = random(0, 2);
      int motor1 = random(0, 100);
      int motor2 = random(0, 100);
      float imuX = random(-100, 100) / 10.0;
      float imuY = random(-100, 100) / 10.0;

      String data = String(time) + "," + 
                    String(tof1) + "," + String(tof2) + "," + String(tof3) + "," + String(tof4) + "," + 
                    String(lS1) + "," + String(lS2) + "," + String(lS3) + "," +
                    String(motor1) + "," + String(motor2) + "," +
                    String(imuX, 1) + "," + String(imuY, 1) + "\n";
      time+=500;
      client.print(data);
      delay(500); // Co 100ms
    }
    client.stop();
    Serial.println("Klient rozłączony");
  }
}
