
#include<SPI.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <LiquidCrystal.h>

// wifi details
char ssid[] = "TALKTALKE65547";
const char password[] = "U9QN9RCN";

// ThingSpeak details
const unsigned long channel_id = "1640814";
const char write_api_key[] = "YIJLFCOO24A130RO";
WiFiClient client;

// LCD screen set up
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte degree_symbol[8] =
  {
    0b00111,
    0b00101,
    0b00111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

int gate=11;
volatile unsigned long duration=0;
unsigned char i[5];
unsigned int j[40];
unsigned char value=0;
unsigned answer=0;
int z=0;
int b=1;

void setup() 
{
  // connecting to wifi and ThingSpeak
  Serial.begin(9600);
  delay(100);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected!");
  ThingSpeak.begin(client);

  // initialize screen
  lcd.begin(16, 2);
  lcd.print("Temp = ");
  lcd.setCursor(0,1);
  lcd.print("Humidity = ");
  lcd.createChar(1, degree_symbol);
  lcd. setCursor(9,0);
  lcd.write(1);
  lcd.print("C");
  lcd.setCursor(13,1);
  lcd.print("%");
} 

void loop() 
{
  delay(1000);

  while(1)
  {
    delay(1000);
    pinMode(gate,OUTPUT);
    digitalWrite(gate,LOW);
    delay(20);
    digitalWrite(gate,HIGH);
    pinMode(gate,INPUT_PULLUP);

    duration=pulseIn(gate, LOW);
    if(duration <= 84 && duration >= 72)
    {
      while(1)
      {
        duration=pulseIn(gate, HIGH);
        if(duration <= 26 && duration >= 20)
        {
          value=0;
        }
        else if(duration <= 74 && duration >= 65)
        {
          value=1;
        }
        else if(z==40)
        {
          break;
        }
        i[z/8]|=value<<(7- (z%8));
        j[z]=value;
        z++;      
      }
    }

    answer=i[0]+i[1]+i[2]+i[3];

    if(answer==i[4] && answer!=0)
    {
      lcd.setCursor(7,0);
      lcd.print(i[2]);
      lcd.setCursor(11,1);
      lcd.print(i[0]);

      Serial.println("Temp = ");
      Serial.print(i[2]);
      Serial.println("Humidity = ");
      Serial.print(i[0]);

      ThingSpeak.setField(1, i[2]);
      ThingSpeak.setField(2, i[0]);
      ThingSpeak.writeFields(channel_id, write_api_key);
    }
  
    z=0;
    i[0]=i[1]=i[2]=i[3]=i[4]=0;

  }
}
