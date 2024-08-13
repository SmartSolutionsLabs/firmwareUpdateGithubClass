#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>

#ifndef MAX_ATTEMPTS_QUANTITY
#define MAX_ATTEMPTS_QUANTITY 4
#endif
//#include "variables.h"
//#include "server.h"

#define URL_fw_Version "https://raw.githubusercontent.com/SmartSolutionsLabs/firmwareUpdateGithubClass/main/src/manifestURL.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/SmartSolutionsLabs/firmwareUpdateGithubClass/main/src/firmware.bin"


class GithubFirmwareUpdater{
    private:
        const char* MyHostName;
        uint32_t remainingAttempts;

        String FirmwareVer = {"0.0.1"};
        unsigned long previousMillis;
        unsigned long currentMillis;
        unsigned int interval;
        int num;

    public:

        GithubFirmwareUpdater();

        void init();

        void connect_wifi();

        void firmwareUpdate();

        int firmwareVersionCheck();

        void repeatedCall();

        //AsyncWebServer server;

		static void onConnected(WiFiEvent_t event, WiFiEventInfo_t info);

		static void onAddressed(WiFiEvent_t event, WiFiEventInfo_t info);

		static void onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

		void setNetworkHostname(String hostname);
		/**
		 * Call it when change the credentials.
		 */
		void connect();

        void setCredentials(String ssid, String pass);

};
