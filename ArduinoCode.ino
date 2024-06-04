#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
#include "rfClassifier.h"
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
float instance[2];

int sensorPinTurbidity=A1; //turbidity
void setup()
{
  Serial.begin(115200);
  Serial.println("pH meter experiment!");    //Test the serial monitor
}
void loop()
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical
  {
    // Serial.print("Voltage:");
    //     Serial.print(voltage,2);
        Serial.print("P");
    Serial.print(pHValue,2);
    Serial.print("*");
        printTime=millis();
  }

delay(5000);
Serial.print("T");
Serial.print(getturbidity());
Serial.print("*");

instance[0]=pHValue;
instance[1]= getturbidity();
Serial.print("M");
Serial.print(rfClassifier.predict(instance));
Serial.print("*");

}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

int getturbidity() 
{
  int sensorValue = analogRead(sensorPinTurbidity);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  delay(100);
  return (turbidity);
  
}