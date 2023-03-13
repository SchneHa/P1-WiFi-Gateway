#ifdef NEDERLANDS
void addIntro(String& str){
  char ipstr[20];
  IPAddress ip = WiFi.localIP();
  sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
  str += ("<body><div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-gateway</h2></div><br>");
  str += ("<p style='text-align:right;font-size:11px;color:#aaa'><a href='http://");
  str += ipstr;
  str += ("/Help' target='_blank'>Help</a>");
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
  sprintf_P(strUpTime, PSTR("%d dagen %d uren %d minuten"), dagen, uren, minuten);
  str += strUpTime;
  str += ("</div><div style='text-align:right;font-size:11px;color:#aaa'>");
  str += ipstr;
  str += ("</div>");
}

void addFoot(String& str){
  str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
  
  if (Mqtt) {
    if (MqttConnected) str += F("MQTT link: √ "); else str += F("MQTT – ");
    str += F(" laatste sample: ");
    str += LastReport;
  }
  str += F(" firmware versie: ");
  str += version;
  str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
  str += F("</div></div></body></html>");
}

void addFootBare(String& str){
  str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
  str += F(" firmware versie: ");
  str += version;
  str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
  str += F("</div></div></body></html>");
}

void setupSaved(String& str){
//  str += F("<script>var countDownDate = new Date().getTime()+5000;var x = setInterval(function() {var now = new Date().getTime();var distance = countDownDate - now;var seconds = Math.floor((distance % (1000 * 15)) / 1000);document.getElementById(\"timer\").innerHTML = seconds + \" seconden tot module weer bereikbaar.\";if (seconds < 2) {location.replace(\"http://p1wifi.local\")}}, 1000);</script>");
  str += F("<fieldset>");
  str += F("<fieldset><legend><b>Wifi en module setup</b></legend>");
  str += F("<p><b>De instellingen zijn succesvol bewaard</b><br><br>");
  str += F("<p>De module zal nu herstarten. Dat duurt ongeveer een minuut.</p><br>");
  str += F("<p>De led zal langzaam knipperen tijdens koppelen aan uw WiFi netwerk.</p>");
  str += F("<p>Als de blauwe led blijft branden is de instelling mislukt en zult u <br>");
  str += F("opnieuw moeten koppelen met Wifi netwerk 'P1_Setup' .</p>");
  str += F("<br>");
  //str += F("<p id=\"timer\"></p>");
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
  str += F("<fieldset><fieldset><legend><b>Update firmware</b></legend>");
  str += F("<form method='POST' action='/update' enctype='multipart/form-data'><p>");
  str += F("<b>Firmware</b><input type='file' accept='.bin,.bin.gz' name='firmware'></p>");
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
  str += F("<p><b>De firmware is succesvol bijgewerkt</b><br><br>");
  str += F("<p>De module zal nu herstarten. Dat duurt ongeveer een minuut</p>");
  str += F("<p>De blauwe Led zal 2x oplichten wanneer de module klaar is met opstarten</p>");
  str += F("<p>De led zal langzaam knipperen tijdens koppelen aan uw WiFi netwerk.</p>");
  str += F("<p>Als de blauwe led blijft branden is de instelling mislukt en zult u <br>");
  str += F("opnieuw moeten koppelen met WIfi netwerk 'P1_Setup' met wachtwoord 'configP1'.</p>");
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
  str += F("<form action='/P1' method='post'><button type='p1' class='button bhome'>Meterdata</button></form>");
  #if GRAPH == 1
    str += F("<form action='/Graphs' method='GET'><button type='submit'>Grafieken</button></form>");
    // str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
  #endif
  str += F("</fieldset>");
  str += F("<fieldset><legend><b> Setup </b></legend>");
  str += F("<form action='/Setup' method='post'><button type='Setup'>Setup</button></form>");
  str += F("<form action='/Update' method='GET'><button type='submit'>Update firmware</button></fieldset></form>");
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
   str+= setupToken;
   str+=  F("'>");
   str += F("<fieldset><legend><b>&nbsp;Login&nbsp;</b></legend>");
   str += F("<p><b>Admin password</b><br>");
   str += F("<input type='text' class='form-control' name='adminPassword' value='' </p>");
   str+=  F("</fieldset>");
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
  str+= setupToken;
  str+=  F("'>");
  str += F("<fieldset><legend><b>&nbsp;Login&nbsp;</b></legend>");
  str += F("<p><b>Admin password</b><br>");
  str += F("<input type='text' class='form-control' name='adminPassword' value='' </p>");
  str+=  F("</fieldset>");
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
  str += F("<p><b>Admin password is incorrect.</b><br>");
  str+=  F("</fieldset>");
  str += F("<form action='/");
  str += returnpage;
  str += F("' method='POST'><button class='button bhome'>Opnieuw</button></form></p>");
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
  str += F("<p><b>admin password</b><br>");
  str += F("<input type='text' class='form-control' name='adminPassword' value='");
  str += config_data.adminPassword;
  str += F("'></p></fieldset>");
       
  str += F("<fieldset><legend><b>&nbsp;Wifi parameters&nbsp;</b></legend>");
  str += F("<p><b>SSId</b><br>");
  str += F("<input type='text' class='form-control' name='ssid' value='");
  str += config_data.ssid;
  str += F("'></p>");
  str += F("<p><label><b>Password</b></label><br><input type='password' class='form-control' name='password' value='");
  str += config_data.password;
  str += F("'></p></fieldset>");
      
  str += F("<fieldset><legend><b>&nbsp;Domoticz parameters&nbsp;</b></legend>");
      
  str += F("<p><b>Rapporteer aan Domoticz?</b><input type='checkbox' class='form-control' name='domo' id='domo' ");
      
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

  str += F("<fieldset><legend><b>&nbsp;MQTT parameters&nbsp;</b></legend>");
  
  str += F("<p><b>Rapporteer aan MQTT broker?</b><input type='checkbox' class='form-control' name='mqtt' id='mqtt' ");
  
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
  str += F("<p><b>cfos Power Brain Zähleranzeige in VA (statt in W) </b><input type='checkbox' class='form-control' name='cfosVA' id='cfosVA' ");
  if (config_data.cfosVA[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  str += F("</fieldset>");

  str += F("<fieldset><legend><b>&nbsp;Weitere Einstellungen&nbsp;</b></legend>");
  str += F("<b>Meetinterval in sec (> 10 sec)</b><input type='text' class='form-control' name='interval' value='");
  str += config_data.interval; 
  str += F("'></p><p>");
  str += F("<p><b>Rapporteer in watt (ipv van in kWh) </b><input type='checkbox' class='form-control' name='watt' id='watt' ");
  if (config_data.watt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  str += F("<p><b>Activeer Telnet poort (23) </b><input type='checkbox' class='form-control' name='telnet' id='telnet' ");
  if (config_data.telnet[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  str += F("<p><b>Debug via MQTT </b><input type='checkbox' class='form-control' name='debug' id='debug' ");
  if (config_data.debug[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  str += F("</fieldset><div></div>");
  str += F("<p><button type='submit'>Save</button></form>");
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

  // str += ("</p>");
  str += F("<fieldset>");
  str += F("<form ><fieldset><legend><b>Meetwaarden</b></legend>");
  // str += F("<form action='/' method='post'>");

  str += "<p><div class='row'><div class='column'><b>Verbruik laag tarief: <br>totaal</b><input type='text' class='form-control c6' value='";
  str += electricityUsedTariff1;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>vandaag</b><input type='text' class='form-control c7' value='";
  str += atof(electricityUsedTariff1) - atof(log_data.dayE1);
  str += eenheid;
  str += "</div></p>";

  str += "<p><div class='row'><div class='column'><b>Verbruik hoog tarief: <br>totaal</b><input type='text' class='form-control c6' value='";
  str += electricityUsedTariff2;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>vandaag</b><input type='text' class='form-control c7' value='";
  str += atof(electricityUsedTariff2) - atof(log_data.dayE2);
  str += eenheid;
  str += "</div></p>";

  str += "<p><div class='row'><div class='column'><b>Retour laag tarief: <br>totaal</b><input type='text' class='form-control c6' value='";
  str += electricityReturnedTariff1;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>vandaag</b><input type='text' class='form-control c7' value='";
  str += atof(electricityReturnedTariff1) - atof(log_data.dayR1);
  str += eenheid;
  str += "</div></p>";
  
  str += "<p><div class='row'><div class='column'><b>Retour hoog tarief: <br>totaal</b><input type='text' class='form-control c6' value='";
  str += electricityReturnedTariff2;
  str += eenheid;
  str += "<div class='column' style='text-align:right'><br><b>vandaag</b><input type='text' class='form-control c7' value='";
  str += atof(electricityReturnedTariff2) - atof(log_data.dayR2);
  str += eenheid;
  str += "</div></p>";
  
  str += "<p><div class='row'><div class='column1'><b>Actueel verbruik</b><input type='text' class='form-control c6' value='";
  str += actualElectricityPowerDelivered;
  str += eenheid2;
  str += "</div></p>";

  str += "<p><div class='row'><div class='column1'><b>Actuele teruglevering</b><input type='text' class='form-control c6' value='";
  str += actualElectricityPowerReturned;
  str += eenheid2;
  str += "</div></p>";
  
//  if (P1prot == 5){
    str += "<p><div class='row'><div class='column3'><b>Voltage: <br>L1</b><input type='text' class='form-control c6' value='";
    str += instantaneousVoltageL1;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L2</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL2;
    str += " V'></div>";
    str += "<div class='column3'><br><b>L3</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL2;
    str += " V'></div></div></p>";

    str += "<p><div class='row'><div class='column3'><b>Amperage: <br>L1</b><input type='text' class='form-control c6' value='";
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
  
  str += "<p><div class='row'><div class='column'><b>Gasverbruik: <br>totaal</b><input type='text' class='form-control c6' value='";
  str += gasReceived5min;
  str += F(" m3'></div>");
  str += "<div class='column' style='text-align:right'><br><b>vandaag</b><input type='text' class='form-control c7' value='";
  str += atof(gasReceived5min) - atof(log_data.dayG);
  str += " m3'></div></div></p>";
  str += F("</fieldset></form>");
  str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
  addUptime(str);
  addFoot(str);
  server.send(200, "text/html", str);
  webstate = DATA;
//  }
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

void handleHelp(){
  char ipstr[20];
  IPAddress ip = WiFi.localIP();
  sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
  
  String str;
  addHead(str,0);
  addIntro(str);
  str += F("<fieldset>");
  str += F("<fieldset><legend><b>Help</b></legend>");
  str += F("<p><b>De P1 wifigateway kan op 4 manieren data afleveren.</b><br><br>");
  str += F("<p>Altijd beschikbaar zijn de kernwaarden via het P1 menu (of via ");
  str += ipstr;
  str += F("/P1, of via p1wifi.local/P1 )</p><br>");
  
  str += F("<p>De ruwe data is beschikbaar via ");
  str += ipstr;
  str += F("/Data, of via p1wifi.local/Data</p><br><br>");
  str += F("<p>Meer gangbaar is het gebruik van de gateway met een domotica systeem.</p>");
  str += F("<p><b>Domoticz</b> accepteert json berichten. Geef hiertoe het IP adres van <br>");
  str += F("uw Domoticz server op en het poortnummer waarop deze kan worden benaderd (doorgaans 8080).</p>");
  str += F("De benodigde Idx voor gas en electra verkrijgt u door eerst in Domoticz virtule sensors ");
  str += F("voor beiden aan te maken. Na creatie verschijnen de Idxen onder het tabblad 'devices'.</p><br>");
  str += F("");
  str += F("Data kan ook (door Domoticz bijvoorbeeld) worden uitgelezen via poort 23 van de module (p1wifi.local:23).");
  str += F("Dit is de manier waarop Domoticz hardware device [P1 Smart Meter with LAN interface] data kan ophalen.");
  str += F("Aan de p1wifi kant hoeft daarvoor niets te worden ingesteld (geen vinkjes bij json en mqtt). ");

  str += F("Voor andere systemen kunt u gebruik maken van een MQTT broker. Vul de gegevens van de ");
  str += F("broker in, en stel het root topic in. Voor Home Assistant is dat 'sensors/power/p1meter'.</p>");
  str += F("Daniel de Jong beschijft op zijn <a href='https://github.com/daniel-jong/esp8266_p1meter'>github</a> hoe u HA verder configureert.</p>");
  str += F("Geef met de checkboxes aan welke rapportage methode(n) u wilt gebruiken.</p>");

  str += F("</fieldset></p>");
  str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
  server.send(200, "text/html", str);
}
#endif
