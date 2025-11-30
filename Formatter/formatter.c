// Formats any picture to 640x480 (using bilinear interpolation) and grayscale
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: %s input_image output_image\n", argv[0]);
        return 1;
    }

    // Image loading using stb_image

    const char *input_file = argv[1];
    const char *output_file = argv[2];


    int width, height, channels;
    unsigned char *img = stbi_load(fileName, &width, &height, &channels, 0);
    if (!img) {
        printf("Error loading the image!!\n");
        exit(1);
    }
    printf("Image loaded successfully!\n");

    int newW = 480, newH = 640;

    // Calculate scaling factor to preserve aspect ratio
    float scale_w = (float)newW / width;
    float scale_h = (float)newH / height;
    float scale = (scale_w < scale_h) ? scale_w : scale_h;

    int resizedW = (int)(width * scale);
    int resizedH = (int)(height * scale);

    unsigned char *resized = (unsigned char*)malloc(resizedW * resizedH * channels);
    if (!resized) {
        printf("Failed to allocate memory for resized image :(\n");
        stbi_image_free(img);
        return 1;
    }

    stbir_resize_uint8(img, width, height, 0, resized, resizedW, resizedH, 0, channels);

    unsigned char *gray = (unsigned char*)calloc(newW * newH, 1);

    if (!gray) {
        printf("Failed to allocate memory for grayscale image\n");
        free(resized);
        stbi_image_free(img);
        return 1;
    }

    // Center the resized image in the output
    int x_offset = (newW - resizedW) / 2;
    int y_offset = (newH - resizedH) / 2;

    for (int y = 0; y < resizedH; y++) {
        for (int x = 0; x < resizedW; x++) {
            int idx_resized = (y * resizedW + x) * channels;
            unsigned char r = resized[idx_resized + 0];
            unsigned char g = resized[idx_resized + 1];
            unsigned char b = resized[idx_resized + 2];
            unsigned char gray_val = (unsigned char)(0.299*r + 0.587*g + 0.114*b);

            int idx_gray = (y + y_offset) * newW + (x + x_offset);
            gray[idx_gray] = gray_val;
        }
    }



    // Save final image
    if (!stbi_write_png(output_file, newW, newH, 1, gray, newW)) {
        printf("Failed to save image: %s\n", output_file);
    } else {
        printf("Saved grayscale image to %s\n", output_file);
    }

    free(gray);
    free(resized);
    stbi_image_free(img);


    return 0;
}


unsigned char rgb_to_gray(unsigned char r, unsigned char g, unisged char b) {
    return (unsigned char)(0.299*r+0.587*g+0.144*b);
}
