#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <cmath>



//helpful bit shifting info
//https://www5.in.tum.de/lehre/vorlesungen/asc/ss18/fft.pdf

using complex64 = std::complex<double>;
using vector64 = std::vector<double>;
using uint32 = unsigned long;
using complexvector = std::vector<std::complex<double>>;
const double PI = acos(-1);
std::complex<double> im = { 0, 1 };

uint32 bitReverse(uint32 num, uint32 N) {
    auto depth = (uint32)log2(N);///N is the length of the array
    num = (num & 0xFFFF0000) >> 16 | (num & 0x0000FFFF) << 16;
    num = (num & 0xFF00FF00) >> 8 | (num & 0x00FF00FF) << 8;
    num = (num & 0xF0F0F0F0) >> 4 | (num & 0x0F0F0F0F) << 4;
    num = (num & 0xCCCCCCCC) >> 2 | (num & 0x33333333) << 2;
    num = (num & 0xAAAAAAAA) >> 1 | (num & 0x55555555) << 1; //the above shifting assumes a 32 bit int
    num = num >> (32 - depth); //correct if below 32 bit int by shifting important bits back to right 

    return num;
}

//unsigned int bitReverse64LUT(unsigned int num) {
//
//    //This is from Bit Twidling Hacks (Standford)
//
//    static const unsigned char BitReverseTable256[256] =
//    {
//    #   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
//    #   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
//    #   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
//        R6(0), R6(2), R6(1), R6(3)
//    };
//
//    unsigned int reversednum; // c will get v reversed
//
//    reversednum = (BitReverseTable256[num & 0xff] << 24) |
//        (BitReverseTable256[(num >> 8) & 0xff] << 16) |
//        (BitReverseTable256[(num >> 16) & 0xff] << 8) |
//        (BitReverseTable256[(num >> 24) & 0xff]);
//    return reversednum;
//}

//void checkreversal(unsigned int num)
//{
//    int n = 256;
//
//    auto startmine = std::chrono::high_resolution_clock::now();
//    bitReverse(n,n);
//    auto stopmine = std::chrono::high_resolution_clock::now();
//
//    auto startlut = std::chrono::high_resolution_clock::now();
//    bitReverse64LUT(n);
//    auto stoplut = std::chrono::high_resolution_clock::now();
//
//
//
//    std::cout << bitReverse(n,n) << std::endl;
//    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stopmine - startmine).count() << std::endl;
//    std::cout << std::endl; 
//    std::cout << bitReverse64LUT(n) << std::endl;
//    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stoplut - startlut).count() << std::endl;
//
//}

complexvector bitReversedVector(complexvector signal) {
    uint32 n = signal.size();
    complexvector reversed;

    for (uint32 i = 0; i < n; i++) {
        reversed.push_back(signal[bitReverse(i, n)]);
    }

    return reversed;
}

complexvector twidle(uint32 n) {
    complexvector twidlevector;

    for (uint32 k = 0; k < n; k++) {
        twidlevector.push_back(exp(-im * 2.0 * PI * (double)k / (double)n));
    }

    return twidlevector;
}

complexvector fftbutterfly(complexvector reversedsignal) {

    int N = reversedsignal.size();
    complex64 temp = 1;
    complexvector twidleFactors = twidle(N);

    for (int n = 2; n <= N; n *= 2) {//loop over the size of the signal (i.e. first pair first and second elements, then first and third, etc. 
        for (int k = 0; k < N / 2; k += n) {//loop over one level
            for (int j = 0; j < N / 2; j++) {//perform butterfly of one level - j is the twiddle permutator


                temp = twidleFactors[j] * reversedsignal[k + j + (n / 2)];



                reversedsignal[k + j] = reversedsignal[k + j] + temp;
                reversedsignal[k + j + (n / 2)] = reversedsignal[k + j] - temp;


            }

        }


    }

    return reversedsignal;

}

complexvector testSignal(uint32 n) {
    complexvector testsignal;

    for (uint32 i = 0; i < n; i++) {
        testsignal.push_back(sin((double)i / 10));
    }

    return testsignal;
}




int main() {
    complexvector test = testSignal(16);
    complexvector result = fftbutterfly(bitReversedVector(test));


    for (complex64 i : result) {
        std::cout << i << std::endl;

    }




}

