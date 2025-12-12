#include "Clock.hpp"
#include <math.h>
#include <complex>
#include <cstring>
#include <assert.h>
#include <numbers>

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
std::complex<double> complex_exp(unsigned int i, unsigned int n)
{
  // NOTE: this had to be changed to use std::numbers::pi to compile since M_PI did not work for me
  return std::complex<double>{cos(-2 * std::numbers::pi * i / n), sin(-2 * std::numbers::pi * i / n)};
}

void fft_naive(std::complex<double> *vec, const unsigned int n)
{
  std::complex<double> *result = new std::complex<double>[n];
  if (n == 1)
    result[0] = vec[0];
  else
  {
    std::complex<double> *evens = new std::complex<double>[n / 2];
    std::complex<double> *odds = new std::complex<double>[n / 2];
    for (unsigned int i = 0; i < n / 2; ++i)
    {
      evens[i] = vec[2 * i];
      odds[i] = vec[2 * i + 1];
    }
    fft_naive(evens, n / 2);
    fft_naive(odds, n / 2);

    for (unsigned int i = 0; i < n / 2; ++i)
      result[i] = evens[i] + complex_exp(i, n) * odds[i];
    for (unsigned int i = n / 2; i < n; ++i)
      result[i] = evens[i - n / 2] + complex_exp(i, n) * odds[i - n / 2];

    delete[] evens;
    delete[] odds;
  }
  std::copy(result, result + n, vec);
  delete[] result;
}
// END NOTE

// fixme: put any helper functions, classes, etc. that you need right here:

__attribute__((always_inline)) inline void bit_reverse_and_shuffle(std::complex<double> *vec, const unsigned int log_n)
{
  const unsigned int N = 1ul << log_n;

  unsigned int rev = 0;
  for (unsigned int i = 1; i < N; ++i)
  {
    // Compute the next rev from the previous rev
    // Find the rightmost 0 bit in i-1 and flip it and all bits to its right.
    unsigned int bit = N >> 1;
    while (rev & bit)
    {
      rev ^= bit; // flip the bit from 1 to 0
      bit >>= 1;
    }
    rev |= bit; // flip the rightmost 0 to 1

    // Only swap if i hasn't been swapped before
    if (i < rev)
    {
      std::swap(vec[i], vec[rev]);
    }
  }
}

