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
 * @file graph.ino
 * @author Ronald Leenes
 * @date 28.12.2022
 * @version 1.0u 
 *
 * @brief This file contains the graphing functions
 *
 * @see http://esp8266thingies.nl
 */


#if GRAPH == 1

  void handleGraphMenu(){
    String str="";
 
    addHead(str,0);
//  addHead(str);
    addIntro(str);
  
    #ifdef NEDERLANDS 
      str += F("<html>\n<head>\n");
      str += F("<title>Slimme meter</title>");
      str += F("</head><body>");
 
      str += F("<fieldset><legend><b> Grafieken </b></legend>");
      str += F("<fieldset><legend><b> periode </b></legend>"); 
      str += F("<form action='/SelectGraph' method='POST'>");
      str += F("<div>");
      str += F("<input type='radio' id='day' name='period' value='day' checked><label for='day'>vandaag</label>");
      str += F("<input type='radio' id='week' name='period' value='week'><label for='week'>deze week</label>");
      str += F("<input type='radio' id='month' name='period' value='month'><label for='month'>deze maand</label>");
      str += F("<input type='radio' id='year' name='period' value='year'><label for='year'>dit jaar</label></br>");
      str += F("</div>");
  
 //  str += F("<fieldset>");
      str += F("<button name='graphtype' value='1' type='submit'>Energieverbruik T1/T2</button>");
      str += F("<p><button name='graphtype' value='2' type='submit'>Energie retour T1/T2</button>");
      str += F("<p><button name='graphtype' value='3' type='submit'>Energie totaal/retour</button>");
      str += F("<p><button name='graphtype' value='4' type='submit'>Gasverbruik</button>");
      str += F("</form>");
      str += F("</fieldset>");
 
      str += F("<fieldset><legend><b> Jaaroverzicht</b></legend>");
      str += F("<form action='/GasCalendar' method='POST'>");
      str += F("<p><button type='submit'>Gasverbruik dit jaar</button></form>");
      str += F("</fieldset></form>");
  
      str += F("</fieldset>"); // grafieken
 //  str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
      str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button>");
    #endif

    #ifdef GERMAN
      str += F("<html>\n<head>\n");
      str += F("<title>Smart-Meter</title>");
      str += F("</head><body>");
 
      str += F("<fieldset><legend><b> Grafiken </b></legend>");
      str += F("<fieldset><legend><b> Periode </b></legend>"); 
      str += F("<form action='/SelectGraph' method='POST'>");
      str += F("<div>");

      str += F("<div ><table width='95%' ><tr>");
      str += F("<td width='25%'><input type='radio' id='day' name='period' value='day' checked /><center><label for='day'>heute</label></center></td>");
      str += F("<td width='25%'><input type='radio' id='week' name='period' value='week' /><center><label for='week'>diese Woche</label></center></td>");
      str += F("<td width='25%'><input type='radio' id='month' name='period' value='month' /><center><label for='month'>diesen Monat</label></center></td>");
      str += F("<td width='25%'><input type='radio' id='year' name='period' value='year' /><center><label for='year'>dieses Jahr</label></center></td></tr></table></br>");
      str += F("</div>");
       
/*
    str += F("<div ><table width='95%' ><tr>");
    str += F("<td width='25%'><input type='radio' id='day' name='period' value='day' checked /><label for='day'>heute</label>"); 
    str += F("<td width='25%'><input type='radio' id='week' name='period' value='week' /><label for='week'>diese Woche</label>");
    str += F("<td width='25%'><input type='radio' id='month' name='period' value='month' /><label for='month'>diesen Monat</label>"); 
    str += F("<td width='25%'><input type='radio' id='year' name='period' value='year' /><label for='year'>dieses Jahr</label></br>");
    str += F("</div>");
    
    str += F("<p><input type='radio' id='day' name='period' value='day' checked><label for='day'>heute</label>"); 
    str += F("<p><input type='radio' id='week' name='period' value='week'><label for='week'>diese Woche</label>");
    str += F("<p><div><input type='radio' id='month' name='period' value='month'><label for='month'>diesen Monat</label></div></p>"); 
    str += F("<p><div><input type='radio' id='year' name='period' value='year'><label for='year'>dieses Jahr</label></div></br></p>");
    str += F("</div>");
    
    str += F("<input type='radio' id='day' name='Periode' value='Tag' checked><label for='day'>heute</label>");
    str += F("<input type='radio' id='week' name='Periode' value='Woche'><label for='week'>diese Woche</label>");
    str += F("<input type='radio' id='month' name='Periode' value='Monat'><label for='month'>diesen Monat</label>");
    str += F("<input type='radio' id='year' name='Periode' value='Jahr'><label for='year'>dieses Jahr</label></br>");
  //  str += F("</div>");
*/  
      str += F("<fieldset>");
      str += F("<button name='graphtype' value='1' type='submit'>Energieverbrauch T1/T2</button>");
      str += F("<p><button name='graphtype' value='2' type='submit'>Energie zurück T1/T2</button>");
      str += F("<p><button name='graphtype' value='3' type='submit'>Energie insgesamt/zurück</button>");
      str += F("<p><button name='graphtype' value='4' type='submit'>Gasverbrauch</button>");
      str += F("</form>");
      str += F("</fieldset>");
 
      str += F("<fieldset><legend><b> Jahresübersicht</b></legend>");
      str += F("<form action='/GasCalendar' method='POST'>");
      str += F("<p><button type='submit'>Gasverbrauch dieses Jahr</button></form>");
      str += F("</fieldset></form>");
  
      str += F("</fieldset>"); // grafieken
 //  str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
      str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button>");
    #endif

    #ifdef FRENCH 
      str += F("<html>\n<head>\n");
      str += F("<title>Compteur intelligent</title>");
      str += F("</head><body>");
 
      str += F("<fieldset><legend><b> Graphiques </b></legend>");
      str += F("<fieldset><legend><b> Période </b></legend>"); 
      str += F("<form action='/SelectGraph' method='POST'>");
      str += F("<div>");
      str += F("<input type='radio' id='day' name='period' value='day' checked><label for='day'>aujourd'hui</label>");
      str += F("<input type='radio' id='week' name='period' value='week'><label for='week'>cette semaine</label>");
      str += F("<input type='radio' id='month' name='period' value='month'><label for='month'>ce mois-ci</label>");
      str += F("<input type='radio' id='year' name='period' value='year'><label for='year'>cette année</label></br>");
      str += F("</div>");
  
 //  str += F("<fieldset>");
      str += F("<button name='graphtype' value='1' type='submit'>Consommation d'énergie T1/T2</button>");
      str += F("<p><button name='graphtype' value='2' type='submit'>Retour d'énergie T1/T2</button>");
      str += F("<p><button name='graphtype' value='3' type='submit'>Énergie totale/retour</button>");
      str += F("<p><button name='graphtype' value='4' type='submit'>Consommation de gaz</button>");
      str += F("</form>");
      str += F("</fieldset>");
 
      str += F("<fieldset><legend><b> Aperçu annuel</b></legend>");
      str += F("<form action='/GasCalendar' method='POST'>");
      str += F("<p><button type='submit'>Consommation de gaz cette année</button></form>");
      str += F("</fieldset></form>");
  
      str += F("</fieldset>"); // grafieken
 //  str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
      str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button>");
    #endif

    #ifdef SWEDISH 
      str += F("<html>\n<head>\n");
      str += F("<title>Smart mätare</title>");
      str += F("</head><body>");
 
      str += F("<fieldset><legend><b> Grafer </b></legend>");
      str += F("<fieldset><legend><b> Period </b></legend>"); 
      str += F("<form action='/SelectGraph' method='POST'>");
      str += F("<div>");
      str += F("<input type='radio' id='day' name='period' value='day' checked><label for='day'>i dag</label>");
      str += F("<input type='radio' id='week' name='period' value='week'><label for='week'>denna vecka</label>");
      str += F("<input type='radio' id='month' name='period' value='month'><label for='month'>den här månaden</label>");
      str += F("<input type='radio' id='year' name='period' value='year'><label for='year'>det här året</label></br>");
      str += F("</div>");
  
 //  str += F("<fieldset>");
      str += F("<button name='graphtype' value='1' type='submit'>Energiförbrukning T1/T2</button>");
      str += F("<p><button name='graphtype' value='2' type='submit'>Energi tillbaka T1/T2</button>");
      str += F("<p><button name='graphtype' value='3' type='submit'>Energisumma/avkastning</button>");
      str += F("<p><button name='graphtype' value='4' type='submit'>Gasförbrukning</button>");
      str += F("</form>");
      str += F("</fieldset>");
 
      str += F("<fieldset><legend><b> Årsöversikt</b></legend>");
      str += F("<form action='/GasCalendar' method='POST'>");
      str += F("<p><button type='submit'>Gasförbrukning i år</button></form>");
      str += F("</fieldset></form>");
  
      str += F("</fieldset>"); // grafieken
 //  str += F("<form action='/Dir' method='GET'><button type='submit'>Directory</button></form>");
      str += F("<form action='/' method='POST'><button class='button bhome'>Meny</button>");
    #endif
  
    addFootBare(str); 
    server.send(200, "text/html", str);
  }

  void DumpDataFiles(){
    listDir("/");
    debugln();
  }

  void selectGraph() {
    char switcher[3];
    strncpy(switcher, server.arg("graphtype").c_str(), server.arg("graphtype").length() );
    switcher[1] ='\0';

    char period[6];
    strncpy(period, server.arg("period").c_str(), server.arg("period").length() );
    period[server.arg("period").length()] ='\0';
      
    debug("Selected graph: ");
      debugln(switcher);
    debug("Selected period: ");
      debugln(period);
  
    #ifdef NEDERLANDS
    if (server.method() == HTTP_POST){
  //  int switcher = atoi(server.arg("datatype").c_str());
      debugln(switcher);
      switch (atoi(switcher)){
        case 1: //delivered T1/T2
//        theGraph("E1.log", "E2.log", "Elektricitet gebruik T1", "Elektriciteit gebruik T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          theGraph("E1", "E2", "Elektricitet använda T1", "Elektricitet använda T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          break;
        case 2: // returned T1/T2
//        theGraph("R1.log", "R2.log","Elektriciteit retour T1", "Elektriciteit retour T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("R1", "R2","Elektricitet retour T1", "Elektricitet retour T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          break;
        case 3: //Total E
//        theGraph("TE.log", "TR.log", "Elektriciteit totaal gebruik", "Elektriciteit totaal retour", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("TE", "TR", "Elektricitet total använda", "Elektricitet total retour", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);        
          break; 
        case 4: //Gas
//        theGraph("G.log", "","Gas","", "[{label:'uur', type:'number'},{label:'kubieke meter', type:'number'}],", period);
          theGraph("G", "","Gas","", "[{label:'uur', type:'number'},{label:'kubisk meter', type:'number'}],", period);
          break;  
        default:
          break; 
      }
    }
  #endif
  
  #ifdef GERMAN
    if (server.method() == HTTP_POST){
  //  int switcher = atoi(server.arg("datatype").c_str());
      debugln(switcher);
      switch (atoi(switcher)){
        case 1: //delivered T1/T2
//        theGraph("E1.log", "E2.log", "Elektrizität Verbrauch T1", "Elektrizität Verbrauch T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          theGraph("E1", "E2", "Elektrizität Verbrauch T1", "Elektrizität Verbrauch T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          break;
        case 2: // returned T1/T2
//        theGraph("R1.log", "R2.log","Elektrizität zurück T1", "Elektrizität zurück T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("R1", "R2","Elektrizität zurück T1", "Elektrizität zurück T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          break;
        case 3: //Total E
//        theGraph("TE.log", "TR.log", "Elektrizität insgesamt Verbrauch", "Elektrizität insgesamt zurück", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("TE", "TR", "Elektrizität insgesamt Verbrauch", "Elektrizität insgesamt zurück", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);        
          break; 
        case 4: //Gas
//        theGraph("G.log", "","Gas","", "[{label:'uur', type:'number'},{label:'Kubikmeter', type:'number'}],", period);
          theGraph("G", "","Gas","", "[{label:'uur', type:'number'},{label:'Kubikmeter', type:'number'}],", period);        
          break;  
        default:
          break; 
      }
    }     
  #endif

  #ifdef FRENCH
    if (server.method() == HTTP_POST){
  //  int switcher = atoi(server.arg("datatype").c_str());
      debugln(switcher);
      switch (atoi(switcher)){
        case 1: //delivered T1/T2
//        theGraph("E1.log", "E2.log", "Elektriciteit gebruik T1", "Elektriciteit gebruik T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          theGraph("E1", "E2", "Consommation d'électricité T1", "Consommation d'électricité T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          break;
        case 2: // returned T1/T2
//        theGraph("R1.log", "R2.log","Elektriciteit retour T1", "Elektriciteit retour T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("R1", "R2","Retour d'électricité T1", "Retour d'électricité T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          break;
        case 3: //Total E
//        theGraph("TE.log", "TR.log", "Elektriciteit totaal gebruik", "Elektriciteit totaal retour", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("TE", "TR", "Consommation totale d'électricité", "Rendement total de l'électricité", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);        
          break; 
        case 4: //Gas
//        theGraph("G.log", "","Gas","", "[{label:'uur', type:'number'},{label:'kubieke meter', type:'number'}],", period);
          theGraph("G", "","Gaz","", "[{label:'uur', type:'number'},{label:'mètres cubes', type:'number'}],", period);
          break;  
        default:
          break; 
      }
    }
  #endif

  #ifdef SWEDISH
    if (server.method() == HTTP_POST){
  //  int switcher = atoi(server.arg("datatype").c_str());
      debugln(switcher);
      switch (atoi(switcher)){
        case 1: //delivered T1/T2
//        theGraph("E1.log", "E2.log", "Elektriciteit gebruik T1", "Elektriciteit gebruik T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          theGraph("E1", "E2", "Elektricitet använda T1", "Elektricitet använda T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period );
          break;
        case 2: // returned T1/T2
//        theGraph("R1.log", "R2.log","Elektriciteit retour T1", "Elektriciteit retour T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("R1", "R2","Elektricitet retour T1", "Elektricitet retour T2", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          break;
        case 3: //Total E
//        theGraph("TE.log", "TR.log", "Elektriciteit totaal gebruik", "Elektriciteit totaal retour", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);
          theGraph("TE", "TR", "Elektricitet total använda", "Elektricitet total retour", "[{label:'uur', type:'number'},{label:'kWh', type:'number'}],", period);        
          break; 
        case 4: //Gas
//        theGraph("G.log", "","Gas","", "[{label:'uur', type:'number'},{label:'kubieke meter', type:'number'}],", period);
          theGraph("G", "","Gas","", "[{label:'uur', type:'number'},{label:'kubisk meter', type:'number'}],", period);
          break;  
        default:
          break; 
      }
    }
  #endif
}

