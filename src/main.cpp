
#include <Arduino.h>
#include "ESP32OTAPull.h"

#define JSON_URL   "https://raw.githubusercontent.com/SmartSolutionsLabs/firmwareUpdateGithubClass/main/src/manifestURL.json" // this is where you'll post your JSON filter file
#define SSID 	   "SELECTRONICS"
#define PASS       "CSSAC202"
#define VERSION    "0.0.0" // The current version of this program

const char *errtext(int code);

void DisplayInfo();

void callback(int offset, int totallength);

void setup()
{
	Serial.begin(115200);
	delay(2000); // wait for ESP32 Serial to stabilize
  Serial.println("esp32 On");
	DisplayInfo();

	Serial.printf("Connecting to WiFi '%s'...", SSID);
	WiFi.begin(SSID, PASS);
	while (!WiFi.isConnected())
	{
		Serial.print(".");
		delay(250);
	}
	Serial.printf("\n\n");

	// First example: update should NOT occur, because Version string in JSON matches local VERSION value.
	ESP32OTAPull ota;

	Serial.printf("Check for update and download it, but don't reboot.  Display dots.\n");
	int ret = ota
		.SetCallback(callback)
		.AllowDowngrades(true)
		.SetConfig("")
		.CheckForOTAUpdate(JSON_URL, "0.0.0", ESP32OTAPull::UPDATE_AND_BOOT);
	Serial.printf("CheckForOTAUpdate returned %d (%s)\n\n", ret, errtext(ret));

	delay(3000);

}

void loop()
{
}

const char *errtext(int code)
{
	switch(code)
	{
		case ESP32OTAPull::UPDATE_AVAILABLE:
			return "An update is available but wasn't installed";
		case ESP32OTAPull::NO_UPDATE_PROFILE_FOUND:
			return "No profile matches";
		case ESP32OTAPull::NO_UPDATE_AVAILABLE:
			return "Profile matched, but update not applicable";
		case ESP32OTAPull::UPDATE_OK:
			return "An update was done, but no reboot";
		case ESP32OTAPull::HTTP_FAILED:
			return "HTTP GET failure";
		case ESP32OTAPull::WRITE_ERROR:
			return "Write error";
		case ESP32OTAPull::JSON_PROBLEM:
			return "Invalid JSON";
		case ESP32OTAPull::OTA_UPDATE_FAIL:
			return "Update fail (no OTA partition?)";
		default:
			if (code > 0)
				return "Unexpected HTTP response code";
			break;
	}
	return "Unknown error";
}


void DisplayInfo()
{
	char exampleImageURL[256];
	snprintf(exampleImageURL, sizeof(exampleImageURL), 
		"https://github.com/SmartSolutionsLabs/firmwareUpdateGithubClass/releases/download/%s/firmware.bin",
			//ARDUINO_BOARD, 
			VERSION);

	Serial.printf("Basic-OTA-Example v%s\n", VERSION);
	Serial.printf("You need to post a JSON (text) file similar to this:\n");
	Serial.printf("{\n");
	Serial.printf("  \"Configurations\": [\n");
	Serial.printf("    {\n");
	Serial.printf("      \"Board\": \"%s\",\n", ARDUINO_BOARD);
	Serial.printf("      \"Device\": \"%s\",\n", WiFi.macAddress().c_str());
	Serial.printf("      \"Version\": %s,\n", VERSION);
	Serial.printf("      \"URL\": \"%s\"\n", exampleImageURL);
	Serial.printf("    }\n");
	Serial.printf("  ]\n");
	Serial.printf("}\n");
	Serial.printf("\n");
	Serial.printf("(Board, Device, Config, and Version are all *optional*.)\n");
	Serial.printf("\n");
	Serial.printf("Post the JSON at, e.g., %s\n", JSON_URL);
	Serial.printf("Post the compiled bin at, e.g., %s\n\n", exampleImageURL);
}

void callback(int offset, int totallength)
{
	Serial.printf("Updating %d of %d (%02d%%)...\n", offset, totallength, 100 * offset / totallength);
}
