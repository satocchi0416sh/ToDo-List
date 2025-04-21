#pragma once
#include "MainWindow.h"
#include <wx/wx.h>
#include "Task.h"
#include <string>
#include <fstream>
#include <algorithm>

MainWindow::MainWindow(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
	AddControls();
	AddSizers();
	BindEventHandlers();
	LoadTaskFromTxtFile();
	UpdateTaskDisplay();
}

void MainWindow::AddControls()
{
	// 0 means choose a suitable width.
	// 36 means the font is 36 pixels tall.
	HeadlineFont = new wxFont(wxFontInfo(wxSize(0, 36)).Bold());
	MainFont = new wxFont(wxFontInfo(wxSize(0, 24)));

	Panel = new wxPanel(this);
	Panel->SetFont(*MainFont);

	Heading = new wxTextCtrl(Panel, wxID_ANY, "TO - DO LIST", wxDefaultPosition,
													 wxDefaultSize, wxTE_CENTRE | wxTE_READONLY | wxBORDER_NONE);
	Heading->SetFont(*HeadlineFont);

	// 検索機能のコントロール
	SearchLabel = new wxStaticText(Panel, wxID_ANY, "検索：");
	SearchBox = new wxTextCtrl(Panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	ClearSearchButton = new wxButton(Panel, wxID_ANY, "クリア", wxDefaultPosition, wxDefaultSize);

	TextBox = new wxTextCtrl(Panel, wxID_ANY, "Enter a task here", wxDefaultPosition,
													 wxDefaultSize, wxTE_PROCESS_ENTER);

	AddButton = new wxButton(Panel, wxID_ANY, "追加");
	EditButton = new wxButton(Panel, wxID_ANY, "編集");

	CheckList = new wxCheckListBox(Panel, wxID_ANY);

	// フィルタリング用のラジオボックス
	wxArrayString filterChoices;
	filterChoices.Add("すべて表示");
	filterChoices.Add("完了タスクのみ");
	filterChoices.Add("未完了タスクのみ");
	FilterRadioBox = new wxRadioBox(Panel, wxID_ANY, "フィルター",
																	wxDefaultPosition, wxDefaultSize,
																	filterChoices, 1, wxRA_SPECIFY_ROWS);

	ClearButton = new wxButton(Panel, wxID_ANY, "クリア");
}

void MainWindow::AddSizers()
{
	OuterSizer = new wxBoxSizer(wxHORIZONTAL);
	InnerSizer = new wxBoxSizer(wxVERTICAL);

	InnerSizer->Add(Heading, wxSizerFlags().Expand());
	InnerSizer->AddSpacer(10);

	// 検索コントロールの配置
	SearchSizer = new wxBoxSizer(wxHORIZONTAL);
	SearchSizer->Add(SearchLabel, wxSizerFlags().Border(wxRIGHT, 5).CenterVertical());
	SearchSizer->Add(SearchBox, wxSizerFlags().Proportion(1));
	SearchSizer->AddSpacer(5);
	SearchSizer->Add(ClearSearchButton);

	InnerSizer->Add(SearchSizer, wxSizerFlags().Expand());
	InnerSizer->AddSpacer(10);

	// タスク入力フォームの配置
	InputSizer = new wxBoxSizer(wxHORIZONTAL);
	InputSizer->Add(TextBox, wxSizerFlags().Proportion(1));
	InputSizer->AddSpacer(5);
	InputSizer->Add(AddButton);
	InputSizer->AddSpacer(5);
	InputSizer->Add(EditButton);

	InnerSizer->Add(InputSizer, wxSizerFlags().Expand());
	InnerSizer->AddSpacer(10);
	InnerSizer->Add(CheckList, wxSizerFlags().Proportion(1).Expand());
	InnerSizer->AddSpacer(10);

	// フィルターとクリアボタンの配置
	FilterSizer = new wxBoxSizer(wxHORIZONTAL);
	FilterSizer->Add(FilterRadioBox, wxSizerFlags().Proportion(1).Expand());
	FilterSizer->AddSpacer(10);
	FilterSizer->Add(ClearButton, wxSizerFlags().CenterVertical());

	InnerSizer->Add(FilterSizer, wxSizerFlags().Expand());

	OuterSizer->Add(InnerSizer, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 25));

	Panel->SetSizer(OuterSizer);
	OuterSizer->SetSizeHints(this); // The frame now respects the min size of OuterSizer
}

