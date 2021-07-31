#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <string.h>

const char *ssid = "Frontier1312";
const char *password = "2891547465";

ESP8266WebServer server(80);

int alarmHour, alarmMin, state = 0, timeCounter = 0, threshold = 400;

#define LIGHT 4
#define LIGHT_BUTTON 5

void setup() {
    Serial.begin(115200);
    setWifi();
    setOn();
    server.begin();

    pinMode(LIGHT, OUTPUT);
    pinMode(LIGHT_BUTTON, INPUT);
}

void loop() {
    server.handleClient();
    checkState();
} 


void setWifi(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}

void setOn(){
    server.on ("/", handleRoot);
    server.on ("/input", [](){
        String rawAlarm = server.arg("input");
        Serial.print("Alarm set for: ");

        String rawHour = "";
        rawHour += rawAlarm[0];
        rawHour += rawAlarm[1];
        alarmHour = rawHour.toInt();

        String rawMinute = "";
        rawMinute += rawAlarm[3];
        rawMinute += rawAlarm[4];
        alarmMin = rawMinute.toInt();

        Serial.print(alarmHour);
        Serial.print(":");
        Serial.println(alarmMin);
        handleRoot();
    });

    server.on("/toggle", []() {
        toggle();
        handleRoot();
    });
}

void handleRoot() {
    String mainpage = "";
    mainpage += "<!DOCTYPE html>";
    mainpage += "<html>";
    mainpage += "<head>";
    mainpage += "<h1 id=\"header\">Jerome's Fan and Lights</h1>";
    mainpage += "</head>";
    mainpage += "<body>";
    mainpage += "<button type=\"button\" class=\"button\" onclick=\"location.href='/toggle';\">Lights</button>";
    mainpage += "<button type=\"button\" class=\"button\" onclick=\"location.href='/toggle';\">Fan</button>";
    mainpage += "<form action=\"input\">";
    mainpage += "<br>Set Wake Up Time<br>";
    mainpage += "<input type=\"time\" name=\"input\">";
    mainpage += "<input type=\"submit\" value=\"Set Alarm\">";
    mainpage += "</form>";
    mainpage += "</body>";
    mainpage += "</html>";

    mainpage += "<style>";
    mainpage += ".button {";
    mainpage += "border-radius: 8px;";
    mainpage += "border: none;";
    mainpage += "background-color: #1a73e8;";
    mainpage += "padding: 15px 32px;";
    mainpage += "margin: 2px 6px;";
    mainpage += "cursor: pointer;";
    mainpage += "text-align: center;";
    mainpage += "font-size: 18px;";
    mainpage += "color: black;";
    mainpage += "width: 100%;";
    mainpage += "}";

    mainpage += "body{";
    mainpage += "background-color: rgba(0, 0, 0, 0.1);";
    mainpage += "color:black;";
    mainpage += "}";
    mainpage += "h1 {";
    mainpage += "color:black;";
    mainpage += "text-align:center;";
    mainpage += "}";
    mainpage += "@media (prefers-color-scheme: dark) {";
    mainpage += ".button{";
    mainpage += "color: white;";
    mainpage += "}";
    mainpage += "body{";
    mainpage += "background-color: rgba(0, 0, 0, 0.85);";
    mainpage += "color:white;";
    mainpage += "}";
    mainpage += "h1 {";
    mainpage += "color:white;";
    mainpage += "}";
    mainpage += "}";
    mainpage += "</style>";

    server.send(200, "text/html", mainpage);
}

void checkState(){
    if(digitalRead(LIGHT_BUTTON) == 0 && (millis() - timeCounter) > threshold){
        toggle();
        timeCounter = millis();
    }
}

void toggle(){
    if(state == 1){
        digitalWrite(LIGHT, HIGH);
        state = 0;
    }
    else{
        digitalWrite(LIGHT, LOW);
        state = 1;
    }
}
