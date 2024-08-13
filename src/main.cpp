#include "githubFirmwareUpdater.hpp"
#include <Arduino.h>

GithubFirmwareUpdater board;

void setup(){
    Serial.begin(115200);
    Serial.println("setup try");
    vTaskDelay(1000);
    Serial.println("");
    board.init();
    board.setCredentials("SELECTRONICS","CSSAC202");

    board.connect();

}
void loop(){
    board.repeatedCall();
    Serial.println("despedite de tu Firmware Lince ");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}