/*Segundo parcial, Diego Uthurburu 1 B*/

#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Servo.h>

/**/
bool manejar_sistema(long opcion, float* maxima);
void mostrar_lcd(int cursor, char* texto, float tem=0);
void detectar_incendio(float maxima, float temperatura);
void controlar_leds(int led, int estado);


// SE DEFINEN LOS BOTONES 
#define boton_power 4278238976
#define boton_1 4010852096
#define boton_2 3994140416
#define boton_3 3977428736
#define boton_4 3944005376


#define LED_RED 9
#define LED_GREEN 8
#define SERVO 10
#define IR 11


int voltage;
float temperatura;
float temperatura_maxima = 60;
bool estado= true;
char estacion[20];
unsigned long valor_recibido;

Servo myServo;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Configuración de los pines



void setup() 
{
	Serial.begin(9600);
  	myServo.attach(SERVO, 500, 2500);
	IrReceiver.begin(IR, DISABLE_LED_FEEDBACK);
	lcd.begin(16,2);
  	pinMode(LED_RED, OUTPUT);
  	pinMode(LED_GREEN, OUTPUT);
  	myServo.write(90);//El servo comienza cerrado
}

/*##################MAIN######################*/
void loop() 
{
float voltage = analogRead(A0) * (5.0 / 1023.0);  // Leer el valor analógico y convertirlo a voltaje
temperatura = (voltage - 0.5) * 100.0;  // Convertir el voltaje a temperatura en grados Celsius

if (IrReceiver.decode()) 
{
  valor_recibido = IrReceiver.decodedIRData.decodedRawData;
  estado = manejar_sistema(valor_recibido, &temperatura_maxima);	
  
  
IrReceiver.resume();
}
   if(!estado)
  {
    mostrar_lcd(1, estacion);
   	mostrar_lcd(3,"",temperatura);
  }
  detectar_incendio(temperatura_maxima, temperatura);
  Serial.println(temperatura_maxima);
  delay (100);

}


/*##################   FIN  ########################*/


void detectar_incendio(float maxima, float temperatura)
{
	if(temperatura >= maxima && digitalRead(LED_GREEN)== HIGH)
    {	
      	lcd.clear();
    	myServo.write(180);
      	mostrar_lcd(1, "Incendio");
        mostrar_lcd(2, "Detectado");
    }
  	else
    {
    	myServo.write(90);
    }

}

bool manejar_sistema(long opcion, float* maxima)
{
	lcd.clear();
	switch(valor_recibido)
  {
    case boton_power:
      if (estado){
        mostrar_lcd(1, "Sistema");
        mostrar_lcd(2, "Encendido");
        controlar_leds(LED_GREEN,HIGH);
        controlar_leds(LED_RED,LOW);
      }
      else
      {
        mostrar_lcd(1, "Sistema");
        mostrar_lcd(2, "Apagado");
        controlar_leds(LED_GREEN,LOW);
        controlar_leds(LED_RED,HIGH);
      }
    estado = !estado; 
    delay(1000);  
    break;
    case boton_1:
    detectar_incendio(60, temperatura);
    strcpy(estacion, "Primavera");
    *maxima = 50;
    break;
    case boton_2:
    strcpy(estacion, "Otoño");
    *maxima = 45;
    break;
    case boton_3:
    strcpy(estacion, "Invierno");
    *maxima = 40;
    break;
    case boton_4:
    strcpy(estacion, "Verano"); 
    *maxima = 60;
    break;
    default:

    break;
	}

	return estado; 
}

void mostrar_lcd(int cursor, char* texto, float tem)
{ 
  	//lcd.clear();
  	
	if (cursor == 1)
    {	
		lcd.setCursor(0,0);//se setea el cursor
		lcd.print(texto);//se escribe el mesaje
    }
  	else if (cursor == 2) 
  	{
		lcd.setCursor(0,1);
    	lcd.print(texto);
  	}
  	else
  	{
      	lcd.setCursor(0,1);
    	lcd.print(tem);
      	lcd.print("°C");
  	}
}

void controlar_leds(int led, int estado)
{
	digitalWrite(led, estado);

}

	



