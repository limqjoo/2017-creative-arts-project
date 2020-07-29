#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	int lresult(-1);
}

//--------------------------------------------------------------
void ofApp::update(){
	do
	{
		D3D_FEATURE_LEVEL lFeatureLevel;

		HRESULT hr(E_FAIL);

		// Create device
		for (UINT DriverTypeIndex = 0; DriverTypeIndex < gNumDriverTypes; ++DriverTypeIndex)
		{
			hr = D3D11CreateDevice(
				nullptr,
				gDriverTypes[DriverTypeIndex],
				nullptr,
				0,
				gFeatureLevels,
				gNumFeatureLevels,
				D3D11_SDK_VERSION,
				&lDevice,
				&lFeatureLevel,
				&lImmediateContext);

			if (SUCCEEDED(hr))
			{
				// Device creation success, no need to loop anymore
				break;
			}

			lDevice.Release();

			lImmediateContext.Release();
		}

		if (FAILED(hr))
			break;

		Sleep(100);

		if (lDevice == nullptr)
			break;

		// Get DXGI device
		CComPtrCustom<IDXGIDevice> lDxgiDevice;

		hr = lDevice->QueryInterface(IID_PPV_ARGS(&lDxgiDevice));

		if (FAILED(hr))
			break;

		// Get DXGI adapter
		CComPtrCustom<IDXGIAdapter> lDxgiAdapter;
		hr = lDxgiDevice->GetParent(
			__uuidof(IDXGIAdapter),
			reinterpret_cast<void**>(&lDxgiAdapter));

		if (FAILED(hr))
			break;

		lDxgiDevice.Release();

		UINT Output = 0;

		// Get output
		CComPtrCustom<IDXGIOutput> lDxgiOutput;
		hr = lDxgiAdapter->EnumOutputs(
			Output,
			&lDxgiOutput);

		if (FAILED(hr))
			break;

		lDxgiAdapter.Release();

		hr = lDxgiOutput->GetDesc(
			&lOutputDesc);

		if (FAILED(hr))
			break;

		// QI for Output 1
		CComPtrCustom<IDXGIOutput1> lDxgiOutput1;

		hr = lDxgiOutput->QueryInterface(IID_PPV_ARGS(&lDxgiOutput1));

		if (FAILED(hr))
			break;

		lDxgiOutput.Release();

		// Create desktop duplication
		hr = lDxgiOutput1->DuplicateOutput(
			lDevice,
			&lDeskDupl);

		if (FAILED(hr))
			break;

		lDxgiOutput1.Release();

		// Create GUI drawing texture
		lDeskDupl->GetDesc(&lOutputDuplDesc);

		D3D11_TEXTURE2D_DESC desc;

		desc.Width = lOutputDuplDesc.ModeDesc.Width;

		desc.Height = lOutputDuplDesc.ModeDesc.Height;

		desc.Format = lOutputDuplDesc.ModeDesc.Format;

		desc.ArraySize = 1;

		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

		desc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

		desc.SampleDesc.Count = 1;

		desc.SampleDesc.Quality = 0;

		desc.MipLevels = 1;

		desc.CPUAccessFlags = 0;

		desc.Usage = D3D11_USAGE_DEFAULT;

		hr = lDevice->CreateTexture2D(&desc, NULL, &lGDIImage);

		if (FAILED(hr))
			break;

		if (lGDIImage == nullptr)
			break;


		// Create CPU access texture

		desc.Width = lOutputDuplDesc.ModeDesc.Width;

		desc.Height = lOutputDuplDesc.ModeDesc.Height;

		desc.Format = lOutputDuplDesc.ModeDesc.Format;

		desc.ArraySize = 1;

		desc.BindFlags = 0;

		desc.MiscFlags = 0;

		desc.SampleDesc.Count = 1;

		desc.SampleDesc.Quality = 0;

		desc.MipLevels = 1;

		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = lDevice->CreateTexture2D(&desc, NULL, &lDestImage);

		if (FAILED(hr))
			break;

		if (lDestImage == nullptr)
			break;

		CComPtrCustom<IDXGIResource> lDesktopResource;
		DXGI_OUTDUPL_FRAME_INFO lFrameInfo;

		int lTryCount = 4;

		do
		{

			Sleep(100);

			// Get new frame
			hr = lDeskDupl->AcquireNextFrame(
				250,
				&lFrameInfo,
				&lDesktopResource);

			if (SUCCEEDED(hr))
				break;

			if (hr == DXGI_ERROR_WAIT_TIMEOUT)
			{
				continue;
			}
			else if (FAILED(hr))
				break;

		} while (--lTryCount > 0);

		if (FAILED(hr))
			break;

		// QI for ID3D11Texture2D

		hr = lDesktopResource->QueryInterface(IID_PPV_ARGS(&lAcquiredDesktopImage));

		if (FAILED(hr))
			break;

		lDesktopResource.Release();

		if (lAcquiredDesktopImage == nullptr)
			break;

		// Copy image into GDI drawing texture

		lImmediateContext->CopyResource(lGDIImage, lAcquiredDesktopImage);


		// Draw cursor image into GDI drawing texture

		CComPtrCustom<IDXGISurface1> lIDXGISurface1;

		hr = lGDIImage->QueryInterface(IID_PPV_ARGS(&lIDXGISurface1));

		if (FAILED(hr))
			break;

		CURSORINFO lCursorInfo = { 0 };

		lCursorInfo.cbSize = sizeof(lCursorInfo);

		auto lBoolres = GetCursorInfo(&lCursorInfo);

		if (lBoolres == TRUE)
		{
			if (lCursorInfo.flags == CURSOR_SHOWING)
			{
				auto lCursorPosition = lCursorInfo.ptScreenPos;

				auto lCursorSize = lCursorInfo.cbSize;

				HDC  lHDC;

				lIDXGISurface1->GetDC(FALSE, &lHDC);

				DrawIconEx(
					lHDC,
					lCursorPosition.x,
					lCursorPosition.y,
					lCursorInfo.hCursor,
					0,
					0,
					0,
					0,
					DI_NORMAL | DI_DEFAULTSIZE);

				lIDXGISurface1->ReleaseDC(nullptr);
			}

		}

		// Copy image into CPU access texture

		lImmediateContext->CopyResource(lDestImage, lGDIImage);


		// Copy from CPU access texture to bitmap buffer

		D3D11_MAPPED_SUBRESOURCE resource;
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		lImmediateContext->Map(lDestImage, subresource, D3D11_MAP_READ_WRITE, 0, &resource);

		BITMAPINFO	lBmpInfo;

		// BMP 32 bpp

		ZeroMemory(&lBmpInfo, sizeof(BITMAPINFO));

		lBmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		lBmpInfo.bmiHeader.biBitCount = 32;

		lBmpInfo.bmiHeader.biCompression = BI_RGB;

		lBmpInfo.bmiHeader.biWidth = lOutputDuplDesc.ModeDesc.Width;

		lBmpInfo.bmiHeader.biHeight = lOutputDuplDesc.ModeDesc.Height;

		lBmpInfo.bmiHeader.biPlanes = 1;

		lBmpInfo.bmiHeader.biSizeImage = lOutputDuplDesc.ModeDesc.Width
			* lOutputDuplDesc.ModeDesc.Height * 4;


		std::unique_ptr<BYTE> pBuf(new BYTE[lBmpInfo.bmiHeader.biSizeImage]);


		UINT lBmpRowPitch = lOutputDuplDesc.ModeDesc.Width * 4;

		BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
		BYTE* dptr = pBuf.get() + lBmpInfo.bmiHeader.biSizeImage - lBmpRowPitch;

		UINT lRowPitch = std::min<UINT>(lBmpRowPitch, resource.RowPitch);


		for (size_t h = 0; h < lOutputDuplDesc.ModeDesc.Height; ++h)
		{

			memcpy_s(dptr, lBmpRowPitch, sptr, lRowPitch);
			sptr += resource.RowPitch;
			dptr -= lBmpRowPitch;
		}

		// Save bitmap buffer into the file ScreenShot.bmp

		WCHAR lMyDocPath[MAX_PATH];

		hr = SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, lMyDocPath);

		if (FAILED(hr))
			break;

		std::wstring lFilePath = std::wstring(lMyDocPath) + L"\\ScreenShot.bmp";



		FILE* lfile = nullptr;

		auto lerr = _wfopen_s(&lfile, lFilePath.c_str(), L"wb");

		if (lerr != 0)
			break;

		if (lfile != nullptr)
		{

			BITMAPFILEHEADER	bmpFileHeader;

			bmpFileHeader.bfReserved1 = 0;
			bmpFileHeader.bfReserved2 = 0;
			bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lBmpInfo.bmiHeader.biSizeImage;
			bmpFileHeader.bfType = 'MB';
			bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

			fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, lfile);
			fwrite(&lBmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, lfile);
			fwrite(pBuf.get(), lBmpInfo.bmiHeader.biSizeImage, 1, lfile);

			fclose(lfile);

			ShellExecute(0, 0, lFilePath.c_str(), 0, 0, SW_SHOW);

			lresult = 0;

		}

	} while (false);

	return lresult;
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
