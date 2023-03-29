#include <string>
#include <iostream>

int isGzipped(std::string filename);

int main() {

  int a = isGzipped("sam.txt");

  std::cout << a << "\n";

  return 9;

}

int isGzipped(std::string filename) {
  std::string::size_type iszip = filename.find(".gz");
  return iszip;
}
