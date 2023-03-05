/*
  Copyright (c) 2022 Ronald Leenes

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
   @file P1WG2022current.ino
   @author Ronald Leenes
           Haans Schneider
   @date 28.12.2022
         14.02.2023
         03.03.2023
   @version 1.2

   @brief This file contains the main file for the P1 wifi gatewway

   @see http://esp8266thingies.nl
*/

/*
   P1 wifi gateway 2022

   Deze software brengt verschillende componenten bij elkaar in een handzaam pakket.

   De software draait op een ESP8285 of ESP8266 en heeft een bescheiden voetafdruk.

   De data processing is gebaseerd op: http://domoticx.com/p1-poort-slimme-meter-data-naar-domoticz-esp8266/
   De captive portal is gebaseerd op een van de ESP8266 voorbeelden, de OTA eenheid eveneens.

   De module levert data aan een host via JSON of MQTT messages, maar kan ook via de webinterface van de module of via telnet worden uitgelezen

   De module zal bij opstarten eerst de buffercondensator laden.
   Vervolgens knippert hij 2x en zal eerst proberen te koppelen aan je wifi netwerk. Gedurende dat proces zal de LED knipperen
   met een periode van 0.5 Hz.
   Wanneer geen verbinding kan worden gemaakt start de modiule een Access point onder de naam P1_
   Verbind met dit netwerk en surf naar 192.168.4.1

   Het menuscherm van de module verschijnt dan. Vul daar SSID en wachtwoord van je wifi netwerk in.
   Daaronder vul je gegevens van de ontvangede server in (IP en poortnummer). Dit kan je DOmotociz server zijn
   In dat geval moet je ook het IDx dat Domoticz maakt voor een Gas device en een Energy device invoeren.
   Tenslotte geef je aan per hoeveel seconde je data naar de server wilt laten sturen.
   DSMR4 meters hanteren een meetinterval van 10 sec. SMR5 meters leveren iedere seconde data. Dat lijkt me wat veel en
   kost de module ook veel stroom waardoor het maar de vraag is of hij parasitair (door de meter gevoed) kan werken.
   Ik raad een interval van 15 sec of hoger aan. Het interval dat je hier invoert zal niet altijd synchroon lopen met
   het data interval van de meter, ga er dus niet van uit dat je exact iedere x seconden een meetwaarde ziet in DOmoticz.
   Tussen metingen 'slaapt' de module (de modem wordt afgeschakeld waardoor het stroomverbruik van zo'n 70mA terugvalt naar 15 mA).
   Dit geeft de bufferelco tijd omm op te laden voor de stroompiekjes die de wifi zender van de module produceert
   (en het bespaart hoe dan ook wat stroom (die weliswaar door je energieleverancier wordt betaald, maar toch). Alle kleine
   beetjes helpen..

    informatie, vragen, suggesties ed richten aan romix@macuser.nl



    versie: 1.2
    datum:  3 Mar 2023
    auteur: Ronald Leenes
            Hans Schneider

    1.2: bug fixes
         improvements for T211 SmartMeter in Belgium
         implemented support for HTTP input meter in cFos Power Brain wallbox

    1.1aa: bug fixes
    1.1: implemented graphs


    ud: fixed refreshheader
    ua: fixed setup field issue
    u:
      password on Setup and Firmware Update
      made mqtt re-connect non-blocking
      incorporated "DSMR Reader" mqtt topics
      fixed hardboot daycounters reset
      fixed sending empty MQTT messages

    t: improvements on powermanagement, overall (minor) cleanup
    ta: fix for Telnet reporting

    s: added German localisation
          Added mqtt output for Swedish specific OBIS codes

    r: speed improvements and streamlining of the parser
        localisations for: NL, SE

    q: added daily values
    p: incorporated equipmentID in mqtt set
    o: fixed gas output, fixed mqtt reconnect
    n: empty call to SetupSave now redirects to main menu instead of resetting settings ;-)
        fixed kWh/Wh inversion
    m: setupsave fix, relocate to p1wifi.local na 60 sec
        mqtt - kw/W fix
    l: wifireconnect after wifi loss
    k: fixed big BUG, softAP would not produce accessible webserver.
    j: raw data on port 23
        major code rewrite
        implemented data reporting methods:
          parsed data: json, mqtt, p1wifi.local/P1
          raw data: telnet on port 23, p1wifi.local/Data

    i:  extended max line length for TD210-D meters, which have a really loong 1-0:99.97.0 line
    h:  extended mqtt set with instant powers, voltages, actual tarif group. power outages, power drops
    g: fixed mqtt

    Generic ESP8285 module
    Flash Size: 2mb (FS: 128Kb, OTA: –960Kb)

    needed files:
    this one (obv),
    CRC16.h
    JSON.ino
    MQTT.ino
    TELNET.ino
    debug.ino
    decoder.ino
    functions.ino
    graph.ino
    webserver.ino
    webserverDE.ino
    webserverSE.ino
    webserverNL.ino
    wifi.ino
*/
bool zapfiles = false; //false; //true;

