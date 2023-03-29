#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "eigen/Eigen/Dense"
#include "fastmatrix.h"
#include "tclap/CmdLine.h"
#include <gzstream.h>

int main(int argc, char *argv[]) {

  try {
    TCLAP::CmdLine cmd("Command description message", ' ', "0.1");

    TCLAP::ValueArg<std::string> chromosomesToAnalyseArg(
        "c", "chromosomes", "comma seperated list of chromosomes you want to analyse", true, "homer", "string");
    cmd.add(chromosomesToAnalyseArg);

    TCLAP::ValueArg<std::string> prefixArg("p", "prefix", "prefix of filenames you want to analuse", true, "homer",
                                           "string");
    cmd.add(prefixArg);

    TCLAP::ValueArg<std::string> outputArg("o", "output", "Full output file name - no extension will be added", true,
                                           "homer", "string");
    cmd.add(outputArg);

    TCLAP::ValueArg<std::string> fileType("f", "filetype",
                                          "File type - e.g. chunkcounts or chunklengths - program will "
                                          "automatically detect gzipped files",
                                          true, "homer", "string");
    cmd.add(fileType);

    // TCLAP::SwitchArg normaliseSwitch("a","normalise","normalise the copying
    // matrix before printing (i.e. divide each row by its sum", cmd, false);
    // cmd.add(normaliseSwitch);

    cmd.parse(argc, argv);

    std::string chromosomesToAnalyseString = chromosomesToAnalyseArg.getValue();
    std::string output = outputArg.getValue();
    std::string prefix = prefixArg.getValue();
    std::string filetype = fileType.getValue();

    std::vector chromosomesToAnalyseVector = split_string_to_vector(chromosomesToAnalyseString, ',');

    // run some checks here //

    int n_chr = chromosomesToAnalyseVector.size();

    if (n_chr == 1) {
      std::cerr << "You have only supplied a single chromosome! Exiting...\n";
      return 0;
    }

    std::cout << "Processing " << n_chr << " chromosomes\n";

    // now we want to read in the first file and use that as a base //

    std::string firstFilePath = prefix + chromosomesToAnalyseVector[0] + '.' + filetype;
    const char * firstFilePathChar = firstFilePath.c_str();


    if (isGzipped(firstFilePathChar) == -1) {
      std::cout << "file is unzipped\n";
      Eigen::MatrixXd firstFile = readMatrix(firstFilePath);
    } else {
      std::cout << "file is gzipped\n";
      //Eigen::MatrixXd firstFile = readMatrixGzip(firstFilePath);
    }
    
    Eigen::MatrixXd firstFile = readMatrix(firstFilePath);
    
    
    int ff_rows = firstFile.rows();
    int ff_cols = firstFile.cols();

    // remove first element from the vector - we don't need any more
    chromosomesToAnalyseVector.erase(chromosomesToAnalyseVector.begin());

    for (std::string i : chromosomesToAnalyseVector) {
      std::string infile = prefix + i + '.' + filetype;
      std::cout << "Processing infile: " << infile << "\n";

      Eigen::MatrixXd current = readMatrix(infile);

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
  double buff[MAXBUFSIZE];

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
    // std::cout << line << std::endl;

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
};

void readMatrixgz(const char* filename) {
  
  int cols = 0, rows = 0;
  double buff[MAXBUFSIZE];

  igzstream in(filename);
  std::string line;

  while (getline(in, line)) {
    std::cout << line << std::endl;
  }
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

int writeMatrixOutput(Eigen::MatrixXd MatrixOutput, std::string filename) {

  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cout << "Cannot open output file connection\n";
    return 0;
  }

  file << "Writing output to file\n" << MatrixOutput << '\n';

  return 0;
}

int isGzipped(std::string filename) {
  std::string::size_type iszip = filename.find(".gz");
  return iszip;
}

  

