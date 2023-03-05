// HTML statics

void addHead(String& str)
{
  str += F("<!DOCTYPE html><html lang='en' class=''>");
  str += F("<meta charset='utf-8'><meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>");
  str += F("<head><title>Smart-Meter</title>");
  str += F("<style>");
  str += F("div, fieldset, input, select {padding: 5px; font-size: 1em;}");//5px 1em
  str += F("fieldset {background: #ECEAE4;}");
  str += F("p {margin: 0.5em 0;}");
  str += F("input {width: 100%; box-sizing: border-box; -webkit-box-sizing: border-box; -moz-box-sizing: border-box; background: #ffffff; color: #000000;}"); // Ausgabeboxen
  str += F("input[type=range] {width: 90%;}");
  str += F("select {width: 100%; background: #ffffff; color: #000000;}"); //100%
  str += F("textarea {resize: vertical; width: 100%; height: 318px; padding: 5px; overflow: auto; background: #ffffff; color: #000000;}"); //100%
  str += F("body {text-align: center; font-family: verdana, sans-serif; background: #ffffff;}");
  str += F("td {padding: 0px;}");//0px
  str += F("button {border: 0; border-radius: 0.3rem; background: #97C1A9; color: #ffffff; line-height: 2.4rem; font-size: 1.2rem; width: 100%; -webkit-transition-duration: 0.4s; transition-duration: 0.4s; cursor: pointer;}");
  str += F("button:hover {background: #0e70a4;}");
  str += F(".bred {background: #d43535;}");
  str += F(".bred:hover {background: #931f1f;}");
  str += F(".bhome {background: #55CBCD;}");
  str += F(".bhome:hover {background: #A2E1DB;}");
  str += F("a {color: #1fa3ec;text-decoration: none;}");
  str += F(".p {float: left; text-align: left;}");  
  str += F(".q {float: right;text-align: right;}");
  str += F(".r {border-radius: 0.3em; padding: 2px; margin: 6px 2px;}");
  str += F(".column {float: left;width: 48%;}"); //48%
  str += F(".column1 {float: left;width: 98%;}");
  str += F(".column3 {float: left; width: 31.5%;}"); //31%
  str += F(".row:after {content: \"\";display: table; clear: both;}");
 // str += F(".row1: {float: left; width: 90%;}{after content: \"\";display: table; clear: both;}"); 
  str += F("input.c4 {text-align:left}");
  str += F("input.c6 {text-align:right}");
  str += F("input.c7 {text-align:right; color:#97C1A9}");
  str += F("</style></head>");
}

void addRefreshHead(String& str)
{
  str += F("<!DOCTYPE html><html lang='en' class=''>");
  str += F("<META http-equiv=\"refresh\" content=\"20;URL=/\">");
  str += F("<meta charset='utf-8'><meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>");
  str += F("<head><title>Smart-Meter</title>");
  str += F("<style>");
  str += F("div, fieldset, input, select {padding: 5px; font-size: 1em;}");
  str += F("fieldset {background: #ECEAE4;}");
  str += F("p {margin: 0.5em 0;}");
  str += F("body {text-align: center; font-family: verdana, sans-serif; background: #ffffff;}");
  str += F(".p {float: left; text-align: left;}");  
  str += F("</style></head>");
}

void addGraphHead(String& str){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  // here begin chunked transfer
  server.send(200, "text/html", "");
  
  str += F("<!DOCTYPE html><html lang='en' class=''>");
  str += F("<meta charset='utf-8'><meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>");
  str += F("<head><title>Smart-Meter</title>");
  str += F("<style>");
  str += F("div, fieldset, input, select {padding: 5px; font-size: 1em;}");
  str += F("fieldset {background: #ECEAE4;}");
  str += F("p {margin: 0.5em 0;}");
  str += F("body {text-align: center; font-family: verdana, sans-serif; background: #ffffff;}");
  str += F("button {border: 0; border-radius: 0.3rem; background: #97C1A9; color: #ffffff; line-height: 2.4rem; font-size: 1.2rem; width: 100%; -webkit-transition-duration: 0.4s; transition-duration: 0.4s; cursor: pointer;}");
  str += F("button:hover {background: #0e70a4;}");
  str += F(".bhome {background: #55CBCD;}");
  str += F(".bhome:hover {background: #A2E1DB;}");
  str += F(".p {float: left; text-align: left;}</style>");
}

