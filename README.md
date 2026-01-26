### P1-WiFi-Gateway
P1-WiFi-Gateway mit Unterstützung der cFos Power Brain Wallbox

P1 wifi gateway 2022

**(English Version below)**

Basierend auf einem Repo von Ronald Leenes: https://github.com/romix123/P1-wifi-gateway

Diese Software vereint verschiedene Komponenten in einem handlichen Paket.
Die Software läuft auf einem ESP8266 board wie dem Wemos D1 mini oder mini Pro und hat einen bescheidenen Platzbedarf.

Die Datenverarbeitung basiert auf: http://domoticx.com/p1-poort-smart-meter-data-to-domoticz-esp8266 
Das Captive Portal basiert auf einem der ESP8266-Beispiele, die OTA-Einheit ebenfalls.

Das Modul liefert Daten über JSON- oder MQTT-Nachrichten an einen Host, kann aber auch über das Webinterface des Moduls oder per Telnet ausgelesen werden. Die Daten können an einen virtuellen Zähler "HTTP input" in der Power Brain Wallbox von cFos übertragen werden und dort für das Lastmanagement genutzt werden. Damit das funktioniert, muss der Charging Manager der Wallbox mindestens auf die Firmwareversion 1.17.4-beta gebracht werden.

Das Modul lädt beim Start zuerst den Pufferkondensator auf. Es blinkt dann 2x und versucht zunächst, sich mit Ihrem WLAN-Netzwerk zu verbinden. Während dieses Vorgangs blinkt die LED mit einer Periode von 0,5 Hz. Kommt keine Verbindung zustande, startet das Modul einen Access Point mit dem Namen P1_Setup. Verbinden Sie sich mit diesem Netzwerk und surfen Sie zu 192.168.4.1

Daraufhin erscheint der Menübildschirm des Moduls. Geben Sie dort die SSID und das Passwort Ihres WLAN-Netzwerks ein. Darunter tragen Sie die Daten des empfangenden Servers (IP und Portnummer) ein. Dies kann Ihr Domotociz-Server sein, in diesem Fall müssen Sie auch die IDx eingeben, die Domoticz für ein Gas- und ein Energiegerät erstellt. 
Auch die Daten für einen (z. B. auf einem Raspberry Pi selbst gehosteten) MQTT-Broker können jetzt eingegeben werden.

Wenn die Daten für das Lastmanagement einer cFos Power Brain Wallbox genutzt werden sollen, ist dies ebenfalls hier möglich.

Schließlich geben Sie an, wie viele Sekunden Daten an den Server gesendet werden sollen. DSMR4-Messgeräte verwenden ein Messintervall von 10 Sekunden. SMR5-Zähler liefern jede Sekunde Daten. Das erscheint  viel und das Modul benötigt auch viel Strom, daher ist die Frage, ob es parasitär (powered by the meter) arbeiten kann. Empfohlen ist  ein Intervall von 15 Sekunden oder höher. Das hier eingegebene Intervall wird nicht immer synchron zum Datenintervall des Zählers sein, gehen Sie also nicht davon aus, dass Sie in Domoticz, MQTT oder im Charging Manager der cFos Wallbox  genau alle x Sekunden einen Messwert sehen. 

Das Modul „schläft“ zwischen den Messungen (das Modem wird ausgeschaltet, so dass die Stromaufnahme von ca. 70 mA auf 15 mA sinkt). Dies gibt dem Pufferkondensator Zeit, sich für die vom WLAN-Sender des Moduls erzeugten Stromspitzen aufzuladen (und es spart trotzdem etwas Strom (der von Ihrem Energieversorger bezahlt wird, aber trotzdem)). Jedes bisschen hilft.

Siehe http://esp8266thingies.nl für weitere Informationen über das Modul und http://www.esp8266thingies.nl/wp/forum-2/ für das Diskussionsforum.

Ergänzung vom 31.07.2023: Seit heute sind die PCB Dateien im KiCAD Format online. Das Board wurde schon produziert und getestet. Ebenso stehen die Gerber Dateien zur Verfügung.

