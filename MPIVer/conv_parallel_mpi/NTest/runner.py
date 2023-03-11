import os

Nlist = [i for i in range(100, 205, 20)]
Versions = [0, 1, 2]
repeatTimes = 3

def runSerial():
    serialTime = 0.0
    for i in range(repeatTimes):
        os.system("./serial > tempResult")
        resf = open("./tempResult", "r")
        serialTime += (float)(resf.readline())  # type: ignore
        resf.close()
    serialTime /= (float)(repeatTimes)
    return serialTime

def runPara(version):
    paraTime = 0.0
    for i in range(repeatTimes):
        # if(version == 0):
        #     os.system("mpirun -np 8 ./para0 > tempResult")
        # else:
        #     os.system("mpirun --use-hwthread-cpus -np 16 ./para%d > tempResult" % (version))
        os.system("mpiexec ./para%d -np 8 > tempResult" % (version))
        resf = open("./tempResult", "r")
        paraTime += (float)(resf.readline())  # type: ignore
        resf.close()
    paraTime /= (float)(repeatTimes)
    return paraTime


def main():
    serialTime = []
    parallelTime = [[], [], []]
    ratio = [[], [], []]
    os.mkdir("results")
    log = open("./results/runnerLog.txt", "w")
    log.write("Start running:\n")
    for N in Nlist:
        header = open("./defs.h", "w")
        header.write("#define I_SIZE %d\n#define BATCHSIZE 10000\n" % (N))
        header.close()
        os.system("make")
        sT = runSerial()
        serialTime.append(sT)
        for i in Versions:
            pT = runPara(i)
            parallelTime[i].append(pT)
            ratio[i].append(sT/pT)
        log.write("N = {0}\nserial time:{1}\npara0 time:{2}\npara1 time:{3}\npara2 time:{4}\nboost ratio0:{5}\nboost ratio1:{6}\nboost ratio2:{7}\n\n".format(N, sT, parallelTime[0][-1], parallelTime[1][-1], parallelTime[2][-1], sT/parallelTime[0][-1], sT/parallelTime[1][-1], sT/parallelTime[2][-1]))
        print("N = {0}\nserial time:{1}\npara0 time:{2}\npara1 time:{3}\npara2 time:{4}\nboost ratio0:{5}\nboost ratio1:{6}\nboost ratio2:{7}\n\n".format(N, sT, parallelTime[0][-1], parallelTime[1][-1], parallelTime[2][-1], sT/parallelTime[0][-1], sT/parallelTime[1][-1], sT/parallelTime[2][-1]))
    metadata = open("./results/metadata.txt","w")
    metadata.write("serialTime:\n"+str(serialTime)+'\n')
    metadata.write("parallelTime:\n"+str(parallelTime)+'\n')
    metadata.write("boost ratio:\n"+str(ratio)+'\n')
    metadata.close()
    log.close()

if (__name__ == "__main__"):
    main()