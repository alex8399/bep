#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <CL/opencl.hpp>
#include <iostream>
#include <vector>

int main()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    std::cout << "Platforms found: " << platforms.size() << "\n\n";

    for (size_t i = 0; i < platforms.size(); ++i) {
        std::cout << "Platform [" << i << "]\n";
        std::cout << "  Name:    " << platforms[i].getInfo<CL_PLATFORM_NAME>() << "\n";
        std::cout << "  Vendor:  " << platforms[i].getInfo<CL_PLATFORM_VENDOR>() << "\n";
        std::cout << "  Version: " << platforms[i].getInfo<CL_PLATFORM_VERSION>() << "\n";

        std::vector<cl::Device> devices;
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

        for (size_t j = 0; j < devices.size(); ++j) {
            std::cout << "  Device [" << j << "]\n";
            std::cout << "    Name:           " << devices[j].getInfo<CL_DEVICE_NAME>() << "\n";
            std::cout << "    Type:           " << devices[j].getInfo<CL_DEVICE_TYPE>() << "\n";
            std::cout << "    GPU:           " << (devices[j].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU) << "\n";
            std::cout << "    CPU:           " << (devices[j].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU) << "\n";
            std::cout << "    Compute units:  " << devices[j].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
            std::cout << "    Global memory:  " << (devices[j].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() >> 20) << " MB\n";
            std::cout << "    Max work group: " << devices[j].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n";
            std::cout << "    OpenCL version: " << devices[j].getInfo<CL_DEVICE_VERSION>() << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}