#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "/home/samm/projects/fastmatrix/include/eigen/Eigen/Dense"

#define MAXBUFSIZE  ((int) 1e6)

Eigen::MatrixXd readMatrix(const char *filename);

int main() {
 
    Eigen::MatrixXd test = readMatrix("test.txt");     

    std::cout << "Success!\n";

    return 0;
}

Eigen::MatrixXd readMatrix(const char *filename) {
    
    int cols = 0, rows = 0;
    double buff[MAXBUFSIZE];

    // Read numbers from file into buffer.
    
    std::ifstream infile;
    infile.open(filename);

    if(!infile.is_open()) {
        std::cerr << "file doesn't exist\n";
        std::exit;
    } else {
        std::cout << "file is open\n";
    
    }

    while (!infile.eof())  {
        
        std::string line;
        std::getline(infile, line);
        std::cout << line << std::endl;

        int temp_cols = 0;
        std::stringstream stream(line);
        
        while(!stream.eof())
            stream >> buff[cols*rows+temp_cols++];

        if (temp_cols == 0)
            continue;

        if (cols == 0)
            cols = temp_cols;

        rows++;
    }

    infile.close();

    rows--;

    Eigen::MatrixXd result(rows,cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i,j) = buff[ cols*i+j ];

    return result;
};
