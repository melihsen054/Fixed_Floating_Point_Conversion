#include <stdio.h>   // for printf, scanf
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <math.h>  // for strtol
#include <string.h>

#define NTH_BIT(b, n) ((b >> n) & 0x1)

#define BYTE_TO_BIN(b)   (( b & 0x80 ) ) |\
            (( b & 0x40 ) ) |\
            (( b & 0x20 ) ) |\
            (( b & 0x10 ) ) |\
            (( b & 0x08 ) ) |\
            (( b & 0x04 ) ) |\
            (( b & 0x02 ) ) |\
            ( b & 0x01 )

#define MANTISSA_TO_BIN(b)  (( b & 0x400000 ) ) |\
             (( b & 0x200000 ) ) |\
             (( b & 0x100000 ) ) |\
             (( b &  0x80000 ) ) |\
             (( b &  0x40000 ) ) |\
             (( b &  0x20000 ) ) |\
             (( b &  0x10000 ) ) |\
             (( b &  0x8000 ) ) |\
             (( b &  0x4000 ) ) |\
             (( b &  0x2000 ) ) |\
             (( b &  0x1000 ) ) |\
             (( b &  0x800 ) ) |\
             (( b &  0x400 ) ) |\
             (( b &  0x200 ) ) |\
             (( b &  0x100 ) ) |\
             (( b &  0x80 ) ) |\
             (( b &  0x40 ) ) |\
             (( b &  0x20 ) ) |\
             (( b &  0x10 ) ) |\
             (( b &  0x08 ) ) |\
             (( b &  0x04 ) ) |\
             (( b &  0x02 ) ) |\
              ( b & 0x01 )