#define      GERMAN // GERMAN / FRENCH / NEDERLANDS / SWEDISH

#ifdef GERMAN
  String lang = "DE";
#endif
#ifdef FRENCH
  String lang = "FR";
#endif
#ifdef NEDERLANDS
  String lang = "NL";
#endif
#ifdef SWEDISH
  String lang = "SE";
#endif

String version = "1.2 – "+lang;

#define HOSTNAME "p1meter"

#define GRAPH 1
#define V3
#define DEBUG 1 //0 //1 //0 //1 // 1 is on serial only, 2 is serial + telnet, 
#define ESMR5 1
//#define SLEEP_ENABLED


const uint32_t  wakeTime = 90000; // stay awake wakeTime millisecs
const uint32_t  sleepTime = 5000; //sleep sleepTime millisecs

#if DEBUG == 1
const char* host = "P1test";
#define BLED LED_BUILTIN
#define debug(x) Serial.print(x)
#define debugf(x) Serial.printf(x)
#define debugf(x,y) Serial.printf(x,y)
#define debugff(x,y,z) Serial.printf(x,y,z)
#define debugln(x) Serial.println(x)
#elif DEBUG == 2
#define BLED LED_BUILTIN
#define debug(x) Serial.print(x);if(telnetConnected)telnet.print(x)
#define debugf(x) Serial.printf(x)
#define debugf(x,y) Serial.printf(x,y)
#define debugff(x,y,z) Serial.printf(x,y,z)
#define debugln(x) Serial.println(x);if(telnetConnected)telnet.println(x)
#else
const char* host = "P1wifi";
#define BLED LED_BUILTIN
#define debug(x)
#define debugln(x)
#define debugf(x)
#define debugf(x,y)
#define debugff(x,y,z)
#endif

#define errorHalt(msg) {debugln(F(msg)); return;}

#define NUM(off, mult) ((P1timestamp[(off)] - '0') * (mult) )   // macro for getting time out of timestamp, see decoder

#define ToggleLED  digitalWrite(BLED, !digitalRead(BLED));
#define LEDoff  digitalWrite(BLED, HIGH);
#define LEDon   digitalWrite(BLED, LOW);
#define OE  16 //IO16 OE on the 74AHCT1G125 
#define DR  4  //IO4 is Data Request


#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "CRC16.h"
//#include <base64.h>

#if GRAPH == 1
#include "FS.h"
#include <LittleFS.h>
//#include "SPIFFS.h"
File file;
#endif


// van ESP8266WiFi/examples/WiFiShutdown/WiFiShutdown.ino
#ifndef RTC_config_data_SLOT_WIFI_STATE
#define RTC_config_data_SLOT_WIFI_STATE 33u
#endif
#include <include/WiFiState.h>  // WiFiState structure details
//#include <WiFiState.h>  // WiFiState structure details
WiFiState WiFistate;


