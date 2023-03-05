/*
 * Copyright (c) 2022 Ronald Leenes
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @file debug.ino
 * @author Ronald Leenes
 * @date 28.12.2022
 *
 * @brief This file contains debug functions 
 *
 * @see http://esp8266thingies.nl
 */
 
void readVoltage() { // read internal VCC
  volts = ESP.getVcc();
  debugf("The internal VCC reads %1.2f volts\n", volts / 1000);
}

void PrintConfigData(){
  debug("dataSet : ");
    debugln(config_data.dataSet);
  debug("Admin password : ");
    debugln(config_data.adminPassword);
  debug("SSID : ");
    debugln(config_data.ssid);
  debug("password : ");
    debugln(config_data.password);
  debug("DomoticxIP : ");
    debugln(config_data.domoticzIP);
  debug("DomoticzPort : ");
    debugln(config_data.domoticzPort);
  debug("domotixzGasIdx : ");
   debugln(config_data.domoticzGasIdx);
  debug("domotixzEnergyIdx : ");
   debugln(config_data.domoticzEnergyIdx);

  debug("mqttIP : ");
    debugln(config_data.mqttIP);
  debug("mqttPort : ");
    debugln(config_data.mqttPort);
  debug("mqttUser : ");
    debugln(config_data.mqttUser);
  debug("mqttPass : ");
    debugln(config_data.mqttPass);
  debug("mqttTopic : ");
    debugln(config_data.mqttTopic);
    
  debug("cfosIP : ");
    debugln(config_data.cfosIP);
  debug("cfosPort : ");
    debugln(config_data.cfosPort);
  debug("cfosUsr : ");
    debugln(config_data.cfosUsr);
  debug("cfosPwd : ");
    debugln(config_data.cfosPwd);
  debug("cfosID : ");
    debugln(config_data.cfosID);
  debug("cfosModel : ");
    debugln(config_data.cfosModel);
  debug("cfosVA : ");
    debugln(config_data.cfosVA);
  debug("cfosIsVA : ");
    debugln(cfosIsVA);
  debug("interval : ");
    debugln(config_data.interval);
  debug("json : ");
    debugln(config_data.domo);
  debug("mqtt : ");
    debugln(config_data.mqtt);
  debug("cfos : ");
    debugln(config_data.cfos);  
  debug("watt : ");
    debugln(config_data.watt);
  debug("telnet : ");
    debugln(config_data.telnet);
}
