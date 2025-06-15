#include <stdio.h>
#include <stdlib.h>

struct rgb_data
{
    int r, g, b;
};

struct bitmap_file_header
{
    unsigned char bitmap_type[2]; // 2 bytes
    int file_size;                // 4 bytes
    short reserved1;              // 2 bytes
    short reserved2;              // 2 bytes
    unsigned int offset_bits;     // 4 bytes
} bfh;

// bitmap image header (40 bytes)
struct bitmap_image_header
{
    unsigned int size_header;   // 4 bytes
    unsigned int width;         // 4 bytes
    unsigned int height;        // 4 bytes
    short int planes;           // 2 bytes
    short int bit_count;        // 2 bytes
    unsigned int compression;   // 4 bytes
    unsigned int image_size;    // 4 bytes
    unsigned int ppm_x;         // 4 bytes
    unsigned int ppm_y;         // 4 bytes
    unsigned int clr_used;      // 4 bytes
    unsigned int clr_important; // 4 bytes
} bih;

int makeImg(int width, int height)
{
    const int dpi = 10;
    int img_size = width * height;
    int file_size = 54 + 4 * img_size;
    int ppm = dpi * 39.375;
    FILE *img;

    memcpy(&bfh.bitmap_type, "BM", 2);
    bfh.file_size = file_size;
    bfh.reserved1 = 0;
    bfh.reserved2 = 0;
    bfh.offset_bits = 0;

    bih.size_header = sizeof(bih);
    bih.width = width;
    bih.height = height;
    bih.planes = 1;
    bih.bit_count = 24;
    bih.compression = 0;
    bih.image_size = file_size;
    bih.ppm_x = ppm;
    bih.ppm_y = ppm;
    bih.clr_used = 0;
    bih.clr_important = 0;

    img = fopen("image.bmp", "wb");

    // compiler woes so we will just use the constant 14 we know we have
    fwrite(&bfh, 1, 14, img);
    fwrite(&bih, 1, sizeof(bih), img);

    int width = 400,
        height = 400,
        dpi = 96;

    rgb_data *pixels = new rgb_data[width * height];

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int a = y * width + x;

            if ((x > 50 && x < 350) && (y > 50 && y < 350))
            {
                pixels[a].r = 255;
                pixels[a].g = 255;
                pixels[a].b = 5;
            }
            else
            {
                pixels[a].r = 55;
                pixels[a].g = 55;
                pixels[a].b = 55;
            }
        }
    }

    // write out pixel data, one last important this to know is the ordering is backwards
    // we have to go BGR as opposed to RGB
    for (int i = 0; i < img_size; i++)
    {
        rgb_data BGR = data[i];

        float red = (BGR.r);
        float green = (BGR.g);
        float blue = (BGR.b);

        // if you don't follow BGR image will be flipped!
        unsigned char color[3] = {
            blue, green, red};

        fwrite(color, 1, sizeof(color), image);
    }

    fclose(image);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        perror("incorrect number of arguments. Usage cmd <height> <width>\n");
        return -1;
    }

    makeImg(argv[1], argv[2]);

    return 0;
}