__attribute__((always_inline)) inline void fft_iterative(std::complex<double> *vec, const unsigned int log_n)
{
  const unsigned int N = 1ul << log_n;

  // Loop for each power of 2
  for (unsigned int s = 1; s <= log_n; ++s)
  {
    const unsigned int current_size = 1ul << s;
    const unsigned int size_divided_by_two = current_size >> 1;

    // Calculate main twiddle factor
    const double theta = -2.0 * std::numbers::pi / static_cast<double>(current_size);
    double sin_theta = sin(theta);
    double sin_half_theta = sin(theta / 2.0);
    double cos_val_minus_one = -2.0 * sin_half_theta * sin_half_theta;
    const std::complex<double> first_twiddle_minus_one{cos_val_minus_one, sin_theta};
    const std::complex<double> twiddle_factor(cos(theta), sin(theta)); // e^−2πi/m

    for (unsigned int k = 0; k < N; k += current_size)
    {
      std::complex<double> twiddle(1.0, 0.0);
#include "Clock.hpp"
#include <math.h>
#include <complex>
#include <cstring>
#include <assert.h>
#include <numbers>

      // NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
      std::complex<double> complex_exp(unsigned int i, unsigned int n)
      {
        // NOTE: Add to change M_PI to std::numbers::pi for it to run on my computer
        return std::complex<double>{cos(-2 * std::numbers::pi * i / n), sin(-2 * std::numbers::pi * i / n)};
      }

      void fft_naive(std::complex<double> * vec, const unsigned int n)
      {
        std::complex<double> *result = new std::complex<double>[n];
        if (n == 1)
          result[0] = vec[0];
        else
        {
          std::complex<double> *evens = new std::complex<double>[n / 2];
          std::complex<double> *odds = new std::complex<double>[n / 2];
          for (unsigned int i = 0; i < n / 2; ++i)
          {
            evens[i] = vec[2 * i];
            odds[i] = vec[2 * i + 1];
          }
          fft_naive(evens, n / 2);
          fft_naive(odds, n / 2);

          for (unsigned int i = 0; i < n / 2; ++i)
            result[i] = evens[i] + complex_exp(i, n) * odds[i];
          for (unsigned int i = n / 2; i < n; ++i)
            result[i] = evens[i - n / 2] + complex_exp(i, n) * odds[i - n / 2];

          delete[] evens;
          delete[] odds;
        }
        std::copy(result, result + n, vec);
        delete[] result;
      }
      // END NOTE

      // fixme: put any helper functions, classes, etc. that you need right here:
      unsigned long bit_reversed(const unsigned long i, const unsigned int num_bits)
      {
        unsigned long result = 0;
        for (unsigned int b = 0; b < num_bits; ++b)
        {
          if (i & (1ul << b))
            result |= 1ul << (num_bits - 1 - b);
        }
        return result;
      }

      void shuffle(std::complex<double> *__restrict vec, const unsigned int log_n)
      {
        const unsigned long N = 1ul << log_n;
        for (unsigned long i = 0; i < N; ++i)
        {
          if (const unsigned long rev_i = bit_reversed(i, log_n); i < rev_i)
          {
            std::swap(vec[i], vec[rev_i]);
          }
        }
      }
      template <unsigned int LOG_N>
      class FFTThreshold
      {
      public:
        static constexpr unsigned int THRESHOLD = (LOG_N > 24) ? LOG_N - 8 : 16;
      };

      template <unsigned int LOG_N>
      class FFTHelper
      {
      public:
        static void apply(std::complex<double> *__restrict vec)
        {
          constexpr unsigned long long N = 1ull << LOG_N;

          if constexpr (LOG_N > FFTThreshold<LOG_N>::THRESHOLD)
          {
#pragma omp task
            FFTHelper<LOG_N - 1>::apply(vec);

#pragma omp task
            FFTHelper<LOG_N - 1>::apply(vec + N / 2);

#pragma omp taskwait
          }
          else
          {
            FFTHelper<LOG_N - 1>::apply(vec);
            FFTHelper<LOG_N - 1>::apply(vec + N / 2);
          }

          constexpr double theta = -2.0 * std::numbers::pi / N;
          const double sin_val = sin(theta);
          const double sin_half_angle = sin(theta * 0.5);
          const double cos_val_minus_1 = -2.0 * sin_half_angle * sin_half_angle;

          const std::complex<double> first_twiddle_minus_1(cos_val_minus_1, sin_val);

          std::complex<double> twiddle(1.0, 0.0);

          for (unsigned int i = 0; i < N / 2; ++i)
          {
            const std::complex<double> result_at_i = vec[i] + twiddle * vec[i + N / 2];
            const std::complex<double> result_at_i_plus_n_over_2 = vec[i] - twiddle * vec[i + N / 2];

            vec[i] = result_at_i;
            vec[i + N / 2] = result_at_i_plus_n_over_2;

            twiddle = twiddle * first_twiddle_minus_1 + twiddle;
          }
        }
      };

      template <>
      class FFTHelper<0u>
      {
      public:
        static void apply(std::complex<double> *__restrict vec)
        {
          // Do nothing
        }
      };

      template <unsigned int LOG_N>
      class FFT
      {
      public:
        static void apply(std::complex<double> *__restrict dest)
        {
          shuffle(dest, LOG_N);
          FFTHelper<LOG_N>::apply(dest);
        }
      };

      __attribute__((noinline)) void fft(std::complex<double> * vec, const unsigned long n)
      {
        // fft_naive(vec, n);
        unsigned long log_n = 0;
        while ((1ul << log_n) < n)
        {
          ++log_n;
        }
        // std::cout << n << std::endl;
        // std::cout << log_n << std::endl;

        // This looks horrible but works, would clean it up if I had more time, but I need to go to bed
        switch (log_n)
        {
        case 16:
          FFT<16>::apply(vec);
          break;
        case 17:
          FFT<17>::apply(vec);
          break;
        case 18:
          FFT<18>::apply(vec);
          break;
        case 19:
          FFT<19>::apply(vec);
          break;
        case 20:
          FFT<20>::apply(vec);
          break;
        case 21:
          FFT<21>::apply(vec);
          break;
        case 22:
          FFT<22>::apply(vec);
          break;
        case 23:
          FFT<23>::apply(vec);
          break;
        case 24:
          FFT<24>::apply(vec);
          break;
        case 25:
          FFT<25>::apply(vec);
          break;
        case 26:
          FFT<26>::apply(vec);
          break;
        case 27:
          FFT<27>::apply(vec);
          break;
        case 28:
          FFT<28>::apply(vec);
          break;
        case 29:
          FFT<29>::apply(vec);
          break;
        case 30:
          FFT<30>::apply(vec);
          break;
        case 31:
          FFT<31>::apply(vec);
          break;
        case 32:
          FFT<32>::apply(vec);
          break;
        case 33:
          FFT<33>::apply(vec);
          break;
        case 34:
          FFT<34>::apply(vec);
          break;
        case 35:
          FFT<35>::apply(vec);
          break;
        case 36:
          FFT<36>::apply(vec);
          break;
        case 37:
          FFT<37>::apply(vec);
          break;
        case 38:
          FFT<38>::apply(vec);
          break;
        case 39:
          FFT<39>::apply(vec);
          break;
        case 40:
          FFT<40>::apply(vec);
          break;
        case 41:
          FFT<41>::apply(vec);
          break;
        case 42:
          FFT<42>::apply(vec);
          break;
        case 43:
          FFT<43>::apply(vec);
          break;
        case 44:
          FFT<44>::apply(vec);
          break;
        case 45:
          FFT<45>::apply(vec);
          break;
        case 46:
          FFT<46>::apply(vec);
          break;
        case 47:
          FFT<47>::apply(vec);
          break;
        case 48:
          FFT<48>::apply(vec);
          break;
        case 49:
          FFT<49>::apply(vec);
          break;
        case 50:
          FFT<50>::apply(vec);
          break;
        case 51:
          FFT<51>::apply(vec);
          break;
        case 52:
          FFT<52>::apply(vec);
          break;
        case 53:
          FFT<53>::apply(vec);
          break;
        case 54:
          FFT<54>::apply(vec);
          break;
        case 55:
          FFT<55>::apply(vec);
          break;
        case 56:
          FFT<56>::apply(vec);
          break;
        case 57:
          FFT<57>::apply(vec);
          break;
        case 58:
          FFT<58>::apply(vec);
          break;
        case 59:
          FFT<59>::apply(vec);
          break;
        case 60:
          FFT<60>::apply(vec);
          break;
        case 61:
          FFT<61>::apply(vec);
          break;
        case 62:
          FFT<62>::apply(vec);
          break;
        case 63:
          FFT<63>::apply(vec);
          break;
        default:
          std::cout << "Error" << std::endl;
        }
      }

      // NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
      int main(int argc, char **argv)
      {
        if (argc != 3)
          std::cerr << "usage: <log_problem_size> <seed>" << std::endl;
        else
        {
          const unsigned log_n = atoi(argv[1]);

          assert(log_n >= 16 && log_n <= 63);

          const unsigned long n = 1ul << log_n;
          const unsigned seed = atoi(argv[2]);
          srand(seed);

          std::complex<double> *vec = new std::complex<double>[n];
          std::complex<double> *vec_copy = new std::complex<double>[n];

          constexpr int MAX_MAGNITUDE_PLUS_ONE = 1024;
          auto rand_real = [](const double magnitude)
          {
            constexpr unsigned MAX_RANDOM = 9999999;
            double result = (rand() % (MAX_RANDOM + 1)) / double(MAX_RANDOM);
            result -= 0.5;
            result *= 2 * magnitude;
            return result;
          };

          for (unsigned long i = 0; i < n; ++i)
            vec_copy[i] = vec[i] = std::complex<double>{rand_real(rand() % MAX_MAGNITUDE_PLUS_ONE), 0.};

          Clock c;
          fft(vec, n);
          double mult_time = c.tock();
          c.ptock();

          c.tick();
          fft_naive(vec_copy, n);
          double naive_mult_time = c.tock();

          double max_l1_error = 0.0;
          std::cout << "ref " << vec[0] << " " << vec[1] << " " << vec[2] << "..." << std::endl;
          std::cout << "you " << vec_copy[0] << " " << vec_copy[1] << " " << vec_copy[2] << "..." << std::endl;
          for (unsigned long i = 0; i < n; ++i)
          {
            std::complex<double> difference = vec[i] - vec_copy[i];
            double error_magnitude = sqrt((difference * conj(difference)).real());

            max_l1_error = std::max(max_l1_error, error_magnitude);
          }
          std::cout << "Numeric error: " << max_l1_error << std::endl;

          double speedup = naive_mult_time / mult_time;
          std::cout << "Speedup: " << speedup << std::endl;

          std::cout << std::endl;

          // Verify numeric error:
          bool pass = true;
          if (max_l1_error >= 1e-6)
          {
            std::cerr << "FAIL: Numeric error is too high" << std::endl;
            pass = false;
          }
          else
            std::cout << "PASS: Numeric error" << std::endl;

          // Verify speedup:
          if (speedup < 15)
          {
            std::cerr << "FAIL: Speedup is too low" << std::endl;
            pass = false;
          }
          else
            std::cout << "PASS: Speedup" << std::endl;

          if (pass)
            std::cout << "OVERALL PASS" << std::endl;
          else
          {
            std::cout << "OVERALL FAIL" << std::endl;
            exit(1);
          }
        }

        return 0;
      }

      for (unsigned int j = 0; j < size_divided_by_two; ++j)
      {
        // t = twiddle * odd
        const std::complex<double> t = twiddle * vec[k + j + size_divided_by_two];
        // u = even
        const std::complex<double> u = vec[k + j];

        vec[k + j] = u + t;
        vec[k + j + size_divided_by_two] = u - t;

        twiddle += twiddle * first_twiddle_minus_one;
      }
    }
  }
}

