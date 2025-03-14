#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h> // Library untuk parsing JSON

// Ganti dengan kredensial WiFi Anda
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

// URL API GitHub untuk mendapatkan release terbaru
const char* githubApiUrl = "https://api.github.com/repos/pemilik_repo/nama_repo/releases/latest";

void setup() {
  Serial.begin(115200);

  // Mulai koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Dapatkan URL firmware terbaru
  String firmwareUrl = getLatestFirmwareUrl();
  if (firmwareUrl != "") {
    Serial.println("URL firmware terbaru: " + firmwareUrl);
    performOTA(firmwareUrl);
  } else {
    Serial.println("Gagal mendapatkan URL firmware terbaru.");
  }
}

void loop() {
  // Tidak ada yang perlu dilakukan di loop
}

String getLatestFirmwareUrl() {
  WiFiClient client;
  HTTPClient http;

  // Kirim request ke GitHub API
  http.begin(client, githubApiUrl);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Response dari GitHub API: " + payload);

    // Parsing JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    // Ambil URL aset (file .bin)
    String firmwareUrl = doc["assets"][0]["browser_download_url"].as<String>();
    http.end();
    return firmwareUrl;
  } else {
    Serial.printf("Gagal mendapatkan data dari GitHub API. Error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return "";
  }
}

void performOTA(String url) {
  Serial.println("Memulai OTA update...");

  WiFiClient client;
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); // Gunakan LED onboard untuk status update

  // Lakukan update
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);

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
