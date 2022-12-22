#include <limits.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;

struct Path {
  string path;

  Path(string path) : path(path) {}
};

struct File : Path {
  size_t sz;

  File(string path, size_t sz): Path(path), sz(sz) {}
};

struct Dir : Path {
  list<Dir> dirs = list<Dir>();
  list<File> files = list<File>();
  size_t totalSz = 0;

  Dir(string path) : Path(path) {}

  size_t updateSize() {
    totalSz = 0;
    for (auto& dir : dirs) {
      totalSz += dir.updateSize();
    }

    for (auto& file : files) {
      totalSz += file.sz;
    }

    return totalSz;
  }

  void print(int indent = 0) {
    string indentString { string(indent * 2, ' ') };
    cout << indentString << path << " [" << totalSz << "]" << endl;

    cout << indentString << "# Files (" << files.size() << "): " << endl;
    for (auto& file : files) {
      cout << indentString << "- " << file.path << " [" << file.sz << "]" << endl;
    }

    cout << indentString << "# Directories (" << dirs.size() << "): " << endl;
    for (auto& dir : dirs) {
      dir.print(indent + 1);
    }
  }
};

enum class Mode {
  Command,
  LSOutput,
};

class InputProcessor {
public:
  void process(istream &stream) {
    assertm("Stack not empty", stack_.empty());

    string line;
    stack_.push(&root_);

    while (getline(stream, line)) {
      stringstream linestream(line);

      string start;
      linestream >> start;

      if (start == "$") {
        mode_ = Mode::Command;
        processCommand(linestream);
      } else if (mode_ == Mode::LSOutput) {
        linestream.seekg(0);
        processEntry(linestream);
      } else {
        assertm("Unknown state while processing lines", false);
      }
    }

    root_.updateSize();
  }

  void processCommand(istream &line) {
    string cmd;
    line >> cmd;

    if (cmd == "ls") {
      mode_ = Mode::LSOutput;
    } else if (cmd == "cd") {
      string dir_change;
      line >> dir_change;
      if (dir_change == "..") {
        assertm("Going back on an empty stack", !stack_.empty());
        stack_.pop();
      } else if (dir_change == "/") {
        while (stack_.size() > 1)
          stack_.pop();
        assertm("Top of stack is not /",
                (stack_.top()->path.compare("/") == 0));
      } else {
        list<Dir> &cur_dirs = stack_.top()->dirs;
        auto it =
            find_if(cur_dirs.begin(), cur_dirs.end(),
                    [dir_change](auto x) { return x.path == dir_change; });
        if (it != cur_dirs.end()) {
          auto elem = *it;
          stack_.push(&elem);
        } else {
          auto& new_dir = stack_.top()->dirs.emplace_back(dir_change);
          stack_.push(&new_dir);
        }
      }
    } else {
      assertm("Unknown command", false);
    }
  }

  void processEntry(istream &line) {
    string firstWord;
    string fileName;
    line >> firstWord >> fileName;

    if (firstWord == "dir") {
      auto it =
        find_if(stack_.top()->dirs.begin(),
                stack_.top()->dirs.end(),
                [fileName](auto x) { return x.path == fileName; });
      if (it != stack_.top()->dirs.end()) {
        stack_.top()->dirs.emplace_back(fileName);
      }
    } else {
      stack_.top()->files.emplace_back(fileName, stoul(firstWord));
    }
  }

  Dir &getRoot() { return root_; }

private:
  Dir root_{{"/"}};
  Mode mode_{Mode::Command};
  stack<Dir *> stack_;
};

size_t totalSmall(const Dir& root) {
  size_t total = 0;

  if (root.totalSz <= 100000) {
    total += root.totalSz;
  }

  for (auto& dir : root.dirs) {
    total += totalSmall(dir);
  }

  return total;
}

size_t smallestDirHelper(const Dir& root, size_t minSize) {
  size_t bestSize = root.totalSz >= minSize ? root.totalSz : SIZE_MAX;

  for (auto& dir : root.dirs) {
    size_t curSize = smallestDirHelper(dir, minSize);
    if (curSize < bestSize) {
      bestSize = curSize;
    }
  }

  return bestSize;
}

size_t smallestDir(const Dir& root) {
  size_t total = 70000000;
  size_t required = 30000000;
  size_t available = total - root.totalSz;
  size_t toEmpty = required - available;

  return smallestDirHelper(root, toEmpty);
}

int main(int argc, char *argv[]) {
  InputProcessor ip;
  ip.process(cin);

  cout << totalSmall(ip.getRoot()) << endl;
  cout << smallestDir(ip.getRoot()) << endl;

  return 0;
}
