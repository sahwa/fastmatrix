#define MAXBUFSIZE ((int)1e6)

Eigen::MatrixXd readMatrix(std::string);
std::vector<std::string> split_string_to_vector(std::string original, char separator);
int writeMatrixOutput(Eigen::MatrixXd MatrixOutput, std::string filename);
