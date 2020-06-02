#include "GLideNUI.h"
#include "../Config.h"
#include "util.h"
#include <Windows.h>
#include "../GLideNHQ/inc/png.h"

void write_png_file(const wchar_t * file_name, int width, int height, const uint8_t *buffer)
{
#pragma warning(disable: 4996)
	FILE *fp = _wfopen(file_name, L"wb");
#pragma warning(default: 4996)
	if (!fp) return;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return;
	}

	png_init_io(png_ptr, fp);

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return;
	}

	png_byte bit_depth = 8;
	png_byte color_type = PNG_COLOR_TYPE_RGB;
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return;
	}

	int pixel_size = 3;
	int p = 0;
	png_bytep * row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* height);
	for (int y = height - 1; y >= 0; y--) {
		row_pointers[y] = (png_byte*)malloc(width*pixel_size);
		for (int x = 0; x < width; x++) {
			row_pointers[y][x*pixel_size + 0] = buffer[p++];
			row_pointers[y][x*pixel_size + 1] = buffer[p++];
			row_pointers[y][x*pixel_size + 2] = buffer[p++];
		}
	}
	png_write_image(png_ptr, row_pointers);

	for (int y = 0; y < height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return;
	}
	png_write_end(png_ptr, NULL);
	fclose(fp);
}

EXPORT void CALL SaveScreenshot(const wchar_t * _folder, const char * _name, int _width, int _height, const unsigned char * _data)
{
	const wchar_t * fileExt = L"png";

	std::wstring folder = _folder;
	if (folder.size() > 1 && folder[folder.size() - 1] == L'\\') folder.resize(folder.size() - 1);

	WIN32_FIND_DATA	FindData = { 0 };
	HANDLE hFindFile = FindFirstFile(folder.c_str(), &FindData); // Find anything
	if (hFindFile == INVALID_HANDLE_VALUE) {
		::CreateDirectory(folder.c_str(), NULL);
		hFindFile = FindFirstFile(folder.c_str(), &FindData);
	}
	if (hFindFile != INVALID_HANDLE_VALUE)
		FindClose(hFindFile);

	if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		return;

	std::wstring romName = ToUTF16(_name);
	for (size_t i = 0, n = romName.size(); i < n; i++) {
		if (romName[i] == L' ') romName[i] = L'_';
		if (romName[i] == L':') romName[i] = L';';
	}

	wchar_t fileName[MAX_PATH];
	int i;
	for (i = 0; i < 1000; ++i) {
#pragma warning(disable: 4996)
		swprintf(fileName, L"%ls\\GLideN64_%ls_%03i.%s", folder.c_str(), romName.c_str(), i, fileExt);
#pragma warning(default: 4996)
		hFindFile = FindFirstFile(fileName, &FindData);
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			break;
		}
		FindClose(hFindFile);
	}
	if (i == 1000)
		return;
	
	write_png_file(fileName, _width, _height, _data);
}
