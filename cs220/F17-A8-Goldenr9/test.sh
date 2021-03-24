(date && whoami) | tee -a results.txt
make -k test estimator | tee -a results.txt
./test | tee -a results.txt
./estimator | tee -a results.txt
