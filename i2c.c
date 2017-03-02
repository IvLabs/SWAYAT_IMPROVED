#include <wiringPiI2C.h>
#include <stdio.h>
#include <math.h>

int fd;                                                //variable for device address
int acclX, acclY, acclZ;
int gyroX, gyroY, gyroZ;
double acclX_scaled, acclY_scaled, acclZ_scaled;
double gyroX_scaled, gyroY_scaled, gyroZ_scaled;

//function 1
//Reads 16 bit of data from register address given as argument.
int read_word_2c(int addr)
{
int val;
val = wiringPiI2CReadReg8(fd, addr);//first 8 bits 
val = val << 8;  
val += wiringPiI2CReadReg8(fd, addr+1);//next 8 bits
if (val >= 0x8000)
val = -(65536 - val);
return val;
}


//function 2
double dist(double a, double b)
{
return sqrt((a*a) + (b*b));
}


//function 3
double get_y_rotation(double x, double y, double z)
{
double radians;
radians = atan2(x, dist(y, z));
return -(radians * (180.0 / M_PI));
}


//function 4
double get_x_rotation(double x, double y, double z)
{
double radians;
radians = atan2(y, dist(x, z));
return (radians * (180.0 / M_PI));
}





int main()
{
fd = wiringPiI2CSetup (0x68);       //068 is device address
wiringPiI2CWriteReg8 (fd,0x6B,0x00);//disable sleep mode 
                                    //0x6b is register address

printf("set 0x6B=%X\n",wiringPiI2CReadReg8 (fd,0x6B));

while(1) {

//the accleratormeter data is 16 bit data.Higher 8 bits at one address ,lower 8 bits at next address
acclX = read_word_2c(0x3B);//address where High byte of accelerometer X-axis data is stored
acclY = read_word_2c(0x3D);//address where High byte of accelerometer y-axis data is stored
acclZ = read_word_2c(0x3F);//address where High byte of accelerometer z-axis data is stored
acclX_scaled = acclX / 16384.0;
acclY_scaled = acclY / 16384.0;
acclZ_scaled = acclZ / 16384.0;

printf("My acclX_scaled: %f\n", acclX_scaled);
printf("My acclY_scaled: %f\n", acclY_scaled);
printf("My acclZ_scaled: %f\n", acclZ_scaled);
printf("My X rotation: %f\n", get_x_rotation(acclX_scaled, acclY_scaled, acclZ_scaled));
printf("My Y rotation: %f\n", get_y_rotation(acclX_scaled, acclY_scaled, acclZ_scaled));

delay(100);
}
return 0;
}

