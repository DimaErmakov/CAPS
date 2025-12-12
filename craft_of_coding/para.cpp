#include <math.h>
#include "Clock.hpp"

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
__attribute__((noinline))
void naive_mult(double*__restrict C, double*__restrict B, double*__restrict A,
const unsigned int N) {
    for (unsigned int i=0; i<N; ++i)
        for (unsigned int j=0; j<N; ++j) {
            C[i*N+j] = 0.0;
            for (unsigned int k=0; k<N; ++k)
                C[i*N+j] += A[i*N+k]*B[k*N+j];
        }
}

// END NOTE

// fixme: put any helper functions, classes, etc. that you need up here.

void base_case (float* C, float* B, float* A, const unsigned int N){
    for (unsigned int i = 0; i < N*N ; ++i)
            C[i] = 0.0;
    for (unsigned int i = 0; i < N; ++i) {
        unsigned i_n = i * N;
        for (unsigned int k = 0; k < N; ++k) {
            const float a_i_k = A[i_n + k];
            for (unsigned int j = 0; j < N; ++j) {
                const float b_k_j = B[k*N + j];
                C[i_n + j] += a_i_k * b_k_j;
            }
        }
    }
}

void add(float*C, const float* A, const float* B, unsigned N) {
    for(unsigned int i = 0; i < N*N; ++i)
            C[i] = A[i] + B[i];
        }
void sub(float*C, const float* A, const float* B, unsigned N) {
    for(unsigned int i = 0; i < N*N; ++i)
            C[i] = A[i] - B[i];
        }
void split_matrix(float* M11, float* M12, float* M21, float* M22, const float* M, unsigned N) {
    unsigned S = N/2;
    for (unsigned int i = 0; i < S; ++i)
        for (unsigned int j = 0; j < S; ++j) {
            M11[i*S + j] = M[i*N + j];
            M12[i*S + j] = M[i*N + j + S];
            M21[i*S + j] = M[(i+S)*N + j];
            M22[i*S + j] = M[(i+S)*N + j + S];
        }
}
//fixme

float* allocate_mem(unsigned long int N) {
    unsigned long mem_half = N / 2;
    unsigned long new_mem = 17 * mem_half * mem_half;
    float* mem = new float[new_mem];
    return mem;
}
void strassen(float* C, float* B, float* A, float* mem, const unsigned int N) {
    constexpr int Block_size = 128;
    if (N < Block_size) {
        base_case(C, B, A, N);
        return;
    }

    unsigned S = N / 2;
    unsigned S2 = S * S;
    float* ptr = mem;
    // Allocate slices from the memory block
    float* A11 = ptr; ptr += S2;
    float* A12 = ptr; ptr += S2;
    float* A21 = ptr; ptr += S2;
    float* A22 = ptr; ptr += S2;
    float* B11 = ptr; ptr += S2;
    float* B12 = ptr; ptr += S2;
    float* B21 = ptr; ptr += S2;
    float* B22 = ptr; ptr += S2;
    float* M1 = ptr; ptr += S2;
    float* M2 = ptr; ptr += S2;
    float* M3 = ptr; ptr += S2;
    float* M4 = ptr; ptr += S2;
    float* M5 = ptr; ptr += S2;
    float* M6 = ptr; ptr += S2;
    float* M7 = ptr; ptr += S2;
    float* T1 = ptr; ptr += S2;
    float* T2 = ptr; // last slice
    // Split matrices into blocks
#pragma omp parallel sections
    {
#pragma omp section
        split_matrix(A11, A12, A21, A22, A, N);
#pragma omp section
        split_matrix(B11, B12, B21, B22, B, N);
    }

    // M1 = (A11 + A22) * (B11 + B22)
    // M2 = (A21 + A22) * B11
    // M3 = A11 * (B12 - B22)
    // M4 = A22 * (B21 - B11)
    // M5 = (A11 + A12) * B22
    // M6 = (A21 - A11) * (B11 + B12)
    // Assemble C subblocks
#pragma omp parallel sections
    {
#pragma omp section
        add(T1, A11, A22, S);
        add(T2, B11, B22, S);
        strassen(M1, T2, T1, mem, S);
#pragma omp section
        add(T1, A21, A22, S);
        strassen(M2, B11, T1, mem, S);
#pragma omp section
        sub(T2, B12, B22, S);
        strassen(M3, B11, T1, mem, S);
#pragma omp section
        sub(T2, B21, B11, S);
        strassen(M4, B11, T1, mem, S);
#pragma omp section
        add(T1, A11, A12, S);
        strassen(M5, B11, T1, mem, S);
#pragma omp section
        sub(T1, A21, A11, S);
        add(T2, B11, B12, S);
        strassen(M6, B11, T1, mem, S);
#pragma omp section
        sub(T1, A12, A22, S);
        add(T2, B21, B22, S);
        strassen(M7, B11, T1, mem, S);
}
    for (unsigned int i = 0; i < S; ++i) {
        for (unsigned int j = 0; j < S; ++j) {
            unsigned int idx = i * S + j;
            float c11 = M1[idx] + M4[idx] - M5[idx] + M7[idx];
            float c12 = M3[idx] + M5[idx];
            float c21 = M2[idx] + M4[idx];
            float c22 = M1[idx] - M2[idx] + M3[idx] + M6[idx];
            C[i*N + j] = c11;
            C[i*N + j + S] = c12;
            C[(i + S)*N + j] = c21;
            C[(i + S)*N + j + S] = c22;
        }
    }
}