//van captive portal
/*  ============================================================================================================

    Captive Portal based on:
    https://gitlab.com/MrDIYca/code-samples/-/raw/master/esp8266_setup_portal.ino

    For more info, please watch my video at https://youtu.be/1VW-z4ibMXI
    MrDIY.ca
  ============================================================================================================== */
#include <EEPROM.h>
#include <ESP8266WebServer.h>
ESP8266WebServer    server(80);

#include <WiFiClient.h>
#include <ESP8266mDNS.h>

//#include "ESP8266HTTPUpdateServer.h"
//const char* update_username = "admin";

//ESP8266HTTPUpdateServer httpUpdater;

// mqtt stuff . https://github.com/ict-one-nl/P1-Meter-ESP8266-MQTT/blob/master/P1Meter/P1Meter.ino
#include <PubSubClient.h>
WiFiClient espClient;                   // * Initiate WIFI client
PubSubClient mqtt_client(espClient);    // * Initiate MQTT client
bool MQTT_Server_Fail = false;
long nextMQTTreconnectAttempt;
char outstr[10];
// end mqtt stuff

//// Raw data server
#include "ESPTelnet.h"
ESPTelnet telnet;
uint16_t  port = 23;
IPAddress ip;
bool telnetConnected = false;
// end raw data stuff

#include "vars.h"

struct settings {
  char dataSet[4];
  char ssid[32];
  char password[32];
  char domoticzIP[30] = "IPaddress";
  char domoticzPort[10] = "Port";
  char domoticzEnergyIdx[10] = "EnergyIdx";
  char domoticzGasIdx[10] = "GasIdx";
  char mqttTopic[50] = "sensors/power/p1meter";
  char mqttIP[30] = "";
  char mqttPort[10]  = "";
  char mqttUser[32] = "";
  char mqttPass[32] = "";
  char cfosUsr[32] = "admin";
  char cfosPwd[32] = "1234abcd";
  char cfosIP[30] = "192.168.1.183";
  char cfosPort[10] = "80";
  char cfosID[5] = "M4";
  char cfosModel[30] = "HTTP_Input";
//  char cfosIsVA[10] = "false";
  char interval[3] = "20";
  char cfosVA[4] = "";
  char domo[4] = "j";
  char mqtt[4] = "n";
  char cfos[4] = "j";
  char watt[4] = "n";
  char telnet[4] = "n";
  char debug[4] = "n";
  char adminPassword[32];
} config_data = {};


float RFpower = 0;      //20.5; // (For OdBm of output power, lowest. Supply current~70mA)
//20.5dBm (For 20.5dBm of output, highest. Supply current~85mA)

ADC_MODE(ADC_VCC);  // allows you to monitor the internal VCC level;
float volts;

// energy management vars
int interval;
unsigned long nextUpdateTime = 0;
long last10 = 0;
unsigned long time_to_wake = 0; // calculated wakeup time
unsigned long time_to_sleep = 0;    // calculated sleep time
unsigned long lastSleeptime = 0;
boolean entitledToSleep = false;       // aangezien slaapinterval en meetinterval niet synchroon lopen, moeten we na ontwaken eerst een telegram inlezen en afleveren alvorens ModemSleep mag worden aangeroepen.
bool monitoring = false;          // are we permitted to collect P1 data? Not if in setup or firmware update
bool atsleep = true;
bool bootSetup = false;           // flag for adminpassword bypass

// datagram stuff
#define MAXLINELENGTH 1040        // 0-0:96.13.0 has a maximum lenght of 1024 chars + 11 of its identifier
char telegram[MAXLINELENGTH];     // holds a single line of the datagram
String datagram;                  // holds entire datagram for raw output
bool datagramValid = false;    //
bool dataEnd = false;             // signals that we have found the end char in the data (!)
unsigned int currentCRC = 0;      // the CRC v alue of the datagram
bool gas22Flag = false;        // flag for parsing second gas line on dsmr2.2 meters
bool reportInDecimals = true;
bool CRCcheckEnabled = true;      // by default enable CRC checking
String mtopic = "Een lange MQTT boodschap als placeholder";

