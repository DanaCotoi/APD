#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Wingdi.h>
using namespace std;

#define WINDOW_SIZE  3
#define WINDOW_LENGTH 9

__global__ void median_filter(unsigned short *Input_ImageData, int Image_Width,int Image_Height)
{
    const int row = blockDim.x * blockIdx.x + threadIdx.x;
    const int col = blockDim.y * blockIdx.y + threadIdx.y;

    unsigned char Window[WINDOW_LENGTH] = {0,0,0,0,0,0,0,0,0};
    unsigned char *Output_ImageData;

    //parcurgerea matricei de pixeli
    for(int r = row; r < Image_Height; r++)
    {
        for(int c = col; c < Image_Width; c++)
        {
            //calcularea val pixelilor
            for (int i = 0; i < WINDOW_SIZE; i++)
            {
                for (int j = 0; j < WINDOW_SIZE; j++)
                {
                    Window[i*WINDOW_SIZE+j] = Input_ImageData[(r+i-1)*Image_Width+(c+j-1)];
                }
            }

            //sortarea pixelilor
            for (int x = 0; x < 9; x++)
            {
                for (int y = x + 1; y < 9; y++)
                {
                    if (Window[x] > Window[y])
                    {
                        //Swap the variables.
                        char tmp = Window[x];
                        Window[x] = Window[y];
                        Window[y] = tmp;
                    }
                }
            }

            //stocarea pixelilor calculati in vectorul output de pixeli
            Output_ImageData[row*Image_Width+col] = Window[4];
        }

        __syncthreads();
    }

    return *Output_ImageData;
}


int Modify_Image(const char* location)
{
    unsigned char* datBuff[2] = {nullptr, nullptr}; // Header buffers

    unsigned char* pixels = nullptr; // Pixels

    BITMAPFILEHEADER* bmpHeader = nullptr; // Header
    BITMAPINFOHEADER* bmpInfo   = nullptr; // Info


    //LOAD BMP IMAGE
    std::ifstream file(location, std::ios::binary);
    if(!file)
    {
        std::cout << "Failure to open bitmap file.\n";

        return 1;
    }

    // Allocate byte memory that will hold the two headers
    datBuff[0] = new unsigned char[sizeof(BITMAPFILEHEADER)];
    datBuff[1] = new unsigned char[sizeof(BITMAPINFOHEADER)];

    file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
    file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

    // Construct the values from the buffers
    bmpHeader = (BITMAPFILEHEADER*) datBuff[0];
    bmpInfo   = (BITMAPINFOHEADER*) datBuff[1];

    // First allocate pixel memory
    pixels = new unsigned char[bmpInfo->biSizeImage];

    // Go to where image data starts, then read in image data
    file.seekg(bmpHeader->bfOffBits);
    file.read((char*)pixels, bmpInfo->biSizeImage);

    unsigned char tmpRGB = 0; // Swap buffer
    for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
    {
        tmpRGB        = pixels[i];
        pixels[i]     = pixels[i + 2];
        pixels[i + 2] = tmpRGB;
    }

    // Set width and height to the values loaded from the file
    int w = bmpInfo->biWidth;
    int h = bmpInfo->biHeight;

    int img_size =  w* h * sizeof(char);

    unsigned char** input_image, output_image;
    unsigned char** device_InputImage, device_OutputImage;

    input_image = (char**)malloc(img_size * sizeof(char));
    output_image = (char**)malloc(img_size * sizeof(char));

    cudaMalloc((void**) &device_InputImage, img_size);
    cudaMalloc((void**) &deviceOutputImage, img_size);

    device_InputImage = bmpHeader->bfOffBits; //start address of pixels buffer
    cudaMemcpy( input_image, device_InputImage, N*sizeof(int), cudaMemcpyHostToDevice);

    int nr_threads = h/WINDOW_SIZE;

    deviceOutputImage = median_filter<<1, nr_threads>>(*device_InputImage, w, h)); //call function median_filter
    cudaMemcpy(output_image, deviceOutputImage, size, cudaMemcpyDeviceToHost);

    return deviceOutputImage;

    cudaFree(deviceOutputImage);
    cudaFree(device_InputImage);

    free(output_image);
    free(input_image);
}

int main()
{
    unsigned char* img_location;
    unsigned char** modified_image;

    img_location = "C:\Users\Dana\Desktop\18-05 Cuda_Img";

    modified_image = Modify_Image(&img_location);

    return 0;
}
