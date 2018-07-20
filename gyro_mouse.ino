#include<Wire.h>
#include<Mouse.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double axval,ayval;
const int len = 5;
double arrx[len],arry[len];
double vx,vy;

void setup(){
  pinMode(9,INPUT);                     //left click
  pinMode(10,INPUT);                    //right click
  pinMode(15,INPUT);                    //mouse on-off
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  vx=0;
  vy=0;
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print(double(AcX)/8192);
  Serial.print(double(AcY)/8192);
  Serial.print('\t');
  Serial.print('\t');
  
  for(int i=0;i<len-1;i++){
    arrx[i]=arrx[i+1];                  //left shift
    arry[i]=arry[i+1];
  }
  arrx[len-1]=double(AcX)/8192;
  arry[len-1]=double(AcY)/8192;

  ayval=0;
  axval=0;
  for(int i=0;i<len;i++){
    axval=axval+(arrx[i]/len);                //gives axval in g's
    ayval=ayval+(arry[i]/len);
  }
  Serial.print((double(AcX)/8192)-axval);
  Serial.print('\t');
  Serial.print((double(AcY)/8192)-ayval);
  Serial.print('\t');
  Serial.print('\t');


  if(digitalRead(15) == HIGH){
    Mouse.begin();
//    vx=(double(AcX)/8192);//-axval+vx;
//    vy=(double(AcY)/8192);//-ayval+vy;
    vx=axval;
    vy=ayval;   
    Mouse.move(-20*vx,-30*vy,0);

    if (digitalRead(9)==LOW) {
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }

    if (digitalRead(10)==LOW) {
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    
  } else  {
    Mouse.end();
    vx=0;
    vy=0;
  }
  

//  if ((double(AcX)/8192)>0.02 || (double(AcX)/8192)<-0.02){
//    vx=(double(AcX)/8192)-axval+vx;
//  }
//  if ((double(AcY)/8192)>0.02 || (double(AcY)/8192)<-0.02){
//    vy=(double(AcY)/8192)-ayval+vy;
//  }
  Serial.print(vx);
  Serial.print('\t');
  Serial.println(vy);

//  Serial.print(" | AcZ = "); Serial.print(AcZ);
//  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
//  Serial.print(" | GyX = "); Serial.print(GyX);
//  Serial.print(" | GyY = "); Serial.print(GyY);
//  Serial.print(" | GyZ = "); Serial.println(GyZ);

  delay(25);
}
