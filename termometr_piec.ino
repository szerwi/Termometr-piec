#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Ustawienie adresu ukladu na 0x27
#define termometr 10
#define klawisz 7

OneWire oneWire(termometr);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

float temp;
float suma;
int ilosc;
float srednia;
float mini;
float maxi;
byte ustaw = 0;
float sumamala=0;
int iloscmala=0;
float sredniamala=0;
float sumaduza=0;
int iloscduza=0;
float sredniaduza=0;
byte przekroczenia;
int i;
byte x=0;

void setup() 
{
  lcd.begin(16,2);
  sensors.begin();
  sensors.setResolution(insideThermometer, 9);
  pinMode(klawisz, INPUT_PULLUP);
}

void odczyt()
{
  DeviceAddress deviceAddress;
  sensors.requestTemperatures();
  temp = sensors.getTempC(deviceAddress);

  if(ustaw==0)
  {
    maxi = temp;
    mini = temp;
    ustaw=1;
  }
  
    suma += temp;
    ilosc++;
    srednia = suma/ilosc;

  
    if(temp>maxi)
    {
      maxi = temp;
    }
    if(temp<mini)
    {
      mini = temp;
    }

    
    if((temp>=30.00) && (temp<40.00))
    {
      sumamala += temp;
      iloscmala++;
      sredniamala = sumamala/iloscmala;
    }
    else if((temp>=40.00) && (temp<=55.00))
    {
      sumaduza += temp;
      iloscduza++;
      sredniaduza = sumaduza/iloscduza;
    }

    
    if((x==0) && (temp>=55))
    {
      przekroczenia++;
      x=1;
    }
    else if((x==1) && (temp<55))
    {
      x=0;
    }
    
    lcd.clear(); lcd.home();
    lcd.print("Aktualna: ");
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print("Srednia: ");
    lcd.print(srednia);
}

void przycisk()
{
  byte i = 0;
  if(digitalRead(klawisz) == LOW)
  {
    lcd.clear(); lcd.home();
    lcd.print("Sr 30-40: ");
    lcd.print(sredniamala);
    lcd.setCursor(0,1);
    lcd.print("Sr 40-55: ");
    lcd.print(sredniaduza);
    for(i>=0; i<25; i++)
    {
      delay(200);
      if(digitalRead(klawisz) == LOW)
      {
        i=0;
        lcd.clear(); lcd.home();
        lcd.print("Max: ");
        lcd.print(maxi);
        lcd.setCursor(0,1);
        lcd.print("Min: ");
        lcd.print(mini);
        for(i>=0; i<25; i++)
        {
          delay(200);
          if(digitalRead(klawisz) == LOW)
          {
            i=0;
            lcd.clear(); lcd.home();
            lcd.print("Przekroczenia ");
            lcd.setCursor(0,1);
            lcd.print("55 stopni: ");
            lcd.print(przekroczenia);
            delay(5000);
          }
        }
      }
    }
    lcd.clear(); lcd.home();  
    lcd.print("Aktualna: ");
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print("Srednia: ");
    lcd.print(srednia);
  }
}

void loop() 
{
  i=0;
  odczyt();
  for(i>=0; i<1000; i++)
  {
  przycisk();
  delay(36);
  }
}

