#include <iostream>
#include <complex>
#include <vector>
#include <chrono>


typedef std::complex<double> complex64;

unsigned int bitReverse64(unsigned int num) {
    num = (num & 0xFFFF0000) >> 16 | (num & 0x0000FFFF) << 16;
    num = (num & 0xFF00FF00) >> 8 | (num & 0x00FF00FF) << 8;
    num = (num & 0xF0F0F0F0) >> 4 | (num & 0x0F0F0F0F) << 4;
    num = (num & 0xCCCCCCCC) >> 2 | (num & 0x33333333) << 2;
    num = (num & 0xAAAAAAAA) >> 1 | (num & 0x55555555) << 1;

    return num;
}

unsigned int bitReverse64LUT(unsigned int num) {

    //This is from Bit Twidling Hacks (Standford)

    static const unsigned char BitReverseTable256[256] =
    {
    #   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
    #   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
    #   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
        R6(0), R6(2), R6(1), R6(3)
    };

    unsigned int reversednum; // c will get v reversed

    reversednum = (BitReverseTable256[num & 0xff] << 24) |
        (BitReverseTable256[(num >> 8) & 0xff] << 16) |
        (BitReverseTable256[(num >> 16) & 0xff] << 8) |
        (BitReverseTable256[(num >> 24) & 0xff]);
    return reversednum;
}



int main()
{
    int n = 4096;

    auto startmine = std::chrono::high_resolution_clock::now();
    bitReverse64(n);
    auto stopmine = std::chrono::high_resolution_clock::now();

    auto startLUT = std::chrono::high_resolution_clock::now();
    bitReverse64LUT(n);
    auto stopLUT = std::chrono::high_resolution_clock::now();



    std::cout << bitReverse64(n) << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stopmine - startmine).count() << std::endl;
    std::cout << std::endl;
    std::cout << bitReverse64LUT(n) << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stopLUT - startLUT).count() << std::endl;


}

