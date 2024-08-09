
#include <Arduino.h>
#include "ESP32OTAPull.h"

#define JSON_URL   "n" // this is where you'll post your JSON filter file
#define SSID 	   "SELECTRONICS"
#define PASS       "CSSAC202"
#define VERSION    "1.0.0" // The current version of this program

const char *errtext(int code);

void DisplayInfo();

void callback(int offset, int totallength);

void setup()
{
	Serial.begin(115200);
	delay(2000); // wait for ESP32 Serial to stabilize
#if defined(LED_BUILTIN)
	pinMode(LED_BUILTIN, OUTPUT);
#endif

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

	ota.SetCallback(callback);
	Serial.printf("We are running version %s of the sketch, Board='%s', Device='%s'.\n", VERSION, ARDUINO_BOARD, WiFi.macAddress().c_str());
	Serial.printf("Checking %s to see if an update is available...\n", JSON_URL);
	int ret = ota.CheckForOTAUpdate(JSON_URL, VERSION);
	Serial.printf("CheckForOTAUpdate returned %d (%s)\n\n", ret, errtext(ret));

	delay(3000);

	// Second example: update *will* happen because we are pretending we have an earlier version
	Serial.printf("But if we pretend like we're running version 0.0.0, we SHOULD see an update happen.\n");
	ret = ota.CheckForOTAUpdate(JSON_URL, "0.0.0");
	Serial.printf("(If the update succeeds, the reboot should prevent us ever getting here.)\n");
	Serial.printf("CheckOTAForUpdate returned %d (%s)\n\n", ret, errtext(ret));
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
	snprintf(exampleImageURL, sizeof(exampleImageURL), "https://example.com/Basic-OTA-Example-%s-%s.bin", ARDUINO_BOARD, VERSION);

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
#if defined(LED_BUILTIN) // flicker LED on update
	static int status = LOW;
	status = status == LOW && offset < totallength ? HIGH : LOW;
	digitalWrite(LED_BUILTIN, status);
#endif
}
