all:
	mpic++ another_cov.cpp -g -Wall -o serial
	mpic++ another_cov_mpi.cpp -g -Wall -o para0
	mpic++ another_cov_mpi_opti1.cpp -g -Wall -o para1
	mpic++ another_cov_mpi_opt2.cpp -g -Wall -o para2

clean:
	rm -rf serial para0 para1 para2 convolution*

cleanre:
	rm -rf results