void MainWindow::BindEventHandlers()
{
	Panel->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	Heading->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	TextBox->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	AddButton->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	CheckList->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);
	ClearButton->Bind(wxEVT_LEFT_DOWN, &MainWindow::OnMouseClick, this);

	// 新しいイベントハンドラのバインド
	SearchBox->Bind(wxEVT_TEXT, &MainWindow::OnSearchTextChanged, this);
	ClearSearchButton->Bind(wxEVT_BUTTON, &MainWindow::OnClearSearchButtonClick, this);
	FilterRadioBox->Bind(wxEVT_RADIOBOX, &MainWindow::OnFilterChanged, this);
	EditButton->Bind(wxEVT_BUTTON, &MainWindow::OnEditButtonClick, this);
	CheckList->Bind(wxEVT_LISTBOX_DCLICK, &MainWindow::OnCheckListDoubleClick, this);
	CheckList->Bind(wxEVT_CHECKLISTBOX, &MainWindow::OnCheckListItemCheck, this);

	TextBox->Bind(wxEVT_KEY_DOWN, &MainWindow::OnEnterKeyPress, this);
	AddButton->Bind(wxEVT_BUTTON, &MainWindow::OnAddButtonClick, this);
	CheckList->Bind(wxEVT_KEY_DOWN, &MainWindow::OnDelKeyPress, this);
	CheckList->Bind(wxEVT_KEY_DOWN, &MainWindow::OnNavigationKeyPress, this);
	ClearButton->Bind(wxEVT_BUTTON, &MainWindow::OnClearButtonClick, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnWindowClose, this);
}

void MainWindow::LoadTaskFromTxtFile()
{
	TaskList = ReadFromFile("tasks");
	UpdateTaskDisplay();
}

void MainWindow::OnMouseClick(wxMouseEvent &evt)
{
	if (CheckList->GetSelection() != -1)
	{
		// An item in the checklist is selected.
		CheckList->DeselectAll();
	}
	evt.Skip();
}

void MainWindow::AddTaskToChecklist()
{
	wxString taskText = TextBox->GetValue(); // Can also use GetLineText(0);
	if (!taskText.IsEmpty())
	{
		// タスクをタスクリストに追加
		struct Task newTask;
		newTask.TaskName = taskText;
		newTask.IsChecked = false;
		TaskList.push_back(newTask);

		// 表示を更新
		UpdateTaskDisplay();

		TextBox->Clear();
	}
	TextBox->SetFocus();
}

void inline MainWindow::OnAddButtonClick(wxCommandEvent &evt)
{
	AddTaskToChecklist();
}

void MainWindow::OnEnterKeyPress(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == WXK_RETURN || evt.GetKeyCode() == WXK_NUMPAD_ENTER)
	{
		if (TextBox->HasFocus())
		{
			// タスク追加モード
			if (!IsEditing)
			{
				AddTaskToChecklist();
			}
			// タスク編集モード
			else
			{
				ExitEditMode(true);
			}
		}
		else if (SearchBox->HasFocus())
		{
			// 検索実行（すでにOnSearchTextChangedで処理されているため追加の処理は不要）
			TextBox->SetFocus();
		}
	}
	else if (evt.GetKeyCode() == WXK_ESCAPE && IsEditing)
	{
		// 編集モードを終了（変更を破棄）
		ExitEditMode(false);
	}

	evt.Skip();
}

