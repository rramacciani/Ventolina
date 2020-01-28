/* Conf: 
 *  Arduino Nano
 *  ATmega328P (Old Bootloader)
 *  9600 baud
 */

#define potPin  A0  //Analog IN Temperatura
#define RELE_S  12  //Rele pin S
#define LED7    7   //Led Esterno
#define LED_INT 13  //Led Interno

#define TEMP_HI   35  //Temperatura MAX
#define TEMP_LOW  32  //Temperatura MIN


bool isHot = false;
const long interval = 1000;
unsigned long previousMillis = 0;

void fanOFF();
void fanOn();
bool isFanOn();

void setup(){
  Serial.begin(9600);
  delay(100);
  
  pinMode(RELE_S, OUTPUT);
  pinMode(LED_INT, OUTPUT);
  pinMode(LED7, OUTPUT);
}

void fanOFF(){
  digitalWrite(LED7, !digitalRead(LED7));
  digitalWrite(RELE_S, LOW);
  digitalWrite(LED_INT, LOW);
}

void fanOn(){
    digitalWrite(LED7, HIGH);
    digitalWrite(RELE_S, HIGH);
    digitalWrite(LED_INT, HIGH);
}

bool isFanOn(){
  return digitalRead(RELE_S);
}

void loop (){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    //Leggo il valore dal sensore di t.
    int aRead = analogRead(potPin);
  
    //Conversione
    float tempC = aRead * 0.48875;
  
    Serial.print("T=[");
    Serial.print(tempC);
    Serial.print("]");
    if(isFanOn()){
      Serial.print(" FAN=[ON]");
    }else{
      Serial.print(" FAN=[OFF]");
    }
    
    //Controllo Rele'
    if(tempC >= TEMP_HI){
      isHot = true;
      //Accende tutto
      fanOn();
    }else if((tempC > TEMP_LOW) && (tempC < TEMP_HI)) {
      //Siamo nella temperatura intermedia, continuiamo a fare quello che facevamo
      if(isHot){
        //eravamo in fase di raffreddamento, continuiamo
        fanOn();
      }else{
        //eravamo in fase spenta, continuiamo
        //Fa lampeggiare il led per controllare
        fanOFF();
      }
    }else if(tempC <= TEMP_LOW) {
      isHot = false;
      //Fa lampeggiare il led per controllare
      fanOFF();
    }

    Serial.println("");
  }
  
}