__attribute__((noinline)) void fft(std::complex<double> *vec, const unsigned int n)
{
  // Right now, this code is very fast but the error is too high for large inputs.
  unsigned int log_n = 0;
  while ((1ul << log_n) < n)
  {
    ++log_n;
  }

  bit_reverse_and_shuffle(vec, log_n);
  fft_iterative(vec, log_n);
}

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
int main(int argc, char **argv)
{
  if (argc != 3)
    std::cerr << "usage: <log_problem_size> <seed>" << std::endl;
  else
  {
    const unsigned log_n = atoi(argv[1]);

    assert(log_n >= 16 && log_n <= 63);

    const unsigned int n = 1ul << log_n;
    const unsigned seed = atoi(argv[2]);
    srand(seed);

    std::complex<double> *vec = new std::complex<double>[n];
    std::complex<double> *vec_copy = new std::complex<double>[n];

    constexpr int MAX_MAGNITUDE_PLUS_ONE = 1024;
    auto rand_real = [](const double magnitude)
    {
      constexpr unsigned MAX_RANDOM = 9999999;
      double result = (rand() % (MAX_RANDOM + 1)) / double(MAX_RANDOM);
      result -= 0.5;
      result *= 2 * magnitude;
      return result;
    };

    for (unsigned int i = 0; i < n; ++i)
      vec_copy[i] = vec[i] = std::complex<double>{rand_real(rand() % MAX_MAGNITUDE_PLUS_ONE), 0.};

    Clock c;
    fft(vec, n);
    double mult_time = c.tock();
    c.ptock();

    c.tick();
    fft_naive(vec_copy, n);
    double naive_mult_time = c.tock();

    double max_l1_error = 0.0;
    std::cout << "ref " << vec[0] << " " << vec[1] << " " << vec[2] << "..." << std::endl;
    std::cout << "you " << vec_copy[0] << " " << vec_copy[1] << " " << vec_copy[2] << "..." << std::endl;
    for (unsigned int i = 0; i < n; ++i)
    {
      std::complex<double> difference = vec[i] - vec_copy[i];
      double error_magnitude = sqrt((difference * conj(difference)).real());

      max_l1_error = std::max(max_l1_error, error_magnitude);
    }
    std::cout << "Numeric error: " << max_l1_error << std::endl;

    double speedup = naive_mult_time / mult_time;
    std::cout << "Speedup: " << speedup << std::endl;

    std::cout << std::endl;

    // Verify numeric error:
    bool pass = true;
    if (max_l1_error >= 1e-6)
    {
      std::cerr << "FAIL: Numeric error is too high" << std::endl;
      pass = false;
    }
    else
      std::cout << "PASS: Numeric error" << std::endl;

    // Verify speedup:
    if (speedup < 15)
    {
      std::cerr << "FAIL: Speedup is too low" << std::endl;
      pass = false;
    }
    else
      std::cout << "PASS: Speedup" << std::endl;

    if (pass)
      std::cout << "OVERALL PASS" << std::endl;
    else
    {
      std::cout << "OVERALL FAIL" << std::endl;
      exit(1);
    }
  }

  return 0;
}