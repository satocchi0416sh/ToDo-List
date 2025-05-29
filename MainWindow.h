#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "Task.h"
#include <vector>
#include <string>

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString &title);

	wxFont *HeadlineFont = nullptr;
	wxFont *MainFont = nullptr;

	wxPanel *Panel = nullptr;
	wxTextCtrl *Heading = nullptr;
	wxTextCtrl *TextBox = nullptr;
	wxButton *AddButton = nullptr;
	wxCheckListBox *CheckList = nullptr;
	wxButton *ClearButton = nullptr;

	wxBoxSizer *OuterSizer = nullptr;
	wxBoxSizer *InnerSizer = nullptr;
	wxBoxSizer *InputSizer = nullptr;

	std::vector<Task> TaskList = {};

	void AddControls();
	void AddSizers();
	void BindEventHandlers();
	void LoadTaskFromTxtFile();

	void OnMouseClick(wxMouseEvent &evt);
	void OnAddButtonClick(wxCommandEvent &evt);
	void OnClearButtonClick(wxCommandEvent &evt);
	void OnDelKeyPress(wxKeyEvent &evt);
	void OnEnterKeyPress(wxKeyEvent &evt);
	void OnNavigationKeyPress(wxKeyEvent &evt);
	void OnWindowClose(wxCloseEvent &evt);

	void AddTaskToChecklist();
	void SaveTaskInTxtFile();
};
