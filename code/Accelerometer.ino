#define SAMPLE_COUNT 400  //for accelerometer calibration. Should lie between 1 and 65535.

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

float accelOneG = 9.80665;

#define STANDARD_GRAVITY 9.80665

#define ACCELO_RANGE 4*STANDARD_GRAVITY //Range is +- 2g
const float ACCELO_SCALING_FACTOR=ACCELO_RANGE/65536.0;

uint8_t accelSampleCount=0;

int16_t accelSample[3] = {0,0,0};
float accelRate[3] = {0.0,0.0,0.0};
float accelBias[3] = {0.0,0.0,0.0};

extern int16_t accelRaw[3];

void measureAccel() {
  //readMPU6000Accel();TODO:Wirte appropriate code.

  accelRate[XAXIS] = accelRaw[XAXIS]* ACCELO_SCALING_FACTOR + accelBias[XAXIS];
  accelRate[YAXIS] = accelRaw[YAXIS]* ACCELO_SCALING_FACTOR + accelBias[YAXIS];
  accelRate[ZAXIS] = accelRaw[ZAXIS]* ACCELO_SCALING_FACTOR + accelBias[ZAXIS];
}

void measureAccelSum() {
  //readMPU6000Accel();TODO:Wirte appropriate code.
  accelSample[XAXIS] += accelRaw[XAXIS];
  accelSample[YAXIS] += accelRaw[YAXIS];
  accelSample[ZAXIS] += accelRaw[ZAXIS];

  accelSampleCount++;
}

void evaluateAccel() //WARNING:AccelSampleCount!=0
{
  accelRate[XAXIS] = accelSample[XAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR + accelBias[XAXIS];
  accelRate[YAXIS] = accelSample[YAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR + accelBias[YAXIS];
  accelRate[ZAXIS] = accelSample[ZAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR + accelBias[ZAXIS];
  
  accelSample[XAXIS]=0;
  accelSample[YAXIS]=0;
  accelSample[ZAXIS]=0;
  
  accelSampleCount = 0;
}

void computeaccelBias() {
  for (uint16_t samples = 0; samples < SAMPLE_COUNT; samples++) 
  {
    //readMPU6000Sensors();TODO:Wirte appropriate code.
    measureAccelSum();
    delayMicroseconds(2500);
  }

  accelRate[XAXIS] = accelSample[XAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR;
  accelRate[YAXIS] = accelSample[YAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR;
  accelRate[ZAXIS] = accelSample[ZAXIS]/accelSampleCount* ACCELO_SCALING_FACTOR;
  
  accelSampleCount = 0;

  accelBias[XAXIS] = -accelRate[XAXIS];
  accelBias[YAXIS] = -accelRate[YAXIS];
  accelBias[ZAXIS] = -STANDARD_GRAVITY - accelRate[ZAXIS];

  accelOneG = abs(accelRate[ZAXIS] + accelBias[ZAXIS]);
}
