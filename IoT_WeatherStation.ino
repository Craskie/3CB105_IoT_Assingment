// libraries
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// declaring digital pin for sensor
#define DHTPIN 2

// defining sensor type
#define DHTTYPE DHT11

// initialiazing sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  Serial.println("-----------------------------------------");
  
  // screen set up
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.setCursor(10,0);
  lcd.print("*C");
  lcd.setCursor(14,1);
  lcd.print("%");

  // sensor turn on
  dht.begin();
}

void loop() 
{
  // wait couple seconds between measurements
  delay(2000);

  //reading temp and humidity 
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // check to make sure sensor has read data
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.print("Failed to read from DHT sensor!");
    return;
  }

  // outputting to serial monitor
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.println(" %\t");

  // outputting to screen
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(5,0);
  lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.setCursor(9,1);
  lcd.print(h);
}
