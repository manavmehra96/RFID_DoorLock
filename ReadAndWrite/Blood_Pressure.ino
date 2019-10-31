#define NZEROS 8
#define NPOLES 8
#define GAIN   2.998077659e+01
#define GAIN1  2.059100943e+13
float xv[NZEROS+1], yv[NPOLES+1];
unsigned long int milli_time;    //variable to hold the time
float voltage,opv,opv1,ip;         //variable to hold the voltage form A0
int count=0;

void setup() {
  Serial.begin(9600);               //Fastest baudrate
  Serial.println("CLEARDATA");        //This string is defined as a 
                                      // commmand for the Excel VBA 
                                      // to clear all the rows and columns
  inita();
  Serial.println("LABEL,Computer Time,Milli second,Volt, opv, opv1");  
                                      //LABEL command creates label for 
                                      // columns in the first row with bold font
}
void inita()
{
   int i;
   for ( i=0 ; i<9; i++ )
   {
    xv[i]=0;
    yv[i]=0;
   }
}
void loop() {
  milli_time = millis();
  ip=analogRead(A5);
  voltage = (5.0 * ip) / 1023.0;
  Serial.print("DATA,TIME,");
   milli_time = millis();
   Serial.print(milli_time);
   Serial.print(",");
  opv = filterloop(voltage);
  Serial.print(milli_time);
  Serial.print(",");
  Serial.print(voltage);
  Serial.print(",");
  Serial.println(opv);
    
  delay(10);                    //Take samples every one second
}

float filterloop(float ipv)
  { for (;;)
      {  xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6]; xv[6] = xv[7]; xv[7] = xv[8]; 
        xv[8] = ipv / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6]; yv[6] = yv[7]; yv[7] = yv[8]; 
        yv[8] =   (xv[0] + xv[8]) - 4 * (xv[2] + xv[6]) + 6 * xv[4]
                     + ( -0.1572575620 * yv[0]) + ( -0.2314246263 * yv[1])
                     + (  0.7190311952 * yv[2]) + (  1.0610321243 * yv[3])
                     + ( -1.3817661995 * yv[4]) + ( -1.7156352979 * yv[5])
                     + (  1.2305708310 * yv[6]) + (  1.4753463815 * yv[7]);
        opv = yv[8];
        return(opv);
      }
  }
  