__attribute__((noinline))
void mult(double* C, double* B, double* A, const unsigned int N) {
    // change name here
    // allocate float buffers
    float* Af = new float[N*N];
    float* Bf = new float[N*N];
    float* Cf = new float[N*N];
    float* mem = allocate_mem(N);   // your existing function (still float)
    // cast A,B from double → float

    for (unsigned long i=0; i<N*N; ++i) {
        Af[i] = static_cast<float>(A[i]);
        Bf[i] = static_cast<float>(B[i]);
    }
    // run your existing float Strassen
    strassen(Cf, Bf, Af, mem, N);

    // cast the result back float → double
    for (unsigned long i=0; i<N*N; ++i) {
        C[i] = static_cast<double>(Cf[i]);
    }

    // cleanup
    delete[] Af;
    delete[] Bf;
    delete[] Cf;
    delete[] mem;
}


// fixme: replace the following line with your own multiply code

// (you may declare any other functions, classes, etc. that you need above):
// end fixme






// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:

int main(int argc, char**argv) {
    if (argc != 3)
        std::cerr << "usage: <log_problem_size> <seed>" << std::endl;
    else {
        const unsigned LOG_N=atoi(argv[1]);
        const unsigned long N = 1ul<<LOG_N;
        const unsigned seed=atoi(argv[2]);
        srand(seed);
        double*A=new double[N*N];
        double*B=new double[N*N];
        for (unsigned long i=0; i<N*N; ++i)
            A[i] = (rand() % 1000)/999.0;
        for (unsigned long i=0; i<N*N; ++i)
            B[i] = (rand() % 1000)/999.0;
        double*C=new double[N*N];
        Clock c;
        mult(C,B,A,N);
        double mult_time = c.tock();
        c.ptock();
        double*C_naive=new double[N*N];
        c.tick();
        naive_mult(C_naive,B,A,N);
        double naive_mult_time = c.tock();
        double max_l1_error = 0.0;
        for (unsigned long i=0; i<N*N; ++i)
            max_l1_error = std::max(max_l1_error, fabs(C_naive[i] - C[i]));
        std::cout << "Numeric error: " << max_l1_error << std::endl;
        double speedup = naive_mult_time / mult_time;
        std::cout << "Speedup: " << speedup << std::endl;
        std::cout << std::endl;
        // Verify numeric error:
        bool pass = true;
        if (max_l1_error >= 1e-4) {
            std::cerr << "FAIL: Numeric error is too high" << std::endl;
            pass = false;
        }
        else
            std::cout << "PASS: Numeric error" << std::endl;
        // Verify speedup:
        if (speedup < 5) {
            std:: cerr << "FAIL: Speedup is too low" << std::endl;
            pass = false;
        }
        else
            std::cout << "PASS: Speedup" << std::endl;
        if (pass)
            std::cout << "OVERALL PASS" << std::endl;
        else {
            std::cout << "OVERALL FAIL" << std::endl;
            exit(1);
        }
    }
    return 0;
}