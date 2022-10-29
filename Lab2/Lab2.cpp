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

// 1/2 * arcCosec(3 * A^2 + 4 * B) => 1/2 * arcsin(1 / (3 * A^2 + 4 * B))
void Task3() {
    // Нехай A = 0, a B = 0.5
    const double A = 0, B = 0.5, THREE = 3, ONEANDHALF = 0.5, 
        FOUR = 4, ONE = 1, ONEHUNDREEDEIGHTEEN = 180, PI = 3.14159;
    double resultInRadians, resultInDegrees, fourMultiplyB, arcCosecValue;

    _asm
    {
        finit                   ; inits FPU

        fld B                   ; st(0) = B
        fmul FOUR               ; st(0) = 4 * B
        fstp fourMultiplyB      ; fourMultiplyB = 4 * B => pop

        fld A                   ; st(0) = A
        fmul A                  ; st(0) = A * A
        fmul THREE              ; st(0) = 3 * A * A

        fadd fourMultiplyB      ; st(0) = 3 * A ^ 2 + 4 * B

        fstp arcCosecValue      ; arcCosecValue = 3 * A ^ 2 + 4 * B => pop
        
        fld ONE                 ; st(0) = 1
        fdiv arcCosecValue      ; st(0) = 1 / arcCosecValue

        fld st                  ; arcSin operation
        fld st
        fld1
        fsubr st(2), st
        fadd
        fmul
        fsqrt
        fpatan                   ; arcSin(1 / arcCosecValue) = arcCosec(arcCosecValue)

        fstp resultInRadians     ; resultInRadians = arcSec(1 / arcCosecValue) = > pop

        fld ONEHUNDREEDEIGHTEEN  ; st(0) = 180
        fdiv PI                  ; st(0) = 180 / PI
        fmul resultInRadians     ; st(0) = resultInRadians * 180/PI => degrees
        
        fmul ONEANDHALF          ; st(0) = 0.5 * st(0)

        fstp resultInDegrees     ; resultInDegrees = st(0) => pop

        fld resultInRadians
        fmul ONEANDHALF          ; st(0) = 0.5 * resultInRadians
        fstp resultInRadians     ; resultInRadians = 0.5 * resultInRadians = > pop
    }

    std::cout << "Result in radians: " << resultInRadians << std::endl
        << "Result in degrees: " << resultInDegrees << std::endl;
}

// Y = 6 * lg(cos(x))                          ; x = 8 => x += 5(degree) => 7 values of Y
void Task4() {
    double x = 8, cosResult = 0, firstLog, secondLog;
    const double ONEHUNDREEDEIGHTEEN = 180, PI = 3.14159, SIX = 6, FIVE = 5, TEN = 10;

    double results[8];

    _asm
    {
        finit                       ; inits FPU

        mov ecx, 7
    iteration:

        ; Convert radiant to degree => PI * x / 180

        fld x                       ; st(0) = x
        fdiv ONEHUNDREEDEIGHTEEN    ; st(0) = x / 180
        fmul PI                     ; st(0) = PI * x / 180

        fcos                        ; st(0) = cos(x)

        fstp cosResult              ; cosResult = cos(x)

        ; lg (cos(x)) =>  operation implementing

        fld1                        ; push 1 from the FPU stack
        fld cosResult               ; push result from the FPU stack
        fyl2x                       ; 1 * log2(result)
        fstp firstLog               ; first_log = log2(result)

        FLDL2T                      ; push log2(10) from the FPU stack

        fld firstLog                ; push first_log from the FPU stack

        fdiv st(0), st(1)           ; first_log / log2(10)

        fmul SIX                    ; 6 * lg(cos(x))

        fstp results[ecx * 8]

        ; x += 5

        fld x                       
        fadd FIVE
        fstp x

        loop iteration
    }

    for (int i = 7; i >= 1; --i) {
        std::cout << "Result " << i << ": " << results[i] << std::endl;
    }
}

int main()
{
    std::cout << "Task 1: " << std::endl;
    Task1();

    std::cout << std::endl << "Task 2: " << std::endl;
    Task2();

    std::cout << std::endl << "Task 3: " << std::endl;
    Task3();

    std::cout << std::endl << "Task 4: " << std::endl;
    Task4();

    return 0;
}