// Vars to store meter readings
// we capture all data in char arrays or Strings for longer hard to delineate data
String P1header;
char P1version[8];
int P1prot;             // 4 or 5 based on P1version 1-3:0.2.8
char P1timestamp[30]  = "\0";
char equipmentId[100] = "\0";
char electricityUsedTariff1[12];
char electricityUsedTariff2[12];
char electricityReturnedTariff1[12];
char electricityReturnedTariff2[12];
char tariffIndicatorElectricity[8];
char actualElectricityPowerDelivered[8];
char actualElectricityPowerReturned[8];
char numberPowerFailuresAny[6];
char numberLongPowerFailuresAny[6];
String longPowerFailuresLog;
char numberVoltageSagsL1[7];
char numberVoltageSagsL2[7];
char numberVoltageSagsL3[7];
char numberVoltageSwellsL1[7];
char numberVoltageSwellsL2[7];
char numberVoltageSwellsL3[7];
String textMessage;
char instantaneousVoltageL1[8];
char instantaneousVoltageL2[8];
char instantaneousVoltageL3[8];
char instantaneousCurrentL1[9];
char instantaneousCurrentL2[9];
char instantaneousCurrentL3[9];
char activePowerL1P[9];
char activePowerL2P[9];
char activePowerL3P[9];
char activePowerL1NP[9];
char activePowerL2NP[9];
char activePowerL3NP[9];

// Swedish specific
char cumulativeActiveImport[12];    // 1.8.0 
char cumulativeActiveExport[12];    // 2.8.0
char cumulativeReactiveImport[12];  // 3.8.0
char cumulativeReactiveExport[12];  // 4.8.0
char momentaryActiveImport[12];     // 1.7.0
char momentaryActiveExport[12];     // 2.7.0
char momentaryReactiveImport[12];   // 3.7.0
char momentaryReactiveExport[12];   // 4.7.0
char momentaryReactiveImportL1[12]; // 23.7.0
char momentaryReactiveImportL2[12]; // 43.7.0
char momentaryReactiveImportL3[12]; // 63.7.0
char momentaryReactiveExportL1[12]; // 24.7.0
char momentaryReactiveExportL2[12]; // 44.7.0
char momentaryReactiveExportL3[12]; // 64.7.0

char reactivePowerL1P[9]; // Sweden uses these 6
char reactivePowerL2P[9];
char reactivePowerL3P[9];
char reactivePowerL1NP[9];
char reactivePowerL2NP[9];
char reactivePowerL3NP[9];

// end Swedish
char deviceType[5];
char gasId[100] = "\0";;
char gasReceived5min[12];
char gasDomoticz[12];       //Domoticz wil gas niet in decimalen?

char prevGAS[12];           // not an P1 protocol var, but holds gas value


// char dayStartGaz[12];
// char dayStartUsedT1[12];
// char dayStartUsedT2[12];
// char dayStartReturnedT1[12];
// char dayStartReturnedT2[12];

// char monthStartGaz[12];
// char monthStartUsedT1[12];
// char monthStartUsedT2[12];
// char monthStartReturnedT1[12];
// char monthStartReturnedT2[12];

// process stuff
#define DISABLED 0
#define WAITING 1
#define READING 2
#define CHECKSUM 3
#define DONE 4
#define FAILURE 5
int state = DISABLED;

#define CONFIG  0   // getting basic Meter data to select correct parse rules
#define GOTMETER 1
#define RUNNING 2
int devicestate = CONFIG;

#define NONE 0
#define MAIN 1
#define DATA 2
#define CONFIG 3
#define UPDATE 4
int webstate = NONE;


bool wifiSta = false;
bool breaking = false;
bool softAp = false;
bool Json = false;
bool Telnet = false;

