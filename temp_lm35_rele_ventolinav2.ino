/* Conf: 
 *  Arduino Nano
 *  ATmega328P (Old Bootloader)
 *  9600 baud
 */

#define potPin  A0  //Analog IN Temperatura
#define RELE_S  12  //Rele pin S
#define LED7    7   //Led Esterno
#define LED_INT 13  //Led Interno

#define TEMP_HI   32  //Temperatura MAX
#define TEMP_LOW  28  //Temperatura MIN


bool isHot = false;
char incomingCMD;

void fanOFF();
void fanOn();


void setup(){
  Serial.begin(9600);
  pinMode(RELE_S, OUTPUT);
  pinMode(LED_INT, OUTPUT);
  pinMode(LED7, OUTPUT);
}

void fanOFF(){
  digitalWrite(RELE_S, LOW);
  digitalWrite(LED_INT, LOW);

  digitalWrite(LED7, !digitalRead(LED7));
}

void fanOn(){
    //Fa cambiare lo status del led
    digitalWrite(LED7, HIGH);
    digitalWrite(RELE_S, HIGH);
    digitalWrite(LED_INT, HIGH);
    Serial.print(" FAN=[ON]");
}

void loop (){

  //Leggo il valore dal sensore di t.
  int aRead = analogRead(potPin);

  //Conversione
  float tempC = aRead * 0.48875;

  Serial.print("T=[");
  Serial.print(tempC);
  Serial.print("]");

  incomingCMD = Serial.read();

  if(incomingCMD == 'a'){
    do{
      Serial.println("ACCESO!!");
      Serial.println(incomingCMD);
      isHot = true;
      //Accende tutto
      fanOn();
      incomingCMD = Serial.read();
      
      Serial.println(incomingCMD);
    }while( incomingCMD == ' ' );
  }else{
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
  }

  Serial.println("");
  delay(1000);
}
