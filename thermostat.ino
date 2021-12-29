
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>
#include "Adafruit_MCP9808.h"
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <WiFiManager.h> 


//const char* ssid     = "SFR_8930";
//const char* password = "iormoywyksirpoch4kno";

int Normal = 22 ,Eco =19;
float Deltan=0.5,Deltae=0.5;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE,2,0);
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
ESP8266WebServer server ( 80 );
WiFiClient client;
HTTPClient http;

float temp;
char chaine[5];
String etat;
String ip = "192.168.1.86";
float data[24];
float mini=100,maxi=0,moyi=0;
int j,h,m,s,hbis,flag =0;
long int debut,mnt;
float consigne,delta;
char planning[7][24];
String prog = "111111111000000000111111111111111000000000111111111111111000011111111111111111111000000000111111111111111000000000111111111111111000000000111111111111111111111111111111";
String jour[7]= {"Lundi","Mardi","Mercredi","Jeudi","Vendredi","Samedi","Dimanche"};

String getPage(){
  String page;
page += " <!DOCTYPE html> " ; 
page += " " ; 
page += " " ; 
page += " <html> " ; 
page += " " ; 
page += " <head> " ; 
page += " <meta charset='utf-8' /> " ; 
page += " <meta http-equiv='X-UA-Compatible' content='IE=edge'> " ; 
page += " <title>Thermostat</title> " ; 
page += " <meta name='viewport' content='width=device-width, initial-scale=1'> " ; 
page += " " ; 
page += " <script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script> " ; 
page += " </head> " ; 
page += " " ; 
page += " <body> " ; 
page += " <header> " ; 
page += " <h2>THERMOSTAT</h2> " ; 
page += " </header> " ; 
page += " " ; 
page += " " ; 
page += " <main> " ; 
page += " <div id='data'> " ; 
page += " <p id='temp'>"+String(temp)+"°C</p> " ; 
page += " <p id='etat'>Circulateur :";
  if (etat=="1")page +=" ON";
  else page +=" OFF";

page+= "</p> " ; 
page += " </div> " ;


page += " <form action='/' method='GET'> " ; 
page += " " ; 
page += " <h3>Réglages des modes </h3> " ; 
page += " <table class='consigne' width='80%'> " ;  
page += " " ; 
page += " <th> " ; 
page += " <td>Consigne</td> " ; 
page += " <td>Delta</td> " ; 
page += " </th> " ; 
page += " <tr> " ; 
page += " <td>Normal</td> " ; 
page += " <td class='consigne'> <input type='number' size='3' id='mode' placeholder='"+String(Normal)+"°' name='Normal'></td> " ; 
page += " <td class='consigne'> <input type='number' id='mode' placeholder='"+String(Deltan)+"°' size='3' name='Deltan'></td> " ; 
page += " </tr> " ; 
page += " <tr> " ; 
page += " <td>Eco</td> " ; 
page += " <td class='consigne'> <input type='number' id='mode' placeholder='"+String(Eco)+"°' size='3' name='Eco'></td> " ; 
page += " <td class='consigne'> <input type='number' id='mode' placeholder='"+String(Deltae)+"°' size='3' name='Deltae'></td> " ; 
page += " </tr> " ; 
page += " </table> " ; 
page += " <h3>IP du circulateur</h3> " ; 
page += " <input type='text' size='12' id='ip' placeholder='"+ip+"' name='ip'> " ; 
page += " <br> <br> " ; 

page += " <input type='submit' Value='Valider'> " ;  
page += " </form> " ; 
page += " <br> " ; 
page += " " ; 
page += " </main> " ; 

page += " <main> " ; 
page += " <h3>Programmateur </h3> " ; 
page += " <div class='prog'> " ; 
page += " <table class='prog'> " ; 
page += " <tr> " ; 
page += " <td class='prog' id='c1'> </td> " ; 
page += " <td class='prog'>00</td> " ; 
page += " <td class='prog'>01</td> " ; 
page += " <td class='prog'>02</td> " ; 
page += " <td class='prog'>03</td> " ; 
page += " <td class='prog'>04</td> " ; 
page += " <td class='prog'>05</td> " ; 
page += " <td class='prog'>06</td> " ; 
page += " <td class='prog'>07</td> " ; 
page += " <td class='prog'>08</td> " ; 
page += " <td class='prog'>09</td> " ; 
page += " <td class='prog'>10</td> " ; 
page += " <td class='prog'>11</td> " ; 
page += " <td class='prog'>12</td> " ; 
page += " <td class='prog'>13</td> " ; 
page += " <td class='prog'>14</td> " ; 
page += " <td class='prog'>15</td> " ; 
page += " <td class='prog'>16</td> " ; 
page += " <td class='prog'>17</td> " ; 
page += " <td class='prog'>18</td> " ; 
page += " <td class='prog'>19</td> " ; 
page += " <td class='prog'>20</td> " ; 
page += " <td class='prog'>21</td> " ; 
page += " <td class='prog'>22</td> " ; 
page += " <td class='prog'>23</td> " ; 
page += " <td class='prog'>24</td> " ; 
page += " </tr> " ; 
page += " </table> " ; 
page += " " ; 
page += " <table class='prog'> " ;
int c=0;
for(int j=1;j<=7;j++){
  page += " <tr> " ; 
  page += " <td class='prog'>"+jour[j-1]+"</td> " ; 
  for(int i=1;i<=24;i++){
    c++;
    planning[j-1][i-1]=prog[c-1];
    page += " <td class='prog' style='color:rgba(1, 1, 1, 0);' onclick='color("+String(c)+")' id='"+String(c)+"' bgcolor='";
   if (prog[c-1]=='1')page+="#2196F3";
   else page+="#fff";
  page+="'>00</td> " ; 
}
page += " </tr> " ; 
}
page += " </table> " ; 
page += " </div> " ; 
page += " <br> " ; 
page += " <input type='submit' Value='Valider' onclick='send()'> " ; 
page += " </main> " ;
page += " <main> " ; 
page += " <h3>Statistiques</h3> " ; 
page += " " ; 
page += " " ; 
page += " <table class='consigne' width='65%'> " ; 
page += " <tr> " ; 
page += " <td colspan='2'> <b>Temps de fonctionnement</b></td> " ; 
page += " </tr> " ; 
page += " <tr> " ; 
page += " <td class='consigne'> Circulateur </td> " ; 
page += " <td> "+String(h)+"h "+String(m)+"m "+String(s)+"s </td> " ; 
page += " </tr> " ; 
page += " " ; 
page += " <tr><td><br></td></tr> " ; 
page += " <tr> " ; 
page += " <td colspan='2'> <b>Températures</b></td> " ; 
page += " </tr> " ; 

int n=0;
moyi=0;
for (int i=0;i<=23;i++){
  if(data[i]!=0 && data[i]> maxi) maxi =data[i];
  if(data[i]!=0 && data[i]< mini) mini =data[i];
  if(data[i]!=0){
    moyi=data[i]+moyi;
    n++;
  }
  
}
moyi=moyi/n;

page += " <tr> " ; 
page += " <td class='consigne'> Maximum </td> " ; 
page += " <td> "+String(maxi)+"°C</td> " ; 
page += " </tr> " ; 
page += " <tr> " ; 
page += " <td class='consigne'> Minimum </td> " ; 
page += " <td> "+String(mini)+"°C</td> " ; 
page += " </tr> " ; 
page += " <tr> " ; 
page += " <td class='consigne'> Moyenne </td> " ; 
page += " <td> "+String(moyi)+"°C</td> " ; 
page += " </tr> " ; 
page += " " ; 
page += " </table> " ; 
page += " <div id='curve_chart'></div> " ; 
page += " " ; 
page += " </main> " ; 
page += " " ; 
page += " <script type='text/javascript'> " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " function color(var1) { " ; 
page += " var x = document.getElementById(var1).bgColor; " ; 
page += " console.log(x); " ; 
page += " if (x === '#2196F3') { " ; 
page += " x = '#fff'; " ; 
page += " } " ; 
page += " else { " ; 
page += " x = '#2196F3'; " ; 
page += " } " ; 
page += " document.getElementById(var1).bgColor = x; " ; 
page += " " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " function send() { " ; 
page += " var x; " ; 
page += " var chaine = ''; " ; 
page += " for (var i = 1; i < 169; i++) { " ; 
page += " x = document.getElementById(i).bgColor; " ; 
page += " if (x === '#2196F3') { " ; 
page += " chaine += '1'; " ; 
page += " } " ; 
page += " else { " ; 
page += " chaine += '0'; " ; 
page += " } " ; 
page += " } " ; 
page += " document.location.href = '/?prog=' + chaine; " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " google.charts.load('current', { 'packages': ['corechart'] }); " ; 
page += " google.charts.setOnLoadCallback(drawChart); " ; 
page += " google.charts.load('current', { 'packages': ['corechart'] }); " ; 
page += " google.charts.setOnLoadCallback(drawChart); " ; 
page += " " ; 
page += " " ; 
page += " function drawChart() { " ; 
page += " var data = google.visualization.arrayToDataTable([ " ; 
page += " ['Heure', 'Température'], " ; 

for (int i=0;i<=23;i++){
page += " [["+String(i)+", 0], "+String(data[i])+"]" ; 
if (i!=23)page+=",";
}/*
page += " [[0, 0], 21], " ; 
page += " [[1, 0], 21], " ; 
page += " [[2, 0], 21], " ; 
page += " [[3, 0], 21], " ; 
page += " [[4, 0], 21], " ; 
page += " [[5, 0], 21], " ; 
page += " [[6, 0], 20.5], " ; 
page += " [[7, 0], 20.5], " ; 
page += " [[8, 0], 20.5], " ; 
page += " [[9, 0], 20.5], " ; 
page += " [[10, 0], 20.5], " ; 
page += " [[11, 0], 20.5], " ; 
page += " [[12, 0], 20.5], " ; 
page += " [[13, 0], 20], " ; 
page += " [[14, 0], 20], " ; 
page += " [[15, 0], 20], " ; 
page += " [[16, 0], 20], " ; 
page += " [[17, 0], 19.9], " ; 
page += " [[18, 0], 20], " ; 
page += " [[19, 0], 20.5], " ; 
page += " [[20, 0], 21], " ; 
page += " [[21, 0], 21], " ; 
page += " [[22, 0], 21.1], " ; 
page += " [[23, 0], 21] " ; */
page += " " ; 
page += " " ; 
page += " ]); " ; 
page += " " ; 
page += " var options = { " ; 
page += " title: 'Evolution de la température', " ; 
page += " legend: 'none', " ; 
page += " hAxis: { " ; 
page += " format: 'HH:mm ', " ; 
page += " title: 'Temps' " ; 
page += " }, " ; 
page += " vAxis: { " ; 
page += " title: 'Température' " ; 
page += " }, " ; 
page += " }; " ; 
page += " " ; 
page += " var chart = new google.visualization.LineChart(document.getElementById('curve_chart')); " ; 
page += " " ; 
page += " chart.draw(data, options); " ; 
page += " } " ; 
page += " </script> " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " </body> " ; 
page += " " ; 
page += " </html> " ; 
page += " " ; 
page += " " ; 
page += " <style> " ; 
page += " header { " ; 
page += " padding-top: 5%; " ; 
page += " background-color: #3A86FF; " ; 
page += " width: 100%; " ; 
page += " height: 50px; " ; 
page += " margin: 0em; " ; 
page += " font-family: Avantgarde, TeX Gyre Adventor, URW Gothic L, sans-serif; " ; 
page += " } " ; 
page += " " ; 
page += " h2 { " ; 
page += " " ; 
page += " font-size: 1.5em; " ; 
page += " color: white; " ; 
page += " margin: 0; " ; 
page += " margin-left: 5px; " ; 
page += " } " ; 
page += " " ; 
page += " label { " ; 
page += " color: #4F4F4F; " ; 
page += " margin-left: 10px; " ; 
page += " font-size: 1.2em; " ; 
page += " vertical-align: top; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " body { " ; 
page += " margin: 0em; " ; 
page += " background-color: #e8eaed; " ; 
page += " font-family: Gill, 'Gill Sans MT', 'Calibri', 'Trebuchet MS', sans-serif; " ; 
page += " color: #4F4F4F; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " main { " ; 
page += " width: 90%; " ; 
page += " background-color: white; " ; 
page += " margin: auto; " ; 
page += " margin-top: 5%; " ; 
page += " box-shadow: 2px 2px 2px gray; " ; 
page += " padding-top: 1px; " ; 
page += " padding-bottom: 10px; " ; 
page += " padding-left: 10px; " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " h3 { " ; 
page += " margin-left: 10px; " ; 
page += " font-size: 1.1em; " ; 
page += " } " ; 
page += " " ; 
page += " table.consigne { " ; 
page += " border-collapse: collapse; " ; 
page += " " ; 
page += " margin-top: 20px; " ; 
page += " margin-bottom: 20px; " ; 
page += " font-family: Avantgarde, TeX Gyre Adventor, URW Gothic L, sans-serif; " ; 
page += " font-size: 1em; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " #c1 { " ; 
page += " padding-left: 62px; " ; 
page += " " ; 
page += " margin-right: 10px; " ; 
page += " } " ; 
page += " " ; 
page += " div.prog { " ; 
page += " width: 95%; " ; 
page += " overflow: auto; " ; 
page += " " ; 
page += " padding-right: 10px; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " td.prog { " ; 
page += " border: black 1px solid; " ; 
page += " text-align: center; " ; 
page += " width: 30px; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " table.prog { " ; 
page += " " ; 
page += " margin-right: 10px; " ; 
page += " border-collapse: collapse; " ; 
page += " " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " #data { " ; 
page += " font-family: Avantgarde, TeX Gyre Adventor, URW Gothic L, sans-serif; " ; 
page += " } " ; 
page += " " ; 
page += " #temp { " ; 
page += " margin: 0px; " ; 
page += " text-align: center; " ; 
page += " font-size: 5em; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " #etat { " ; 
page += " text-align: center; " ; 
page += " font-size: 2em; " ; 
page += " margin: 0px; " ; 
page += " margin-bottom: 20px; " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " " ; 
page += " #planning { " ; 
page += " /*width:90%;*/ " ; 
page += " border-collapse: collapse; " ; 
page += " margin: auto; " ; 
page += " margin-top: 20px; " ; 
page += " margin-bottom: 20px; " ; 
page += " } " ; 
page += " " ; 
page += " #titre { " ; 
page += " word-spacing: 15px; " ; 
page += " } " ; 
page += " " ; 
page += " #colonneplan { " ; 
page += " " ; 
page += " border-color: #4F4F4F; " ; 
page += " border-style: solid; " ; 
page += " width: 25px; " ; 
page += " height: 22px; " ; 
page += " " ; 
page += " " ; 
page += " } " ; 
page += " input[type=text] { " ; 
page += " border-radius: 0; " ; 
page += " -webkit-appearance: none; " ; 
page += " " ; 
page += " margin-left:20px; " ; 
page += " border-style: solid; " ; 
page += " border:solid #4F4F4F 1px; " ; 
page += " text-align: center; " ; 
page += " } " ; 
page += " input[type=number] { " ; 
page += " border-radius: 0; " ; 
page += " -webkit-appearance: none; " ; 
page += " width:50px; " ; 
page += " margin: 0em; " ; 
page += " border-style: solid; " ; 
page += " border-color: white; " ; 
page += " text-align: center; " ; 
page += " } " ; 
page += " " ; 
page += " input[type=submit] { " ; 
page += " border-radius: 0; " ; 
page += " -webkit-appearance: none; " ; 
page += " margin-left: 10px; " ; 
page += " border-style: solid; " ; 
page += " border-color: white; " ; 
page += " text-align: center; " ; 
page += " background-color: #3A86FF; " ; 
page += " color: white; " ; 
page += " } " ; 
page += " " ; 
page += " " ; 
page += " #mode { " ; 
page += " " ; 
page += " border: 1px #4F4F4F solid; " ; 
page += " " ; 
page += " } " ; 
page += " " ; 
page += " #curve_chart { " ; 
page += " " ; 
page += " " ; 
page += " width: 100%; " ; 
page += " height: 200px; " ; 
page += " " ; 
page += " " ; 
page += " } " ; 



return page;
}
/*
void connexion()
{
  u8g2.setFont(u8g2_font_5x7_tf );
  u8g2.clearBuffer();

  u8g2.setCursor(40, 10); 
  WiFi.begin(ssid, password);
  u8g2.print("Connecting");
  u8g2.setCursor(1, 20);
  while (WiFi.status() != WL_CONNECTED) {

   
    u8g2.print(".");
    u8g2.sendBuffer();  
    

    delay(500);
  }
   u8g2.clearBuffer();  
   u8g2.drawStr(40, 10,"Connected"); 
   u8g2.sendBuffer();  
  delay(1000);
  u8g2.clearBuffer();  
  u8g2.setCursor(60, 35);
}*/

void refresh_planning(){
   int c=0;
  for(int j=1;j<=7;j++){
    Serial.print(j);
    Serial.print(" ");
  for(int i=1;i<=24;i++){
   c++;
   planning[j-1][i-1]=prog[c-1];
   Serial.print( planning[j-1][i-1]);
   Serial.print(" ");
  }
  Serial.println(" ");
}


}
void handleSubmit() {
  // Actualise le GPIO / Update GPIO 
 if (server.arg("Normal").toInt() != 0 ) Normal= server.arg("Normal").toInt();
 if (server.arg("Eco").toInt() != 0 ) Eco= server.arg("Eco").toInt();
 if (server.arg("Deltan").toFloat() != 0 ) Deltan= server.arg("Deltan").toFloat();
 if (server.arg("Deltae").toFloat() != 0 ) Deltae= server.arg("Deltae").toFloat();
 if (server.arg("prog") != "" ) prog= server.arg("prog");
 if (server.arg("ip") != "" ) ip= server.arg("ip");
   Serial.println(Normal);
   Serial.println(Eco);
   Serial.println(Deltan);
   Serial.println(Deltae);
   Serial.println(prog);
   
   refresh_planning();
}


void handleRoot()
{ 
  if ( server.hasArg("Normal") || server.hasArg("prog") ) {
    handleSubmit();
  } 
   server.send ( 200, "text/html", getPage() ); 
  }



void reset_data(){
  
   if(hour()==0 && minute()==0 && 0<= second()<=2){
    h,m,s=0;
    flag=0;
    maxi=0;
    mini=100;
    
    for(int i=0;i<24;i++){
      data[i]=0;
    }
    delay(3000);
  }
}



void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  u8g2.setFont(u8g2_font_5x7_tf );
  u8g2.clearBuffer();

  u8g2.setCursor(1, 10); 
  u8g2.print("        Config mode");
  u8g2.setCursor(1, 30); 
  u8g2.print("  Entrez vos parametres ");
    u8g2.setCursor(1, 40); 
  u8g2.print("  reseaux sur le point  ");
  u8g2.setCursor(1, 50); 
   u8g2.print("  d'acces : Thermostat ");
 
  u8g2.setCursor(1, 60);
    u8g2.print("  IP :  ");
 
  u8g2.print(WiFi.softAPIP());
  u8g2.sendBuffer();

  
}
void connexion(){
   WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x7_tf );
  u8g2.setCursor(40, 32); 
  u8g2.print("Connecting...");
  u8g2.sendBuffer();  

  wifiManager.autoConnect("Thermostat");
  u8g2.clearBuffer();
  u8g2.setCursor(40, 32); 
  u8g2.print("Connected !");
  u8g2.sendBuffer();
  delay(1000);
}


void setup() 
{
  Serial.begin ( 115200 );
  u8g2.begin(); 

 connexion();
  Serial.println(WiFi.localIP());
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
  tempsensor.begin(0x18);
  tempsensor.setResolution(2);
  
  NTP.begin("fr.pool.ntp.org", 1, true);
  NTP.setInterval(60);

  NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
  if (error) {
    Serial.print("Time Sync error: ");
    if (error == noResponse)
      Serial.println("NTP server not reachable");
    else if (error == invalidAddress)
      Serial.println("Invalid NTP server address");
    }
  else {
    Serial.print("Got NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
});

  refresh_planning();
}







void loop()
{
  if(WiFi.status() != WL_CONNECTED){
    connexion();
  }
  server.handleClient();
  reset_data(); 
  
  
  tempsensor.wake();
  temp = tempsensor.readTempC();
  
  if (temp <= (consigne-delta)) etat ="1";
  if (temp >= (consigne+delta))etat ="0"; 

  http.begin(client,"http://"+ip+"/control?cmd=GPIO,2,"+etat);
  if(http.GET()==-1){
    etat="error";
    Serial.println("error");
  }


  http.end();

 
  
  u8g2.clearBuffer();

  //Ip
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(10, 10);
  u8g2.print(WiFi.localIP());

 //Température
  u8g2.setFont(u8g2_font_crox5hb_tf);
  u8g2.setCursor(10, 35);
  u8g2.print(temp,1);
  u8g2.write(0xB0);
  //Mode
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(80, 10);
  u8g2.print("Mode");
  u8g2.setCursor(80, 20);
  u8g2.setFont(u8g2_font_helvB08_tf);
  j=weekday()-1;
  if(j==0)j+=7;
 

  if(planning[j-1][hour()]=='1'){
    consigne = Normal;
    delta = Deltan;

    u8g2.print("Normal");
  }else {
    consigne = Eco;
    delta = Deltae;

    u8g2.print("Eco");
  }
 //Consigne
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(80, 30);
  u8g2.print("Consigne");
  u8g2.setCursor(80, 40);
   u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.print(consigne);
  u8g2.write(0xB0);
 //Delta
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(80, 50);
  u8g2.print("Delta");
  u8g2.setCursor(80, 60);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.print(delta,1);
  u8g2.write(0xB0);
//Circulateur
   u8g2.setCursor(10, 50);
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.print("Circulateur");
  u8g2.setCursor(10, 60);
  u8g2.setFont(u8g2_font_helvB08_tf);
  if (etat=="1")u8g2.print("On");
  else if(etat=="0") u8g2.print("Off");
  else u8g2.print("Error");


  u8g2.sendBuffer(); 

if(hour()!=hbis){
  data[hour()]=temp;
  hbis=hour();  
}
  
  sprintf(chaine,"%.1f",temp);
  tempsensor.shutdown_wake(1); 
  
if(etat=="1" && flag==0 && now()>1000){
 debut = now();
 flag=1;
}
/*if(etat=="0"){
 flag=0;
}*/
if (etat=="1"){
  mnt = now()-debut;
h=mnt/3600;
m=(mnt%3600)/60;
s=(mnt%60);

}

Serial.println(debut);
Serial.println(mnt);
Serial.print(h);
Serial.print(":");
Serial.print(m);
Serial.print(":");
Serial.println(s);
Serial.println(" ");



  
  delay(1000);

 
  
}
