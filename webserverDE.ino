#ifdef GERMAN
  void addIntro(String& str){
    char ipstr[20];
    IPAddress ip = WiFi.localIP();
    sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
    str += ("<body><div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-Gateway</h2></div><br>");
    str += ("<p style='text-align:right;font-size:11px;color:#aaa'><a href='http://");
    str += ipstr;
    str += ("/Help' target='_blank'>Hilfe</a>");
  }

  void addUptime(String& str){
    char ipstr[20];
    IPAddress ip = WiFi.localIP();
    sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
    
    str += ("<div style='text-align:left;font-size:11px;color:#aaa'><hr/>");
    char strUpTime[40];
    int minuten = millis() / 60000;
    int dagen = minuten / 1440;
    minuten = minuten % 1440;
    int uren = minuten / 60;
    minuten = minuten % 60;
    sprintf_P(strUpTime, PSTR("%d Tage %d Stunden %d Minuten"), dagen, uren, minuten);
    str += strUpTime;
    str += ("<span style='float:right;font-size:11px;color:#aaa'>");
    str += ipstr;
    str += ("</span></div>");
  }

  void addFoot(String& str){
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
  
    if (Mqtt) {
      if (MqttConnected) str += F("MQTT link: √ "); else str += F("MQTT – ");
      str += F(" letztes sample ");
      str += LastReport;
	  str += F("<br>");
    }
    str += F(" Firmware version: ");
    str += version;
	
    char sysmsg[100];
    readVoltage();
    sprintf_P(sysmsg, PSTR(" – %1.2fV"), volts / 1000);
    str += sysmsg;
	
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void addFootBare(String& str){
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
    str += F(" Firmware version: ");
    str += version;
	
    char sysmsg[100];
    readVoltage();
    sprintf_P(sysmsg, PSTR(" – %1.2fV"), volts / 1000);
    str += sysmsg;
	
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void setupSaved(String& str) {
 // str += F("<script>var countDownDate = new Date().getTime()+50000;var x = setInterval(function() {var now = new Date().getTime();var distance = countDownDate - now;var seconds = Math.floor((distance % (1000 * 60)) / 1000);document.getElementById(\"timer\").innerHTML = seconds + \" seconden tot module weer bereikbaar.\";if (seconds < 2) {location.replace(\"http://p1wifi.local\")}}, 1000);</script>");
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>WiFi- und Modul-Setup</b></legend>");
    str += F("<p><b>Die Einstellungen wurden erfolgreich gespeichert.</b><br><br>");
    str += F("<p>Das Modul wird jetzt neu gestartet. Dies dauert etwa eine Minute.<br>Die blaue LED leuchtet 2x auf, wenn das Modul den Bootvorgang beendet hat.</p>");
    str += F("<p>Die LED blinkt langsam, während die Verbindung zum WiFi-Netzwerk hergestellt wird.</p>");
    str += F("<p>Wenn die blaue LED an bleibt, ist die Einstellung fehlgeschlagen <br>und das Modul muss erneut mit dem WiFi-Netzwerk ");
    str += F("'P1_Setup' verbunden werden.</p>");
    str += F("<br>");
 // str += F("<p id=\"timer\"></p>");
    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
  }

  void handleUploadForm() {
    if (strcmp(server.arg("adminPassword").c_str(), config_data.adminPassword) != 0) { // passwords doesn't match
      debugln("Error: update entered with wrong password");
      errorLogin("Update");
      return;
    } else  AdminAuthenticated = true;
    String str="";
    monitoring = false; // stop monitoring data
    addHead(str,0);
    addIntro(str);
    str += F("<fieldset><fieldset><legend><b>Update Firmware</b></legend>");
    str += F("<form method='POST' action='/update' enctype='multipart/form-data'><p>");
    str += F("<b>Firmware</b><input type='file' accept='.bin,.bin.gz' name='Firmware'></p>");
    str += F("<button type='submit'>Update</button>");
    str += F("</form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
    addFootBare(str); 
    webstate = UPDATE;
    server.send(200, "text/html", str);
  }

  void successResponse(){
    String str = "";
    addRefreshHead(str);
    addIntro(str);
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>Firmware update</b></legend>");
    str += F("<p><b>Die Firmware wurde erfolgreich aktualisiert.</b><br><br>");
    str += F("<p>Das Modul sollte nun neu starten. Das dauert etwa eine Minute.</p>");
    str += F("<p>Die blaue LED leuchtet 2x auf wenn das Modul gestartet ist.</p>");
    str += F("<p>Die LED blinkt langsam während des Verbindens mit dem WiFi-Netzwerk.</p>");
    str += F("<p>Wenn die blaue LED permanent leuchtet, ist die Einstellung missglückt und das Modul muss neu mit dem WiFi-Netzwerk<br>");
    str += F("'P1_Setup' verbunden werden.</p>");
    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
    server.send(200, "text/html", str);
    delay(2000);
  }

  void handleRoot(){
    debugln("handleRoot");
    String str = ""; 
    addHead(str,0);
    addIntro(str);

    str += F("<fieldset>");
    str += F("<main class='form-signin'>");
    str += F("<fieldset><legend><b> Data </b></legend>");
    str += F("<form action='/P1' method='post'><button type='p1' class='button bhome'>Messwerte</button></form>");
    #if GRAPH == 1
      str += F("<form action='/Graphs' method='GET'><button type='submit'>Grafiken</button></form>");
    // str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
    #endif
    str += F("</fieldset>");
    str += F("<fieldset><legend><b> Setup </b></legend>");
    str += F("<form action='/Setup' method='post'><button type='Setup'>Setup</button></form>");
    str += F("<form action='/Update' method='GET'><button type='submit'>Update Firmware</button></fieldset></form>");
    addUptime(str);
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = MAIN;
    monitoring = true;
    nextUpdateTime = millis() + 2000;
  }

  void handleLogin(){
    createToken();
    debugln("handleLogin");

    if (millis() < 60000) {
      debug(millis());
      debugln(" – You made it within the timeframe, go to setup without login."); 
      bootSetup = true; // our ticket to handleSetup
      handleSetup();
    }
    String str = "";
    addHead(str,0);
    addIntro(str);
    str += F("<form action='/Setup2' method='POST'><fieldset>");
    str += F("<input type='hidden' name='setuptoken' value='");
    str += setupToken;
    str += F("'>");
    str += F("<fieldset><legend><b>&nbsp;Login&nbsp;</b></legend>");
    str += F("<p><b>Admin Passwort</b><br>");
    str += F("<input type='text' class='form-control' name='adminPassword' value='' </p>");
    str += F("</fieldset>");
    str += F("<p><button type='submit'>Login</button></form>");
    addFoot(str);
    server.send(200, "text/html", str);
  }

  void handleUpdateLogin(){
    createToken();
    debugln("handleUpdateLogin");

    String str = "";
    addHead(str,0);
    addIntro(str);
    str += F("<form action='/uploadDialog' method='POST'><fieldset>");
    str += F("<input type='hidden' name='setuptoken' value='");
    str += setupToken;
    str += F("'>");
    str += F("<fieldset><legend><b>&nbsp;Login&nbsp;</b></legend>");
    str += F("<p><b>Admin Passwort</b><br>");
    str += F("<input type='text' class='form-control' name='adminPassword' value='' </p>");
    str += F("</fieldset>");
    str += F("<p><button type='submit'>Login</button></form>");
    addFoot(str);
    server.send(200, "text/html", str);
  }

  void errorLogin(String returnpage){
    debugln("errorLogin");
    String str = "";
    addHead(str,0);
    addIntro(str);
    str += F("<fieldset><legend><b>Fout</b></legend>");
    str += F("<p><b>Admin Passwort ist falsch.</b><br>");
    str += F("</fieldset>");
    str += F("<form action='/");
    str += returnpage;
    str += F("' method='POST'><button class='button bhome'>wiederholen</button></form></p>");
    addFoot(str);
    server.send(200, "text/html", str);
    bootSetup = false;
  }

  void handleSetup(){
    if (millis() > 60000) {            // if we did not get here directly, check credentials
      debugln("indirect call");
      if (strcmp(server.arg("adminPassword").c_str(), config_data.adminPassword) != 0) {  // passwords don't match
        debugln("Error: handlesetup entered with wrong password");
        errorLogin("Setup");
        return;
      }
    }      
    debugln("direct call");
    monitoring = false; // stop monitoring data

    String str = ""; 
    debugln("handleSetupForm");

    addHead(str,0);
    addIntro(str);
	str += F("<fieldset>");
    str += F("<form action='/SetupSave' method='POST'><fieldset>");
    str += F("<input type='hidden' name='setuptoken' value='");
    str += setupToken;
    str += F("'>");
    str += F("<fieldset><legend><b>&nbsp;Admin&nbsp;</b></legend>");
    str += F("<p><b>Admin Passwort</b><br>");
    str += F("<input type='text' class='form-control' name='adminPassword' value='");
    str += config_data.adminPassword;
    str += F("'></p></fieldset>");
       
    str += F("<fieldset><legend><b>&nbsp;WiFi Parameter&nbsp;</b></legend>");
    str += F("<p><b>SSID</b><br>");
    str += F("<input type='text' class='form-control' name='ssid' value='");
    str += config_data.ssid;
    str += F("'></p>");
    str += F("<p><label><b>Passwort</b></label><br><input type='password' class='form-control' name='password' value='");
    str += config_data.password;
    str += F("'></p></fieldset>");
      
    str += F("<fieldset><legend><b>&nbsp;Domoticz Parameter&nbsp;</b></legend>");
      
    str += F("<p><b>Bericht an Domoticz?</b><input type='checkbox' class='form-control' name='domo' id='domo' ");
      
    if (config_data.domo[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Domoticz IP-Adresse</b><input type='text' class='form-control' name='domoticzIP' value='");
    str += config_data.domoticzIP;
    str += F("'></p><p>");
    str += F("<b>Domoticz Port</b><input type='text' class='form-control' name='domoticzPort' value='");
    str += config_data.domoticzPort;
    str += F("'></p><p>");
    str += F("<b>Domoticz Gas Idx</b><input type='text' class='form-control' name='domoticzGasIdx' value='");
    str += config_data.domoticzGasIdx;
    str += F("'></p><p>");
    str += F("<b>Domoticz Energy Idx</b><input type='text' class='form-control' name='domoticzEnergyIdx' value='");
    str += config_data.domoticzEnergyIdx;
    str += F("'></p>");
    str += F("</fieldset>");

    str += F("<fieldset><legend><b>&nbsp;MQTT Parameter&nbsp;</b></legend>");
  
    str += F("<p><b>Bericht an MQTT Broker?</b><input type='checkbox' class='form-control' name='mqtt' id='mqtt' ");
  
    if (config_data.mqtt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>MQTT Broker IP-Adresse</b><input type='text' class='form-control' name='mqttIP' value='");
    str += config_data.mqttIP;
    str += F("'></p><p>");
    str += F("<b>MQTT Broker Port</b><input type='text' class='form-control' name='mqttPort' value='");
    str += config_data.mqttPort;
    str += F("'></p><p>");
    str += F("<b>MQTT User</b><input type='text' class='form-control' name='mqttUser' value='");
    str += config_data.mqttUser;
    str += F("'></p><p>");
    str += F("<b>MQTT Passwort</b><input type='text' class='form-control' name='mqttPass' value='");
    str += config_data.mqttPass;
    str += F("'></p>");
    str += F("<b>MQTT root-topic</b><input type='text' class='form-control' name='mqttTopic' value='");
    str += config_data.mqttTopic;
    str += F("'></p>");
    str += F("</fieldset>");
  
    str += F("<fieldset><legend><b>&nbsp;cFos Power Brain Wallbox Parameter&nbsp;</b></legend>");
  
    str += F("<p><b>Daten an cFos Power Brain?</b><input type='checkbox' class='form-control' name='cfos' id='cfos' ");
  
    if (config_data.cfos[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>cFos Power Brain IP-Adresse</b><input type='text' class='form-control' name='cfosIP' value='");
    str += config_data.cfosIP;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain Port</b><input type='text' class='form-control' name='cfosPort' value='");
    str += config_data.cfosPort;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain User</b><input type='text' class='form-control' name='cfosUsr' value='");
    str += config_data.cfosUsr;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain Passwort</b><input type='text' class='form-control' name='cfosPwd' value='");
    str += config_data.cfosPwd;
    str += F("'></p>");
    str += F("<b>cFos Power Brain Zählermodell</b><input type='text' class='form-control' name='cfosModel' value='");
    str += config_data.cfosModel;
    str += F("'></p>");
    str += F("<b>cFos Power Brain ZählerID</b><input type='text' class='form-control' name='cfosID' value='");
    str += config_data.cfosID;
    str += F("'></p>");
//    str += F("<p><b>cfos Power Brain Zähleranzeige in VA (statt in W) </b><input type='checkbox' class='form-control' name='cfosVA' id='cfosVA' ");
//    if (config_data.cfosVA[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("</fieldset>");

    str += F("<fieldset><legend><b>&nbsp;Weitere Einstellungen&nbsp;</b></legend>");
  
    str += F("<b>Messintervall in sec (> 10 sec)</b><input type='text' class='form-control' name='interval' value='");
    str += config_data.interval; 
    str += F("'></p><p>");
    str += F("<p><b>Bericht in Watt (statt in kW) </b><input type='checkbox' class='form-control' name='watt' id='watt' ");
    if (config_data.watt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Aktiviere Telnet Port (23) </b><input type='checkbox' class='form-control' name='telnet' id='telnet' ");
    if (config_data.telnet[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Debug über MQTT </b><input type='checkbox' class='form-control' name='debug' id='debug' ");
    if (config_data.debug[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  
    str += F("</fieldset><div></div>");
    str += F("<p><button type='submit'>Sichern</button></form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form></p>");
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = CONFIG;
  }

  void handleP1(){
    debugln("handleP1");
    String str = "";
    String eenheid, eenheid2, eenheid3;
    if (reportInDecimals) {
      eenheid = " kWh'></div>";
      dtostrf(atof(instantaneousCurrentL1), 1, 2, CurrentL1);
      dtostrf(atof(instantaneousCurrentL2), 1, 2, CurrentL2);
      dtostrf(atof(instantaneousCurrentL3), 1, 2, CurrentL3);
    } else {
      eenheid = " Wh'></div>";
      dtostrf(atof(instantaneousCurrentL1)/100, 1, 2, CurrentL1);
      dtostrf(atof(instantaneousCurrentL2)/100, 1, 2, CurrentL2);
      dtostrf(atof(instantaneousCurrentL3)/100, 1, 2, CurrentL3);
    }
  //if (reportInDecimals) eenheid = " kWh</div></div>"; else eenheid = " Wh</div></div>";
    if (reportInDecimals) eenheid2 = " kW'></div></p>"; else eenheid2 = " W'></div></p>";
  
    char str2[10];
    int temp;
    addHead(str,0);
    addIntro(str);
	
   // str += ("<p>");
   // str += P1timestamp;
   // str += P1timestamp[7];
   // str += P1timestamp[8];
   // str += P1timestamp[9];
   // str += ("</p>");
    str += F("<fieldset>");
    str += F("<form ><fieldset><legend><b>Messwerte</b></legend>");
   // str += F("<form action='/' method='post'>");

    str += "<p><div class='row'><div class='column'><b>Verbrauch Hochtarif: <br>insgesamt (1.8.1)</b><input type='text' class='form-control c6' value='"; //1.8.1
    str += electricityUsedTariff1; // T211 SmartMeter provides electricityUsedTariff1 as high tariff
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>heute</b><input type='text' class='form-control c7' value='";
    str += atof(electricityUsedTariff1) - atof(log_data.dayE1);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Verbrauch Niedertarif:<br>insgesamt (1.8.2)</b><input type='text' class='form-control c6' value='"; //1.8.2
    str += electricityUsedTariff2; // T211 SmartMeter provides electricityUsedTariff2 as low tariff
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>heute</b><input type='text' class='form-control c7' value='";
    str += atof(electricityUsedTariff2) - atof(log_data.dayE2);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Einspeisung Hochtarif:<br>insgesamt (2.8.1)</b><input type='text' class='form-control c6' value='"; //2.8.1
    str += electricityReturnedTariff1; // T211 SmartMeter provides electricityReturnedTariff1 as high tariff
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>heute</b><input type='text' class='form-control c7' value='";
    str += atof(electricityReturnedTariff1) - atof(log_data.dayR1);
    str += eenheid;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column'><b>Einspeisung Niedertarif:<br>insgesamt (2.8.2)</b><input type='text' class='form-control c6' value='"; //2.8.2
    str += electricityReturnedTariff2; // T211 SmartMeter provides electricityReturnedTariff2 as low tariff
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>heute</b><input type='text' class='form-control c7' value='";
    str += atof(electricityReturnedTariff2) - atof(log_data.dayR2);
    str += eenheid;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column1'><b>Aktuelle Nutzung (1.7.0)</b><input type='text' class='form-control c6' value='";//c6 //<div class='row'
    str += actualElectricityPowerDelivered;
    str += eenheid2;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column1'><b>Aktuelle Einspeisung (2.7.0)</b><input type='text' class='form-control c6' value='";//c6 //<div class='row'
    str += actualElectricityPowerReturned;
    str += eenheid2;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column3'><b>Stromspannung:<br>L1</b><input type='text' class='form-control c7' value='";//c6
    str += instantaneousVoltageL1;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L2</b><input type='text' class='form-control c7' value='";//c7
    str += instantaneousVoltageL2;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L3</b><input type='text' class='form-control c7' value='";//c7
    str += instantaneousVoltageL3;
    str += " V'></div></div></p>";

    str += "<p><div class='row'><div class='column3'><b>Stromstärke: <br>L1</b><input type='text' class='form-control c7' value='";//c6
    str += CurrentL1;
    str += " A'></div>";
    str += "<div class='column3'><br><b>L2</b><input type='text' class='form-control c7' value='";//c7
    str += CurrentL2;
    str += " A'></div>";
    str += "<div class='column3'><br><b>L3</b><input type='text' class='form-control c7' value='";//c7
    str += CurrentL3;
    str += " A'></div></div></p>";

/*
  str += F("<p><b>Voltage dips</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberVoltageSagsL1;
  str += F("'></p>");
  str += F("<p><b>Voltage pieken</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberVoltageSwellsL1;
  str += F("'></p>");
  str += F("<p><b>Stroomonderbrekingen</b><input type='text' class='form-control' style='text-align:right' value='");
  str += numberPowerFailuresAny;
  str += F("'></p>");
*/
  
    str += "<p><div class='row'><div class='column'><b>Gasverbrauch:<br>insgesamt</b><input type='text' class='form-control c6' value='";
    str += gasReceived5min;
    str += F(" m3'></div>");
    str += "<div class='column' style='text-align:right'><br><b>heute</b><input type='text' class='form-control c7' value='";
    str += atof(gasReceived5min) - atof(log_data.dayG);
    str += " m3'></div></div></p>";
    str += F("</fieldset></form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
    addUptime(str);
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = DATA;
  }

  void handleRawData(){
    String str;
    debugln("Raw data requested. Here you go.");
    str = "<html><head></head><body>";
    str += datagram;
    str += "</body></html>";
    if (dataEnd){   
      server.send(200, "text/html", str);
      debugln(datagram);
    }
  }

/*
void uploadDiag(String& str){
  monitoring = false; // stop monitoring data
  addHead(str,0);
  addIntro(str);
  str += F("<fieldset><fieldset><legend><b>Update firmware</b></legend>");
  str += F("<form action='' method='post'><form method='POST' action='' enctype='multipart/form-data'><p>");
  str += F("<b>Firmware</b><input type='file' accept='.bin,.bin.gz' name='firmware'></p>");
  str += F("<button type='submit'>Update Firmware</button>");
  str += F("</form>");
  str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
  addFoot(str);
  webstate = UPDATE;
}
*/

  void handleHelp(){
    char ipstr[20];
    IPAddress ip = WiFi.localIP();
    sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
  
    String str;
    addHead(str,0);
    addIntro(str);
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>Hilfe</b></legend>");
    str += F("<p><b>Das P1 WiFi-Gateway kann Daten auf 4 Arten liefern.</b><br><br>");
    str += F("<p>Die Kernwerte sind immer über das P1-Menü verfügbar (entweder über ");
    str += ipstr;
    str += F("/P1, oder über p1wifi.local/P1).</p><br>");
  
    str += F("<p>Die Rohdaten sind über ");
    str += ipstr;
    str += F("/Data oder über p1wifi.local/Data verfügbar.</p><br><br>");
    str += F("<p>Häufiger ist die Verwendung des Gateways mit einem Hausautomationssystem.</p>");
    str += F("<p><b>Domoticz</b> akzeptiert json-Nachrichten. Geben Sie dazu die IP-Adresse von Ihrem Domoticz-Server ein und die Portnummer, über die darauf zugegriffen werden kann (normalerweise 8080).</p>");
    str += F("Die notwendigen Idx für Gas und Strom erhalten Sie, indem Sie zunächst virtuelle Sensoren in Domoticz für beide erstellen. ");
    str += F("Nach der Erstellung erscheinen die Idxes auf der Registerkarte 'Geräte'.</p><br>");
    str += F("");
    str += F("Über Port 23 des Moduls (p1wifi.local:23) können Daten auch gelesen werden (z.B. von Domoticz). ");
    str += F("Auf diese Weise kann die Domoticz mittels Hardwareeinstellung [P1 Smart Meter mit LAN-Schnittstelle] Daten abrufen. ");
    str += F("Sie brauchen auf der p1wifi Seite nichts einzustellen (Nur Häkchen bei json und mqtt). ");

    str += F("Für andere Systeme können Sie einen MQTT Broker verwenden. Füllen Sie die Details der Broker aus und legen Sie das Stammthema fest. Für Home Assistant ist dies 'sensors/power/p1meter'.</p>");
    str += F("Daniel de Jong beschreibt auf seinem <a href='https://github.com/daniel-jong/esp8266_p1meter'>github</a> wie man HA weiter konfiguriert.</p>");
    str += F("Verwenden Sie die Kontrollkästchen um anzugeben, welche Berichtsmethode(n) Sie verwenden möchten.</p>");

    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
    server.send(200, "text/html", str);
  }
#endif
