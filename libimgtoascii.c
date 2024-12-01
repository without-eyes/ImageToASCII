#include "libimgtoascii.h"

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    unsigned char fileType[2];
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offsetData;
} BMPFileHeader;

typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int sizeImage;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

void readBMP(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Не вдалося відкрити файл");
        return;
    }

    // Прочитати заголовки BMP
    BMPFileHeader fileHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    BMPInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Перевірка формату файлу
    if (fileHeader.fileType[0] != 'B' || fileHeader.fileType[1] != 'M') {
        printf("Це не BMP файл!\n");
        fclose(file);
        return;
    }

    // Виділення пам'яті для пікселів
    int width = infoHeader.width;
    int height = infoHeader.height;
    unsigned char *pixels = (unsigned char *)malloc(infoHeader.sizeImage);
    if (!pixels) {
        perror("Не вдалося виділити пам'ять");
        fclose(file);
        return;
    }

    // Перехід до даних пікселів і їх читання
    fseek(file, fileHeader.offsetData, SEEK_SET);
    fread(pixels, 1, infoHeader.sizeImage, file);

    fclose(file);

    // Вивести пікселі (24-бітні RGB)
    printf("Ширина: %d, Висота: %d\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char blue = pixels[(y * width + x) * 3];
            unsigned char green = pixels[(y * width + x) * 3 + 1];
            unsigned char red = pixels[(y * width + x) * 3 + 2];
            printf("Піксель (%d, %d): R=%d, G=%d, B=%d\n", x, y, red, green, blue);
        }
    }

    free(pixels);
}