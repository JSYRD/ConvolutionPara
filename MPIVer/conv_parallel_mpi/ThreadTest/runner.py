import os

threadList = [i for i in range(1, 18)]
Versions = [1, 2]
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

def runPara(threadnum, version):
    paraTime = 0.0
    for i in range(repeatTimes):
        os.system("mpirun --oversubscribe -np %d ./para%d > tempResult" % (threadnum, version))
        resf = open("./tempResult", "r")
        paraTime += (float)(resf.readline())  # type: ignore
        resf.close()
    paraTime /= (float)(repeatTimes)
    return paraTime


def main():
    serialTime = []
    parallelTime = [[], []]
    ratio = [[], []]
    os.mkdir("results")
    os.system("make")
    log = open("./results/runnerLog.txt", "w")
    log.write("Start running:\n")
    sT = runSerial()
    serialTime.append(sT)
    for threadnum in threadList:
        for i in range(len(Versions)):
            pT = runPara(threadnum, Versions[i])
            parallelTime[i].append(pT)
            ratio[i].append(sT/pT)
        log.write("threadnum = {0}\nserial time:{1}\npara1 time:{2}\npara2 time:{3}\nboost ratio1:{4}\nboost ratio2:{5}\n\n".format(threadnum, sT, parallelTime[0][-1], parallelTime[1][-1], sT/parallelTime[0][-1], sT/parallelTime[1][-1]))
        print("threadnum = {0}\nserial time:{1}\n\npara1 time:{2}\npara2 time:{3}\nboost ratio1:{4}\nboost ratio2:{5}\n".format(threadnum, sT, parallelTime[0][-1], parallelTime[1][-1], sT/parallelTime[0][-1], sT/parallelTime[1][-1]))
    metadata = open("./results/metadata.txt","w")
    metadata.write("serialTime:\n"+str(serialTime)+'\n')
    metadata.write("parallelTime:\n"+str(parallelTime)+'\n')
    metadata.write("boost ratio:\n"+str(ratio)+'\n')
    metadata.close()
    log.close()


if (__name__ == "__main__"):
    main()