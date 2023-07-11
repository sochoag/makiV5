#include "EMGFilters.h"

unsigned long EMG_num = 0;

EMGFilters emgFilters[3];

SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_60HZ;

int getEMGCount(int gforce_envelope);
int calibrationEMG(byte emgPin);

void initEMG()
{
  // Params: Sample Rate, Notch Filter, EnableNotch, EnableLowPass, EnableHighPass
  Serial.begin(115200);
  for (int i = 0; i <= sizeof(emgPins); i++)
  {
    emgFilters[i].init(sampleRate, humFreq, true, true, true);
    calibratedValues[i] = calibrationEMG(i);
  }
}

int readEMG(byte index)
{
  int data = analogRead(emgPins[index]);
  int dataAfterFilter = emgFilters[index].update(data);
  int envelope = sq(dataAfterFilter);
  return envelope;
}

int calibrationEMG(byte index)
{
  Serial.println("Inicio calibracion " + String(index));
  int calibratedValue = 0;
  for (int calibration = 0; calibration <= 10; calibration++)
  {
    int meanValue = 0;
    for (int sample = 0; sample <= samples; sample++)
    {
      meanValue += readEMG(index);
      delayMicroseconds(500);
    }
    meanValue /= samples * 0.3;
    calibratedValue += meanValue;
  }
  return calibratedValue / 3;
  // Serial.println("Inicio calibración");
  // for (int i = 0; i <= 10; i++)
  // {
  //   Serial.println("Calibración " + String(i));
  //   int meanValues[3] = {0, 0, 0};
  //   for (int j = 0; j <= samples; j++)
  //   {
  //     for (int k = 0; k < sizeof(emgPins); k++)
  //     {
  //       meanValues[k] += readEMG(k);
  //       delayMicroseconds(500);
  //     }
  //   }
  //   for (int k = 0; k <= sizeof(emgPins); k++)
  //   {
  //     meanValues[k] /= samples * 0.3;
  //     calibratedValues[k] += meanValues[k];
  //   }
  // }
  // for (int k = 0; k <= sizeof(emgPins); k++)
  // {
  //   calibratedValues[k] /= 3;
  // }
  // Serial.println("Fin calibración");
}

void loopEMG()
{
  Serial.print("SensorValue1:");
  Serial.print(readEMG(0));
  Serial.print("\tSensorValue2:");
  Serial.print(readEMG(1));
  Serial.print("\tSensorValue3:");
  Serial.print(readEMG(2));
  Serial.print("\tCalibratedValue1:");
  Serial.print(calibratedValues[0]);
  Serial.print("\tCalibratedValue2:");
  Serial.print(calibratedValues[1]);
  Serial.print("\tCalibratedValue3:");
  Serial.println(calibratedValues[2]);
  // }
  delayMicroseconds(500);
}

// int getEMGCount(int gforce_envelope)
// {
//   static long integralData = 0;
//   static long integralDataEve = 0;
//   static bool remainFlag = false;
//   static unsigned long timeMillis = 0;
//   static unsigned long timeBeginzero = 0;
//   static long fistNum = 0;
//   static int TimeStandard = 200;
//   /*
//     The integral is processed to continuously add the signal value
//     and compare the integral value of the previous sampling to determine whether the signal is continuous
//    */
//   integralDataEve = integralData;
//   integralData += gforce_envelope;
//   /*
//     If the integral is constant, and it doesn't equal 0, then the time is recorded;
//     If the value of the integral starts to change again, the remainflag is true, and the time record will be re-entered next time
//   */
//   if ((integralDataEve == integralData) && (integralDataEve != 0))
//   {
//     timeMillis = millis();
//     if (remainFlag)
//     {
//       timeBeginzero = timeMillis;
//       remainFlag = false;
//       return 0;
//     }
//     /* If the integral value exceeds 200 ms, the integral value is clear 0,return that get EMG signal */
//     if ((timeMillis - timeBeginzero) > TimeStandard)
//     {
//       integralDataEve = integralData = 0;
//       return 1;
//     }
//     return 0;
//   }
//   else
//   {
//     remainFlag = true;
//     return 0;
//   }
// }