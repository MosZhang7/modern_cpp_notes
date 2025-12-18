// 递归地遍历/mnt/d/ZYY/images目录下所有的文件，找到所有文件名相同的文件，并打印它们的完整路径
#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::string directory = "/mnt/d/ZYY/images";
  std::string duplicate_directory = "/mnt/d/ZYY/small_duplicates";
  std::string outpath = "duplicates.txt";
  if (argc >= 2) directory = argv[1];
  if (argc >= 3) duplicate_directory = argv[2];

  if (!fs::exists(directory)) {
    std::cerr << "Directory does not exist: " << directory << std::endl;
    return 1;
  }

  if (!fs::exists(duplicate_directory)) {
    fs::create_directories(duplicate_directory);
  }

  std::ofstream out(outpath, std::ios::trunc);
  if (!out) {
    std::cerr << "Failed to open output file: " << outpath << std::endl;
    return 1;
  }

  std::map<std::string, std::vector<std::string>> filename_map;

  for (const auto& entry : fs::recursive_directory_iterator(directory)) {
    if (entry.is_regular_file()) {
      // 忽略后缀名的不同
      std::string filename = entry.path().filename().string();
      filename_map[filename].push_back(entry.path().string());
    }
  }
  for (const auto& [filename, paths] : filename_map) {
    if (paths.size() > 1) {
      out << "Duplicate filename: " << filename << "\n";
      for (const auto& path : paths) {
        out << "  " << path << " size: " << fs::file_size(path) << " bytes\n";
      }

      // 保留最大的文件，移动其他重复文件
      auto max_it =
          std::max_element(paths.begin(), paths.end(),
                           [](const std::string& a, const std::string& b) {
                             return fs::file_size(a) < fs::file_size(b);
                           });
      for (const auto& path : paths) {
        if (path != *max_it &&
            fs::file_size(path) < fs::file_size(*max_it) / 2) {
          fs::path target =
              fs::path(duplicate_directory) / fs::path(path).filename();
          try {
            fs::rename(path, target);
            out << "move" << path << "duplicates to: " << duplicate_directory
                << "\n";
          } catch (const std::exception& e) {
            out << "  Failed to move " << path << " to " << target << ": "
                << e.what() << "\n";
          }
        }
      }
    }
  }

  // Also echo to stdout for convenience
  std::ifstream ifs(outpath);
  if (ifs) {
    std::string line;
    while (std::getline(ifs, line)) std::cout << line << std::endl;
  }

  std::cout << "Results written to: " << outpath << std::endl;
  return 0;
}
