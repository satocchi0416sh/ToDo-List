#pragma once
#include <string>
#include <vector>
#include <fstream>

struct Task
{
    std::string TaskName;
    bool IsChecked;
};

void WriteToFile(const std::string filename, const std::vector<Task> TaskList);
std::vector<Task> ReadFromFile(const std::string filename);