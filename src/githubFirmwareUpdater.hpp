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
#define URL_fw_Bin "https://github.com/SmartSolutionsLabs/firmwareUpdateGithubClass/blob/main/src/firmware.bin"


class GithubFirmwareUpdater{
    private:
        const char* MyHostName;
        uint32_t remainingAttempts;

        String FirmwareVer = {"0.0.1"};
        unsigned long previousMillis;
        unsigned long currentMillis;
        unsigned int interval;
        int num;
        const char * rootCACertificate = \
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
        "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
        "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
        "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
        "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
        "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
        "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
        "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
        "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
        "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
        "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
        "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
        "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
        "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
        "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
        "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
        "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
        "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
        "+OkuE6N36B9K\n"
        "-----END CERTIFICATE-----\n";
        // credentials caché
		String SSID;
		String PASSWORD;
    public:

        GithubFirmwareUpdater();

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
