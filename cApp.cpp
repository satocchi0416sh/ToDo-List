#pragma once
#include "cApp.h"
#include "MainWindow.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() {
	window = new MainWindow("C++ GUI");
	window -> SetClientSize(800, 600);
	//800x600 size doesn't count title bar and borders size,
	// unless we mention the size in the constructor itself.
	
	window -> Center();
	window -> Show();
	return true;
}