void MainWindow::OnDelKeyPress(wxKeyEvent &evt)
{
	int SelectedItemIndex = CheckList->GetSelection();
	int TotalItems = CheckList->GetCount();

	if (evt.GetKeyCode() == WXK_DELETE && SelectedItemIndex != wxNOT_FOUND)
	{
		// 実際のフィルタされたリストのインデックスを取得
		int originalIndex = -1;
		if (SelectedItemIndex < FilteredTaskList.size())
		{
			// フィルタリストから元のタスクリストのインデックスを見つける
			wxString selectedTask = CheckList->GetString(SelectedItemIndex);
			for (size_t i = 0; i < TaskList.size(); i++)
			{
				if (TaskList[i].TaskName == selectedTask)
				{
					originalIndex = i;
					break;
				}
			}

			if (originalIndex != -1)
			{
				TaskList.erase(TaskList.begin() + originalIndex);
				UpdateTaskDisplay();

				// 削除後の選択位置を調整
				if (SelectedItemIndex == TotalItems - 1 && SelectedItemIndex - 1 >= 0)
				{
					CheckList->Select(SelectedItemIndex - 1);
				}
				else if (!CheckList->IsEmpty())
				{
					CheckList->Select(SelectedItemIndex);
				}
			}
		}
	}

	if (CheckList->IsEmpty())
	{
		TextBox->SetFocus();
	}
}

void MainWindow::OnNavigationKeyPress(wxKeyEvent &evt)
{
	int SelectedItemIndex = CheckList->GetSelection();
	if (SelectedItemIndex == wxNOT_FOUND)
	{
		return;
	}

	int KeyCode = evt.GetKeyCode();
	int TotalItems = CheckList->GetCount();

	switch (KeyCode)
	{
	case WXK_UP:
		CheckList->Deselect(SelectedItemIndex);
		if (SelectedItemIndex == 0)
		{
			CheckList->Select(TotalItems - 1);
		}
		else
		{
			CheckList->Select(SelectedItemIndex - 1);
		}
		break;

	case WXK_DOWN:
		CheckList->Deselect(SelectedItemIndex);
		if (SelectedItemIndex == TotalItems - 1)
		{
			CheckList->Select(0);
		}
		else
		{
			CheckList->Select(SelectedItemIndex + 1);
		}
		break;
	}

	evt.Skip();
}

void MainWindow::OnClearButtonClick(wxCommandEvent &evt)
{
	// 確認ダイアログを表示
	wxMessageDialog dialog(this, "すべてのタスクを削除してもよろしいですか？",
												 "確認", wxYES_NO | wxICON_QUESTION);
	int result = dialog.ShowModal();

	if (result == wxID_YES)
	{
		TaskList.clear();
		UpdateTaskDisplay();
		TextBox->SetFocus();
	}
}

// 検索テキストが変更された時のイベント
void MainWindow::OnSearchTextChanged(wxCommandEvent &evt)
{
	UpdateTaskDisplay();
}

// 検索クリアボタンがクリックされた時のイベント
void MainWindow::OnClearSearchButtonClick(wxCommandEvent &evt)
{
	SearchBox->Clear();
	UpdateTaskDisplay();
	TextBox->SetFocus();
}

// フィルターが変更された時のイベント
void MainWindow::OnFilterChanged(wxCommandEvent &evt)
{
	int selection = FilterRadioBox->GetSelection();
	switch (selection)
	{
	case 0:
		CurrentFilter = FILTER_ALL;
		break;
	case 1:
		CurrentFilter = FILTER_COMPLETED;
		break;
	case 2:
		CurrentFilter = FILTER_INCOMPLETE;
		break;
	}

	UpdateTaskDisplay();
}

// 編集ボタンがクリックされた時のイベント
void MainWindow::OnEditButtonClick(wxCommandEvent &evt)
{
	int selectedIndex = CheckList->GetSelection();
	if (selectedIndex != wxNOT_FOUND)
	{
		EnterEditMode(selectedIndex);
	}
	else
	{
		wxMessageBox("編集するタスクを選択してください。", "情報", wxOK | wxICON_INFORMATION);
	}
}

// チェックリストのアイテムをダブルクリックした時のイベント
void MainWindow::OnCheckListDoubleClick(wxCommandEvent &evt)
{
	int selectedIndex = evt.GetInt();
	if (selectedIndex != wxNOT_FOUND)
	{
		EnterEditMode(selectedIndex);
	}
}

