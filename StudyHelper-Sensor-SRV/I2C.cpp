#include "I2C.h"

void I2C::readCoefficients() {
  int16_t a0coeff;
  int16_t b1coeff;
  int16_t b2coeff;
  int16_t c12coeff;

  int16_t a0coeffMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_A0_COEFF_MSB);
  int16_t a0coeffLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_A0_COEFF_LSB);
  int16_t b1coeffMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_B1_COEFF_MSB);
  int16_t b1coeffLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_B1_COEFF_LSB);
  int16_t b2coeffMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_B2_COEFF_MSB);
  int16_t b2coeffLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_B2_COEFF_LSB);
  int16_t c12coeffMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_C12_COEFF_MSB);
  int16_t c12coeffLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_C12_COEFF_LSB);

  a0coeff = (( (uint16_t) a0coeffMSB << 8) | a0coeffLSB);
  b1coeff = (( (uint16_t) b1coeffMSB << 8) | b1coeffLSB);
  b2coeff = (( (uint16_t) b2coeffMSB << 8) | b2coeffLSB);
  c12coeff = (( (uint16_t) (c12coeffMSB << 8) | c12coeffLSB)) >> 2;

  _mpl115a2_a0 = (float)a0coeff / 8;
  _mpl115a2_b1 = (float)b1coeff / 8192;
  _mpl115a2_b2 = (float)b2coeff / 16384;
  _mpl115a2_c12 = (float)c12coeff;
  _mpl115a2_c12 /= 4194304.0;

}

/**************************************************************************/
I2C::I2C() : I2C(* new DbParameters("I2C", "ilid", "i2clist", "value", "iid", "i2c", "i2ctemp")) { }

I2C::I2C(DbParameters newDbParameters) {
  _mpl115a2_a0 = 0.0F;
  _mpl115a2_b1 = 0.0F;
  _mpl115a2_b2 = 0.0F;
  _mpl115a2_c12 = 0.0F;
	setParametersToSensor(newDbParameters);
	newDbParameters.setMasterField("i2cpres");
	setParametersToSensor(newDbParameters);
}

/**************************************************************************/
/*!
@brief Setups the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
void I2C::begin() {
  fd = wiringPiI2CSetup(MPL115A2_ADDRESS);
  // Read factory coefficient values (this only needs to be done once)
  readCoefficients();
}

/**************************************************************************/
/*!
@brief Gets the floating-point pressure level in kPa
*/
/**************************************************************************/
float I2C::getPressure() {
  float pressureComp,centigrade;

  getPT(&pressureComp, &centigrade);
  return pressureComp;
}


/**************************************************************************/
/*!
@brief Gets the floating-point temperature in Centigrade
*/
/**************************************************************************/
float I2C::getTemperature() {
  float pressureComp, centigrade;

  getPT(&pressureComp, &centigrade);
  return centigrade;
}

/**************************************************************************/
/*!
@brief Gets both at once and saves a little time
*/
/**************************************************************************/
void I2C::getPT(float *P, float *T) {
  uint16_t pressure, temp;
  float pressureComp;

  // Get raw pressure and temperature settings
  wiringPiI2CWrite(fd, MPL115A2_REGISTER_STARTCONVERSION);

  // Wait a bit for the conversion to complete (3ms max)
  delay(5);

  int16_t pressureMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_PRESSURE_MSB);
  int16_t pressureLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_PRESSURE_LSB);
  int16_t tempMSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_TEMP_MSB);
  int16_t tempLSB = wiringPiI2CReadReg8(fd, MPL115A2_REGISTER_TEMP_LSB);

  pressure = (( (uint16_t) pressureMSB << 8) | pressureLSB) >> 6;
  temp = (( (uint16_t) tempMSB << 8) | tempLSB) >> 6;
  
  // See datasheet p.6 for evaluation sequence
  pressureComp = _mpl115a2_a0 + (_mpl115a2_b1 + _mpl115a2_c12 * temp ) * pressure + _mpl115a2_b2 * temp;

  // Return pressure and temperature as floating point values
  *P = (((65.0F / 1023.0F) * pressureComp) + 50.0F) * 1.4503777 * 0.1F; // PSI
  *T = ((float) temp - 498.0F) / - 5.35F + 25.0F; // C
  cout << "In temp: " << temp << endl;
  cout << "in Pressure: " << pressure << endl;
  
}