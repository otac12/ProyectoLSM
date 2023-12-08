#include <stdio.h>
#include <stdlib.h>

#pragma pack(2)
typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

#pragma pack(2)
typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;

void combineImages(const char *image1Path, const char *image2Path, const char *outputPath) {
    FILE *file1 = fopen(image1Path, "rb");
    FILE *file2 = fopen(image2Path, "rb");
    FILE *outputFile = fopen(outputPath, "wb");

    if (file1 == NULL || file2 == NULL || outputFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    BMPHeader header1, header2;
    BMPInfoHeader infoHeader1, infoHeader2;

    fread(&header1, sizeof(BMPHeader), 1, file1);
    fread(&infoHeader1, sizeof(BMPInfoHeader), 1, file1);

    fread(&header2, sizeof(BMPHeader), 1, file2);
    fread(&infoHeader2, sizeof(BMPInfoHeader), 1, file2);

    // Assuming both images have the same height
    int totalWidth = infoHeader1.width + infoHeader2.width;
    int height = infoHeader1.height;

    // Update the headers for the new image
    BMPHeader newHeader = {0x4D42, totalWidth * height * 3 + sizeof(BMPHeader) + sizeof(BMPInfoHeader), 0, 0, sizeof(BMPHeader) + sizeof(BMPInfoHeader)};
    BMPInfoHeader newInfoHeader = {sizeof(BMPInfoHeader), totalWidth, height, 1, 24, 0, 0, 0, 0, 0};

    // Write the headers to the output file
    fwrite(&newHeader, sizeof(BMPHeader), 1, outputFile);
    fwrite(&newInfoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    // Allocate memory for pixel data
    unsigned char *pixelData1 = (unsigned char *)malloc(infoHeader1.imageSize);
    unsigned char *pixelData2 = (unsigned char *)malloc(infoHeader2.imageSize);

    // Read pixel data from the input files
    fread(pixelData1, sizeof(unsigned char), infoHeader1.imageSize, file1);
    fread(pixelData2, sizeof(unsigned char), infoHeader2.imageSize, file2);

    // Write the pixel data to the output file
    fwrite(pixelData1, sizeof(unsigned char), infoHeader1.width * height * 3, outputFile);

    // Skip padding bytes in the first image
    fseek(file2, header2.offset - sizeof(BMPHeader) - sizeof(BMPInfoHeader) - infoHeader2.imageSize, SEEK_SET);

    // Write the pixel data from the second image (including padding)
    for (int i = 0; i < height; ++i) {
        fread(pixelData2, sizeof(unsigned char), infoHeader2.width * 3, file2);
        fwrite(pixelData2, sizeof(unsigned char), infoHeader2.width * 3, outputFile);

        // Skip padding bytes in the second image
        fseek(file2, (4 - (infoHeader2.width * 3) % 4) % 4, SEEK_CUR);
    }

    // Clean up
    free(pixelData1);
    free(pixelData2);
    fclose(file1);
    fclose(file2);
    fclose(outputFile);
}

int main() {
    combineImages("imagen1.bmp", "imagen2.bmp", "output.bmp");
    return 0;
}