// チェックリストのアイテムをチェック/アンチェックした時のイベント
void MainWindow::OnCheckListItemCheck(wxCommandEvent &evt)
{
	int checkedIndex = evt.GetInt();
	if (checkedIndex != wxNOT_FOUND && checkedIndex < FilteredTaskList.size())
	{
		// フィルタリストから元のタスクリストのインデックスを見つける
		wxString checkedTask = CheckList->GetString(checkedIndex);
		bool isChecked = CheckList->IsChecked(checkedIndex);

		for (size_t i = 0; i < TaskList.size(); i++)
		{
			if (TaskList[i].TaskName == checkedTask)
			{
				TaskList[i].IsChecked = isChecked;
				break;
			}
		}

		// フィルタが完了/未完了の場合は表示を更新
		if (CurrentFilter != FILTER_ALL)
		{
			UpdateTaskDisplay();
		}
	}
}

// 編集モードに入る
void MainWindow::EnterEditMode(int index)
{
	if (index >= 0 && index < FilteredTaskList.size())
	{
		// 元のタスクリストのインデックスを見つける
		wxString selectedTask = CheckList->GetString(index);
		int originalIndex = -1;

		for (size_t i = 0; i < TaskList.size(); i++)
		{
			if (TaskList[i].TaskName == selectedTask)
			{
				originalIndex = i;
				break;
			}
		}

		if (originalIndex != -1)
		{
			IsEditing = true;
			EditingIndex = originalIndex;
			OriginalTaskText = TaskList[originalIndex].TaskName;

			// 編集中のテキストを入力ボックスに設定
			TextBox->SetValue(OriginalTaskText);
			TextBox->SetFocus();
			TextBox->SelectAll();

			// ボタンのラベルを変更
			AddButton->SetLabel("保存");
			EditButton->SetLabel("キャンセル");
		}
	}
}

// 編集モードを終了
void MainWindow::ExitEditMode(bool saveChanges)
{
	if (saveChanges && EditingIndex >= 0 && EditingIndex < TaskList.size())
	{
		// 変更を保存
		TaskList[EditingIndex].TaskName = TextBox->GetValue();
	}

	// 編集状態をリセット
	IsEditing = false;
	EditingIndex = -1;
	TextBox->Clear();

	// ボタンのラベルを元に戻す
	AddButton->SetLabel("追加");
	EditButton->SetLabel("編集");

	// 表示を更新
	UpdateTaskDisplay();
}

// タスク表示を更新（検索とフィルタリングを適用）
void MainWindow::UpdateTaskDisplay()
{
	// 検索テキストを取得
	wxString searchText = SearchBox->GetValue().Lower();

	// フィルタリストをクリア
	FilteredTaskList.clear();

	// フィルタと検索条件に基づいてタスクをフィルタリング
	for (const Task &task : TaskList)
	{
		// 検索条件のチェック
		bool matchesSearch = searchText.IsEmpty() ||
												 wxString(task.TaskName).Lower().Find(searchText) != wxNOT_FOUND;

		// フィルタ条件のチェック
		bool matchesFilter = (CurrentFilter == FILTER_ALL) ||
												 (CurrentFilter == FILTER_COMPLETED && task.IsChecked) ||
												 (CurrentFilter == FILTER_INCOMPLETE && !task.IsChecked);

		// 両方の条件を満たす場合はフィルタリストに追加
		if (matchesSearch && matchesFilter)
		{
			FilteredTaskList.push_back(task);
		}
	}

	// チェックリストを更新
	UpdateCheckList();
}

// フィルタリングされたタスクリストでチェックリストを更新
void MainWindow::UpdateCheckList()
{
	// 現在のチェックリストをクリア
	CheckList->Clear();

	// フィルタリングされたタスクリストを使用してチェックリストを更新
	for (size_t i = 0; i < FilteredTaskList.size(); i++)
	{
		CheckList->AppendString(FilteredTaskList[i].TaskName);
		CheckList->Check(i, FilteredTaskList[i].IsChecked);
	}
}

void MainWindow::SaveTaskInTxtFile()
{
	WriteToFile("tasks", TaskList);
}

void MainWindow::OnWindowClose(wxCloseEvent &evt)
{
	SaveTaskInTxtFile();
	evt.Skip();
}