# fastmatrix

fastmatrix is simple program to sum many matrices which produced from ChromoPainter output. 

All the dependencies are provided in this repository, the only requirement is that you have ``zlib.h`` somewhere in your path.

To run and use simply:

```
git clone https://github.com/sahwa/fastmatrix
cd fastmatrix
make 
```

Usage:

```
USAGE:

   bin/fastmatrix  -f <string> -o <string> -p <string> -c <string> [--]
                   [--version] [-h]


Where:

   -f <string>,  --filetype <string>
     (required)  File type - e.g. chunkcounts or chunklengths - program
     will automatically detect gzipped files

   -o <string>,  --output <string>
     (required)  Full output file name - no extension will be added

   -p <string>,  --prefix <string>
     (required)  prefix of filenames you want to analuse

   -c <string>,  --chromosomes <string>
     (required)  comma seperated list of chromosomes you want to analyse

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```
