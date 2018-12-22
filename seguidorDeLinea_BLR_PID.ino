/*
Seguidor de linea PID con Basic Line Robot V1.0
Por: MakingMario 
De: Making Robotics Colombia    
*/
#include <OrangutanMotors.h>
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LEDTX   1          //PD1
#define LED1    9          //PB1 (izquierdo)
#define LED2    7          //PD7 (derecho)

#define PUSH1   10         //botón PB2 (izquierdo)
#define PUSH2   4          //botón PD4 (derecho)

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define interruptPin  2       //encoder derecho
volatile byte state = LOW;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int inByte = '0';        
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int varADC__ = 0;
int sensor=1000;

OrangutanMotors motors;
unsigned int  LL_VALORES[6], _lastValue; //para la función LEER_LINEA()

int posicion;
int proportional;
int derivative;
int last_proportional;
int integral;
int power_difference;
int maximo = 80;

char BluetoothData;
int Red_value=0;
int Green_value=0;
int Blue_value=0;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  
  pinMode(LEDTX, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
 
  pinMode(PUSH1, INPUT);
  digitalWrite(PUSH1, HIGH);//ACTIVA PULL UP

  pinMode(PUSH2, INPUT);
  digitalWrite(PUSH2, HIGH);//ACTIVA PULL UP

//  pinMode(interruptPin,INPUT);
//  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, HIGH);

  motors.setSpeeds(0, 0);
  inByte = 0;
  sensor = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
 /*
 Serial.begin(9600);//el LEDTX ya no indica estados 
 
 while(1)
 {
  
      //-----------------------------------------------------------------------------------------
      //----------------------------------IMPERIUM ENTRADAS--------------------------------------
      //-----------------------------------------------------------------------------------------
      //Envio valores de sensores
      Serial.print("*C"+String(analogRead(A6))+"*"); //A6
      Serial.print("*T"+String(analogRead(A3))+"*"); //A3
      Serial.print("*E"+String(analogRead(A2))+"*"); //A2
      Serial.print("*D"+String(analogRead(A1))+"*"); //A1
      Serial.print("*B"+String(analogRead(A0))+"*"); //A0
      //Envio color deseado para los sensores
      if (analogRead(A6)<100) Serial.print("*GR255G0B0*"); //Red                              //A6
      if (analogRead(A6)>=100&&analogRead(A6)<=700) Serial.print("*GR255G200B0*"); //Orange   //A6
      if (analogRead(A6)>700) Serial.print("*GR0G255B0*"); //Green                            //A6
      
      if (analogRead(A3)<100) Serial.print("*LR255G0B0*"); //Red                              //A3
      if (analogRead(A3)>=100&&analogRead(A3)<=700) Serial.print("*LR255G200B0*"); //Orange   //A3
      if (analogRead(A3)>700) Serial.print("*LR0G255B0*"); //Green                            //A3

      if (analogRead(A2)<100) Serial.print("*NR255G0B0*"); //Red                              //A2
      if (analogRead(A2)>=100&&analogRead(A2)<=700) Serial.print("*NR255G200B0*"); //Orange   //A2
      if (analogRead(A2)>700) Serial.print("*NR0G255B0*"); //Green                            //A2
      
      if (analogRead(A1)<100) Serial.print("*MR255G0B0*"); //Red                              //A1
      if (analogRead(A1)>=100&&analogRead(A1)<=700) Serial.print("*MR255G200B0*"); //Orange   //A1
      if (analogRead(A1)>700) Serial.print("*MR0G255B0*"); //Green                            //A1

      if (analogRead(A0)<100) Serial.print("*HR255G0B0*"); //Red                              //A0
      if (analogRead(A0)>=100&&analogRead(A0)<=700) Serial.print("*HR255G200B0*"); //Orange   //A0
      if (analogRead(A0)>700) Serial.print("*HR0G255B0*"); //Green                            //A0

      //-----------------------------------------------------------------------------------------
      //Envio dato del pulsador
      Serial.print("*K"+String(digitalRead(PUSH1))+"*");
      //Envio color
      if (digitalRead(PUSH1)==HIGH) Serial.print("*OR0G255B0*"); //green
      else Serial.print("*OR255G0B0*"); //Red

      //-----------------------------------------------------------------------------------------
      //Envio dato del pulsador
      Serial.print("*I"+String(digitalRead(PUSH2))+"*");
      //Envio color
      if (digitalRead(PUSH2)==HIGH) Serial.print("*JR0G255B0*"); //green
      else Serial.print("JOR255G0B0*"); //Red
      //-----------------------------------------------------------------------------------------
       if(digitalRead(PUSH1)==LOW)digitalWrite(LED1,HIGH); else digitalWrite(LED1,LOW); 
       if(digitalRead(PUSH2)==LOW)digitalWrite(LED2,HIGH); else digitalWrite(LED2,LOW); 
      //-----------------------------------------------------------------------------------------       
  } //while(1)
 */ 
   if(digitalRead(PUSH1)==LOW) 
   {
    delay(100);
    digitalWrite(LED1,HIGH);
    inByte ++;
    if(inByte>=6) inByte=1;
    delay(150);
    digitalWrite(LED1,LOW);
   }
     
   if(digitalRead(PUSH2)==LOW) 
   {
    delay(100);
    for(int i = 1; i<= inByte; i ++)
    {
      digitalWrite(LED2,HIGH);
      delay(100);
      digitalWrite(LED2,LOW);
      delay(400);
    }
    sensor = inByte;
   }

   if(sensor != 0)
   {
    posicion=LEER_LINEA();  //Devuelve un valor entre 1000 y 3000
    proportional = ((int)posicion) - 2000;  

    // Calcula el término derivativo y el término integral 
    derivative = proportional - last_proportional;
    integral += proportional;

    // Registra la última posición proporcional
    last_proportional = proportional;
      
    if(inByte==1)//si llegó 1?
    {
    power_difference = proportional/25 + integral/10000 + derivative*1; 
    maximo = 60;//
    }
    
    if(inByte==2)//si llegó 2?
    {
    power_difference = proportional/22 + integral/10000 + derivative*1;
    maximo = 70;//
    }

    if(inByte==3)//si llegó 3?
    {
    power_difference = proportional/19 + integral/10000 + derivative*1; 
    maximo = 80;//
    }

    if(inByte==4)//si llegó 4?
    {
    power_difference = proportional/16 + integral/10000 + derivative*1; //proportional/6 + integral/10000 + derivative*2;
    maximo = 90;//
    }
    
    if(inByte==5)//si llegó 5? aún no está lista
    {
    power_difference = proportional/15 + integral/10000 + derivative*3/2; 
    maximo = 100;//
    }
    
    if(power_difference > maximo)
      power_difference = maximo; 
    if(power_difference < -maximo)
      power_difference = -maximo;

    //Esta estructura if else se anula si se desea que el robot "no se mueva"
    if(power_difference < 0)
    motors.setSpeeds(maximo, maximo+power_difference+5);//+5 a causa del motor derecho 
    else
    motors.setSpeeds(maximo-power_difference, maximo-5);//-5 a causa del motor derecho 
   }     
///-------      
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void blink() 
{
  state = !state;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
       ......
     .A3 A2 A1.
   .            .      
 ..A6          A0..
 ||.   BASIC    .||
 ||.   LINE     .||
 ||.   ROBOT    .||
 ...   V1.0     ...
   .            .          
      ........
*/
int LEER_LINEA(void)
  {
char LL_CH[5]={0,A1,A2,A3,0};//marcados como 1 2 3
unsigned char LL_CONT, on_line ;
unsigned long avg;
unsigned int sum; 


    for(LL_CONT=1;LL_CONT<=3;LL_CONT++)
    {
     LL_VALORES[LL_CONT]=0;
    }

    for(LL_CONT=1;LL_CONT<=3;LL_CONT++)
    {                                                                                  
     LL_VALORES[LL_CONT] = (map(analogRead( LL_CH[LL_CONT]), 0, 1023, 0, 1000));
    }
        avg = 0;
        sum = 0;
    on_line = 0;

    for(LL_CONT=1;LL_CONT<=3;LL_CONT++)
    {
        int value = LL_VALORES[LL_CONT];
        //si el valor obtenido es mayor a éste se considera en línea
        if(value > 200) //200
        { on_line = 1;}


        // para valores que están por encima del valor del umbral de ruido
        if(value > 50) //50
        {
            avg += (long)(value) * ((LL_CONT)* 1000);  
            sum += value;
        }
    }

  if(!on_line)//Si no está en línea
  {
        if(_lastValue < 2000)//para definir hacia que lado se salió
        return 1000;         //si el último valor es menor que 2000 entonces asigne 1000
        else                 
        return 3000;         //si el último valor es mayor que 2000 entonces asigne 3000
    }

    _lastValue = avg/sum;

    return _lastValue;
  }
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