![grafik](https://github.com/SchneHa/P1-WiFi-Gateway-mirrored/assets/6528455/9e9915d1-fa41-47f5-a661-10172123fd67)

![IMG_3425](https://github.com/SchneHa/P1-WiFi-Gateway-mirrored/assets/6528455/e56add0c-77cc-4f08-b666-bfb82792363d)

![IMG_3427](https://github.com/SchneHa/P1-WiFi-Gateway/assets/6528455/9c930514-7913-4a67-9cd8-5ad7e0333a02)

![IMG_3428](https://github.com/SchneHa/P1-WiFi-Gateway/assets/6528455/7ad5568e-1ae7-4946-8396-3d993154d132)


Ich habe noch einige PCBs aus der ersten Produktion übrig, bei Bedarf bitte melden.
```
PCB unbestückt: 5€
PCB mit Bauteilen: 10€
PCB fertig bestückt: 15€
D1 mini ESP8266 mit Headern: 10€
inklusive Versand innerhalb Deutschlands
hans(dot)schneider(at)gmx(dot)ch
```

```
Version: 1.2e Datum: 02.11.2025
         Checkt nun, ob eine Verbindung zum WiFi besteht. Wenn nicht, wird die Verbindung neu hergestellt.
Version: 1.2d Datum: 15.01.2025
         nach Ändern des voreingestellten Passworts für die cFos Power Brain wird das Passwort nun nicht mehr vergessen
Version: 1.2c Datum: 23.05.2024
         die Anzeige der Seite mit den Messwerten wird nun alle 20 Sekunden erneuert
         P1meter sendet nun positive Werte für actualElectricityPowerDelivered und negative Werte für
         actualElectricityPowerReturned zur cFos Power Brain Wallbox (das ist wichtig um korrekte Werte für die Einspeisung
         ins Stomnetz zu erhalten)
Version: 1.2b Datum: 22.05.2023
         Anpassungen für die Firmware 1.19.4 der cFos Power Brain Wallbox
         (power_va ist nun power_w, is_va existiert nicht mehr)
         Korrekturen der niederländischen, französischen und schwedischen Version
Version: 1.2a Datum: 13.03.2023
         Fertigstellung der französischen und schwedischen Version
         diverse Verbesserungen und Fehlerbehebungen
Version: 1.2 Datum: 05.03.2023, Autor: Hans Schneider
         Unterstützung für den HTTP_input Zähler der cFos Power Brain Wallbox hinzugefügt
         diverse Verbesserungen und Fehlerbehebungen
Version: 1.1adc Datum: 12.02.2023 Autor: Ronald Leenes
1.1adc: Umfangreiche Protokollierung implementiert
        Schwedische Version aktualisiert, um sie an die niederländische Version anzugleichen 
        (Deutsche und Französische Version haben keine Sicherheitsfunktionen bei Setup/Update)
1.1aa: Fehlerkorrekturen 1.1: Graphen implementiert BerichtstarifIndicatorElectricity behoben
ud: Kopfzeilen aktualisieren behoben ua: Problem mit Setup-Feldern behoben
u: Passwortschutz für Setup- und Update-Firmware-Menüs hinzugefügt
   Gehen Sie beim ersten Booten von Version 1.0u innerhalb von 60 Sekunden zum Setup-Menü, um den Passwortschutz zu umgehen
   Out-of-the-Box-Unterstützung für die Integration von Home Assistant "DSMR Reader" hinzugefügt 
   Verwenden Sie „dsmr“ als MQTT-Präfix und Daten werden automatisch im „DSMR Reader“-Format an den MQTT-Broker geliefert
   MQTT-Wiederverbindung behoben, es ist jetzt nicht blockierend
   Zurücksetzen der Hardboot-Tageszähler behoben
   Senden leerer MQTT-Nachrichten behoben
   Insgesamt verbesserte Stabilität
t: Verbesserungen bei der Energieverwaltung
   allgemeine (kleinere) Bereinigung
ta: Korrektur für Telnet-Berichte
    französische Lokalisierung hinzugefügt
s: deutsche Lokalisierung hinzugefügt 
   mqtt-Ausgabe für schwedische spezifische OBIS-Codes hinzugefügt
r: Geschwindigkeitsverbesserungen und Rationalisierung der Parser-Lokalisierungen für: NL, SE
q: Tageswerte hinzugefügt 
p: EquipmentID in mqtt set eingebaut 
o: fixe Gasleistung, fixes mqtt reconnect 
n: leerer Aufruf von SetupSave leitet jetzt zum Hauptmenü weiter statt Einstellungen zurückzusetzen ;-)
   behobene kWh/Wh Invertierung 
m: setupsave fix, relocate to p1wifi.local nach 60 Sek. mqtt 
   kw/W fix 
l: wifireconnect nach WLAN-Verlust 
k: großer BUG behoben, SoftAP würde keinen erreichbaren Webserver erzeugen 
j: Rohdaten auf Port 23
   Hauptcodeumschreibung 
   implementierte Datenberichtsmethoden: geparste Daten: json, mqtt, p1wifi.local/P1 
   Rohdaten: Telnet auf Port 23, p1wifi.local/Data
i: erweiterte maximale Leitungslänge für TD210-D-Zähler, die eine wirklich lange 1-0:99.97.0-Leitung haben 
h: erweitertes mqtt-Set mit sofortigen Leistungen, Spannungen, aktueller Tarifgruppe 
   Stromausfälle, Stromabfälle 
g: feste mqtt
```

Empfohlenes Board:

Wemos D1 mini Pro, Flash Größe: 16 MB, FS: 14 MB, OTA: ~1019 kB

ebenso möglich:

Wemos D1 R2 & mini, Flash Größe: 4 MB, FS: 2 MB, OTA: ~1019 kB oder jedes andere kompatible board

```
Benötigte Dateien:
  P1WG2022current.ino
  CRC16.h 
  JSON.ino 
  MQTT.ino 
  TELNET.ino 
  debug.ino 
  decoder.ino 
  functions.ino 
  graph.ino
  logging.ino
  vars.h
  webserver.ino
  webserverDE.ino
  webserverFR.ino
  webserverNL.ino
  webserverSE.ino
  wifi.ino
```
Anmerkung: <Del>Zurzeit sind nur die deutsche und die niederländische Versionen lauffähig.</Del> Bei der französischen und der schwedischen Version muss ggf. die Übersetzung korrigiert werden.


**English Version:**

P1-WiFi-Gateway with support for cFos Power Brain Wallbox

P1 wifi gateway 2022

Based on a repo of Ronald Leenes: https://github.com/romix123/P1-wifi-gateway

This software combines various components in one handy package.
The software runs on an ESP8266 board like the Wemos D1 mini or mini Pro and has a modest footprint.

The data processing is based on: http://domoticx.com/p1-poort-smart-meter-data-to-domoticz-esp8266
The captive portal is based on one of the ESP8266 samples, as is the OTA unit.

The module delivers data to a host via JSON or MQTT messages, but can also be read out via the module's web interface or via Telnet. It is possible to send data to a virtual meter "HTTP input" of the Power Brain Wallbox of cFos which can be used to implement load management. For that to work Charging Manager of cFos Power Brain Wallbox has to be at least on firmware version 1.17.4-beta.

When starting, the module first charges the buffer capacitor. It will then blink 2x and first try to connect to your WiFi network. During this process, the LED flashes with a period of 0.5 Hz. If no connection is established, the module starts an access point with the name P1_Setup. Connect to this network and surf to 192.168.4.1

The module menu screen appears. Enter the SSID and password of your WiFi network there. Below this, enter the data of the receiving server (IP and port number). This can be your Domotociz server, in this case you also have to enter the IDx that Domoticz creates for a gas and an energy device.
The data for an MQTT broker (e.g. hosted on a Raspberry Pi itself) can now also be entered.

If the data is to be used for load management of a cFos Power Brain Wallbox, this is also possible here.

Finally, specify how many seconds of data should be sent to the server. DSMR4 gauges use a measurement interval of 10 seconds. SMR5 counters provide data every second. That seems a lot to me and the module also draws a lot of current, so the question is whether it can operate parasitically (powered by the meter). I recommend an interval of 15 seconds or higher. The interval entered here will not always be in sync with the meter's data interval, so don't assume that you will see a reading in Domoticz, MQTT or in Charging Manager of the cFos Wallbox exactly every x seconds. 

The module "sleeps" between the measurements (the modem is switched off so that the current consumption drops from approx. 70 mA to 15 mA). This gives the buffer capacitor time to charge up for the current spikes generated by the module's WiFi transmitter (and it still saves some electricity (which is paid for by your utility, but still)). Every little helps.

See http://esp8266thingies.nl for more information about the module and http://www.esp8266thingies.nl/wp/forum-2/ for the discussion forum.

Addition from 07/31/2023: Since today the PCB files in KiCAD format are online. The board has already been produced and tested. The Gerber files are also available.

![grafik](https://github.com/SchneHa/P1-WiFi-Gateway-mirrored/assets/6528455/9e9915d1-fa41-47f5-a661-10172123fd67)

![IMG_3425](https://github.com/SchneHa/P1-WiFi-Gateway-mirrored/assets/6528455/e56add0c-77cc-4f08-b666-bfb82792363d)

![IMG_3427](https://github.com/SchneHa/P1-WiFi-Gateway/assets/6528455/9c930514-7913-4a67-9cd8-5ad7e0333a02)

![IMG_3428](https://github.com/SchneHa/P1-WiFi-Gateway/assets/6528455/7ad5568e-1ae7-4946-8396-3d993154d132)

I still have some PCBs left from the first batch, please let me know if you need one.

```
PCB unassembled: 5€
PCB with parts: 10€
PCB assembled: 15€
D1 mini ESP8266 with headers: 10€
shipping to Germany included
hans(dot)schneider(at)gmx(dot)ch
```

```
Version: 1.2e Date: 02.11.2025
         now checks connection to wifi in every loop, if not connected it will be reconnected
Version: 1.2d Date: 15.01.2025
         after changing cFos Power Brain password from it's default the new password isn't forgotten anymore now
Version: 1.2c Date: 23.05.2024
         measurement page refreshes every 20 seconds now
         P1meter now sends positive values for actualElectricityPowerDelivered and negative values for
         actualElectricityPowerReturned to cFos Power Brain wallbox (that is important to calculate correct
         values for feeding energy into the power grid)
Version: 1.2b Date: 22.05.2023
         changes for cFos Power Brain Firmware 1.19.4
         (power_va is now power_w, is_va doesn't exist anymore)
         improvements of dutch, french and swedish language
Version: 1.2a Date: 13.03.2023
         Finishing the french and swedish Version
         various Improvements and bug fixes
Version: 1.2 Date: 03/05/2023, Author: Hans Schneider
         added Support for the HTTP_input meter of cFos Power Brain Wallbox
         various improvements and bug fixes
Version: 1.1adc Date: 02/12/2023 Author: Ronald Leenes
1.1adc: Extensive logging implemented
        Swedish version updated to match Dutch version 
        (German and French versions have no security features on setup/update)
1.1aa: Bug fixes 1.1: Graphs implemented
       Reporting TariffIndicatorElectricity fixed
ud: fixed update headers 
ao: fixed problem with setup fields
u: Added password protection for setup and update firmware menus when booting version 
1.0u for the first time, go to setup menu within 60 seconds to bypass password protection
     Added out-of-the-box support for Home Assistant "DSMR Reader" integration 
     Use "dsmr" as MQTT prefix and data is automatically delivered to the MQTT broker in "DSMR Reader" format
     Fixed MQTT reconnection, it's non-blocking now
     Fixed resetting hardboot day counters
     Fixed sending empty MQTT messages
     Improved overall stability
t: Power management improvements
   general (minor) cleanup 
ta: Fix for telnet reports
    added French localization
s: added german localization
   added mqtt output for swedish specific OBIS codes
r: Speed improvements and streamlining of parser 
   localizations for: NL, SE
q: daily values added 
p: EquipmentID built into mqtt set 
o: fixed gas power
   fixed mqtt reconnect 
n: empty call to SetupSave now redirects to main menu instead of resetting settings ;-)
   fixed kWh/Wh inversion 
m: setupsave fix, relocate to p1wifi.local after 60 sec. 
   mqtt - kw/W fix 
l: wifi reconnect after WLAN loss 
k: major BUG fixed
   SoftAP would not create an accessible web server 
j: raw data on port 23 
   main code rewrite
   implemented data reporting methods: parsed data: json, mqtt, p1wifi.local/P1 
   raw data: telnet on port 23, p1wifi.local/Data
i: extended maximum line length for TD210-D meters that have a really long 1-0:99.97.0 line 
h: extended mqtt set with instantaneous powers, voltages, current tariff group
   blackouts, blackouts 
g: fixed mqtt
```

Recommended board:

Wemos D1 mini Pro, Flash Größe: 16 MB, FS: 14 MB, OTA: ~1019 kB

also possible:

Wemos D1 R2 & mini, Flash Größe: 4 MB, FS: 2 MB, OTA: ~1019 kB or any other compatible board


Files needed:
```
  P1WG2022current.ino
  CRC16.h
  JSON.ino
  MQTT.ino
  TELNET.ino
  debug.ino
  decoder.ino
  functions.ino
  graph.ino
  logging.ino
  var.h
  webserver.ino
  webserverDE.ino
  webserverFR.ino
  webserverNL.ino
  webserverSE.ino
  wifi.ino
```

Note: <Del>Only the German and Dutch versions are currently working.</Del> The French and the Swedish version needs improvement of translation.
