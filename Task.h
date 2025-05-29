#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

struct Task
{
    std::string TaskName;
    bool IsChecked;
};

void WriteToFile(const std::string filename, const std::vector<Task> TaskList);
std::vector<Task> ReadFromFile(const std::string filename);

void WriteToJsonFile(const std::string &filename, const std::vector<Task> &TaskList);
std::vector<Task> ReadFromJsonFile(const std::string &filename);

nlohmann::json TaskToJson(const Task &task);
Task JsonToTask(const nlohmann::json &j);