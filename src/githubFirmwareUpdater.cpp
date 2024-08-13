#include "githubFirmwareUpdater.hpp"
#include <Arduino.h>


GithubFirmwareUpdater::GithubFirmwareUpdater(void){

}

void GithubFirmwareUpdater::init(){
  this->interval = 1000;
  WiFi.mode(WIFI_STA);
	WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname("EnvSens");
	// Delete old configuration
	WiFi.disconnect(true);

	WiFi.onEvent(onConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
	WiFi.onEvent(onAddressed, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
	WiFi.onEvent(onDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

}

void GithubFirmwareUpdater::onConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
	Serial.print("WiFi.connected\n");
}

void GithubFirmwareUpdater::onAddressed(WiFiEvent_t event, WiFiEventInfo_t info) {
	digitalWrite(9, HIGH);
	Serial.print("WiFi.addressed: ");
	Serial.println(WiFi.localIP());
}

void GithubFirmwareUpdater::onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
	digitalWrite(9, LOW);
	Serial.print("WiFi.disconnected\n");

	// Reattempt the connection
	WiFi.reconnect();
}
void GithubFirmwareUpdater::setCredentials(String ssid, String pass){
  this->SSID = ssid;
  this->PASSWORD = pass;
}
void GithubFirmwareUpdater::setNetworkHostname(String newhostname){
	WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  	WiFi.setHostname(newhostname.c_str());
}

void GithubFirmwareUpdater::connect() {
	if(this->SSID == "" || this->PASSWORD == "") {
		Serial.print("No net credentials.");
		return;
	}

	// Reset the counter
	this->remainingAttempts = MAX_ATTEMPTS_QUANTITY;
  // Attempt the connection
	WiFi.begin("SELECTRONICS","CSSAC202");
}

void GithubFirmwareUpdater::repeatedCall() {

  this->currentMillis = millis();

  if ((currentMillis - this->previousMillis) >= this->interval){
    //save the last time you blinked the LED
    previousMillis = currentMillis;
    if (this->firmwareVersionCheck()) {
      this->firmwareUpdate();
    }
  }
}

void GithubFirmwareUpdater::connect_wifi() {
  Serial.println("Waiting for WiFi");
  WiFi.begin("SELECTRONICS", "CSSAC202");
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(1000);
    WiFi.reconnect();
    Serial.println("Trying connection to wifi");
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected");
  }
}

void GithubFirmwareUpdater::firmwareUpdate(void) {
  WiFiClientSecure client;
  client.setCACert(this->rootCACertificate);
  httpUpdate.setLedPin(9, LOW);
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

int GithubFirmwareUpdater::firmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if(client){
    client->setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin( * client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      vTaskDelay(1000);
      httpCode = https.GET();
      vTaskDelay(1000);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      } else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK){
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return 0;
    }
    else
    {
      Serial.println(payload);
      Serial.println("New firmware detected");
      return 1;
    }
  }
  return 0;
}