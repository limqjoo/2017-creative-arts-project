#pragma once
#ifndef SCREENGRAB_H
#define SCREENGRAB_H
#include <iostream>
#include <windows.h>
#include "ofPixels.h"

inline void createBMPFile(LPCSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) {
	HANDLE hf;                 // file handle
	BITMAPFILEHEADER hdr;       // bitmap file-header
	PBITMAPINFOHEADER pbih;     // bitmap info-header
	LPBYTE lpBits;              // memory pointer
	DWORD dwTotal;              // total count of bytes
	DWORD cb;                   // incremental count of bytes
	BYTE *hp;                   // byte pointer
	DWORD dwTmp;
	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	if (!lpBits)
		//errhandler("GlobalAlloc", hwnd);
		cout << "Error: GlobalAlloc" << endl;
	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		//errhandler("GetDIBits", hwnd);
		cout << "Error: GetDIBits" << endl;
	}
	// Create the .BMP file.
	hf = CreateFileA(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
		//errhandler("CreateFile", hwnd);
		cout << "Error: CreateFile" << endl;
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
	// Compute the size of the entire file.
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	// Compute the offset to the array of color indices.
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);
	// Copy the BITMAPFILEHEADER into the .BMP file.
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		//errhandler("WriteFile", hwnd);
		cout << "Error: WriteFile" << endl;
	}
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		//errhandler("WriteFile", hwnd);
		cout << "Error: WriteFile" << endl;
	// Copy the array of color indices into the .BMP file.
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		//errhandler("WriteFile", hwnd);
		cout << "Error: WriteFile" << endl;
	// Close the .BMP file.
	if (!CloseHandle(hf))
		//errhandler("CloseHandle", hwnd);
		cout << "Error: WriteFile" << endl;
	// Free memory.
	GlobalFree((HGLOBAL)lpBits);
};

inline PBITMAPINFO createBitmapInfoStruct(HBITMAP hBmp) {
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	// Retrieve the bitmap color format, width, and height.  
	if (!GetObjectA(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		//errhandler("GetObject", hwnd);
		cout << "Error: GetObject" << endl;
	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;
	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  
	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));
	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));
	// Initialize the fields in the BITMAPINFO structure.  
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;
	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
};
inline ofPixels convertToOfImage(HBITMAP hBmp, HDC hDC, PBITMAPINFO pbi) {
	BITMAP bmp;
	//PBITMAPINFOHEADER pbih;     // bitmap info-header  
	//	LPBYTE lpBits;              // memory pointer  
	BYTE* ScreenData = 0;
	// Retrieve the bitmap color format, width, and height.  
	GetObjectA(hBmp, sizeof(BITMAP), (LPSTR)&bmp);
	// Initialise ofPixels object
	ofPixels pixels;
	pixels.allocate(bmp.bmWidth, bmp.bmHeight, 3);
	if (ScreenData)
		free(ScreenData);
	ScreenData = (BYTE*)malloc(4 * bmp.bmWidth * bmp.bmHeight);

	GetDIBits(hDC, hBmp, 0, bmp.bmHeight, ScreenData, pbi, DIB_RGB_COLORS);
	for (int y = 0; y < bmp.bmHeight; y++) {
		for (int x = 0; x < bmp.bmWidth; x++) {
			if (ScreenData) {
				ofColor color = ofColor(ScreenData[4 * ((y*bmp.bmWidth) + x) + 2], ScreenData[4 * ((y*bmp.bmWidth) + x) + 1], ScreenData[4 * ((y*bmp.bmWidth) + x)]); //bitmap colour info is in BGR values, so flip them around for RGB ofPixels format
				pixels.setColor(x, bmp.bmHeight - y - 1, color);
			}
		}
	}
	delete ScreenData;
	return pixels;
};
inline ofPixels screenGrab(POINT a, POINT b) {
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL); //retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen (parameter: NULL)
	HDC     hDC = CreateCompatibleDC(hScreen); //creates a memory DC compatible with the specified device
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y)); //creates a bitmap compatible with the device that is associated with the specified device context
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap); //selects an object into the specified DC. The new object replaces the previous object of the same type.
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY); //performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context.

	PBITMAPINFO pbmi = createBitmapInfoStruct(hBitmap);
	//createBMPFile("image.bmp", pbmi, hBitmap, hDC);
	ofPixels pixels = convertToOfImage(hBitmap, hDC, pbmi);

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return pixels;
};
#endif // !SCREENGRAB_H