typedef union UnFloatingPointIEEE754
 {
 struct
  {
   unsigned int mantissa : 23;
   unsigned int exponent : 8;
   unsigned int sign : 1;
  } raw;
float f;
} UFloatingPointIEEE754;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
uint32_t pack754_32 ( float f )
{
  UFloatingPointIEEE754 ieee754;
  uint32_t    floatingToIntValue = 0;
  ieee754.f = f;
  floatingToIntValue = (((NTH_BIT(ieee754.raw.sign, 0) << 8) |
  (BYTE_TO_BIN(ieee754.raw.exponent)))  << 23 ) | MANTISSA_TO_BIN(ieee754.raw.mantissa);
  return floatingToIntValue;
}
float unpack754_32( uint32_t floatingToIntValue )
{
  UFloatingPointIEEE754 ieee754;    unsigned int mantissa = 0;
  unsigned int exponent = 0 ;
  unsigned int sign = 0;
  sign = NTH_BIT(floatingToIntValue, 31);
  for( int ix=0; ix<8; ix++)
    exponent = (exponent | (NTH_BIT(floatingToIntValue, (30-ix))))<<1;
  exponent = exponent>>1;
  for( int ix=0; ix<23; ix++)
    mantissa = (mantissa | (NTH_BIT(floatingToIntValue, (22-ix))))<<1;
  mantissa = mantissa >> 1;
  ieee754.raw.sign = sign;
  ieee754.raw.exponent = exponent;
  ieee754.raw.mantissa = mantissa;
  return ieee754.f;
}
int main (int argc, char *argv[]) {
	int h, k;
	const float pi = 3.141592;
	char fileName[100];
	FILE * fp;
	char fileName1[100];
	FILE * fp1;
    char fileName2[100];
	FILE * fp2;
    char fileName3[100];
	FILE * fp3;
    char fileName4[100];
	FILE * fp4;

	/*
	 get _H and _N as program arguments
	*/
	char *p;
	int _H, _N;

	errno = 0;
	long conv1 = strtol(argv[1], &p, 10);
	// Check for errors: e.g., the string does not represent an integer
	// or the integer is larger than int
	if (errno != 0 || *p != '\0' || conv1 > INT_MAX) {
		// Put here the handling of the error, like exiting the program with
		// an error message
	} else {
		// No error
		_H = conv1;
	}

	errno = 0;
	long conv2 = strtol(argv[2], &p, 10);
		// Check for errors: e.g., the string does not represent an integer
	// or the integer is larger than int
	if (errno != 0 || *p != '\0' || conv2 > INT_MAX) {
		// Put here the handling of the error, like exiting the program with
		// an error message
	} else {
		// No error
		_N = conv2;
	}
    sprintf(fileName, "CosSinValues_H-%d_N-%d.mem" ,_H, _N);
    fp = fopen(fileName,"w");
	if (!fp)
	{
		printf("Failed to open text file\n");
		exit(1);
	}else{
		for(h=0;h<=_H;h++){
			for(k=0;k<_N;k++){
                    uint32_t n;
                    float f;
				//fprintf (fp, "%f\t%f\n",(cosf(2*pi*h*k/_N)),(-1*sinf(2*pi*h*k/_N))); // print decimal

				/*
				The # part gives you a 0x in the output string.
				The 0 and the x count against your "8" characters listed in the 08 part.
				You need to ask for 10 characters if you want it to be the same.
				*/
				//fprintf (fp, "%#010x\t%#010x\n",(int)((cosf(2*pi*h*k/_N)+1)*1000000),(int)((-1*sinf(2*pi*h*k/_N)+1)*1000000)); //print hexadecimal
                fprintf (fp, "%x\t%x\n",pack754_32(cosf(2*pi*h*k/_N)), pack754_32(-1*sinf(2*pi*h*k/_N)));//print ieee 754
                //n = pack754_32(3.5);//convert the number into ieee754
                //f = unpack754_32(n);//convert from ieee 754 to float
                //fprintf (fp,"%x\t%f\n", n,f );
			}
		}
		fclose (fp);
	}
	float Kp_real_initial [5]={40.0/20000*cosf(0),31.5*cosf(0.5*pi)/20000,50*cosf(0.3*pi)/20000,0,0};
	float Kp_imag_initial [5]={0,31.5*sinf(0.5*pi)/20000,50*sinf(0.3*pi)/20000,0,0};
	float Ki_real_initial [5]={15.0/20000*cosf(0),10*cosf(0.5*pi)/20000,10*cosf(0.3*pi)/20000,10*cosf(0.6*pi)/20000,0};
	float Ki_imag_initial [5]={0,10*sinf(0.5*pi)/20000,10*sinf(0.3*pi)/20000,10*sinf(0.6*pi)/20000,0};


	float Kp_real [_H];
	float Kp_imag [_H];
	float Ki_real [_H];
	float Ki_imag [_H];

	for(int ii=0;ii <= _H; ii++){
            Kp_real [ii]=Kp_real_initial[ii%5];
            Kp_imag [ii]=Kp_imag_initial[ii%5];
            Ki_real [ii]=Ki_real_initial[ii%5];
            Ki_imag [ii]=Ki_imag_initial[ii%5];
	}

	sprintf(fileName1, "r_Values_20sin+40.mem");
    fp1 = fopen(fileName1,"w");
    if (!fp1)
	{
		printf("Failed to open text file\n");
		exit(1);
	}else{
			for(k=0;k<_N;k++){
                    uint32_t n;
                    float f;
				//fprintf (fp, "%f\t%f\n",(cosf(2*pi*h*k/_N)),(-1*sinf(2*pi*h*k/_N))); // print decimal

				/*
				The # part gives you a 0x in the output string.
				The 0 and the x count against your "8" characters listed in the 08 part.
				You need to ask for 10 characters if you want it to be the same.
				*/
				//fprintf (fp, "%#010x\t%#010x\n",(int)((cosf(2*pi*h*k/_N)+1)*1000000),(int)((-1*sinf(2*pi*h*k/_N)+1)*1000000)); //print hexadecimal
                fprintf (fp1, "%x\n",pack754_32(20*sinf(2*pi/60*0.5*k)+40));//print ieee 754
                //n = pack754_32(3.5);//convert the number into ieee754
                //f = unpack754_32(n);//convert from ieee 754 to float
                //fprintf (fp,"%x\t%f\n", n,f );
			}

		fclose (fp1);
	}
	sprintf(fileName2, "Var_Values.mem");
    fp2 = fopen(fileName2,"w");
    if (!fp2)
	{
		printf("Failed to open text file\n");
		exit(1);
	}else{
                    uint32_t n;
                    float f;
				fprintf (fp2, "%x\t%x",pack754_32(_N),pack754_32(0.5));//print ieee 754
		fclose (fp2);
	}
	sprintf(fileName3, "Ki_Kp_Values.mem");
    fp3 = fopen(fileName3,"w");
    if (!fp3)
	{
		printf("Failed to open text file\n");
		exit(1);
	}else{
			for(k=0;k<=_H;k++){
                    uint32_t n;
                    float f;
				fprintf (fp3, "%x\t%x\t%x\t%x\n",pack754_32(Kp_real[k]),pack754_32(Kp_imag[k]),pack754_32(Ki_real[k]),pack754_32(Ki_imag[k]));//print ieee 754
                printf("%x\t%x\t%x\t%x\n",pack754_32(Kp_real[k]),pack754_32(Kp_imag[k]),pack754_32(Ki_real[k]),pack754_32(Ki_imag[k]));
			}
		fclose (fp3);
	}
	FILE *dosya;

	char YValues [1500][100];
	float Y_Values [1500];
	int i=0;

	if((dosya=fopen("Yvalues.txt","r"))!=NULL)
	{
		while(!feof(dosya)){
			fscanf(dosya,"%s",&YValues[i]);
            Y_Values[i]=strtod(YValues[i],NULL);
            //printf("%f\n",Y_Values[i]);
			i++;
		}
	}
	else
	{
		printf("YValues_yok");
	}
	sprintf(fileName4, "Y_Values.mem");
    fp4 = fopen(fileName4,"w");
    if (!fp4)
	{
		printf("Failed to open text file\n");
		exit(1);
	}else{
			for(k=0;k<1500;k++){
                    uint32_t n;
                    float f;

				fprintf (fp4, "%x\n",pack754_32(Y_Values[k]));//print ieee 754
			}
		fclose (fp4);
	}
}
