// OUTPUT
// [Running] cd "/Users/liaoziyu/Desktop/课程文件/大三/Amath 483/assignment/assignment 2/" && g++ homework_2.cpp -o homework_2 && "/Users/liaoziyu/Desktop/课程文件/大三/Amath 483/assignment/assignment 2/"homework_2
// Problem 1:
// Single precision machine epsilon (float, 32-bit) = 1.192092895507812e-07
// Double precision machine epsilon (double, 64-bit) = 2.220446049250313e-16
// Problem 3:
// Mutiply Result: -884901888
// Check Value for Problem 2:
// ---- FP 32 ----
// Largest:3.402823466385289e+38
// Normalized Smallest:1.175494350822288e-38
// Denormalized Smallest:1.401298464324817e-45
// ---- FP 64 ----
// Largest:1.797693134862316e+308
// Normalized Smallest:2.225073858507201e-308
// Denormalized Smallest:4.940656458412465e-324
// Check Value for Problem 4:
// Final Value of counter:4294967293

// [Done] exited with code=0 in 0.387 seconds

#include <iostream>
#include <iomanip>
#include <limits>

int main() 
{
    // Problem 1
    std::cout << "Problem 1:" << std::endl;
    // float, 32-bit
    int j_float = 0;
    float epsilon_float;
    while (true) 
    {
        float test_val = 1.0f + 1.0f / (1u << j_float);
        if (test_val == 1.0f) 
        {
            break;
        }
        j_float++;
    }
    epsilon_float = 1.0f / (1u << (j_float - 1));
    std::cout << "Single precision machine epsilon (float, 32-bit) = "
        << std::setprecision(16) << epsilon_float << std::endl;

    // double, 64-bit
    int j_double = 0;
    double epsilon_double;
    while (true) 
    {
        double test_val = 1.0 + 1.0 / (1ULL << j_double);
        if (test_val == 1.0) 
        {
            break;
        }
        j_double++;
    }
    epsilon_double = 1.0 / (1ULL << (j_double - 1));
    std::cout << "Double precision machine epsilon (double, 64-bit) = "
        << std::setprecision(16) << epsilon_double << std::endl;
    
    // Problem 3
    std::cout << "Problem 3:" << std::endl;
    int result = 200 * 300 * 400;
    std::cout << "Mutiply Result: " << result * 500 << std::endl; // avoid "overflow" warning

    // Check Value for Problem 2
    std::cout << "Check Value for Problem 2:" << std::endl;
    std::cout << "---- FP 32 ----" << std::endl;
    std::cout << "Largest:" << std::numeric_limits<float>::max() << std::endl;
    std::cout << "Normalized Smallest:" << std::numeric_limits<float>::min() << std::endl;
    std::cout << "Denormalized Smallest:" << std::numeric_limits<float>::denorm_min() << std::endl;

    std::cout << "---- FP 64 ----" << std::endl;
    std::cout << "Largest:" << std::numeric_limits<double>::max() << std::endl;
    std::cout << "Normalized Smallest:" << std::numeric_limits<double>::min() << std::endl;
    std::cout << "Denormalized Smallest:" << std::numeric_limits<double>::denorm_min() << std::endl;

    // Check Value for Problem 4
    std::cout << "Check Value for Problem 4:" << std::endl;
    unsigned int counter = 0;
    for (int i = 0; i < 3; ++i)
        --counter;
    std::cout << "Final Value of counter:" << counter << std::endl;

    return 0;
}
