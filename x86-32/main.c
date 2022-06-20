#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


extern void swap_blocks(int i, char* start, ImageInfo* ptr);
void shuffle(ImageInfo* ptr);

ImageInfo* readBmp(const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file == NULL)
    {
        return NULL;
    }

    RGBbmpHdr btmhdr;
    if (fread((void*) &btmhdr, sizeof(btmhdr), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (btmhdr.bfType != 0x4D42 ||
        //btmhdr.biPlanes != 3 ||
        btmhdr.biBitCount != 24 ||
        btmhdr.biCompression != 0)
    {
        printf("Invalid bitmap header.\n");
        fclose(file);
        return NULL;
    }

    ImageInfo* imginfo = (ImageInfo *) malloc(sizeof(ImageInfo));
    if (imginfo == NULL)
    {
        fclose(file);
        return NULL;
    }
    imginfo->pImg = NULL;
    imginfo->height = abs(btmhdr.biHeight);
    imginfo->width = btmhdr.biWidth;
    imginfo->line_bytes = imginfo->width * 3;
    if (imginfo->line_bytes % 4 != 0)
        imginfo->line_bytes += 4 - (imginfo->line_bytes % 4);

    if (fseek(file, btmhdr.bfOffBits, SEEK_SET) != 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    imginfo->pImg = (unsigned char*) malloc(btmhdr.biSizeImage);
    if(imginfo->pImg == 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    if (fread(imginfo->pImg, 1, btmhdr.biSizeImage, file) != btmhdr.biSizeImage)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    fclose(file);
    return imginfo;
}

FILE* saveBmp(const char* fileName, ImageInfo *imageinfo)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        return file;
    }
    unsigned int lineBytes = (int) imageinfo->line_bytes;
	unsigned int imgSize = lineBytes * imageinfo->height;
    RGBbmpHdr bmpHead =
	{
	0x4D42,				// unsigned short bfType;
	sizeof(RGBbmpHdr)+imgSize,		// unsigned long  bfSize;
	0, 0,				// unsigned short bfReserved1, bfReserved2;
	sizeof(RGBbmpHdr),	// unsigned long  bfOffBits;
	40,					// unsigned long  biSize;
	(int32_t)imageinfo->width,		// long  biWidth;
	(int32_t)imageinfo->height,		// long  biHeight;
	1,					// short biPlanes;
	24,					// short biBitCount;
	0,					// unsigned long  biCompression;
	(uint32_t) imgSize, // unsigned long  biSizeImage;
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter;
	0,					// unsigned long  biClrUsed;
	0,					// unsigned long  biClrImportant;
	};

	if (fwrite(&bmpHead, sizeof(RGBbmpHdr), 1, file) != 1)
    {
        fclose(file);
        return file;
    }
    printf("%d", sizeof(RGBbmpHdr));
    if (fwrite(imageinfo->pImg, 1, bmpHead.biSizeImage, file) != bmpHead.biSizeImage)
    {
        fclose(file);
        return file;
    }
    fclose(file);
	return file;
}

void freeImage(ImageInfo *imageinfo)
{
    if (imageinfo != NULL)
    {
        if(imageinfo->pImg != NULL)
            free(imageinfo->pImg);
        free(imageinfo);
    }
}

int main(void)
{
    ImageInfo* ptr = readBmp("source.bmp");
    shuffle(ptr);
    saveBmp("dest.bmp", ptr);
    return 0;
}


void shuffle(ImageInfo* ptr)
{
    srand(time(NULL));
    int puzzle[] = { 0,1,2,3,4,5,6,7,8,9,10,11};
    // shuffling the puzzle
    for (int i = 0; i < 12; ++i)
    {
        int idx = rand() % 12;
        int temp = puzzle[i];
        puzzle[i] = puzzle[idx];
        puzzle[idx] = temp;
    }
    char* start = ptr->pImg;
    // swapping blocks in a puzzle, where puzzle[i] is the nr of block that i needs to be swapped with
    for (int i = 0; i < 12; ++i)
    {
    swap_blocks(puzzle[i], start, ptr);
    if (i != 3 && i != 7) {
    // just adding width_for_block
    start += ptr->line_bytes / 4; }
    // shifting the start to new row
    else { start += (ptr->line_bytes * (ptr->height/3)) - (ptr->line_bytes * 3) / 4; }
    }
}