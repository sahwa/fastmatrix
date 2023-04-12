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

Eigen::MatrixXd readMatrixgz(std::string filename) {

  int cols = 0, rows = 0;
  std::vector<double> buff(MAXBUFSIZE);

  igzstream infile;
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