bool AdminAuthenticated = false;
bool Mqtt = false;
bool MqttConnected = false;
bool MqttDelivered = false;
String LastReport = ""; //timestamp of last telegram reported
bool mqtt_dsmr = false; // deliver mqtt data in 'dsmr reader" format
bool MQTT_debug = false;

// for cFos
bool cfos = true;
const char* cfosUsr = "admin";
const char* cfosPwd = "1234abcd";
const char* cfosIP = "192.168.1.183";
const char* cfosPort = "80";
const char* cfosID = "M4";
const char* cfosModel = "HTTP_Input";
const char* cfosIsVA = "true";
char aktuelleLeistungImport[8];
char aktuelleLeistungExport[8];
char CurrentL1[9];
char CurrentL2[9];
char CurrentL3[9];
// for cFos Ende

bool daystart = true;
bool OEstate = false;  // 74125 OE output enable is off by default (EO signal high)
// a bit of a kludge to get the first readings at coldboot to reset the daycounter. the coldboot flags will be reset once the daycounters are reset with valid values. The availability of valid values is signalled by
// the two gotXReadings
bool coldbootE = true;
bool coldbootG = true;
bool gotPowerReading = false;
bool gotGasReading = false;
char setupToken[18];

bool needToInitLogVars = false;
bool needToInitLogVarsGas = false;

void setup() {
  WiFi.forceSleepBegin(sleepTime * 1000000L); //In uS. Must be same length as your delay
  delay(10); // it doesn't always go to sleep unless you delay(10); yield() wasn't reliable
  delay(sleepTime); //Hang out at 15mA for (sleeptime) seconds
  WiFi.forceSleepWake(); // Wifi on

  pinMode(BLED, OUTPUT);
  Serial.begin(115200);
  debugln("Serial.begin(115200);");

  pinMode(OE, OUTPUT);      //IO16 OE on the 74AHCT1G125
  digitalWrite(OE, HIGH);   //  Put(Keep) OE in Tristate mode
  pinMode(DR, OUTPUT);      //IO4 Data Request
  digitalWrite(DR, LOW);    //  DR low (only goes high when we want to receive data)

  blink(3);
  debugln("Booting");
  debugln("Done with Cap charging … ");
  debugln("Let's go …");

  EEPROM.begin(sizeof(struct settings) );
  EEPROM.get( 0, config_data );

  if (config_data.dataSet[0] != 'j') {
    config_data = (settings) {"n", "", "", "192.168.1.12", "8080", "1234", "1235", "sensors/power/p1meter", "192.168.1.12", "1883", "mqtt_user", "mqtt_passwd", "admin", "1234abcd", "192.168.1.183", "80", "M4", "HTTP_Input", "30", "n", "n", "n", "n", "n", "n", "n", "adminpwd"};
  }
  
  (config_data.watt[0] == 'j') ? reportInDecimals = false : reportInDecimals = true;
  (config_data.domo[0] == 'j') ? Json = true : Json = false;
  (config_data.mqtt[0] == 'j') ? Mqtt = true : Mqtt = false;
  (config_data.cfos[0] == 'j') ? cfos = true : cfos = false;
  (config_data.telnet[0] == 'j') ? Telnet = true : Telnet = false;
  (config_data.debug[0] == 'j') ? MQTT_debug = true : MQTT_debug = false;
  if (config_data.cfosVA[0] == 'j')
    cfosIsVA = "true";
  else
    cfosIsVA = "false";
  
  if (strcmp(config_data.mqttTopic, "dsmr") == 0) { // auto detext need to report in 'dsmr reader' mqtt format
    mqtt_dsmr = true;
    // reportInDecimals = true;
  } else {
    mqtt_dsmr = false;
    // reportInDecimals = false;
  }

  debugln("EEprom read: done");
  PrintConfigData();

  interval = atoi(config_data.interval) * 1000;
  debug("interval: ");
  debugln(interval);

  debugln("Trying to connect to your WiFi network:");
  WiFi.mode(WIFI_STA);
  WiFi.begin(config_data.ssid, config_data.password);
  byte tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    ToggleLED
    delay(300);
    debug("o");
    if (tries++ > 30) {
      debugln("");
      debugln("Setting up Captive Portal by the name 'P1_setup'");
      LEDon
      WiFi.mode(WIFI_AP);
      softAp = WiFi.softAP("P1_Setup", "");
      breaking = true;
      break;
    }
  }
  debugln("");
  debugln("Set up WiFi, either in STA or AP mode");
  if (softAp) {
    debugln("running in AP mode, all handles will be initiated");
    start_webservices();
  }

  if (WiFi.status() == WL_CONNECTED) {
    WiFi.setAutoReconnect(true);
    debugln("HTTP server running.");
    debug("IP address: ");
    debugln(WiFi.localIP());
    setRFPower();
    wifiSta = true;
    debugln("WiFi running in STA (normal) mode");
    LEDoff
    ESP.rtcUserMemoryWrite(RTC_config_data_SLOT_WIFI_STATE, reinterpret_cast<uint32_t *>(&WiFistate), sizeof(WiFistate));
    #ifdef SLEEP_ENABLED
      modemSleep();
      modemWake();
    #else
      start_services();
    #endif
    debugln("All systems are ok...");

    state = WAITING;    // signal that we are waiting for a valid start char (aka /)
    devicestate = CONFIG;
    nextUpdateTime = nextMQTTreconnectAttempt = millis();

    monitoring = true; // start monitoring data
    time_to_sleep = millis() + wakeTime;  // we go to sleep wakeTime seconds from now

    // handle Files
    debug("Mounting file system ... ");
    if (!LittleFS.begin()) {
      debugln("LittleFS mount failed");
      debug("Formatting file system ... ");
      LittleFS.format();
      if (!LittleFS.begin()) {
        debugln("LittleFS mount failed AGAIN");
      } else
      {
        debugln("done.");
        if (zapfiles) zapFiles();
      }
    }

    File logData = LittleFS.open("/logData.txt", "r");
    if (logData) {
      logData.read((byte *)&log_data, sizeof(log_data));
      logData.close();
    } else {
      debugln("Failed to open logData.txt for reading");
      needToInitLogVars = true;
      needToInitLogVarsGas = true;
    }
    if (zapfiles) zapFiles();

  } // WL connected
  //debugln("Something is terribly wrong, no network connection");
}