void handleSetupSave() {
  debugln("handleSetupSave");
  debug("Server args: ");
  debugln(server.args());
  debugln(server.arg("plain"));

  if (server.args() == 0) {
    debugln("lege submit, dus redirect naar handleRoot");
    handleRoot();
    debugln("En terug naar mainloop");
    return;
  }
  
  String str = ""; 
  
  if (server.method() == HTTP_POST) {
    if (strncmp(setupToken, server.arg("setuptoken").c_str(), 16) != 0) {
      debug("Error: non matching tokens: ");
      debug(setupToken);
      debug(" -> returned: ");
      debugln(server.arg("setuptoken"));
      return;
    }
    
    strncpy(config_data.adminPassword,     server.arg("adminPassword").c_str(),     server.arg("adminPassword").length() );

    if (server.arg("domo") == "on") config_data.domo[0] = 'j'; else config_data.domo[0] = 'n';
    strncpy(config_data.ssid,     server.arg("ssid").c_str(),     server.arg("ssid").length() );
    strncpy(config_data.password, server.arg("password").c_str(), server.arg("password").length() );
    strncpy(config_data.domoticzIP, server.arg("domoticzIP").c_str(), server.arg("domoticzIP").length() );
    strncpy(config_data.domoticzPort, server.arg("domoticzPort").c_str(), server.arg("domoticzPort").length() );
    strncpy(config_data.domoticzEnergyIdx, server.arg("domoticzEnergyIdx").c_str(), server.arg("domoticzEnergyIdx").length() );
    strncpy(config_data.domoticzGasIdx, server.arg("domoticzGasIdx").c_str(), server.arg("domoticzGasIdx").length() );

    if (server.arg("mqtt") == "on") config_data.mqtt[0] = 'j'; else config_data.mqtt[0] = 'n';
    strncpy(config_data.mqttIP, server.arg("mqttIP").c_str(), server.arg("mqttIP").length() );
    strncpy(config_data.mqttPort, server.arg("mqttPort").c_str(), server.arg("mqttPort").length() );
    strncpy(config_data.mqttUser, server.arg("mqttUser").c_str(), server.arg("mqttUser").length() );
    strncpy(config_data.mqttPass, server.arg("mqttPass").c_str(), server.arg("mqttPass").length() );
    strncpy(config_data.mqttTopic, server.arg("mqttTopic").c_str(), server.arg("mqttTopic").length() );

    if (server.arg("cfos") == "on") config_data.cfos[0] = 'j'; else config_data.cfos[0] = 'n';
    if (server.arg("cfosVA") == "on") (config_data.cfosVA[0] = 'j'); else (config_data.cfosVA[0] = 'n');
    strncpy(config_data.cfosIP, server.arg("cfosIP").c_str(), server.arg("cfosIP").length() );
    strncpy(config_data.cfosPort, server.arg("cfosPort").c_str(), server.arg("cfosPort").length() );
    strncpy(config_data.cfosUsr, server.arg("cfosUsr").c_str(), server.arg("cfosUsr").length() );
    strncpy(config_data.cfosPwd, server.arg("cfosPwd").c_str(), server.arg("cfosPwd").length() );
    strncpy(config_data.cfosID, server.arg("cfosID").c_str(), server.arg("cfosID").length() );
    strncpy(config_data.cfosModel, server.arg("cfosModel").c_str(), server.arg("cfosModel").length() );
//    strncpy(config_data.cfosIsVA, server.arg("cfosIsVA").c_str(), server.arg("cfosIsVA").length() );

    strncpy(config_data.interval, server.arg("interval").c_str(), server.arg("interval").length() );

    if (server.arg("watt") == "on") config_data.watt[0] = 'j'; else config_data.watt[0] = 'n';
    if (server.arg("telnet") == "on") config_data.telnet[0] = 'j'; else config_data.telnet[0] = 'n';
    if (server.arg("debug") == "on") config_data.debug[0] = 'j'; else config_data.debug[0] = 'n';

    config_data.dataSet[0] = 'j';
    config_data.dataSet[1] =
    config_data.adminPassword[server.arg("adminPassword").length()] =
    config_data.ssid[server.arg("ssid").length()] = 
    config_data.password[server.arg("password").length()] = 
    config_data.domoticzIP[server.arg("domoticzIP").length()] = 
    config_data.domoticzPort[server.arg("domoticzPort").length()] = 
    config_data.domoticzGasIdx[server.arg("domoticzGasIdx").length()] = 
    config_data.domoticzEnergyIdx[server.arg("domoticzEnergyIdx").length()] = 
    config_data.interval[server.arg("interval").length()] =
    config_data.mqttIP[server.arg("mqttIP").length()] = 
    config_data.mqttPort[server.arg("mqttPort").length()] = 
    config_data.mqttUser[server.arg("mqttUser").length()] = 
    config_data.mqttPass[server.arg("mqttPass").length()] = 
    config_data.mqttTopic[server.arg("mqttTopic").length()] = 
    config_data.cfosUsr[server.arg("cfosUsr").length()] = 
    config_data.cfosPwd[server.arg("cfosPwd").length()] = 
    config_data.cfosIP[server.arg("cfosIP").length()] =
    config_data.cfosPort[server.arg("cfosPort").length()] =
    config_data.cfosID[server.arg("cfosID").length()] = 
    config_data.cfosModel[server.arg("cfosModel").length()] = 
//    config_data.cfosIsVA[server.arg("cfosIsVA").length()] = 
    config_data.cfosVA[1] =      
    config_data.domo[1] = //server.arg("domo").length()] = 
    config_data.mqtt[1] = //server.arg("mqtt").length()] = 
    config_data.cfos[1] =
    config_data.watt[1] = //server.arg("watt").length()] = 
    config_data.telnet[1] = //server.arg("telnet").length()] = 
    config_data.debug[1] = 
    '\0';
    
    EEPROM.put(0, config_data);
    EEPROM.commit();

    addRefreshHead(str);
    addIntro(str);
    setupSaved(str);
    server.send(200, "text/html", str);    
    delay(500);
    ESP.reset();
  } 
}
