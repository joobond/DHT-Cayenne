#include <Arduino.h>  //lib do arduino
#include <Adafruit_Sensor.h> //lib sensor DHT11
//---------------------sensor temperatura------------------//

#include <DHT.h> 
#define DHTPIN D6 // pino que conectaremos o sensor DHT11
#define DHTTYPE DHT11 // DHT11 � o tipo do sensor que utilizaremos (importante para o construtor)

#define pinLed D7 //led que indica que o esp8266 est� ligado
#define led1 D5  //led  vermelho na porta D5 do esp8266
#define led2 D4  // led azul na porta D4 do esp8266
#define rele D8


DHT dht(DHTPIN, DHTTYPE);

//---------------------Define Cayenne------------------//
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// ------------------------WiFi network info----------//


char ssid[] = "BOND";
char wifiPassword[] = "12345678";

// -----Cayenne authentication info. This should be obtained from the Cayenne Dashboard----//
char username[] = "325909d0-e476-11e8-a056-c5cffe7f75f9";
char password[] = "c6c2560389cc3901eb777c69bd03748826450cfb";
char clientID[] = "97441820-e47c-11e8-9c33-75e6b356cec4";

 
void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
	dht.begin(); // inicializa��o para se comunicar com o sensor
	pinMode(pinLed,OUTPUT);//led pisca
	pinMode(led1,OUTPUT);  //led vermelho controle pelo Dashboard
	pinMode(led2,OUTPUT);  //led azul controle pelo Dashboard
	pinMode(rele, OUTPUT);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	

	int h = dht.readHumidity(); // faz a leitura da umidade do sensor
	float t = dht.readTemperature(); // faz a leitura da temperatura do sensor

    

    Serial.print("TEMP -> ");
    Serial.print(t); // escreve a temperatura em tela
    Serial.println("C"); // coloca o �C� para indicar que � graus Celcius
    Serial.print("UMIDADE-> ");
    Serial.print(h); // escreve a umidade em tela
    Serial.println("%\n"); // escreve o s�mbolo de �porcentagem� para indicar a umidade

	delay(2000);

	//------------led na  D7  - piscando --------//
    digitalWrite(pinLed, HIGH);
    delay(500);
    digitalWrite(pinLed,LOW);
    delay(500);


// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite()
	Cayenne.digitalSensorWrite(0,h); //'cannel 0' recebe umidade
	Cayenne.celsiusWrite(1, t);     // 'cannel 1 recebe temperatura
	
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);

}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	if(t > 23)digitalWrite(rele,HIGH);
	else digitalWrite(rele,LOW);
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}


CAYENNE_IN(4){
	int readLed = getValue.asInt();
	if(readLed)digitalWrite(led1,LOW);
	else digitalWrite(led1,HIGH);
}
CAYENNE_IN(5){
	int readLed = getValue.asInt();
	if(readLed)digitalWrite(led2,LOW);
	else digitalWrite(led2,HIGH);
}