#ifdef FRENCH
  void addIntro(String& str){
    char ipstr[20];
    IPAddress ip = WiFi.localIP();
    sprintf_P(ipstr, PSTR("%u.%u.%u.%u"), ip[0], ip[1], ip[2], ip[3]);
    str += ("<body><div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-gateway</h2></div><br>");
    str += ("<p style='text-align:right;font-size:11px;color:#aaa'><a href='http://");
    str += ipstr;
    str += ("/Help' target='_blank'>Aider</a>");
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
    sprintf_P(strUpTime, PSTR("%d jours %d heures %d minutes"), dagen, uren, minuten);
    str += strUpTime;
    str += ("<span style='float:right;font-size:11px;color:#aaa'>");
    str += ipstr;
    str += ("</span></div>");
  }

  void addFoot(String& str){
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
  
    if (Mqtt) {
      if (MqttConnected) str += F("MQTT link: √ "); else str += F("MQTT – ");
      str += F(" letztes sample: ");
      str += LastReport;
  }  
	str += F(" Firmware version: ");
	str += version;
	
//    char sysmsg[100];
//    sprintf_P(sysmsg, PSTR(" – %1.2fV"), volts / 1000);
//    str += sysmsg;
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void addFootBare(String& str){
    str += F("<div style='text-align:right;font-size:11px;color:#aaa;'><hr/>");
    str += F(" firmware version: ");
    str += version;
    str += F("<br><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a>");
    str += F("</div></div></body></html>");
  }

  void setupSaved(String& str){
//  str += F("<script>var countDownDate = new Date().getTime()+5000;var x = setInterval(function() {var now = new Date().getTime();var distance = countDownDate - now;var seconds = Math.floor((distance % (1000 * 15)) / 1000);document.getElementById(\"timer\").innerHTML = seconds + \" seconden tot module weer bereikbaar.\";if (seconds < 2) {location.replace(\"http://p1wifi.local\")}}, 1000);</script>");
    str += F("<fieldset>");
    str += F("<fieldset><legend><b>Wifi et configuration du module</b></legend>");
    str += F("<p><b>Les paramètres ont été enregistrés avec succès.</b><br><br>");
    str += F("<p>Le module va maintenant redémarrer. Cela prendra environ une minute.</p><br>");
    str += F("<p>La Led bleue s'allumera 2x lorsque le module aura fini de démarrer.</p>");
    str += F("<p>La LED clignotera lentement lors de la connexion à votre réseau WiFi.</p>");
    str += F("<p>Si la LED bleue reste allumée, c'est que le réglage a échoué. Reconnectez vous alors au réseau WiFi 'P1_Setup' pour corriger les paramètres.</p>");
    str += F("<br>");
  //str += F("<p id=\"timer\"></p>");
    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
  }

  void handleUploadForm(){
    if (strcmp(server.arg("adminPassword").c_str(), config_data.adminPassword) != 0) {  // passwords doesn't match
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
//    str += F("<fieldset><legend><b>Wifi et configuration du module</b></legend>");
    str += F("<fieldset><legend><b>Mise à jour du firmware</b></legend>");
//    str += F("<p><b>Les paramètres ont été enregistrés avec succès.</b><br><br>");
	str += F("<p><b>Le firmware a été mis à jour avec succès.</b><br><br>");	
    str += F("<p>Le module va maintenant redémarrer. Cela prendra environ une minute.</p><br>");
    str += F("<p>La Led bleue s'allumera 2x lorsque le module aura fini de démarrer.</p>");
    str += F("<p>La LED clignotera lentement lors de la connexion à votre réseau WiFi.</p>");
    str += F("<p>Si la LED bleue reste allumée, Si la LED bleue reste allumée, c'est que le réglage a échoué. Reconnectez vous alors au réseau WiFi 'P1_Setup' pour corriger les paramètres.</p>");
    str += F("<br>");
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
    str += F("<form action='/P1' method='post'><button type='p1' class='button bhome'>Valeurs mesurées</button></form>");
    #if GRAPH == 1
      str += F("<form action='/Graphs' method='GET'><button type='submit'>Graphique</button></form>");
    // str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
    #endif
    str += F("</fieldset>");
    str += F("<fieldset><legend><b> Setup </b></legend>");	
    str += F("<form action='/Setup' method='post'><button type='Setup'>Configuration</button></form>");
    str += F("<form action='/update' method='GET'><button type='submit'>Mise à jour du firmware</button></form>");
    addUptime(str);
    addFoot(str);
    server.send(200, "text/html", str);
    webstate = MAIN;
    monitoring = true;
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
    str += F("<p><b>Le mot de passe administrateur est incorrect.</b><br>");
    str += F("</fieldset>");
    str += F("<form action='/");
    str += returnpage;
    str += F("' method='POST'><button class='button bhome'>Encore</button></form></p>");
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
    str += F("<p><b>Mot de passe admin</b><br>");
    str += F("<input type='text' class='form-control' name='adminPassword' value='");
    str += config_data.adminPassword;
    str += F("'></p></fieldset>");
       
    str += F("<fieldset><legend><b>&nbsp;Paramètres Wi-Fi&nbsp;</b></legend>");
    str += F("<p><b>SSID</b><br>");
    str += F("<input type='text' class='form-control' name='ssid' value='");
    str += config_data.ssid;
    str += F("'></p>");
    str += F("<p><label><b>Mot de passe</b></label><br><input type='password' class='form-control' name='password' value='");
    str += config_data.password;
    str += F("'></p></fieldset>");
      
    str += F("<fieldset><legend><b>&nbsp;Paramètres Domoticz&nbsp;</b></legend>");
      
    str += F("<p><b>Signaler à Domoticz?</b><input type='checkbox' class='form-control' name='domo' id='domo' ");
      
    if (config_data.domo[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Adresse IP Domoticz</b><input type='text' class='form-control' name='domoticzIP' value='");
    str += config_data.domoticzIP;
    str += F("'></p><p>");
    str += F("<b>Port Domoticz</b><input type='text' class='form-control' name='domoticzPort' value='");
    str += config_data.domoticzPort;
    str += F("'></p><p>");
    str += F("<b>Domoticz Gaz Idx</b><input type='text' class='form-control' name='domoticzGasIdx' value='");
    str += config_data.domoticzGasIdx;
    str += F("'></p><p>");
    str += F("<b>Domoticz Energy Idx</b><input type='text' class='form-control' name='domoticzEnergyIdx' value='");
    str += config_data.domoticzEnergyIdx;
    str += F("'></p>");
    str += F("</fieldset>");

    str += F("<fieldset><legend><b>&nbsp;Paramètres MQTT&nbsp;</b></legend>");
  
    str += F("<p><b>Activer le protocole MQTT?</b><input type='checkbox' class='form-control' name='mqtt' id='mqtt' ");
  
    if (config_data.mqtt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Adresse IP du serveur MQTT</b><input type='text' class='form-control' name='mqttIP' value='");
    str += config_data.mqttIP;
    str += F("'></p><p>");
    str += F("<b>Port du serveur MQTT</b><input type='text' class='form-control' name='mqttPort' value='");
    str += config_data.mqttPort;
    str += F("'></p><p>");
    str += F("<b>Utilisateur MQTT</b><input type='text' class='form-control' name='mqttUser' value='");
    str += config_data.mqttUser;
    str += F("'></p><p>");
    str += F("<b>Mot de passe MQTT</b><input type='text' class='form-control' name='mqttPass' value='");
    str += config_data.mqttPass;
    str += F("'></p>");
    str += F("<b>Rubrique racine MQTT</b><input type='text' class='form-control' name='mqttTopic' value='");
    str += config_data.mqttTopic;
    str += F("'></p>");
    str += F("</fieldset>");
  
    str += F("<fieldset><legend><b>&nbsp;Paramètres cFos Power Brain Wallbox&nbsp;</b></legend>");
  
    str += F("<p><b>Signaler à cFos Power Brain?</b><input type='checkbox' class='form-control' name='cfos' id='cfos' ");
  
    if (config_data.cfos[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Adresse IP cFos Power Brain</b><input type='text' class='form-control' name='cfosIP' value='");
    str += config_data.cfosIP;
    str += F("'></p><p>");
    str += F("<b>Port du serveur cFos Power Brain</b><input type='text' class='form-control' name='cfosPort' value='");
    str += config_data.cfosPort;
    str += F("'></p><p>");
    str += F("<b>Utilisateur cFos Power Brain</b><input type='text' class='form-control' name='cfosUsr' value='");
    str += config_data.cfosUsr;
    str += F("'></p><p>");
    str += F("<b>Mot de passe cFos Power Brain</b><input type='text' class='form-control' name='cfosPwd' value='");
    str += config_data.cfosPwd;
    str += F("'></p>");
    str += F("<b>cFos Power Brain modèle de compteur</b><input type='text' class='form-control' name='cfosModel' value='");
    str += config_data.cfosModel;
    str += F("'></p>");
    str += F("<b>cFos Power Brain ID de compteur</b><input type='text' class='form-control' name='cfosID' value='");
    str += config_data.cfosID;
    str += F("'></p>");
    str += F("<p><b>cfos Power Brain affichage du compteur dans VA (statt in W) </b><input type='checkbox' class='form-control' name='cfosVA' id='cfosVA' ");
    if (config_data.cfosVA[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("</fieldset>");

    str += F("<fieldset><legend><b>&nbsp;Plus de réglages&nbsp;</b></legend>");
  
    str += F("<b>Intervalle de mesure en sec (> 10 sec)</b><input type='text' class='form-control' name='interval' value='");
    str += config_data.interval; 
    str += F("'></p><p>");
    str += F("<p><b>Rapport en Watts (au lieu de kW) </b><input type='checkbox' class='form-control' name='watt' id='watt' ");
    if (config_data.watt[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Activer le port Telnet (23) </b><input type='checkbox' class='form-control' name='telnet' id='telnet' ");
    if (config_data.telnet[0] =='j') str += F(" checked></p>"); else str += F("></p>");
    str += F("<p><b>Debug via MQTT </b><input type='checkbox' class='form-control' name='debug' id='debug' ");
    if (config_data.debug[0] =='j') str += F(" checked></p>"); else str += F("></p>");
  
    str += F("</fieldset><div></div>");
    str += F("<p><button type='submit'>Sauvegarder</button></form>");
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
	
  //  str += ("<p>");
  //  str += P1timestamp;
   // str += P1timestamp[7];
   // str += P1timestamp[8];
   // str += P1timestamp[9];
 // str += ("</p>");
    str += F("<fieldset>");
    str += F("<form ><fieldset><legend><b>Valeurs mesurées</b></legend>");
 // str += F("<form action='/' method='post'>");

    str += "<p><div class='row'><div class='column'><b>Consommation heures creuses: total</b><input type='text' class='form-control c6' value='";
    str += electricityUsedTariff1;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>aujourd'hui</b><input type='text' class='form-control c7' value='";
    str += atof(electricityUsedTariff1) - atof(log_data.dayE1);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Consommation heures pleines: total</b><input type='text' class='form-control c6' value='";
    str += electricityUsedTariff2;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>aujourd'hui</b><input type='text' class='form-control c7' value='";
    str += atof(electricityUsedTariff2) - atof(log_data.dayE2);
    str += eenheid;
    str += "</div></p>";

    str += "<p><div class='row'><div class='column'><b>Production heures creuses: total</b><input type='text' class='form-control c6' value='";
    str += electricityReturnedTariff1;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>aujourd'hui</b><input type='text' class='form-control c7' value='";
    str += atof(electricityReturnedTariff1) - atof(log_data.dayR1);
    str += eenheid;
    str += "</div></p>";
  
    str += "<p><div class='row'><div class='column'><b>Production heures pleines: total</b><input type='text' class='form-control c6' value='";
    str += electricityReturnedTariff2;
    str += eenheid;
    str += "<div class='column' style='text-align:right'><br><b>aujourd'hui</b><input type='text' class='form-control c7' value='";
    str += atof(electricityReturnedTariff2) - atof(log_data.dayR2);
    str += eenheid;
    str += "</div></p>";
  

    str += "<p><div class='row'><b>Consommation instantanée de courant</b><input type='text' class='form-control c6' value='";
    str += actualElectricityPowerDelivered;
    str += eenheid2;


    str += "<p><div class='row'><b>Production instantanée de courant</b><input type='text' class='form-control c6' value='";
    str += actualElectricityPowerReturned;
    str += eenheid2;

    str += "<p><div class='row'><div class='column3'><b>Voltage: L1</b><input type='text' class='form-control c6' value='";
    str += instantaneousVoltageL1;
    str += " V'></div>";
    str += "<div class='column3' style='text-align:right'><b>L2</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL2;
    str += " V'></div>";
    str += "<div class='column3' style='text-align:right'><b>L3</b><input type='text' class='form-control c7' value='";
    str += instantaneousVoltageL2;
    str += " V'></div></div></p>";

    str += "<p><div class='row'><div class='column3'><b>Ampérage: L1</b><input type='text' class='form-control c6' value='";
    str += CurrentL1;
    str += " A'></div>";
    str += "<div class='column3' style='text-align:right'><b>L2</b><input type='text' class='form-control c7' value='";
    str += CurrentL2;
    str += " A'></div>";
    str += "<div class='column3' style='text-align:right'><b>L3</b><input type='text' class='form-control c7' value='";
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
    str += "<p><div class='row'><div class='column'><b>Consommation de gaz: total</b><input type='text' class='form-control c6' value='";
    str += gasReceived5min;
    str += F(" m3'></div>");
    str += "<div class='column' style='text-align:right'><b>aujourd'hui</b><input type='text' class='form-control c7' value='";
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
    str += F("<fieldset><fieldset><legend><b>Mise à jour du firmware</b></legend>");
    str += F("<form action='' method='post'><form method='POST' action='' enctype='multipart/form-data'><p>");
    str += F("<b>Micrologiciel</b><input type='file' accept='.bin,.bin.gz' name='firmware'></p>");
    str += F("<button type='submit'>Mettre à jour</button>");
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
    str += F("<fieldset><legend><b>Aider</b></legend>");
    str += F("<p><b>La passerelle WiFi P1 peut fournir des données de 4 manières.</b><br><br>");
    str += F("<p>Les valeurs fondamentales sont toujours disponibles via le menu P1 (ou via ");
    str += ipstr;
    str += F("/P1, ou via p1wifi.local/P1)</p><br>");
  
    str += F("<p>Les données brutes sont disponibles via  ");
    str += ipstr;
    str += F("/Data, ou via p1wifi.local/Data</p><br><br>");
    str += F("<p>Le plus courante est l'utilisation de la passerelle avec un système domotique.</p>");
    str += F("<p><b>Domoticz</b> accepte les messages json. Pour ce faire, entrez l'adresse IP de votre serveur Domoticz et le numéro de port sur lequel il est accessible (généralement 8080).</p>");
    str += F("Vous obtenez les Idx nécessaires pour le gaz et l'électricité en créant d'abord des capteurs virtuels pour les deux dans Domoticz. Une fois créés, les Idx apparaîtront sous l'onglet 'devices'.</p><br>");
    str += F("");
    str += F("Les données peuvent également être lues (par Domoticz par exemple) via le port 23 du module (p1wifi.local:23).C'est ainsi que le dispositif matériel Domoticz (P1 Smart Meter avec interface LAN) peut récupérer des données, il n'a rien à régler (ne pas cochesr le json et le MQTT). Pour les autres systèmes, vous pouvez utiliser un serveur MQTT. Remplissez les paramètres du serveur et définissez le sujet racine. Pour Home Assistant, il s'agit de 'capteurs/alimentation/p1mètre'. ");
    str += F("Daniel de Jong décrit sur son github comment configurer davantage HA. <br>Utilisez les cases à cocher pour indiquer la ou les méthodes de déclaration que vous souhaitez utiliser.</p>");

    str += F("</fieldset></p>");
    str += F("<div style='text-align:right;font-size:11px;'><hr/><a href='http://esp8266thingies.nl' target='_blank' style='color:#aaa;'>esp8266thingies.nl</a></div></div></fieldset></body></html>");
    server.send(200, "text/html", str);
  }
#endif
