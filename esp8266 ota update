#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h> // Library WiFiManager

// URL file firmware di GitHub Release
const char* firmwareUrl = "https://github.com/pemilik_repo/nama_repo/releases/download/v1.0/firmware.bin";

void setup() {
  Serial.begin(115200);

  // Inisialisasi WiFiManager
  WiFiManager wifiManager;

  // Menghapus pengaturan WiFi yang tersimpan (opsional)
  // wifiManager.resetSettings();

  // Membuat portal konfigurasi WiFi
  if (!wifiManager.autoConnect("ESP8266-AP")) {
    Serial.println("Gagal terhubung dan timeout terjadi. Restarting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Terhubung ke WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Lakukan OTA update
  performOTA();
}

void loop() {
  // Tidak ada yang perlu dilakukan di loop
}

void performOTA() {
  Serial.println("Memulai OTA update...");

  WiFiClient client;
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); // Gunakan LED onboard untuk status update

  // Lakukan update
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("Update gagal. Error: %s\n", ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("Tidak ada update tersedia.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("Update berhasil. Restarting...");
      ESP.restart();
      break;
  }
}
