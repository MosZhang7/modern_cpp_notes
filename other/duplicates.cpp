#include <bits/stdc++.h>
#include <openssl/sha.h>

#include <atomic>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;
// ------------------------------------------------------------
// SHA256 工具函数
// ------------------------------------------------------------
std::string sha256_of_file(const fs::path& path) {
  FILE* fp = fopen(path.string().c_str(), "rb");
  if (!fp) return "";

  SHA256_CTX ctx;
  SHA256_Init(&ctx);

  const size_t BUF_SZ = 1024 * 1024;
  std::vector<unsigned char> buf(BUF_SZ);

  while (true) {
    size_t n = fread(buf.data(), 1, BUF_SZ, fp);
    if (n == 0) break;
    SHA256_Update(&ctx, buf.data(), n);
  }
  fclose(fp);

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_Final(hash, &ctx);

  std::ostringstream oss;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return oss.str();
}

// ------------------------------------------------------------
// 多线程任务：计算 SHA 并写入 thread-local 文件
// ------------------------------------------------------------
struct Task {
  std::vector<fs::path> files;
  int thread_id;
  fs::path out_file;
};

void worker(Task t) {
  std::ofstream ofs(t.out_file, std::ios::app);

  for (auto& f : t.files) {
    std::string h = sha256_of_file(f);
    if (!h.empty()) {
      ofs << h << " " << f.string() << "\n";
    }
  }
}

// ------------------------------------------------------------
// 工具主流程
// ------------------------------------------------------------
int main(int argc, char** argv) {
  fs::path scan_dir = "/mnt/d/ZYY/images";
  fs::path out_dir = "/mnt/d/ZYY/images_duplicates";

  if (argc < 3) {
    std::cout << "Usage: stest <scan_dir> <out_dir>\n";
    std::cout << "Using default paths:\n";
    std::cout << "  scan_dir: " << scan_dir << "\n";
    std::cout << "  out_dir:  " << out_dir << "\n";
  } else {
    scan_dir = fs::path(argv[1]);
    out_dir = fs::path(argv[2]);
  }

  if (!fs::exists(out_dir)) {
    fs::create_directories(out_dir);
  }

  // --------------------------------------------------------
  // 1) 按大小分桶
  // --------------------------------------------------------
  std::unordered_map<uintmax_t, std::vector<fs::path>> buckets;

  std::cout << "[1] Scanning files..." << std::endl;

  for (auto& p : fs::recursive_directory_iterator(scan_dir)) {
    if (p.is_regular_file()) {
      try {
        uintmax_t sz = fs::file_size(p.path());
        buckets[sz].push_back(p.path());
      } catch (...) {
      }
    }
  }

  std::cout << "Total file size buckets: " << buckets.size() << "\n";

  // --------------------------------------------------------
  // 2) 针对每个大小分组进行处理（外排 + 多线程）
  // --------------------------------------------------------
  int hw = std::thread::hardware_concurrency();
  if (hw <= 0) hw = 4;

  std::cout << "[2] Using " << hw << " threads\n";

  for (auto& [sz, vec] : buckets) {
    if (vec.size() <= 1) continue;

    std::cout << "Processing size group: " << sz << "  (" << vec.size()
              << " files)\n";

    // 分桶文件名
    fs::path raw_hash_file = "hash_tmp_" + std::to_string(sz) + ".txt";

    // 删除旧文件
    if (fs::exists(raw_hash_file)) fs::remove(raw_hash_file);

    // ----------------------------------------------------
    // 多线程：每个线程写到自己的 hash 文件
    // ----------------------------------------------------
    std::vector<std::thread> threads;
    std::vector<fs::path> thread_files;

    size_t total = vec.size();
    size_t chunk = (total + hw - 1) / hw;

    for (int i = 0; i < hw; i++) {
      size_t begin = i * chunk;
      if (begin >= total) break;
      size_t end = std::min(begin + chunk, total);

      Task t;
      t.thread_id = i;
      t.out_file = fs::path("hash_tmp_" + std::to_string(sz) + "_" +
                            std::to_string(i) + ".txt");
      thread_files.push_back(t.out_file);

      for (size_t k = begin; k < end; k++) {
        t.files.push_back(vec[k]);
      }

      threads.emplace_back(worker, t);
    }

    // 等待线程
    for (auto& th : threads) th.join();

    // ----------------------------------------------------
    // 合并 thread 文件 -> 一个大 hash 文件
    // ----------------------------------------------------
    {
      std::ofstream ofs(raw_hash_file);
      for (auto& tf : thread_files) {
        std::ifstream ifs(tf);
        ofs << ifs.rdbuf();
        fs::remove(tf);
      }
    }

    // ----------------------------------------------------
    // 外部排序（系统 sort 非常快，不吃内存）
    // ----------------------------------------------------
    fs::path sorted_file = "hash_tmp_" + std::to_string(sz) + "_sorted.txt";

    {
      std::string cmd =
          "sort " + raw_hash_file.string() + " > " + sorted_file.string();
      system(cmd.c_str());
    }

    fs::remove(raw_hash_file);

    // ----------------------------------------------------
    // 单次扫描寻找重复 SHA
    // ----------------------------------------------------
    std::ifstream ifs(sorted_file);
    std::string line;

    std::string last_hash;
    std::vector<std::string> dup_group;

    while (std::getline(ifs, line)) {
      if (line.size() < 10) continue;

      std::string hash = line.substr(0, 64);
      std::string fname = line.substr(65);

      if (hash == last_hash) {
        dup_group.push_back(fname);
      } else {
        if (dup_group.size() >= 2) {
          // 处理重复文件
          for (size_t i = 1; i < dup_group.size(); i++) {
            fs::path target = out_dir / fs::path(dup_group[i]).filename();
            try {
              fs::rename(dup_group[i], target);
            } catch (...) {
            }
          }
        }
        // 开始新的组
        dup_group.clear();
        dup_group.push_back(fname);
      }

      last_hash = hash;
    }

    // 最后一组检查
    if (dup_group.size() >= 2) {
      for (size_t i = 1; i < dup_group.size(); i++) {
        fs::path target = out_dir / fs::path(dup_group[i]).filename();
        try {
          fs::rename(dup_group[i], target);
        } catch (...) {
        }
      }
    }

    fs::remove(sorted_file);
  }

  std::cout << "DONE.\n";
  return 0;
}
