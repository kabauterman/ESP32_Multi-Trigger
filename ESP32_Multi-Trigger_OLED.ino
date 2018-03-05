//   Copyright 2018 David Schöppe
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include "SSD1306.h"

//Display init
SSD1306  display(0x3c, 5, 4);

//pin config

const int shutter = 14;
const int focus = 12;
const int ground = 13;

//declare variables

//interval in milliseconds
int in_ms;
//runtime in milliseconds
int rt_ms;
//runtime ms for calulation
int rt_ms_cal = 0;
//start button value returned from website
String start;
//interval value returned from website
int in;
//interval for Display
int in_disp;
//runtime for Display
int rt_disp;
//runtime value returned from website
int rt;
//timer value returned from website
int timer=0;
//timer value in milliseconds
int timer_ms=0;
//shot button value returned from website
String shot;
//bulb button value returned from website
String bulb;


/* change your ssid and password here */
const char* ssid = "Multi-Trigger";
const char* password = "12345678";

ESP32WebServer server(80);

//html code for intervalometer setup page

const char* intervalohtml = "<html><head><title>WLAN Intervalometer</title>"
                            "<style>div.content {border: #B6B6B4 solid 1px;}"
                            "ol#toc {height: 8em;margin: 0;padding: 0;}ol#toc a {font-weight:bold;font-size:30px;text-decoration:none;"
                            "color: #000;float: left;line-height: 400%;width: 8em;text-align:center;padding-right: 10px;}"
                            "ol#toc li.current {background-color: #B6B6B4;}ol#toc li.current a {color: #fff;}ol#toc li {"
                            "background: #E5E4E2;float: left;padding-left: 10px;}body{background:#FFFFFF;}"
                            ".bt{width:90%;padding:20%;text-align:center;color:BLACK;border-radius:50%;font-weight:bold;font-size:35px;}"
                            ".r{background:#E5E4E2;} .g{background:#B6B6B4;} .b{background:#E5E4E2;}"
                            "</style></head><body>"
                                "<ol id='toc'><li class='current'><a href='intervalo'>Intervalometer</a></li><li><a href='trigger'>Trigger</a></li>"
                                "<li><a href='Intervalo'>Reserved</a></li></ol>"
                                "<div class='content'><table width='100%'><colgroup width='50%' span='2'</colgroup>"
                                "<tr><td><form action='rt' method='get'><input class='bt b' type='number' name='rt' value='1' min='1'></td>"
                                "<td><input class='bt b' type='submit' value='Runtime (m)'></form></td></tr>"
                                "<tr><td><form action='in' method='get'><input class='bt b' type='number' name='in' value='1' min='1'></td>"
                                "<td><input class='bt b' type='submit' value='Interval (s)'></form></td></tr>"
                                "<tr><td><form action='start' method='get'><input class='bt g' type='submit' name='start' value='Start'></td>"
                                "<td><input class='bt r' type='submit' name='start' value='Stop'></form></td></tr>"
                              "</table></div>"
                              "</body></html>";

//html code for trigger page

const char* triggerhtml = "<html><head><title>WLAN Intervalometer</title>"
                          "<style>div.content {border: #B6B6B4 solid 1px;}"
                          "ol#toc {height: 8em;margin: 0;padding: 0;}ol#toc a {font-weight:bold;font-size:30px;text-decoration:none;"
                          "color: #000;float: left;line-height: 400%;width: 8em;text-align:center;padding-right: 10px;}"
                          "ol#toc li.current {background-color: #B6B6B4;}ol#toc li.current a {color: #fff;}ol#toc li {"
                          "background: #E5E4E2;float: left;padding-left: 10px;}body{background:#FFFFFF;}"
                          ".bt{width:90%;padding:20%;text-align:center;color:BLACK;border-radius:50%;font-weight:bold;font-size:35px;}"
                          ".r{background:#E5E4E2;} .g{background:#B6B6B4;} .b{background:#E5E4E2;}"
                          "</style></head>"
                           "<body>"
                              "<ol id='toc'><li><a href='intervalo'>Intervalometer</a></li><li class='current'><a href='trigger'>Trigger</a></li>"
                              "<li><a href='intervalo'>Reserved</a></li></ol>"
                              "<div class='content'><table width='100%'><colgroup width='50%' span='2'</colgroup>"
                              "<tr><td><form action='timer' method='get'><input class='bt b' type='number' name='timer' value='1' min='1'></td>"
                              "<td><input class='bt b' type='submit' value='Self Timer (s)'></form></td></tr>"
                              "<tr><td><form action='shot' method='get'><input class='bt g' type='submit' name='shot' value='Shot'></form></td></tr>"
                              "<tr><td><form action='bulb' method='get'><input class='bt g' type='submit' name='bulb' value='BULB-Start'></td>"
                              "<td><input class='bt r' type='submit' name='bulb' value='BULB-Stop'></form></td></tr>"
                            "</table></div>"
                            "</body></html>";

