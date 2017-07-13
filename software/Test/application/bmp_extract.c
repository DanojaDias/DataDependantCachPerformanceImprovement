#include "bmp_extract.h"

int8_t g_file_buffer[67108864];	// Create and Enormous buffer to avoid dynamic memory allocation
Bmp_header g_input_file_header;

int bmp_extract_header(Bmp_data *pic_data) {
	int i;

	if (pic_data->buffer[0] == 'B' && pic_data->buffer[1] == 'M') {
		for (i = 0; i < 52; i++) {
			pic_data->header->BMPRawData[i] = pic_data->buffer[i + 2];
		}

		// Calculate the effective size of the data field
		pic_data->header->BMPDataSize = pic_data->header->BMPHeight * pic_data->header->BMPWidth
				* (pic_data->header->BMPBitsPerPixel / 8);

		pic_data->bitmap = &pic_data->buffer[pic_data->header->BMPDataOffset];
		return 1;
	}
	return 0;
}

void bmp_rearrange_comps(Bmp_data *pic_data) {
	// BMP images start from the bottom left, and its components are ordered
	// B - G - R. The jpeg encoder starts at the top left, and is ordered
	// R - G - B. This function puts all the bytes in their right place.

	int row, column;
	int8_t r, g, b, *row_ptr, *dest;

	// <*dest> starts from the top left of the image and moves downwards.
	// <*row_ptr> starts from the bottom left of the image and moves upwards.
	dest = pic_data->bitmap;

	for (row = pic_data->header->BMPHeight - 1; row >= pic_data->header->BMPHeight / 2; row--) {
		// We loop until <*dest> and <*row_ptr> meet in the middle of the image.

		row_ptr = &(pic_data->bitmap[row * pic_data->header->BMPWidth * 3]);
		for (column = 0; column < pic_data->header->BMPWidth * 3; column += 3) {
			// Not only do we move data from <*dest> to <*row_ptr> and vice-versa,
			// we also need to reshuffle RGB components.
			r = dest[2];
			g = dest[1];
			b = dest[0];

			*dest++ = row_ptr[column + 2];
			*dest++ = row_ptr[column + 1];
			*dest++ = row_ptr[column];

			row_ptr[column] = r;
			row_ptr[column + 1] = g;
			row_ptr[column + 2] = b;
		}
	}
}

int bmp_extract(char *file_name, Bmp_data *pic_data) {
	FILE *bmp_file = NULL;
	int bmp_file_size, bytes_read;

	bmp_file = fopen(file_name, "rb");
	if(bmp_file == NULL) {
		printf("Error opening the file %s\n", file_name);
		return 1;
	}

	// Get the size of the file
	fseek(bmp_file, 0, SEEK_END);
	bmp_file_size = ftell(bmp_file);
	rewind(bmp_file);

	pic_data->buffer = g_file_buffer;		// (int8_t *)malloc(bmp_file_size);

	// Read the whole file into the buffer
	bytes_read = fread((void *)pic_data->buffer, sizeof(int8_t), bmp_file_size, bmp_file);

	fclose(bmp_file);

	if(bytes_read != bmp_file_size)
	{
		printf("Error occurred while reading the file\n\n");
		return 2;
	}

	pic_data->header = &g_input_file_header;	// (Bmp_header *)malloc(sizeof(Bmp_header));

	bmp_extract_header(pic_data);
	bmp_rearrange_comps(pic_data);

	printf("Size of the file: %d\n", (int)pic_data->header->BMPFileSize);
	printf("Size of the height: %d\n", (int)pic_data->header->BMPHeight);
	printf("Size of the width: %d\n", (int)pic_data->header->BMPWidth);
	printf("Size of the offset: %d\n", (int)pic_data->header->BMPDataOffset);
	printf("Data pointer: 0x%08x\n\n", (unsigned int)pic_data->bitmap);

	return 0;
}
