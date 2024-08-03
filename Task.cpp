#pragma once
#include "Task.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

void WriteToFile(const std::string filename, const std::vector<Task> TaskList){
    std::ofstream FileWrite(filename + ".txt");
    if (FileWrite.is_open()){
        FileWrite << TaskList.size();
        
        for (int i = 0; i < TaskList.size(); i++){
            std::string Description = TaskList[i].TaskName;
            std::replace(Description.begin(), Description.end(), ' ', '_');
            FileWrite << "\n" << Description << ' ' << TaskList[i].IsChecked; 
        }
    }
    
    FileWrite.close();
}

std::vector<Task> ReadFromFile(const std::string filename){
    std::ifstream FileRead(filename + ".txt");
    std::vector <Task> TaskList = {};

    if (FileRead.is_open() && !FileRead.eof()){
        int n;
        std::string Description;
        bool IsCheck;

        FileRead >> n;

        for (int i = 0; i < n; i++){
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