void readTelegram() {
  if (Serial.available()) {
    memset(telegram, 0, sizeof(telegram));
    while (Serial.available()) {
      int len = Serial.readBytesUntil('\n', telegram, MAXLINELENGTH);
      telegram[len] = '\n';
      telegram[len + 1] = 0;
      yield();
      ToggleLED
      if (decodeTelegram(len + 1)) { // test for valid CRC
        break;
      } else { // we don't have valid data, but still may need to report to Domo
        if (dataEnd && !CRCcheckEnabled) { //this is used for dsmr 2.2 meters
          // yield(); //state = WAITING;
        }
      }
    }
    LEDoff
  }
}


void loop() {
  if ((millis() > nextUpdateTime) && monitoring) {
    if (!OEstate) { // OE is disabled  == false
      RTS_on();
      Serial.flush();
    }
  } // update window
  if (OEstate) readTelegram();

  if ((millis() > time_to_sleep) && !atsleep && wifiSta) { // not currently sleeping and sleeptime
  #ifdef SLEEP_ENABLED
    modemSleep();
  #endif
  }

  if (wifiSta && (millis() > time_to_wake) && (WiFi.status() != WL_CONNECTED)) { // time to wake, if we're not already awake
  #ifdef SLEEP_ENABLED
    modemWake();
  #endif
  }

  if (datagramValid && (state == DONE) && (WiFi.status() == WL_CONNECTED)) {

    if (Mqtt) {
      doMQTT();
      if (MqttDelivered) {
        datagramValid = false;
        state = WAITING;
        MqttDelivered = false;
      }
    }
//    if (cfos) {
//      doCFOS();
//      datagramValid = false;
//      state = WAITING;
//    }
    if (Json) {
      doJSON();
      doCFOS();
      datagramValid = false;
      state = WAITING;    
    }
    if (Telnet) {
      TelnetReporter();
      datagramValid = false;
      state = WAITING;
    }
    if (MQTT_debug) MQTT_Debug();
    
    nextUpdateTime = millis() + interval;
    if (ESP.getFreeHeap() < 2000) ESP.reset(); // watchdog, we do have a memory leak (still)
    state = WAITING;

    checkCounters();
    resetFlags();
  }

  if (softAp || (WiFi.status() == WL_CONNECTED)) {
    server.handleClient(); //handle web requests
    MDNS.update();
    if (Telnet) telnet.loop();
  }
  //yield();
}

