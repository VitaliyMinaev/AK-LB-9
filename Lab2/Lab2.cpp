#include <iostream>

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

// Y = (2.5 + 3*n) ^ 1/2 = sqr(2.5 + 3 * n)         ; n - ? => Y > 100
void Task2() {
    double n = 0, testRes = 0, anotherRes, garbage = 0;

    double resultPerIteration = 0;

    const double THREE = 3, TWOANDHALF = 2.5, ONEHUNDRED = 100, ONE = 1;
    _asm
    {
        finit; inits FPU

    iteration:

        fld n                       ; st(0) = n
        fmul THREE                  ; st(0) = 3 * n
        fadd TWOANDHALF             ; st(0) = 2.5 + 3 * n
        
        fsqrt                       ; sqr(2.5 + 3 * n)

        fstp testRes



        fld testRes                 ; push result to the FPU stack
        fld ONEHUNDRED              ; push ONEHUNDRED to the FPU stack
        fucomip st(0), st(1)        ; compare 100 > result

        fstp testRes                ; pop result from the FPU stack
		fstp garbage                ; pop garbage from the FPU stack

        fld n
        fadd ONE                    ; n += 1
        fstp n

        jnc iteration               ; hump if CF = 0

        fld n
        fsub ONE                    ; n += 1
        fstp n
    }

    std::cout << "Result: " << testRes << " ; N: " << n << std::endl;
}
int main()
{
    std::cout << "Task 1: " << std::endl;
    Task1();
    std::cout << std::endl << "Task 2: " << std::endl;
    Task2();
    // std::cout << sqroot(42) << std::endl;
}