void theGraph(const char* type1, const char* type2, String title1, String title2, String label, String period) {
  monitoring = false;
  String str ="";
  char buffer[64];
  char path1[20];
  char path2[20];
  char totaal1[12];
  char totaal2[12];
  String pageTitle ="";

  #ifdef NEDERLANDS
    if (period =="day") {
      strcpy(path1, "/Hour");
      strcat(path1, type1);
      strcpy(path2, "/Hour");
      strcat(path2, type2);
      pageTitle = " Vandaag ";
    } else if (period =="week") {
      strcpy(path1, "/Week");
      strcat(path1, type1);
      strcpy(path2, "/Week");
      strcat(path2, type2);
      pageTitle = " Deze week ";
    } else if (period =="month") {
      strcpy(path1, "/Month");
      strcat(path1, type1);
      strcpy(path2, "/Month");
      strcat(path2, type2);
      pageTitle = " Deze maand ";
    } else if (period =="year") {
      strcpy(path1, "/Year");
      strcat(path1, type1);
      strcpy(path2, "/Year");
      strcat(path2, type2);
      pageTitle = " Dit jaar ";
    }
  #endif

  #ifdef GERMAN
    if (period =="day") {
      strcpy(path1, "/Hour");
      strcat(path1, type1);
      strcpy(path2, "/Hour");
      strcat(path2, type2);
      pageTitle = " Heute ";
    } else if (period =="week") {
      strcpy(path1, "/Week");
      strcat(path1, type1);
      strcpy(path2, "/Week");
      strcat(path2, type2);
      pageTitle = " Diese Woche ";
    } else if (period =="month") {
      strcpy(path1, "/Month");
      strcat(path1, type1);
      strcpy(path2, "/Month");
      strcat(path2, type2);
      pageTitle = " Diesen Monat ";
    } else if (period =="year") {
      strcpy(path1, "/Year");
      strcat(path1, type1);
      strcpy(path2, "/Year");
      strcat(path2, type2);
      pageTitle = " Dieses Jahr ";
    }  
  #endif

  #ifdef FRENCH
    if (period =="day") {
      strcpy(path1, "/Hour");
      strcat(path1, type1);
      strcpy(path2, "/Hour");
      strcat(path2, type2);
      pageTitle = " Aujourd'hui ";
    } else if (period =="week") {
      strcpy(path1, "/Week");
      strcat(path1, type1);
      strcpy(path2, "/Week");
      strcat(path2, type2);
      pageTitle = " Cette semaine ";
    } else if (period =="month") {
      strcpy(path1, "/Month");
      strcat(path1, type1);
      strcpy(path2, "/Month");
      strcat(path2, type2);
      pageTitle = " Ce mois-ci ";
    } else if (period =="year") {
      strcpy(path1, "/Year");
      strcat(path1, type1);
      strcpy(path2, "/Year");
      strcat(path2, type2);
      pageTitle = " Cette année ";
    }
  #endif

  #ifdef SWEDISH
    if (period =="day") {
      strcpy(path1, "/Hour");
      strcat(path1, type1);
      strcpy(path2, "/Hour");
      strcat(path2, type2);
      pageTitle = " I dag ";
    } else if (period =="week") {
      strcpy(path1, "/Week");
      strcat(path1, type1);
      strcpy(path2, "/Week");
      strcat(path2, type2);
      pageTitle = " Denna vecka ";
    } else if (period =="month") {
      strcpy(path1, "/Month");
      strcat(path1, type1);
      strcpy(path2, "/Month");
      strcat(path2, type2);
      pageTitle = " Den här månaden ";
    } else if (period =="year") {
      strcpy(path1, "/Year");
      strcat(path1, type1);
      strcpy(path2, "/Year");
      strcat(path2, type2);
      pageTitle = " Det här året ";
    }
  #endif
  
  strcat(path1, ".log");
  strcat(path2, ".log");

//debug("file1 to read: ");
//debugln(path1);
//debug("file2 to read: ");
//debugln(path2);

//https://stackoverflow.com/questions/44159990/how-to-add-a-total-to-a-chart-in-google-charts

//File file1 = LittleFS.open(path1, "r");
//File file2 = LittleFS.open(path2, "r");
  File file1 = FST.open(path1, "r");
  File file2 = FST.open(path2, "r");


  addGraphHead(str);
  str += F("<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>");
  str += F("<script type=\"text/javascript\">google.charts.load('current', {'packages':['corechart']});");
  str += F("google.charts.setOnLoadCallback(drawChart1);");
  if (type2[0] != '\0'){
    str += F("google.charts.setOnLoadCallback(drawChart2);");
  }

  server.sendContent(str);
//debugln(str);

  str = F("function drawChart1() { var data = new google.visualization.arrayToDataTable([");
  str += label; //F("[\"uur\", \"m^3\"], ");
  server.sendContent(str);
//debugln(str);

  if (!file1) {
    debugln("Failed to open file for reading");
    server.sendContent("['0', 0],");
//    debug("['0', 0], ");
  } else {
    while (file1.available()) {
      int l = file1.readBytesUntil('\n', buffer, sizeof(buffer));
      buffer[l] = 0;
      server.sendContent(buffer);
 //     debugln(buffer);
    }
    file1.close();
  }
  delay(200);
  str = F("]);");
  str += F("var options = {title: '");
  str += title1; 
  str += F(" '};"); //'Gasverbruik per uur in m^3'};");
  if (type1[0] == 'T') 
    str += F("var chart = new google.visualization.LineChart(document.getElementById('Chart1'));");
  else str += F("var chart = new google.visualization.ColumnChart(document.getElementById('Chart1'));");
  str += F("chart.draw(data, options); }");
  server.sendContent( str);
// debugln(str);

  delay(200);
 
  str ="";

  if (type2[0] != '\0'){
    str = F("function drawChart2() { var data = new google.visualization.arrayToDataTable([");
    str += label; //F("[\"uur\", \"m^3\"], ");
    server.sendContent( str);
//  debugln(str);

    if (!file2) {
      debugln("Failed to open file for reading");
      server.sendContent("['0', 0],");
      debug("['0', 0], ");
    } else {
      while (file2.available()) {
        int l = file2.readBytesUntil('\n', buffer, sizeof(buffer));
        buffer[l] = 0;
        server.sendContent(buffer);
        debug(str);
      }
      file2.close();
      delay(200);
    }
    str = F("]);");
    str += F("var options = {title: '");
    str += title2; //'Gasverbruik per uur in m^3'
    str += F(" '};"); 
    if (type2[0] == 'T')     
      str += F("var chart = new google.visualization.LineChart(document.getElementById('Chart2'));");
    else  str += F("var chart = new google.visualization.ColumnChart(document.getElementById('Chart2'));");
    str += F("chart.draw(data, options); }");
    server.sendContent ( str);
//  debugln(str);
  }  //only if we have a second file to display

  #ifdef NEDERLANDS
    str = F("</script>");
    str += F("</head><body>");
    str +=("<div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-Gateway</h2></div><br>");

    str += F("<fieldset>");
    str += F("<fieldset><legend><b>");
    str += pageTitle;
    str += F("</b></legend>");
    str += F("<div id=\"Chart1\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
    str += F("Totaal deze periode: ");
    str += "nog te implementeren</br><hr>";
    if (type2[0] != '\0'){
      str += F("<div id=\"Chart2\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
      str += F("Totaal deze periode: ");
      str += "nog te implementeren</br></fieldset>";
    }  
  #endif
  
  #ifdef GERMAN
    str = F("</script>");
    str += F("</head><body>");
    str +=("<div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-Gateway</h2></div><br>");

    str += F("<fieldset>");
    str += F("<fieldset><legend><b>");
    str += pageTitle;
    str += F("</b></legend>");
    str += F("<div id=\"Chart1\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
    str += F("Insgesamt für diese Periode: ");
    str += "noch zu implementieren</br><hr>";
    if (type2[0] != '\0'){
      str += F("<div id=\"Chart2\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
      str += F("Insgesamt für diese Periode: ");
      str += "noch zu implementieren</br></fieldset>";
    }  
  #endif

  #ifdef FRENCH
    str = F("</script>");
    str += F("</head><body>");
    str +=("<div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-Gateway</h2></div><br>");

    str += F("<fieldset>");
    str += F("<fieldset><legend><b>");
    str += pageTitle;
    str += F("</b></legend>");
    str += F("<div id=\"Chart1\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
    str += F("Total de cette période: ");
    str += "encore à mettre en œuvre</br><hr>";
    if (type2[0] != '\0'){
      str += F("<div id=\"Chart2\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
      str += F("Total de cette période: ");
      str += "encore à mettre en œuvre</br></fieldset>";
    }  
  #endif

  #ifdef SWEDISH
    str = F("</script>");
    str += F("</head><body>");
    str += ("<div style='text-align:left;display:inline-block;color:#000000;width:600px;'><div style='text-align:center;color:#000000;'><h2>P1 WiFi-Gateway</h2></div><br>");

    str += F("<fieldset>");
    str += F("<fieldset><legend><b>");
    str += pageTitle;
    str += F("</b></legend>");
    str += F("<div id=\"Chart1\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
    str += F("Totalt denna period: ");
    str += "ännu inte implementerats</br><hr>";
    if (type2[0] != '\0'){
      str += F("<div id=\"Chart2\" style=\"width:100%; max-width:1200px; height:500px\"></div><br>");
      str += F("Totalt denna period: ");
      str += "ännu inte implementerats</br></fieldset>";
    }  
  #endif
     
//}
  str += F("<form action='/' method='POST'><button class='button bhome'>Menu</button></form>");
  addFootBare(str);   
  server.sendContent ( str);
//  debugln(str);
  server.sendContent(F(""));
  monitoring = true;
}

void calendarGas(){
  String str ="";
  char buffer[64];
  File file = LittleFS.open("/YearGc.log", "r");
  monitoring = false;

  addGraphHead(str);

  str += F("<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script><script type='text/javascript'>google.charts.load('current', {packages:['calendar']});google.charts.setOnLoadCallback(drawChart);");
  str += F("function drawChart() {var dataTable=new google.visualization.DataTable(); dataTable.addColumn({ type: 'date', id: 'Datum' }); dataTable.addColumn({type: 'number', id: 'Gebruik'});");
  str += F("dataTable.addRows([");

  server.sendContent(str);
//debugln(str);

  if (!file) {
    debugln("Failed to open file for reading");
    server.sendContent("[new Date(2023,0,1), 0],");
  } else {
    while (file.available()) {
      int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
      buffer[l] = 0;
      server.sendContent("[new Date");
      server.sendContent(buffer);
    }
    file.close();
    delay(200);
  }
  str = "";
 
  #ifdef NEDERLANDS
    str += F("]); var chart = new google.visualization.Calendar(document.getElementById('calendar_gas')); var options = { title: 'Gas gebruiksintensiteit', height: 350};");
  #endif
 
  #ifdef GERMAN
    str += F("]); var chart = new google.visualization.Calendar(document.getElementById('calendar_gas')); var options = { title: 'Gas Verbrauchsintensität', height: 350};");
  #endif

  #ifdef FRENCH
    str += F("]); var chart = new google.visualization.Calendar(document.getElementById('calendar_gas')); var options = { title: 'Intensité de consommation de gaz', height: 350};");
  #endif

  #ifdef SWEDISH
    str += F("]); var chart = new google.visualization.Calendar(document.getElementById('calendar_gas')); var options = { title: 'Gasanvändningsintensitet', height: 350};");
  #endif
 
  str += F("chart.draw(dataTable, options);}</script></head><body><div id='calendar_gas' style='width: 800px; height: 350px;'></div>");

  str += F("<form action='/' method='POST'><button class='button bhome'style='width: 300px'>Menu</button></form>");
  addFootBare(str);   
  server.sendContent (str);
//debugln(str);
  server.sendContent(F(""));
 
  monitoring = true;
}
#endif // GRAPH
