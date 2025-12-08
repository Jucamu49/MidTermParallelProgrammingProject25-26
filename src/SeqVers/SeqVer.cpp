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


const int REPETITIONS = 5; // Número de repeticiones para promediar el tiempo
void apply_erosion_sequential(const unsigned char* input_image, unsigned char* output_image, int width, int height,int kernel_size);
void apply_dilation_sequential(const unsigned char* input_image, unsigned char* output_image, int width, int height,int kernel_size);

int main() {
    const char* filename = "2048Hand.png";//pointer to datasets file name
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

    //Execution and measuring
    double total_time = 0;

    for (int rep = 0; rep < REPETITIONS; ++rep) {
        double start_time = omp_get_wtime(); //We start the measuring

        apply_erosion_sequential(input_image, output_image,width,height,kernel_size);

        double end_time = omp_get_wtime(); //We end the measuring
        total_time+= (end_time - start_time); //We accumulate every iterations time
    }

    double sequential_time = total_time / REPETITIONS; //It calculates the average time

    //Results
    std::cout << "\n=================================================" <<endl;
    std::cout << "Sequential average time: " << sequential_time << " seconds" <<endl;
    std::cout << "=================================================" <<endl;

    //We save our result
    stbi_write_png("ProcessedImage.png", width, height, 1, output_image, width);//we save the output image which we can find in the project folder named as ProcessedImage
    cout << "Imaged processed succesfully" << endl;

    //Cleaning
    stbi_image_free(input_image);//we need to free memory we reserved previously when we charged the input image
    delete[] output_image;//free memory reserved previously for the ouput data

    return 0;
}

void apply_erosion_sequential(const unsigned char* input_image, unsigned char* output_image, int width, int height, int kernel_size) {

    int radius = kernel_size / 2;

    for (int y = radius; y < height - radius; ++y) {
        for (int x = radius; x < width - radius; ++x) {

            unsigned char min_val = 255;

            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {

                    size_t neighbor_index = (size_t)(y + ky) * width + (x + kx);
                    unsigned char neighbor_val = input_image[neighbor_index];

                    if (neighbor_val < min_val) {
                        min_val = neighbor_val;
                    }
                }
            }
            size_t output_index = (size_t)y * width + x;
            output_image[output_index] = min_val;
        }
    }
}

void apply_dilation_sequential(const unsigned char* input_image, unsigned char* output_image, int width, int height,int kernel_size) {
    int radius = kernel_size / 2;

    for (int y = radius; y < height - radius; ++y) {
        for (int x = radius; x < width - radius; ++x) {

           
            unsigned char max_val = 0;

        
            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {

                    size_t neighbor_index = (size_t)(y + ky) * width + (x + kx);
                    unsigned char neighbor_val = input_image[neighbor_index];

                   
                    if (neighbor_val > max_val) {
                        max_val = neighbor_val;
                    }
                }
            }

          
            size_t output_index = (size_t)y * width + x;
            output_image[output_index] = max_val;
        }
    }
}
