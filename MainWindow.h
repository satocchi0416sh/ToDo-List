#pragma once
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "Task.h"
#include <vector>
#include <string>

enum FilterMode
{
	FILTER_ALL,
	FILTER_COMPLETED,
	FILTER_INCOMPLETE
};

// データ保存形式の列挙型
enum StorageFormat
{
	FORMAT_TEXT,
	FORMAT_JSON
};

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

	// 検索機能のためのコントロール
	wxTextCtrl *SearchBox = nullptr;
	wxButton *ClearSearchButton = nullptr;
	wxStaticText *SearchLabel = nullptr;

	// 編集機能のためのコントロール
	wxButton *EditButton = nullptr;
	int EditingIndex = -1;
	bool IsEditing = false;
	wxString OriginalTaskText = "";

	// フィルタリング機能のためのコントロール
	wxRadioBox *FilterRadioBox = nullptr;
	FilterMode CurrentFilter = FILTER_ALL;

	// データ保存形式選択のためのコントロール
	wxRadioBox *FormatRadioBox = nullptr;
	StorageFormat CurrentFormat = FORMAT_JSON; // デフォルトでJSON形式を使用

	wxBoxSizer *OuterSizer = nullptr;
	wxBoxSizer *InnerSizer = nullptr;
	wxBoxSizer *InputSizer = nullptr;
	wxBoxSizer *SearchSizer = nullptr;
	wxBoxSizer *FilterSizer = nullptr;
	wxBoxSizer *FormatSizer = nullptr;
	wxBoxSizer *ButtonSizer = nullptr;

	std::vector<Task> TaskList = {};
	std::vector<Task> FilteredTaskList = {};

	void AddControls();
	void AddSizers();
	void BindEventHandlers();
	void LoadTaskFromFile();

	void OnMouseClick(wxMouseEvent &evt);
	void OnAddButtonClick(wxCommandEvent &evt);
	void OnClearButtonClick(wxCommandEvent &evt);
	void OnDelKeyPress(wxKeyEvent &evt);
	void OnEnterKeyPress(wxKeyEvent &evt);
	void OnNavigationKeyPress(wxKeyEvent &evt);
	void OnWindowClose(wxCloseEvent &evt);

	// 新しいイベントハンドラ
	void OnEditButtonClick(wxCommandEvent &evt);
	void OnSearchTextChanged(wxCommandEvent &evt);
	void OnClearSearchButtonClick(wxCommandEvent &evt);
	void OnFilterChanged(wxCommandEvent &evt);
	void OnFormatChanged(wxCommandEvent &evt); // 保存形式変更イベント
	void OnCheckListDoubleClick(wxCommandEvent &evt);
	void OnCheckListItemCheck(wxCommandEvent &evt);

	void AddTaskToChecklist();
	void SaveTaskInFile();

	// 新しいメソッド
	void EnterEditMode(int index);
	void ExitEditMode(bool saveChanges);
	void UpdateTaskDisplay();
	void ApplyFilter();
	void UpdateCheckList();
};
