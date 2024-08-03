#pragma once
#include "MainWindow.h"
#include <wx/wx.h>
#include "Task.h"
#include <string>
#include <fstream>

MainWindow::MainWindow(const wxString& title): wxFrame(nullptr, wxID_ANY, title) {
	AddControls();
	AddSizers();
	BindEventHandlers();
	LoadTaskFromTxtFile();
}


void MainWindow::AddControls(){
	// 0 means choose a suitable width.
	// 36 means the font is 36 pixels tall.
	HeadlineFont = new wxFont(wxFontInfo(wxSize(0, 36)).Bold());
	MainFont = new wxFont(wxFontInfo(wxSize(0, 24)));


	Panel = new wxPanel(this);
	Panel -> SetFont(*MainFont);

	Heading = new wxTextCtrl(Panel, wxID_ANY, "TO - DO LIST", wxDefaultPosition, 
				wxDefaultSize, wxTE_CENTRE | wxTE_READONLY | wxBORDER_NONE);
	Heading -> SetFont(*HeadlineFont);

	TextBox = new wxTextCtrl(Panel, wxID_ANY, "Enter a task here", wxDefaultPosition, 
				wxDefaultSize, wxTE_PROCESS_ENTER);

	AddButton = new wxButton(Panel, wxID_ANY, "Add");

	CheckList = new wxCheckListBox(Panel, wxID_ANY);

	ClearButton = new wxButton(Panel, wxID_ANY, "Clear");
}


void MainWindow::AddSizers(){
	OuterSizer = new wxBoxSizer(wxHORIZONTAL);
	InnerSizer = new wxBoxSizer(wxVERTICAL);

	InnerSizer -> Add(Heading, wxSizerFlags().Expand());
	InnerSizer -> AddSpacer(25);

	InputSizer = new wxBoxSizer(wxHORIZONTAL);
	InputSizer -> Add(TextBox, wxSizerFlags().Proportion(1));
	InputSizer -> AddSpacer(5);
	InputSizer -> Add(AddButton);
	
	InnerSizer -> Add(InputSizer, wxSizerFlags().Expand());
	InnerSizer -> AddSpacer(10);
	InnerSizer -> Add(CheckList, wxSizerFlags().Proportion(1).Expand());
	InnerSizer -> AddSpacer(10);
	InnerSizer -> Add(ClearButton);

	OuterSizer -> Add(InnerSizer, wxSizerFlags().Proportion(1).Expand().Border(wxALL,25));

	Panel -> SetSizer(OuterSizer);
	OuterSizer -> SetSizeHints(this); // The frame now respects the min size of OuterSizer

}


void MainWindow::BindEventHandlers(){
	Panel -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	Heading -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	TextBox -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	AddButton -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	CheckList -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	ClearButton -> Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	
	TextBox -> Bind(wxEVT_KEY_DOWN, &MainWindow::OnEnterKeyPress, this);
	AddButton -> Bind(wxEVT_BUTTON, &MainWindow::OnAddButtonClick, this);
	CheckList -> Bind(wxEVT_KEY_DOWN, &MainWindow::OnDelKeyPress, this);
	CheckList -> Bind(wxEVT_KEY_DOWN, &MainWindow::OnNavigationKeyPress, this);
	ClearButton -> Bind(wxEVT_BUTTON, &MainWindow::OnClearButtonClick, this);

	this -> Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnWindowClose, this);
}


void MainWindow::LoadTaskFromTxtFile(){
	TaskList = ReadFromFile("tasks");

	for (int i = 0; i < TaskList.size(); i++){
		CheckList -> AppendString(TaskList[i].TaskName);
		CheckList -> Check(i, TaskList[i].IsChecked);
	}
}



void MainWindow::OnMouseClick(wxMouseEvent &evt){
	if (CheckList -> GetSelection() != -1){
		// An item in the checklist is selected.
		CheckList -> DeselectAll();
	}
	evt.Skip();
}


void MainWindow::AddTaskToChecklist(){
	wxString Task = TextBox -> GetValue(); // Can also use GetLineText(0);
	if (!Task.IsEmpty()){
		CheckList -> AppendString(Task);
		// If we want to insert the new task at top instead of bottom, 
		// we can use Insert(Task, 0) function instead.
		TextBox -> Clear();
	}
	TextBox -> SetFocus();
}


void inline MainWindow::OnAddButtonClick(wxCommandEvent &evt){
	AddTaskToChecklist();
}


void MainWindow::OnEnterKeyPress(wxKeyEvent &evt){
	if (evt.GetKeyCode() == WXK_RETURN || evt.GetKeyCode() == WXK_NUMPAD_ENTER  
					&& TextBox -> HasFocus()){
		AddTaskToChecklist();
	}

	evt.Skip();
}


void MainWindow::OnDelKeyPress(wxKeyEvent &evt){
	int SelectedItemIndex = CheckList -> GetSelection();
	int TotalItems = CheckList -> GetCount(); 
	
	if (evt.GetKeyCode() == WXK_DELETE && SelectedItemIndex != wxNOT_FOUND){
		CheckList -> Delete(SelectedItemIndex);

		if (SelectedItemIndex == TotalItems - 1 && SelectedItemIndex - 1 >= 0){
			// If the list has more than two items and the last item is deleted,
			// then the second last items gets selected.
			CheckList -> Select(SelectedItemIndex - 1);
		}
		else if (!CheckList -> IsEmpty()){
			// If the deleted item was not the last item, then select the next
			// item.
			CheckList -> Select(SelectedItemIndex);

			// SelectedItemIndex is used instead of SelectedItemIndex + 1 because
			// when we delete (lets say) 0th element, then 1st element will 
			// become 0th element.
		}
	}

	if (CheckList -> IsEmpty()){
		TextBox -> SetFocus();
	}
}


void MainWindow::OnNavigationKeyPress(wxKeyEvent &evt){
	int SelectedItemIndex = CheckList -> GetSelection();
	if (SelectedItemIndex == wxNOT_FOUND){
		return;
	}
	
	int KeyCode = evt.GetKeyCode();
	int TotalItems = CheckList -> GetCount();

	switch (KeyCode){
	case WXK_UP:
		CheckList -> Deselect(SelectedItemIndex);
		if (SelectedItemIndex == 0){
			CheckList -> Select(TotalItems - 1);
		}else{
			CheckList -> Select(SelectedItemIndex - 1);
		}
		break;
	
	case WXK_DOWN:
		CheckList -> Deselect(SelectedItemIndex);
		if (SelectedItemIndex == TotalItems - 1){
			CheckList -> Select(0);
		}else{
			CheckList -> Select(SelectedItemIndex + 1);
		}
		break;
	}

	evt.Skip();
}


void MainWindow::OnClearButtonClick(wxCommandEvent &evt){
	while (!CheckList -> IsEmpty()){
		CheckList -> Delete(0);
	}
	TextBox -> SetFocus();
}


void MainWindow::SaveTaskInTxtFile(){
	TaskList.clear();

	for (int i = 0; i < CheckList -> GetCount(); i++){
		Task TempTask;
		TempTask.TaskName = CheckList -> GetString(i);
		TempTask.IsChecked = CheckList -> IsChecked(i);

		TaskList.push_back(TempTask);
	}

	WriteToFile("tasks", TaskList);	
}


void MainWindow::OnWindowClose(wxCloseEvent &evt){
	SaveTaskInTxtFile();
	evt.Skip();
}