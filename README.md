# C++ Project – Image Processing

This repository contains two C++ programs along with the images required for testing and a PDF with the tech report.

## Repository Structure

- **src/** → C++ source code(with neccesary .h to be run)  
- **data/** → Input images to test
- **Technical report.pdf** → Tech Report 
- **README.md** → Project information and compilation instructions  

## How to Compile

### Basic compilation 

> Note: The code uses `stb_image.h` and `stb_image_write.h`, so make sure they remain in the **src/** folder.

---

## Visual Studio Build Instructions (recommended for best performance)

To run the program with optimal performance in **Visual Studio 2022**, follow these steps:

### 1. Set build mode to Release
- Go to **Build > Configuration Manager**
- Set the active configuration to **Release**

### 2. Enable OpenMP support
- Go to **Project > Properties**
- Navigate to **C/C++ > Language**
- Set **OpenMP Support** to **Yes (/openmp)**

### 3. Enable compiler optimization
- In **Project > Properties**
- Go to **C/C++ > Optimization**
- Set **Optimization** to **/O2 (Maximize Speed)**
These settings ensure that the program runs efficiently and fully benefits from parallel execution.

## Once this is done,in the sequential version:
Write the name of the image you want to test in the variable "filename",and in the main use the erosion or dilation function.
The resulting image will apear in the project folder as "ProcessedImage".png.


## In the parallel version:
Do the same as in the sequential and adjust the number of threads you want to test with by changing the number in the function omp_set_num_threads() at line 39.

> Note: The kernel size can also be changed in both versions.


Author
[Juan Carrion Muguerza]
