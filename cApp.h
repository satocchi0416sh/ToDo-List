#pragma once
#include <wx/wx.h>
#include "MainWindow.h"

class cApp : public wxApp{
public:
	MainWindow* window = nullptr;

	virtual bool OnInit();
};

