#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define IN_FILE "C13_EXTRA\\spoon.bmp"

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

            //content here maybe change as its useless
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
Pixel *read_bmp(int *width, int *height, char *filename) {
    FILE *f = fopen(filename, "rb");
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

//encoding on random pixels because, why not ?
Pixel *LSB_encode_by_difference(Pixel *pixels, int width, int height, const char *message) {
    int len = strlen(message) + 1;
    int total_pixels = width * height;

    if (len * 3 > total_pixels) {
        fprintf(stderr, "Message too long\n");
        return NULL;
    }

    bool *used = (bool *)calloc(total_pixels, sizeof(bool));
    if (!used) return NULL;

    srand((unsigned int)time(NULL));
    int bits_encoded = 0;

    for (int i = 0; i < len; i++) {
        unsigned char ch = message[i];
        for (int bit = 0; bit < 8; bit++) {
            int channel = bit % 3;
            int pixel_idx;

            // Find unused pixel
            do {
                pixel_idx = rand() % total_pixels;
            } while (used[pixel_idx]);
            used[pixel_idx] = true;

            // Modify selected channel's LSB
            if (channel == 0)
                pixels[pixel_idx].r = (pixels[pixel_idx].r & 0xFE) | ((ch >> bit) & 1);
            else if (channel == 1)
                pixels[pixel_idx].g = (pixels[pixel_idx].g & 0xFE) | ((ch >> bit) & 1);
            else
                pixels[pixel_idx].b = (pixels[pixel_idx].b & 0xFE) | ((ch >> bit) & 1);

            bits_encoded++;
        }
    }

    free(used);
    return pixels;
}

//decoding my random thingy by comparing with original image
char *LSB_decode_by_comparison(const Pixel *original, const Pixel *encoded, int width, int height) {
    int total_pixels = width * height;
    unsigned char current_byte = 0;
    int bit_index = 0;
    int msg_capacity = total_pixels / 3;
    char *message = (char *)malloc(msg_capacity + 1);
    if (!message) return NULL;
    int msg_pos = 0;

    for (int i = 0; i < total_pixels; i++) {
        for (int c = 0; c < 3; c++) {
            unsigned char orig, enc;
            if (c == 0) {
                orig = original[i].r;
                enc = encoded[i].r;
            } else if (c == 1) {
                orig = original[i].g;
                enc = encoded[i].g;
            } else {
                orig = original[i].b;
                enc = encoded[i].b;
            }

            if ((orig & 1) != (enc & 1)) {
                current_byte |= ((enc & 1) << bit_index);
                bit_index++;

                if (bit_index == 8) {
                    message[msg_pos++] = current_byte;
                    if (current_byte == '\0') {
                        message[msg_pos] = '\0';
                        return message;
                    }
                    current_byte = 0;
                    bit_index = 0;
                }
            }
        }
    }

    message[msg_pos] = '\0';
    return message;
}


int main(int argc, char *argv[]) {
    //message to encode
    char message[] = "hello World";

    //reading source image
    int w, h;
    Pixel *pixels = read_bmp(&w, &h, IN_FILE);
    if (!pixels) {
        fprintf(stderr, "Failed to read source image\n");
        return 1;
    }

    //encode message in pixel map
    if (!LSB_encode(pixels, w, h, message)) {
        free(pixels);
        return 1;
    }

    //makes the image with the encoded pixel map
    if (write_bmp("image.bmp", w, h, pixels) != 0) {
        fprintf(stderr, "Failed to write image\n");
        free(pixels);
        return 1;
    }

    free(pixels);

    // Decode from the written image
    Pixel *decoded_pixels = read_bmp(&w, &h, "image.bmp");
    if (!decoded_pixels) {
        fprintf(stderr, "Failed to read encoded image\n");
        return 1;
    }

    char *decoded = LSB_decode(decoded_pixels, w, h);
    if (decoded) {
        printf("Decoded message: %s\n", decoded);
        free(decoded);
    } else {
        fprintf(stderr, "Failed to decode message\n");
    }

    free(decoded_pixels);
    

    Pixel *original = read_bmp(&w, &h, IN_FILE);
    // Make a copy
    Pixel *copy = (Pixel *)malloc(sizeof(Pixel) * w * h);
    memcpy(copy, original, sizeof(Pixel) * w * h);

    // Encode into copy
    LSB_encode_by_difference(copy, w, h, message);
    write_bmp("image_encoded.bmp", w, h, copy);

    // Decode by comparing original and copy -> TODO fix maybe
    Pixel *encoded = read_bmp(&w, &h, "image_encoded.bmp");
    char *msg = LSB_decode_by_comparison(original, encoded, w, h);
    printf("Decoded: %s\n", msg);

    free(copy);
    free(encoded);
    free(msg);
    return 0;
}
