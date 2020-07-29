#pragma once
#include <iostream>
#include <windows.h>
#include "ofMain.h"

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
	cout << bmp.bmWidthBytes << endl;
	GetDIBits(hDC, hBmp, 0, bmp.bmHeight, ScreenData, pbi, DIB_RGB_COLORS);
	for (int y = 0; y < bmp.bmHeight; y++) {
		for (int x = 0; x < bmp.bmWidth; x++) {
			ofColor color = ofColor(ScreenData[4 * ((y*bmp.bmWidth) + x) + 2], ScreenData[4 * ((y*bmp.bmWidth) + x) + 1], ScreenData[4 * ((y*bmp.bmWidth) + x)]); //bitmap colour info is in BGR values, so flip them around for RGB ofPixels format
			pixels.setColor(x, bmp.bmHeight - y - 1, color);
		}
	}
	return pixels;
};
inline ofPixels screenGrab(ofPoint a, ofPoint b) {
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL); //retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen (parameter: NULL)
	HDC     hDC = CreateCompatibleDC(hScreen); //creates a memory DC compatible with the specified device
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y)); //creates a bitmap compatible with the device that is associated with the specified device context
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap); //selects an object into the specified DC. The new object replaces the previous object of the same type.
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY); //performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context.
																								  // save bitmap to file
	PBITMAPINFO pbmi = createBitmapInfoStruct(hBitmap);
	ofPixels pixels = convertToOfImage(hBitmap, hDC, pbmi);
	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return pixels;
};