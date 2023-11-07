#include <iostream>
#include "Convolution.h"

int main()
{
    const char* filePath1 = "convolve_raw_0.dat";
    const char* filePath2 = "convolve_raw_1.dat";
    Convolution cnv = Convolution();
    cnv.ReadDataFromFiles(filePath1, filePath2);
    cnv.Convolve();
    cnv.WriteConvolutionToFile("convolved.dat", "convolvedX.dat", "convolvedY.dat");
}