#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_FILE "E:\\git\\PRG2_Recueil_Exercices_ary\\C13_EXTRA\\spoon.bmp"

#pragma pack(push, 1)
typedef struct {
    unsigned char bfType[2];  // "BM"
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

typedef struct {
    unsigned char r, g, b;
} Pixel;

// Allocate and initialize an image with optional content
Pixel *create_image(int width, int height) {
    Pixel *pixels = (Pixel *)calloc(width * height, sizeof(Pixel));
    if (!pixels) return NULL;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = y * width + x;
            if ((x > 50 && x < width - 50) && (y > 50 && y < height - 50)) {
                pixels[i].r = 255;
                pixels[i].g = 255;
                pixels[i].b = 5;
            } else {
                pixels[i].r = 55;
                pixels[i].g = 55;
                pixels[i].b = 55;
            }
        }
    }

    return pixels;
}

// Write a BMP image from pixel data
int write_bmp(const char *filename, int width, int height, const Pixel *pixels) {
    int row_bytes = (width * 3 + 3) & ~3; // Padded to 4-byte boundary
    int img_size = row_bytes * height;
    int file_size = 54 + img_size;

    BITMAPFILEHEADER fileHeader = {
        .bfType = {'B', 'M'},
        .bfSize = file_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = 54
    };

    BITMAPINFOHEADER infoHeader = {
        .biSize = 40,
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = img_size,
        .biXPelsPerMeter = 3780,
        .biYPelsPerMeter = 3780,
        .biClrUsed = 0,
        .biClrImportant = 0
    };

    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Could not open file for writing");
        return -1;
    }

    fwrite(&fileHeader, sizeof(fileHeader), 1, f);
    fwrite(&infoHeader, sizeof(infoHeader), 1, f);

    unsigned char *row = (unsigned char *)calloc(1, row_bytes);
    if (!row) {
        fclose(f);
        return -1;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = (height - 1 - y) * width + x; // BMP is bottom-up
            row[x * 3 + 0] = pixels[i].b;
            row[x * 3 + 1] = pixels[i].g;
            row[x * 3 + 2] = pixels[i].r;
        }
        fwrite(row, 1, row_bytes, f);
    }

    free(row);
    fclose(f);
    return 0;
}

// reads a bmp image
Pixel *read_bmp(int *width, int *height) {
    FILE *f = fopen(IN_FILE, "rb");
    if (!f) {
        perror("Could not open BMP file");
        return NULL;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    // Read headers
    fread(&fileHeader, sizeof(fileHeader), 1, f);
    fread(&infoHeader, sizeof(infoHeader), 1, f);

    // Validate BMP signature and 24-bit depth
    if (fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M') {
        fprintf(stderr, "Not a valid BMP file.\n");
        fclose(f);
        return NULL;
    }

    if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0) {
        fprintf(stderr, "Only uncompressed 24-bit BMPs are supported.\n");
        fclose(f);
        return NULL;
    }

    *width = infoHeader.biWidth;
    *height = infoHeader.biHeight;

    int row_bytes = ((*width * 3 + 3) & ~3);
    Pixel *pixels = (Pixel *)malloc((*width) * (*height) * sizeof(Pixel));
    if (!pixels) {
        fclose(f);
        return NULL;
    }

    // Seek to pixel data
    fseek(f, fileHeader.bfOffBits, SEEK_SET);

    unsigned char *row = (unsigned char *)malloc(row_bytes);
    if (!row) {
        free(pixels);
        fclose(f);
        return NULL;
    }

    // Read rows bottom-up
    for (int y = 0; y < *height; y++) {
        fread(row, 1, row_bytes, f);
        for (int x = 0; x < *width; x++) {
            int pixel_index = ((*height - 1 - y) * (*width)) + x;
            pixels[pixel_index].b = row[x * 3 + 0];
            pixels[pixel_index].g = row[x * 3 + 1];
            pixels[pixel_index].r = row[x * 3 + 2];
        }
    }

    free(row);
    fclose(f);
    return pixels;
}

// Encode the message bits into pixels LSB
Pixel *LSB_encode(Pixel *pixels, int width, int height, const char* message) {
    int len = strlen(message) + 1;  // +1 for null terminator
    int total_pixels = width * height;

    // We encode 8 bits per character, 3 bits per pixel -> 3 pixels per char minimum
    // Actually 3 pixels give 9 bits, enough for 8 bits of char
    if (len * 3 > total_pixels) {
        fprintf(stderr, "Message is too long to encode in the image.\n");
        return NULL;
    }

    int pixel_index = 0;
    for (int i = 0; i < len; i++) {
        unsigned char ch = message[i];

        // Encode bits into 3 pixels (9 bits total)
        for (int bit = 0; bit < 8; bit++) {
            int channel = bit % 3;    // 0->r,1->g,2->b channel
            int p = pixel_index + bit / 3;

            if (p >= total_pixels) {
                fprintf(stderr, "Ran out of pixels while encoding.\n");
                return NULL;
            }

            // Clear LSB of selected channel and set it to bit value
            if (channel == 0)
                pixels[p].r = (pixels[p].r & 0xFE) | ((ch >> bit) & 1);
            else if (channel == 1)
                pixels[p].g = (pixels[p].g & 0xFE) | ((ch >> bit) & 1);
            else // channel == 2
                pixels[p].b = (pixels[p].b & 0xFE) | ((ch >> bit) & 1);
        }

        // We used 3 pixels per character
        pixel_index += 3;
    }

    return pixels;
}

// Decode the message bits from pixels LSB
char *LSB_decode(const Pixel *pixels, int width, int height) {
    int total_pixels = width * height;
    char *message = (char *)malloc(total_pixels / 3 + 1); // Max chars = total_pixels/3
    if (!message) return NULL;

    int pixel_index = 0;
    int msg_index = 0;

    while (pixel_index + 3 <= total_pixels) {
        unsigned char ch = 0;

        // Read 8 bits from 3 pixels
        for (int bit = 0; bit < 8; bit++) {
            int channel = bit % 3;
            int p = pixel_index + bit / 3;

            unsigned char bit_val = 0;
            if (channel == 0)
                bit_val = pixels[p].r & 1;
            else if (channel == 1)
                bit_val = pixels[p].g & 1;
            else // channel == 2
                bit_val = pixels[p].b & 1;

            ch |= (bit_val << bit);
        }

        message[msg_index++] = ch;
        pixel_index += 3;

        if (ch == '\0') break; // End of string
    }

    message[msg_index] = '\0';
    return message;
}


int main(int argc, char *argv[]) {
    char message[] = "hello World";

    int w, h;
    Pixel *pixels = read_bmp(&w, &h);
    if (!pixels) {
        fprintf(stderr, "Failed to read image\n");
        return 1;
    }

    if (!LSB_encode(pixels, w, h, message)) {
        free(pixels);
        return 1;
    }

    if (write_bmp("image.bmp", w, h, pixels) != 0) {
        fprintf(stderr, "Failed to write image\n");
        free(pixels);
        return 1;
    }

    free(pixels);

    // Decode from the written image
    Pixel *decoded_pixels = read_bmp(&w, &h);
    if (!decoded_pixels) return 1;

    char *decoded = LSB_decode(decoded_pixels, w, h);
    if (decoded) {
        printf("Decoded message: %s\n", decoded);
        free(decoded);
    } else {
        fprintf(stderr, "Failed to decode message\n");
    }

    free(decoded_pixels);
    return 0;
}
