#pragma once

#include "ofMain.h"
#include "D:\_godetia\y3\CAP\gitlab\exploration\code\desktopGrab_250117\screenCapture.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		CComPtrCustom<ID3D11Device> lDevice;
		CComPtrCustom<ID3D11DeviceContext> lImmediateContext;
		CComPtrCustom<IDXGIOutputDuplication> lDeskDupl;
		CComPtrCustom<ID3D11Texture2D> lAcquiredDesktopImage;
		CComPtrCustom<ID3D11Texture2D> lGDIImage;
		CComPtrCustom<ID3D11Texture2D> lDestImage;
		DXGI_OUTPUT_DESC lOutputDesc;
		DXGI_OUTDUPL_DESC lOutputDuplDesc;
		
};
