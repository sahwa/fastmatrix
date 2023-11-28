#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "../include/eigen/Eigen/Dense"
#include "fastmatrix.h"
#include "gzstream.h"
#include "tclap/CmdLine.h"

int main(int argc, char *argv[]) {

  // setup arguments in try catch

  try {
    TCLAP::CmdLine cmd("Command description message", ' ', "0.1");

    TCLAP::ValueArg<std::string> outputArg("o", "output", "Full output file name - no extension will be added", true,
                                           "homer", "string");
    cmd.add(outputArg);

    TCLAP::ValueArg<std::string> fileList("f", "filelist", "List of files to merge, seperated by commas", "true",
                                          "homer", "string");
    cmd.add(fileList);

    cmd.parse(argc, argv);

    // assign arguments to variables
    std::string output = outputArg.getValue();

    std::vector filesToAnalyseVector = split_string_to_vector(fileList, ',');

    // run some checks here //

    int n_chr = filesToAnalyseVector.size();

    // no point summing chromosomes when there's only one!
    if (n_chr == 1) {
      std::cerr << "You have only supplied a single chromosome! Exiting...\n";
      return 0;
    }

    std::cout << "Processing " << n_chr << " chromosomes\n";

    // now we want to read in the first file and use that as a base //

    std::string firstFilePath = filesToAnalyseVector[0];

    std::cout << "first path is: " << firstFilePath << "\n";

    const char *firstFilePathChar = firstFilePath.c_str();

    std::cout << "first path is: " << firstFilePath << "\n";

    // check whether the file is zipped or not (based on postfix) //

    Eigen::MatrixXd firstFile;

    if (isGzipped(firstFilePath) == -1) {
      std::cout << "file is unzipped\n";
      firstFile = readMatrix(firstFilePath);
    } else {
      std::cout << "file is gzipped\n";
      firstFile = readMatrixgz(firstFilePathChar);
    }

    int ff_rows = firstFile.rows();
    int ff_cols = firstFile.cols();

    // remove first element from the vector - we don't need any more
    filesToAnalyseVector.erase(filesToAnalyseVector.begin());

    for (std::string i : filesToAnalyseVector) {

      std::string infile = i;
      const char *infilechar = infile.c_str();

      std::cout << "Processing infile: " << infile << "\n";

      Eigen::MatrixXd current;

      if (isGzipped(firstFilePath) == -1) {
        std::cout << "file is unzipped\n";
        current = readMatrix(infile);
      } else {
        std::cout << "file is gzipped\n";
        current = readMatrixgz(infilechar);
      }

      int current_rows = current.rows();
      int current_cols = current.cols();

      if (ff_rows != current_rows) {
        std::cout << "Current file has wrong number of rows! Exiting\n";
        return 0;
      }

      if (ff_cols != current_cols) {
        std::cout << "Current file has wrong number of cols! Exiting\n";
        return 0;
      }

      firstFile += current;
    }

    std::cout << "Finished summing matrices\n";

    writeMatrixOutput(firstFile, output);

  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

  std::cout << "Success!\n";
  return 0;
}

Eigen::MatrixXd readMatrix(std::string filename) {

  int cols = 0, rows = 0;
  std::vector<double> buff(MAXBUFSIZE);

  std::ifstream infile;
  infile.open(filename);

  if (!infile.is_open()) {
    std::cerr << "file doesn't exist\n";
    std::exit;
  } else {
    std::cout << "file is open\n";
  }

  while (!infile.eof()) {

    std::string line;
    std::getline(infile, line);

    int temp_cols = 0;
    std::stringstream stream(line);

    while (!stream.eof())
      stream >> buff[cols * rows + temp_cols++];

    if (temp_cols == 0)
      continue;

    if (cols == 0)
      cols = temp_cols;

    rows++;
  }

  infile.close();

  rows--;

  Eigen::MatrixXd result(rows, cols);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      result(i, j) = buff[cols * i + j];

  return result;
}

Eigen::MatrixXd readMatrixgz(const char *filename) {

  int cols = 0, rows = 0;
  std::vector<double> buff(MAXBUFSIZE);

  igzstream infile;
  infile.open(filename);

  if (!infile.good()) {
    std::cerr << "file doesn't exist\n";
    std::exit;
  } else {
    std::cout << "file is open\n";
  }

  while (!infile.eof()) {

    std::string line;
    std::getline(infile, line);

    int temp_cols = 0;

    std::stringstream stream(line);

    while (!stream.eof())
      stream >> buff[cols * rows + temp_cols++];

    if (temp_cols == 0)
      continue;

    if (cols == 0)
      cols = temp_cols;

    rows++;
  }

  infile.close();

  rows--;

  Eigen::MatrixXd result(rows, cols);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      result(i, j) = buff[cols * i + j];

  return result;
}

std::vector<std::string> split_string_to_vector(std::string original, char separator) {

  std::vector<std::string> results;
  std::string::const_iterator start = original.begin();
  std::string::const_iterator end = original.end();
  std::string::const_iterator next = std::find(start, end, separator);

  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }

  results.push_back(std::string(start, next));

  return results;
}

bool writeMatrixOutput(const Eigen::MatrixXd& matrixOutput, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open output file: " << filename << "\n";
        return false;
    }
    const static Eigen::IOFormat sepFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "", "", "");
    file << matrixOutput.format(sepFormat) << '\n';
    if (!file) {  // Checks if the write operation was successful
        std::cerr << "Failed to write to the file: " << filename << "\n";
        file.close();
        return false;
    }
    file.close();
    return true;
}
