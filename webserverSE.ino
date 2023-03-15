#ifdef SWEDISH
  void addIntro(String& str){
    char ipstr[20];
    IPAddress ip = WiFi.localIP();
    sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
    str += ("<body><div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-gateway</h2></div><br>");
    str += ("<p style='text-align:right;font-size:11px;color:#aaa'><a href='http://");
    str += ipstr;
    str += ("/Help' target='_blank'>Hjälp</a>");
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
    sprintf_P(strUpTime, PSTR("%d dagar %d timmar %d minuter"), dagen, uren, minuten);
    str += strUpTime;
    str += ("<span style='float:right;font-size:11px;color:#aaa'>");
    str += ipstr;
    str += ("</span></div>");
  }

/*
void addFoot(String& str){
  str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>version: ");
  str += version;
  char sysmsg[100];
  sprintf_P(sysmsg, PSTR(" – %1.2fV"), volts / 1000);
  str += sysmsg;
  str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
  str += F("</div></div></body></html>");
}
*/

  void addFoot(String& str) {
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
  
    if (Mqtt) {
      if (MqttConnected) str += F("MQTT link: √ "); else str += F("MQTT – ");
      str += F(" sista prov ");
      str += LastReport;
	  str += F("<br>");
    }
    str += F(" Firmware version ");
    str += version;
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void addFootBare(String& str) {
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
    str += F(" firmware versie: ");
    str += version;
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void setupSaved(String& str) {
 // str += F("<script>var countDownDate = new Date().getTime()+600000;var x = setInterval(function() {var now = new Date().getTime();var distance = countDownDate - now;var seconds = Math.floor((distance % (1000 * 60)) / 1000);document.getElementById(\"timer\").innerHTML = seconds + \" seconden tot module weer bereikbaar.\";if (seconds < 2) {location.replace(\"http://p1wifi.local\")}}, 1000);</script>");
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>Wifi och moduluppställning</b></legend>");
    str += F("<p><b>Inställningarna har sparats.</b><br><br>");
    str += F("<p>Modulen kommer nu att starta om. Det tar ungefär en minut.</p><br>");
    str += F("<p>Den blå lysdioden tänds 2x när modulen har startat klart.</p>");
    str += F("<p>Lysdioden blinkar långsamt när den ansluter till ditt WiFi-nätverk.</p>");
    str += F("<p>Om den blå lysdioden fortsätter att lysa har inställningen misslyckats och du måste återansluta till WiFi-nätverket 'P1_Setup' .</p>");
    str += F("<br>");
 // str += F("<p id=\"timer\"></p>");
    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
  }

  void handleUploadForm(){
    if (strcmp(server.arg("adminPassword").c_str(), config_data.adminPassword) != 0) {  // passwords don't match
      debugln("Error: update entered with wrong password");
      errorLogin("Update");
      return;
    } else  AdminAuthenticated = true;
    String str="";
    monitoring = false; // stop monitoring data
    addHead(str,0);
    addIntro(str);
    str += F("<fieldset><fieldset><legend><b>Uppdatera firmware</b></legend>");
    str += F("<form method='POST' action='/update' enctype='multipart/form-data'><p>");
    str += F("<b>Firmware</b><input type='file' accept='.bin,.bin.gz' name='Firmware'></p>");
    str += F("<button type='submit'>Uppdatera</button>");
    str += F("</form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
    addFootBare(str); 
    webstate = UPDATE;
    server.send(200, "text/html", str);
  }

  void successResponse() {
    String str = "";
    addRefreshHead(str);
    addIntro(str);
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>Uppdatera firmware</b></legend>");
    str += F("<p><b>Den fasta programvaran har uppdaterats</b><br><br>");
    str += F("<p>Modulen kommer nu att starta om. Det tar ungefär en minut.</p><br>");
    str += F("<p>Den blå lysdioden tänds 2x när modulen har startat klart</p>");
    str += F("<p>Lysdioden blinkar långsamt när du ansluter till ditt WiFi-nätverk.</p>");
    str += F("<p>Om den blå lysdioden fortsätter att lysa har installationen <br>");
    str += F("<p>misslyckats och du kommer att göra det behöver återkoppla  <br>");
    str += F("med WiFi-nätverket 'P1_Setup'.</p>");
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
    str += F("<form action='/P1' method='post'><button type='p1' class='button bhome'>Avläsningar</button></form>");
    #if GRAPH == 1
      str += F("<form action='/Graphs' method='GET'><button type='submit'>Grafik</button></form>");
    // str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
    #endif
    str += F("</fieldset>");
    str += F("<fieldset><legend><b> Setup </b></legend>");
    str += F("<form action='/Setup' method='post'><button type='Setup'>Uppstart</button></form>");
    str += F("<form action='/Update' method='GET'><button type='submit'>Uppdatera firmware</button></fieldset></form>");
    addUptime(str);
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = MAIN;
    monitoring = true;
    nextUpdateTime = millis() + 2000;
  }

  void handleLogin() {
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

  void handleUpdateLogin() {
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

  void errorLogin(String returnpage) {
    debugln("errorLogin");
    String str = "";
    addHead(str,0);
    addIntro(str);
    str += F("<fieldset><legend><b>Fout</b></legend>");
    str += F("<p><b>Administratörslösenordet är fel.</b><br>");
    str += F("</fieldset>");
    str += F("<form action='/");
    str += returnpage;
    str += F("' method='POST'><button class='button bhome'>upprepa</button></form></p>");
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
    str += F("<form action='/SetupSave' method='POST'><fieldset>");
    str += F("<input type='hidden' name='setuptoken' value='");
    str += setupToken;
    str += F("'>");
    str += F("<fieldset><legend><b>&nbsp;Admin&nbsp;</b></legend>");
    str += F("<p><b>Admin Passwort</b><br>");
    str += F("<input type='text' class='form-control' name='adminPassword' value='");
    str += config_data.adminPassword;
    str += F("'></p></fieldset>");
  
    str += F("<fieldset><legend><b>&nbsp;Wifi parametrar&nbsp;</b></legend>");
    str += F("<p><b>SSID</b><br>");
    str += F("<input type='text' class='form-control' name='ssid' value='");
    str += config_data.ssid;
    str += F("'></p>");
    str += F("<p><label><b>Password</b></label><br><input type='password' class='form-control' name='password' value='");
    str += config_data.password;
    str += F("</fieldset>");
  
    str += F("<fieldset><legend><b>&nbsp;Domoticz parameters&nbsp;</b></legend>");
      
    str += F("<p><b>Rapportera till Domoticz?</b><input type='checkbox' class='form-control' name='domo' id='domo' ");
      
    if (config_data.domo[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Domoticz IP address</b><input type='text' class='form-control' name='domoticzIP' value='");
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

    str += F("<fieldset><legend><b>&nbsp;MQTT parametrar&nbsp;</b></legend>");
  
    str += F("<p><b>Rapportera till MQTT-mäklare?</b><input type='checkbox' class='form-control' name='mqtt' id='mqtt' ");
  
    if (config_data.mqtt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>MQTT broker IP address</b><input type='text' class='form-control' name='mqttIP' value='");
    str += config_data.mqttIP;
    str += F("'></p><p>");
    str += F("<b>MQTT broker Port</b><input type='text' class='form-control' name='mqttPort' value='");
    str += config_data.mqttPort;
    str += F("'></p><p>");
    str += F("<b>MQTT user</b><input type='text' class='form-control' name='mqttUser' value='");
    str += config_data.mqttUser;
    str += F("'></p><p>");
    str += F("<b>MQTT password</b><input type='text' class='form-control' name='mqttPass' value='");
    str += config_data.mqttPass;
    str += F("'></p>");
    str += F("<b>MQTT root topic</b><input type='text' class='form-control' name='mqttTopic' value='");
    str += config_data.mqttTopic;
    str += F("'></p>");
    str += F("</fieldset>");

    str += F("<fieldset><legend><b>&nbsp;cFos Power Brain Wallbox Parameter&nbsp;</b></legend>");
  
    str += F("<p><b>Rapportera till cFos Power Brain?</b><input type='checkbox' class='form-control' name='cfos' id='cfos' ");
  
    if (config_data.cfos[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>cFos Power Brain IP address</b><input type='text' class='form-control' name='cfosIP' value='");
    str += config_data.cfosIP;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain port</b><input type='text' class='form-control' name='cfosPort' value='");
    str += config_data.cfosPort;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain user</b><input type='text' class='form-control' name='cfosUsr' value='");
    str += config_data.cfosUsr;
    str += F("'></p><p>");
    str += F("<b>cFos Power Brain password</b><input type='text' class='form-control' name='cfosPwd' value='");
    str += config_data.cfosPwd;
    str += F("'></p>");
    str += F("<b>cFos Power Brain meter modell</b><input type='text' class='form-control' name='cfosModel' value='");
    str += config_data.cfosModel;
    str += F("'></p>");
    str += F("<b>cFos Power Brain meterID</b><input type='text' class='form-control' name='cfosID' value='");
    str += config_data.cfosID;
    str += F("'></p>");
    str += F("<p><b>cfos Power Brain räknardisplay in VA (statt in W) </b><input type='checkbox' class='form-control' name='cfosVA' id='cfosVA' ");
    if (config_data.cfosVA[0] =='j') str += F(" checked></p>"); else str += F("></p>");
//  str += F("'></p>");

    str += F("</fieldset>");
    str += F("<fieldset><legend><b>&nbsp;Fler inställningar&nbsp;</b></legend>");
    str += F("<b>Mätintervall in sec (> 10 sec)</b><input type='text' class='form-control' name='interval' value='");
    str += config_data.interval; 
    str += F("'></p><p>");
    str += F("<p><b>Rapportera in Watt (istället för in kW) </b><input type='checkbox' class='form-control' name='watt' id='watt' ");
    if (config_data.watt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Activera Telnet (23) </b><input type='checkbox' class='form-control' name='telnet' id='telnet' ");
    if (config_data.telnet[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Debug po MQTT </b><input type='checkbox' class='form-control' name='debug' id='debug' ");
    if (config_data.debug[0] =='j') str += F(" checked></p>"); else str += F("></p>");

    str += F("</fieldset><div></div>");
    str += F("<p><button type='submit'>Spara</button></form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Meny</button></form></p>");
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
    str += F("<form ><fieldset><legend><b>Mätardata</b></legend>");
   // str += F("<form action='/' method='post'>");

    str += "<p><div class='row'><div class='column'><b>Aktiv energi uttag</b><input type='text' class='form-control c6' value='";
    str += cumulativeActiveImport;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><b>idag</b><input type='text' class='form-control c7' value='";
    str += atof(cumulativeActiveImport) - atof(log_data.dayE1);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Reaktiv energi uttag</b><input type='text' class='form-control c6' value='";
    str += cumulativeReactiveImport;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><b>idag</b><input type='text' class='form-control c7' value='";
    str += atof(cumulativeReactiveImport) - atof(log_data.dayE2);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Aktiv energi inmatning</b><input type='text' class='form-control c6' value='";
    str += cumulativeActiveExport;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><b>idag</b><input type='text' class='form-control c7' value='";
    str += atof(cumulativeActiveExport) - atof(log_data.dayR1);
    str += eenheid;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column'><b>Reaktiv energi inmatning</b><input type='text' class='form-control c6' value='";
    str += cumulativeReactiveExport;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><b>idag</b><input type='text' class='form-control c7' value='";
    str += atof(cumulativeReactiveExport) - atof(log_data.dayR2);
    str += eenheid;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column1'><b>Aktiv Effekt Uttag Momentan trefaseffekt</b><input type='text' class='form-control c6' value='";
    str += momentaryActiveImport;
    str += eenheid2;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column1'><b>Fasspänning</b><input type='text' class='form-control c6' value='";
    str += momentaryActiveExport;
    str += eenheid2;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column3'><b>Aktiv Effekt Uttag:<br>L1</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL1;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L2</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL2;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L3</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL3;
    str += " V'></div></div></p>";

    str += "<p><div class='row'><div class='column3'><b>Fasström:<br>L1</b><input type='text' class='form-control c7' value='";
    str += CurrentL1;
    str += " A'></div>";
    str += "<div class='column3'><br><b>L2</b><input type='text' class='form-control c7' value='";
    str += CurrentL2;
    str += " A'></div>";
    str += "<div class='column3'><br><b>L3</b><input type='text' class='form-control c7' value='";
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
  
    str += "<p><div class='row'><div class='column'><b>Gasförbrukning:<br> total</b><input type='text' class='form-control c6' value='";
    str += gasReceived5min;
    str += F(" m3'></div>");
    str += "<div class='column' style='text-align:right'><br><b>idag</b><input type='text' class='form-control c7' value='";
    str += atof(gasReceived5min) - atof(log_data.dayG);
    str += " m3'></div></div></p>";
    str += F("</fieldset></form>");
    str += F("<form action='/' method='POST'><button class='button bhome'>Meny</button></form>");
    addUptime(str);
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = DATA;
  }

  void handleRawData(){
    String str;
    debugln("Raw data requested. Here you go.");
    str= "<html><head></head><body>";
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
  str += F("<button type='submit'>Uppdatera firmware</button>");
  str += F("</form>");
  str += F("<form action='/' method='POST'><button class='button bhome'>Meny</button></form>");
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
    str += F("<fieldset><legend><b>Hjälp</b></legend>");
    str += F("<p><b>P1 WiFi-gatewayen kan leverera data på fyra sätt.</b><br><br>");
    str += F("<p>Kärnvärdena är alltid tillgängliga via P1 Meny (of via ");
    str += ipstr;
    str += F("/P1, eller via p1wifi.local/P1 )</p><br>");
  
    str += F("<p>Rådata är tillgänglig via ");
    str += ipstr;
    str += F("/Data, eller via p1wifi.local/Data )</p><br><br>");
    str += F("<p>Vanligare är användningen av gatewayen med ett hemautomationssystem.</p>");
    str += F("<p><b>Domoticz</b> accepterar json-meddelanden. För att göra detta, ange IP-adressen för ");
    str += F("din Domoticz-server och portnumret på vilken den kan nås (vanligtvis 8080).</p>");
    str += F("Du får den nödvändiga Idx för gas och el genom att först använda virtuella sensorer i Domoticz ");
    str += F("att skapa för båda. Efter skapande visas Idxerna under fliken 'devices'.</p><br>");
    str += F("");
    str += F("Data kan också läsas (av till exempel Domoticz) via port 23 på modulene (p1wifi.local:23).");
    str += F("Så här kan Domoticz hårdvaruenhet [P1 Smart Meter med LAN-gränssnitt] hämta data.");
    str += F("Ingenting behöver ställas in på p1wifi-sidan för detta (inga bockar vid json och mqtt ). ");

    str += F("För andra system kan du använda en MQTT-mäklare. Fyll i uppgifterna om ");
    str += F("mäklare och ställ in rotämnet. För Home Assistant alltså 'sensors/power/p1meter'.</p>");
    str += F("Daniel de Jong beskriver i sin <a href='https://github.com/daniel-jong/esp8266_p1meter'>github</a> hur man konfigurerar HA ytterligare.</p>");
    str += F("Använd kryssrutorna för att ange vilken/vilka rapporteringsmetoder du vill använda.</p>");
    str += F("Erik Forsberg beskriver i sin <a href='https://github.com/forsberg/esphome-p1reader'>github</a> hur man konfigurerar esphome ytterligare.</p>");

    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
    server.send(200, "text/html", str);
  }
#endif
