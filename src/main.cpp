#include "githubFirmwareUpdater.hpp"
#include <Arduino.h>

GithubFirmwareUpdater board;

void setup(){
    Serial.begin(115200);
    Serial.println("setup try");
    vTaskDelay(5000);
    Serial.println("");
    board.setCredentials("SmartLabs","20120415H");

    board.connect();

    vTaskDelay(5000);
    //board->repeatedCall();

}
void loop(){
     vTaskDelay(5000);
}