#include "mbed.h"

Serial pc(USBTX, USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
int sample = 800;
float ADCdata[800];
int time1=0, time2 =0;
float freq=0;
int freqArray[3]={0};
int i;
float j;
//seven segment dislay
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

//switch
DigitalIn Switch(SW3);
DigitalOut greenLED(LED2);
DigitalOut redLED(LED1);


int main(){
  for (i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        
        wait(1./sample);  //wait for the time to sample one data
      }

      //Calculate the time difference & frequency
      for (i = 0; i < sample; i++){
        if(ADCdata[i] < 0.02){
          if(time1==0) {
            time1 = i;
            //pc.printf("time1=%d\r\n", i);
          }
          else if(time2==0 && i>time1+5) {
            time2 = i;
            //pc.printf("time2=%d\r\n", i);
          }
          if(time1!=0 && time2!=0) {
            //pc.printf("time1=%d time2=%d break\r\n", time1, time2);
            break;
          }
        }
      }
      for (i = 0; i < sample; i++){
        pc.printf("%f\r\n", ADCdata[i]);
        wait(0.1);
      }
      freq = (time2-time1)*(1./800);
      freq = 1./freq;
      pc.printf("freq = %f\r\n", freq);
      freqArray[0] =freq/100;
      freqArray[1] = (freq- freqArray[0]*100)/10;
      freqArray[2] = freq - freqArray[0]*100 - freqArray[1]*10;

  while(1){      
      for(i=0; i<3; i++){
        pc.printf("freq[%d]=%d \r\n", i, freqArray[i]);
      }
    for(j=0; j<2; j+=0.05 ){
      Aout = 0.5 + 0.5*sin(j*3.14159);
      wait(1./(freq*40));
    }
      
    if(Switch==1){
        greenLED = 0;
        wait(0.001);
        redLED = 1;
        wait(0.001);
        for(int i=0; i<3; i++){
            display = 0;
            //wait(0.1);
        }
    }
    else if(Switch==0){
      //pc.printf("SW2 is off!!\r\n");
      for(int i=0; i<3; i++){
        //pc.printf("freq[%d]=%d \r\n", i, freqArray[i]);
        display = table[freqArray[i]];
        wait(0.1);
        redLED=0;
        greenLED=1;
        
      }
    }
  
    

  }

}