#include <gtest/gtest.h>
#include "Task.h"
#include <fstream>
#include <cstdio>
#include <vector>

// テスト用のファイル名
const std::string TEST_FILENAME = "test_tasks";

// テスト前後の処理
class TaskTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    // テスト前にテストファイルが存在する場合は削除
    std::remove((TEST_FILENAME + ".txt").c_str());
  }

  void TearDown() override
  {
    // テスト後にテストファイルを削除
    std::remove((TEST_FILENAME + ".txt").c_str());
  }
};

// Task構造体の基本テスト
TEST_F(TaskTest, BasicTaskStructure)
{
  Task task;
  task.TaskName = "テストタスク";
  task.IsChecked = false;

  EXPECT_EQ(task.TaskName, "テストタスク");
  EXPECT_FALSE(task.IsChecked);
}

// ファイル書き込みと読み込みのテスト
TEST_F(TaskTest, WriteAndReadFromFile)
{
  // テスト用のタスクリスト作成
  std::vector<Task> tasks;
  Task task1, task2;

  task1.TaskName = "買い物に行く";
  task1.IsChecked = true;

  task2.TaskName = "宿題をする";
  task2.IsChecked = false;

  tasks.push_back(task1);
  tasks.push_back(task2);

  // ファイルに書き込み
  WriteToFile(TEST_FILENAME, tasks);

  // ファイルから読み込み
  std::vector<Task> loadedTasks = ReadFromFile(TEST_FILENAME);

  // 検証
  ASSERT_EQ(loadedTasks.size(), 2);

  EXPECT_EQ(loadedTasks[0].TaskName, "買い物に行く");
  EXPECT_TRUE(loadedTasks[0].IsChecked);

  EXPECT_EQ(loadedTasks[1].TaskName, "宿題をする");
  EXPECT_FALSE(loadedTasks[1].IsChecked);
}

// 空のタスクリストの保存と読み込みテスト
TEST_F(TaskTest, EmptyTaskList)
{
  std::vector<Task> emptyTasks;

  // 空のリストを書き込み
  WriteToFile(TEST_FILENAME, emptyTasks);

  // 読み込み
  std::vector<Task> loadedTasks = ReadFromFile(TEST_FILENAME);

  // 検証
  EXPECT_TRUE(loadedTasks.empty());
}

// スペースを含むタスク名のテスト
TEST_F(TaskTest, TaskNameWithSpaces)
{
  std::vector<Task> tasks;
  Task task;

  task.TaskName = "これは スペースを 含む タスク名です";
  task.IsChecked = true;

  tasks.push_back(task);

  // ファイルに書き込み
  WriteToFile(TEST_FILENAME, tasks);

  // ファイルから読み込み
  std::vector<Task> loadedTasks = ReadFromFile(TEST_FILENAME);

  // 検証
  ASSERT_EQ(loadedTasks.size(), 1);
  EXPECT_EQ(loadedTasks[0].TaskName, "これは スペースを 含む タスク名です");
  EXPECT_TRUE(loadedTasks[0].IsChecked);
}

// 存在しないファイルからの読み込みテスト
TEST_F(TaskTest, ReadFromNonExistentFile)
{
  // 存在しないファイル名を指定
  std::string nonExistentFile = "non_existent_file";

  // ファイルが存在しないことを確認
  std::remove((nonExistentFile + ".txt").c_str());

  // 存在しないファイルから読み込み試行
  std::vector<Task> loadedTasks = ReadFromFile(nonExistentFile);

  // 空のベクトルが返されることを検証
  EXPECT_TRUE(loadedTasks.empty());
}

// 多数のタスクの読み書きテスト
TEST_F(TaskTest, ManyTasks)
{
  std::vector<Task> tasks;
  const int numTasks = 100;

  // 多数のタスクを作成
  for (int i = 0; i < numTasks; i++)
  {
    Task task;
    task.TaskName = "Task " + std::to_string(i);
    task.IsChecked = (i % 2 == 0); // 偶数インデックスのタスクはチェック済み
    tasks.push_back(task);
  }

  // 書き込み
  WriteToFile(TEST_FILENAME, tasks);

  // 読み込み
  std::vector<Task> loadedTasks = ReadFromFile(TEST_FILENAME);

  // 検証
  ASSERT_EQ(loadedTasks.size(), numTasks);

  for (int i = 0; i < numTasks; i++)
  {
    EXPECT_EQ(loadedTasks[i].TaskName, "Task " + std::to_string(i));
    EXPECT_EQ(loadedTasks[i].IsChecked, (i % 2 == 0));
  }
}

// 特殊文字を含むタスク名のテスト
TEST_F(TaskTest, TaskNameWithSpecialChars)
{
  std::vector<Task> tasks;
  Task task;

  // 特殊文字を含むタスク名（アンダースコアはスペースに変換されることに注意）
  task.TaskName = "特殊!@#$%^&*() +{}|:<>?";
  task.IsChecked = true;

  tasks.push_back(task);

  // 書き込み
  WriteToFile(TEST_FILENAME, tasks);

  // 読み込み
  std::vector<Task> loadedTasks = ReadFromFile(TEST_FILENAME);

  // 検証
  ASSERT_EQ(loadedTasks.size(), 1);
  // スペースとアンダースコアの変換を考慮して検証
  EXPECT_EQ(loadedTasks[0].TaskName, "特殊!@#$%^&*() +{}|:<>?");
  EXPECT_TRUE(loadedTasks[0].IsChecked);
}