void setup(void){

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Multi-Trigger");
  display.drawString(0, 16, "by yt/Schießstand");
  display.display();
  
  delay(2000);

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "SSID: " + String(ssid));
  display.drawString(0, 16, "PW: " + String(password));
  display.display();
  
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

//start functions on server subsites to get variables

  server.on( "/", handle_intervalo);
  server.on( "/rt", do_rt);
  server.on( "/in", do_in);
  server.on( "/start", do_start);
  server.on( "/intervalo", handle_intervalo);
  server.on( "/trigger", handle_trigger);
  server.on( "/shot", get_trigger_shot);
  server.on( "/timer", get_timer);
  server.on( "/bulb", get_trigger_bulb);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

//define pins
  pinMode(shutter, OUTPUT);
  pinMode(focus, OUTPUT);
  pinMode(ground, OUTPUT);

  digitalWrite(shutter, HIGH);
  digitalWrite(focus, HIGH);
  digitalWrite(ground, LOW);

}

void handleRoot() {
  server.send(200, "text/html", intervalohtml);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void handle_trigger() {
  server.send(200, "text/html", triggerhtml);
  Serial.print(" Trigger Page delivered ");
  delay(100);
}

void handle_intervalo(){

  server.send(200, "text/html", intervalohtml);
  Serial.print(" Intervalometer Page delivered ");
  delay(100);
}

String do_start(){

//get start from server return it to loop

    server.send(200, "text/html", intervalohtml);
    start=server.arg("start");
    Serial.print("Start has been set to: ");
    Serial.println(start);
    return start;
}

int do_rt(){

//get runtime from server, return it to loop

    server.send(200, "text/html", intervalohtml);
    rt=server.arg("rt").toInt();
    Serial.print("Runtime has been set to: ");
    Serial.println(rt);
    
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Runtime has been set to: ");
    display.drawString(110, 16, String(rt) + "m");
    
    display.display();
    
   
    return rt;
}

int do_in(){

//get interval from server, return it to loop

    server.send(200, "text/html", intervalohtml);
    in=server.arg("in").toInt();
    Serial.print("Interval has been set to: ");
    Serial.println(in);

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Interval has been set to: ");
    display.drawString(110, 16, String(in) + "s");
    
    display.display();
    
    return in;
}

int get_timer(){

//get timer from server, return it to loop

    server.send(200, "text/html", triggerhtml);
    timer=server.arg("timer").toInt();
    Serial.print("Timer has been set to: ");
    Serial.println(timer);

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Timer has been set to: ");
    display.drawString(110, 0, String(timer) + "s");
    
    display.display();
    
    return timer;
}

String get_trigger_shot(){

//get shot from server trigger page return it to loop

    server.send(200, "text/html", triggerhtml);
    shot=server.arg("shot");
    Serial.print("Shot has been set to: ");
    Serial.println(shot);
    return shot;
}

String get_trigger_bulb(){

//get bulb from server trigger page return it to loop

    server.send(200, "text/html", triggerhtml);
    bulb=server.arg("bulb");
    Serial.print("BULB has been set to: ");
    Serial.println(bulb);
    return bulb;
}

void do_bulb(){

// start BULB Exposure when bulb=BULB-Start

 if (bulb == "BULB-Start"){
    Serial.print("BULB Exposure started, ");

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "BULB Exposure started, ");
    
    display.display();
    
    digitalWrite(shutter, LOW);
    digitalWrite(focus, LOW);
    delay(50);
    Serial.print("Okay, all to GND... ");
 }

 if (bulb == "BULB-Stop"){
      Serial.print(" End BULB Exposure, ");

      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 0, " End BULB Exposure, ");
    
      display.display();
      
      digitalWrite(shutter, HIGH);
      digitalWrite(focus, HIGH);
      delay(50);
      Serial.print("Okay, all to HIGH... ");
 }
  
}

