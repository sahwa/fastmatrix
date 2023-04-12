#include "../include/gzstream/gzstream.h"
#include <string>
#include <vector>

#define MAXBUFSIZE 10000

void readMatrixgz(const char *filename);

int main() {

  readMatrixgz("test.gz");

  return 0;

}

void readMatrixgz(const char *filename) {

  int cols = 0, rows = 0;
  std::vector<double> buff[MAXBUFSIZE];

  igzstream in(filename);
  std::string line;

  while (getline(in, line)) {
    std::cout << line << std::endl;
  }
}
