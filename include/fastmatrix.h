#define MAXBUFSIZE ((int)1e6)
#include <chrono>

int isGzipped(std::string filename);

Eigen::MatrixXd readMatrixgz(const char *filename);

Eigen::MatrixXd readMatrix(std::string filename);

std::vector<std::string> split_string_to_vector(std::string original, char separator);

void writeMatrixOutput(Eigen::MatrixXd MatrixOutput, std::string filename);