int do_shot(){

//take one shot in interval

    display.clear();

    Serial.print("Remaining Runtime: ");
    Serial.println((rt_ms / 1000));

    Serial.print("All to GND... ");
    digitalWrite(shutter, LOW);
    digitalWrite(focus, LOW);
    delay(in_ms/2);
    Serial.print(" All to HIGH... ");
    digitalWrite(shutter, HIGH);
    digitalWrite(focus, HIGH);
    delay(in_ms/2);
    
    Serial.print(" Shot has been taken... ");
    
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Shot has been taken...");
    
    rt_ms = rt_ms - in_ms;
    Serial.print("Remaining Runtime lowered to (s): ");
    
    display.drawString(0, 16, "Runtime lowered to (s): ");
    
    Serial.println((rt_ms / 1000));
    
    
    display.drawString(110, 32, String(rt_ms / 1000));
    
    display.drawString(0, 48, "RT(m): " + String(rt_disp) + " INT(s): " + String(in_disp));
    
    display.display();
    
    return rt_ms;
}

void do_one_shot(){

//take one shot in interval

    Serial.print(" Take a shot ...");
    Serial.print("All to GND... ");
    digitalWrite(focus, LOW);
    delay(100);
    digitalWrite(shutter, LOW);
    delay(100);
    Serial.print(" All to HIGH... ");
    digitalWrite(shutter, HIGH);
    digitalWrite(focus, HIGH);
    delay(50);    
    Serial.print(" Shot has been taken... ");
}

void do_one_timer_shot(){

//take one shot in interval

    Serial.print(" Take a shot in: ");
    Serial.print(timer_ms);
    Serial.print(" Milliseconds ");
    delay(timer_ms);
    Serial.print("All to GND... ");
    digitalWrite(focus, LOW);
    delay(100);
    digitalWrite(shutter, LOW);
    delay(100);
    Serial.print(" All to HIGH... ");
    digitalWrite(shutter, HIGH);
    digitalWrite(focus, HIGH);
    delay(50);    
    Serial.print(" Shot has been taken... ");
}

void loop() {
  server.handleClient();

//calculate runtime(ms) from runtime (min)

  if (rt != 0){
      rt_ms = rt * 60 * 1000;
      rt_disp = rt;
    rt = 0;
  }

//calculate interval(ms) from interval (sec)

  if (in != 0){
      in_ms = in * 1000;
      in_disp = in;
    in = 0;
  }

//calculate timer(ms) from timer (sec)

  if (timer != 0){
      timer_ms = timer * 1000;
    timer = 0;
    do_one_timer_shot();
  }

// check if Intervalometer has been started
  
  if (start == "Start"){ 
    if (rt_ms >= 0){
      do_shot();
      handle_intervalo;
    }
    else if (rt_ms <= 0){
    start = "Stop";
    }
  }
  
  if (start == "Stop"){ 
  handle_intervalo;
  display.clear();
  }

// check if BULB Exposure has been started

  if (bulb == "BULB-Start"){ 
      do_bulb();
      handle_trigger;
  }
  
  if (bulb == "BULB-Stop"){
      do_bulb();
      handle_trigger;
      bulb="finish";
  }

// check if shot has been triggered

  if (shot == "Shot"){
      do_one_shot();
      shot="finish";
      handle_trigger;
  }

  
}
