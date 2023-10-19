#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

#define muestras 10

int x, y;
int accion = 0;

//Variable con la dirección del canal por donde se va a transmitir
byte direccion[5] ={'s','t','e','v','e'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);



/******************************************************************
                  Datos del Control
*******************************************************************
0. Analog X
1. Analog Y
2. Boton JoysTick
---------------------
3. Boton A
4. Boton B
5. Boton C
6. Boton D
---------------------
7. Boton Start
8. Boton Pause
*******************************************************************/
int datos[10];



void setup() {
  Serial.begin(115200);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);




  //inicializamos el NRF24L01 
  radio.begin();

  //Abrimos un canal de escritura
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(direccion);


}

void loop() {

  datos[2] = digitalRead(2);
  datos[3] = digitalRead(3);
  datos[4] = digitalRead(4);
  datos[5] = digitalRead(5);
  datos[6] = digitalRead(6);
  datos[7] = digitalRead(7);

  // put your main code here, to run repeatedly:
  x = 0; y = 0;

  
  for(int i = 0; i < muestras; i++){
    x += analogRead(A0);
    y += analogRead(A1);
  }
  
  x /= muestras; y /= muestras;
  x = map(x, 0, 1023, -180, 180);
  y = map(y, 0, 1023, -180, 180);
  
  
  if( x < -100){
      accion = 1; //Ir a la izquierda 
  }
  else if(x > 100){
      accion = 2; //Ir a la derecha
  }
  else if (y > 50){
      accion = 3; //Ir al frente
  }

  else if (y < -50){
      accion = 4; //Ir atras
  }
  else{
      accion = 5; //Detenido
  }

  

  Serial.print("X: "); Serial.print(x); Serial.print("\tY: "); Serial.print(y); Serial.print("\tAccion: "); Serial.print(accion); 

  switch(accion){

    case 1: 
      y < 0 ? y=0.0:y=y;
      //MotorDerecho.write(x*-1);
      //MotorIzquierdo.write(y);
      Serial.print("\tMotor Izquierdo: "); Serial.print(y); Serial.print("\tMotor Derecho: "); Serial.println(x*-1);
      datos[0] = x*-1;
      datos[1] = y;
      break;

    
    case 2:
      y < 0 ? y=0.0:y=y;
      //MotorDerecho.write(y);
      //MotorIzquierdo.write(x);

      Serial.print("\tMotor Izquierdo: "); Serial.print(x); Serial.print("\tMotor Derecho: "); Serial.println(y);
      datos[0] = y;
      datos[1] = x;
      break;
    
    case 3:
      y < 0 ? y=0.0 : y=y;
      //MotorDerecho.write(y);
      //MotorIzquierdo.write(y);

      Serial.print("\tMotor Izquierdo: "); Serial.print(y); Serial.print("\tMotor Derecho: "); Serial.println(y);
      datos[0] = y;
      datos[1] = y;
      break;
    
    case 5:
      y < 0 ? y=0.0:y=y;
      //MotorDerecho.write(y);
      //MotorIzquierdo.write(y);

      Serial.print("\tMotor Izquierdo: "); Serial.print(y); Serial.print("\tMotor Derecho: "); Serial.println(y);
      datos[0] = y;
      datos[1] = y;
      break;

    default: Serial.println();break;
  }

  bool ok = radio.write(datos, sizeof(datos));
  //reportamos por el puerto serial los datos enviados 
  if(ok)
  {
    //Serial.print("Datos enviados: "); 
  }
  else
  {
     //Serial.println("no se ha podido enviar");
  }
  
  delay(0);

}


