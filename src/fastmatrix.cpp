#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string_view>

#include "tclap/CmdLine.h"
#include "eigen/Eigen/Dense"

#define MAXBUFSIZE  ((int) 1e7)

Eigen::MatrixXd readMatrix(const char *filename);

int main(int argc, char *argv[]) {
    
    // parse command line args 
    
    try {
        TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
        
        TCLAP::ValueArg<std::string> chromosomesToAnalyseArg("c", "chromosomes", "comma seperated list of chromosomes you want to analyse",true,"homer","string");
        cmd.add(chromosomesToAnalyseArg);
        
        TCLAP::ValueArg<std::string> prefixArg("p", "prefix", "prefix of filenames you want to analuse", true,"homer","string");
        cmd.add(prefixArg);

        TCLAP::ValueArg<std::string> outputArg("o", "output", "Full output file name - no extension will be added", true, "homer", "string");
        cmd.add(outputArg);
        
        TCLAP::ValueArg<std::string> fileType("f", "filetype", "File type - e.g. chunkcounts or chunklengths - program will automatically detect gzipped files", true, "homer", "string");


        //TCLAP::SwitchArg normaliseSwitch("a","normalise","normalise the copying matrix before printing (i.e. divide each row by its sum", cmd, false);
        //cmd.add(normaliseSwitch);

        cmd.parse(argc, argv);

        std::string name = chromosomesToAnalyseArg.getValue();
        std::string output = outputArg.getValue();


    } catch (TCLAP::ArgException &e)
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }


    Eigen::MatrixXd test = readMatrix("test.txt");     
    Eigen::MatrixXd test2 = readMatrix("test2.txt");

    std::cout << "test has " << test.rows() << " rows and " << test.cols() << " cols" << std::endl;
    std::cout << "test has " << test2.rows() << " rows and " << test2.cols() << " cols" << std::endl;

    Eigen::MatrixXd output = test + test2;
    
    // std::cout << output << std::endl;

    std::cout << "Success!\n";

    return 0;
}

Eigen::MatrixXd readMatrix(const char *filename) {
    
    int cols = 0, rows = 0;
    double buff[MAXBUFSIZE];

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
        // std::cout << line << std::endl;

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