void checkCounters() {
  // see logging.ino
  time_t t = now();
  if (coldbootE && gotPowerReading) {
    if (needToInitLogVars) {
      doInitLogVars();
    }
    resetEnergyDaycount();
    resetEnergyMonthcount();
  }
  if (coldbootG && gotGasReading) {
    if (needToInitLogVarsGas) {
      doInitLogVarsGas();
    }
    resetGasCount();
  }

  if (!CHK_FLAG(logFlags, hourFlag) && minute(t) == 59) doHourlyLog();
  if (!CHK_FLAG(logFlags, dayFlag) && (hour(t) == 23) && (minute(t) == 59)) doDailyLog();
  if (!CHK_FLAG(logFlags, weekFlag) && weekday(t) == 1 && hour(t) == 23 && minute(t) == 59) doWeeklyLog(); // day of the week (1-7), Sunday is day 1
  if (!CHK_FLAG(logFlags, monthFlag) && day(t) == 28 && month(t) == 2 && hour(t) == 23 && minute(t) == 59 && year(t) != 2024 && year(t) != 2028 ) doMonthlyLog();
  if (!CHK_FLAG(logFlags, monthFlag) && day(t) == 29 && month(t) == 2 && hour(t) == 23 && minute(t) == 59 ) doMonthlyLog(); // schrikkeljaren
  if (!CHK_FLAG(logFlags, monthFlag) && day(t) == 30 && (month(t) == 4 || month(t) == 6 || month(t) == 9 || month(t) == 11) && hour(t) == 23 && minute(t) == 59 ) doMonthlyLog();
  if (!CHK_FLAG(logFlags, monthFlag) && day(t) == 31 && (month(t) == 1 || month(t) == 3 || month(t) == 5 || month(t) == 7 || month(t) == 8 || month(t) == 10 || month() == 12) && hour() == 23 && minute() == 59 ) doMonthlyLog();
  if (!CHK_FLAG(logFlags, monthFlag) && day(t) == 31 && month(t) == 12 && hour(t) == 23 && minute(t) == 59 ) doYearlyLog();
}

void resetFlags() {
  if (CHK_FLAG(logFlags, hourFlag) &&  (minute() > 0) ) CLR_FLAG(logFlags, hourFlag);// if (hourFlag &&  (minute() > 24)) hourFlag = false; // CLR_FLAG(logFlags, hourFlag);
  if (CHK_FLAG(logFlags, dayFlag) && (day() > 0)) CLR_FLAG(logFlags, dayFlag);

  if (CHK_FLAG(logFlags, weekFlag) &&  (weekday() > 1)) CLR_FLAG(logFlags, weekFlag);
  if (CHK_FLAG(logFlags, monthFlag) &&  (day() > 0)) CLR_FLAG(logFlags, monthFlag);
  if (CHK_FLAG(logFlags, yearFlag) &&  (day() == 1) && month() == 1) CLR_FLAG(logFlags, yearFlag);
  debug("logFlags : ");
  Serial.println(logFlags);
}
