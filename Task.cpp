#pragma once
#include "Task.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

void WriteToFile(const std::string filename, const std::vector<Task> TaskList)
{
    std::ofstream FileWrite(filename + ".txt");
    if (FileWrite.is_open())
    {
        FileWrite << TaskList.size();

        for (int i = 0; i < TaskList.size(); i++)
        {
            std::string Description = TaskList[i].TaskName;
            std::replace(Description.begin(), Description.end(), ' ', '_');
            FileWrite << "\n"
                      << Description << ' ' << TaskList[i].IsChecked;
        }
    }

    FileWrite.close();
}

std::vector<Task> ReadFromFile(const std::string filename)
{
    std::ifstream FileRead(filename + ".txt");
    std::vector<Task> TaskList = {};

    if (FileRead.is_open() && !FileRead.eof())
    {
        int n;
        std::string Description;
        bool IsCheck;

        FileRead >> n;

        for (int i = 0; i < n; i++)
        {
            FileRead >> Description >> IsCheck;
            std::replace(Description.begin(), Description.end(), '_', ' ');

            Task TempTask;
            TempTask.TaskName = Description;
            TempTask.IsChecked = IsCheck;

            TaskList.push_back(TempTask);
        }
    }
    FileRead.close();

    return TaskList;
}

// Task構造体とJSONの相互変換
nlohmann::json TaskToJson(const Task &task)
{
    nlohmann::json j;
    j["name"] = task.TaskName;
    j["completed"] = task.IsChecked;
    return j;
}

Task JsonToTask(const nlohmann::json &j)
{
    Task task;
    task.TaskName = j["name"].get<std::string>();
    task.IsChecked = j["completed"].get<bool>();
    return task;
}

// JSONファイルへの書き込み
void WriteToJsonFile(const std::string &filename, const std::vector<Task> &TaskList)
{
    nlohmann::json j;

    // タスクの配列をJSONに変換
    nlohmann::json tasksArray = nlohmann::json::array();
    for (const auto &task : TaskList)
    {
        tasksArray.push_back(TaskToJson(task));
    }

    j["tasks"] = tasksArray;
    j["count"] = TaskList.size();

    // JSONファイルに保存
    std::ofstream file(filename + ".json");
    if (file.is_open())
    {
        file << j.dump(4); // インデント付きで整形
        file.close();
    }
}

// JSONファイルからの読み込み
std::vector<Task> ReadFromJsonFile(const std::string &filename)
{
    std::vector<Task> TaskList;

    std::ifstream file(filename + ".json");
    if (file.is_open())
    {
        try
        {
            nlohmann::json j;
            file >> j;

            // タスク配列を取得して変換
            if (j.contains("tasks") && j["tasks"].is_array())
            {
                for (const auto &taskJson : j["tasks"])
                {
                    TaskList.push_back(JsonToTask(taskJson));
                }
            }
        }
        catch (nlohmann::json::exception &e)
        {
            // JSONのパースエラー処理
            // 実際のアプリではエラーログを出力するなどの処理を追加
        }
        file.close();
    }

    return TaskList;
}
