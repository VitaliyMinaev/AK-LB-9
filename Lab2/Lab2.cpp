#include <iostream>

const double NONE_VALUE = 0000.00001;

// Y = (x - 7) / (x + 20)^ 1/2 = (x - 7) / sqr(x + 20)
void Task1() {
    double x = 3;           // += 2.5
    long counter = 0;       // < 5
    double resultSqrt = 0, resultTask = 0;

    long sqrootIterator = 0, sqroot, two = 2;

    const double SEVEN = 7, TWENTY = 20, TWOANDHALF = 2.5;

    double results[6];
    results[0] = 10000;

    _asm
    {
        lea EBX, results

        finit               ; inits FPU
        mov ecx, 5

    iteration:
        fld x               ; st(0) = x
        fadd TWENTY         ; st(0) = 23 + x

        fsqrt               ; st(0) = sqr(x + 20)

        fstp resultSqrt     ; resultSqrt = sqr(x + 20) => pop value

        fld x
        fsub SEVEN

        fdiv resultSqrt

        fstp results[ecx * 8]
        
        fld x
        fadd TWOANDHALF     ; x += 2.5
        fstp x

        loop iteration
    }

    for (int i = 5; i >= 1; --i) {
        std::cout << "Result " << i << ": " << results[i] << std::endl;
    }
}

int main()
{
    Task1();
    // std::cout << sqroot(42) << std::endl;
}