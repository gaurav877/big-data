
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

int led=13; // declering LED in 13th pin
#define pulsein 8 //defining the pulse input pin 8
unsigned int pusle_count=0; //Declaring Pulse count type as integer
float units=0; //Declaring units as Float
unsigned int rupees=0; //declaring rupees types as unsigned integer

unsigned int temp=0,i=0; //declaring temp& i as unsigned integer

char str[70],flag1=0,flag2=0; // character length as string and declaring flags


void setup() 
{
  lcd.begin(16,2);  // initializing LCD type as 16x2
  Serial.begin(9600); // initializing serial baud rate as 9600
  pinMode(led, OUTPUT);
  pinMode(pulsein, INPUT);
   digitalWrite(pulsein, HIGH);
  lcd.setCursor(0,0);
  lcd.print("Wireless Energy");
  lcd.setCursor(0,1);
  lcd.print("      Meter    ");
  delay(2000);
  lcd.clear();
  lcd.print("By   ");
  delay(2000);
  lcd.clear();
  lcd.print("GSM Initilizing...");
  gsm_init();
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("AT+CNMI=2,2,0,0,0");
  init_sms();
  send_data("System Ready");
  send_sms();
  delay(1000);
  digitalWrite(led, LOW);
  lcd.clear();
}

void loop()    
{
    serialEvent();  // checking serial event
    units=rupees/5.0;  // calculation of unit
    lcd.setCursor(0,0);
    lcd.print("Units:");
    lcd.print(units);
    lcd.print("      ");
    lcd.setCursor(0,1);
    lcd.print("Rupees:");
    lcd.print(rupees);
    lcd.print("      ");
    read_pulse();
    
    if(temp==1)
    {
          check();
    }
}

void serialEvent() // serial event for receiving and checking the incoming sms
{
  while(Serial.available()) 
  {
    if(Serial.find("#"))
    {
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
      while (Serial.available()) 
      {
      char inChar=Serial.read();
      str[i++]=inChar;
      if(inChar=='*')
      {
        temp=1;
        return;
      } 
      } 
    }
   }
 }

void init_sms() //initializing & configuration of sending message
{
   Serial.println("AT+CMGF=1");
   delay(200);
   Serial.println("AT+CMGS=\"+919626071110\"");
   delay(200);
}

void send_data(String message)
{
  Serial.println(message);
  delay(200);
}

void send_sms()
{
  Serial.write(26);
}

//Reading Pulse from Energy Meter
void read_pulse()
{
    if(!digitalRead(pulsein))
    {
      digitalWrite(led, HIGH);
      units++;
      rupees=units*5;
      while(!digitalRead(pulsein));
      digitalWrite(led,LOW);
    
    }
}

//Checking the status of Message 
void check()
{
     
     if(!(strncmp(str,"units",5)))
      {
        Serial.println("AT+CMGF=1");
   delay(200);
   Serial.println("AT+CMGS=\"+918056921941\"");
   delay(200);
       lcd.setCursor(0,1);
       send_data("Energy Meter Unit:");
       Serial.println(units);
       send_data("\nRupees:");
       Serial.println(rupees);
       delay(200);
       Serial.write(26);
       flag1=1;
       flag2=1;
       temp=0;
      }
     }



void message_sent()
{
  lcd.clear();
  lcd.print("Message Sent.");
  delay(1000);
}

void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    delay(1000);
  }

  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }

  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0)
    {
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}

