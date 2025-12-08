#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include <algorithm> 
#include <omp.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

//Morphological operations
const int REPETITIONS = 5;
void apply_erosion_parallel(const unsigned char* input_image, unsigned char* output_image, int width, int height, int kernel_size);
void apply_dilation_parallel(const unsigned char* input_image, unsigned char* output_image, int width, int height, int kernel_size);


int main() {
    const char* filename = "4096J.png";//pointer to datasets file name
    int width, height, channels;
    unsigned char* input_image = stbi_load(filename, &width, &height, &channels, 1);//we charge our dataset in the variable input_image,as a grey scale image(1 channel)

    if (input_image == nullptr) {//if pointer is pointing nowhere,there is an error in the image charging steps we did before
        cerr << "Error:The image couldn't be found'" << filename << "'." << endl;
        return 1;
    }

    cout << "Image charged succesfully: " << width << "x" << height << " pixels." << endl;

    //We prepare memory
    size_t img_size = (size_t)width * height;//image length,width x height pixels
    unsigned char* output_image = new unsigned char[img_size];//pointer to the pixels array where we will fill it with the output image

    //To control kernel size
    int kernel_size = 30;

    //Thread Configuration
    omp_set_num_threads(8);//to set the current number of threads
#pragma omp parallel//to "wake the threads up"
    {
        if (omp_get_thread_num() == 0) {
            cout << ">> The program is being runned by " << omp_get_num_threads() << " threads" << endl;
        }
    }

    //Execution and measuring
    double total_time = 0;

    for (int rep = 0; rep < REPETITIONS; ++rep) {
        double start_time = omp_get_wtime(); //Measuring starts

        apply_erosion_parallel(input_image, output_image, width, height, kernel_size);

        double end_time = omp_get_wtime(); //Measuring finishes
        total_time += (end_time - start_time); //We accumulate the time for each iteration in order to make an average
    }

    double parallel_time = total_time / REPETITIONS; //It calculates the average time 

    //Results
    std::cout << "\n=================================================" << endl;
    std::cout << "Parallel average time: " << parallel_time << " seconds" << std::endl;
    std::cout << "=================================================" << std::endl;

    //We save our result
    stbi_write_png("ProcessedImage.png", width, height, 1, output_image, width);//we save the output image which we can find in the project folder named as ProcessedImage
    cout << "Imaged processed succesfully" << endl;

    //Cleaning
    stbi_image_free(input_image);//we need to free memory we reserved previously when we charged the input image
    delete[] output_image;//free memory reserved previously for the ouput data

    return 0;
}

//Parallel functions

void apply_erosion_parallel(const unsigned char* input_image, unsigned char* output_image, int width, int height, int kernel_size) {
    int radius = kernel_size / 2;
    int y, x, ky, kx;
    unsigned char min_val;

#pragma omp parallel for schedule(static)  default(none) shared(input_image, output_image, width, height, radius) private(x, y, ky, kx, min_val)

    for (y = radius; y < height - radius; ++y) {

        for (x = radius; x < width - radius; ++x) {
            min_val = 255;

            for (ky = -radius; ky <= radius; ++ky) {
                for (kx = -radius; kx <= radius; ++kx) {
                    size_t idx = (size_t)(y + ky) * width + (x + kx);
                    unsigned char val = input_image[idx];
                    if (val < min_val) min_val = val;
                }
            }
            size_t output_index = (size_t)y * width + x;
            output_image[output_index] = min_val;
        }
    }
}


void apply_dilation_parallel(const unsigned char* input_image, unsigned char* output_image, int width, int height, int kernel_size) {
    int radius = kernel_size / 2;
    int y, x, ky, kx;
    unsigned char max_val;

#pragma omp parallel for schedule(guided)  default(none)shared(input_image, output_image, width, height, radius) private(x, y, ky, kx, max_val)

    for (y = radius; y < height - radius; ++y) {
        for (x = radius; x < width - radius; ++x) {
            max_val = 0;

            for (ky = -radius; ky <= radius; ++ky) {
                for (kx = -radius; kx <= radius; ++kx) {
                    size_t idx = (size_t)(y + ky) * width + (x + kx);
                    unsigned char val = input_image[idx];
                    if (val > max_val) max_val = val;
                }
            }
            size_t output_index = (size_t)y * width + x;
            output_image[output_index] = max_val;
        }
    }
}