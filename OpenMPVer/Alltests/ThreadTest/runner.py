import os

# Nlist = [i for i in range(7, 102, 5)]
threadList = [i for i in range(1, 21)]
Versions = ["Origin", "O_1", "O_2"]
repeatTimes = 5

def runSerial():
    serialTime = 0.0
    for i in range(repeatTimes):
        os.system("./matrix_serial > tempResult")
        resf = open("./tempResult", "r")
        serialTime += (float)(resf.readline())  # type: ignore
        resf.close()
    serialTime /= (float)(repeatTimes)
    return serialTime

def runPara(threadnum):
    paraTime = 0.0
    for i in range(repeatTimes):
        os.system("./matrix_parallel %d > tempResult" % (threadnum))
        resf = open("./tempResult", "r")
        paraTime += (float)(resf.readline())  # type: ignore
        resf.close()
    paraTime /= (float)(repeatTimes)
    return paraTime



def run(path):
    serialTime = []
    parallelTime = []
    ratio = []
    os.chdir(path)
    os.mkdir("results")
    os.system("make")
    log = open("./results/runnerLog.txt", "w")
    log.write("Start running:\n")
    sT = runSerial()
    serialTime.append(sT)
    for threadnum in threadList:
        pT = runPara(threadnum)
        parallelTime.append(pT)
        ratio.append(sT/pT)
        log.write("threadnum = {0}\nserial time:{1}\nparallel time:{2}\nboost ratio:{3}\n".format(threadnum, sT, pT, sT/pT))
        print("threadnum = {0}\nserial time:{1}\nparallel time:{2}\nboost ratio:{3}\n".format(threadnum, sT, pT, sT/pT))
    metadata = open("./results/metadata.txt","w")
    metadata.write("serialTime:\n"+str(serialTime)+'\n')
    metadata.write("parallelTime:\n"+str(parallelTime)+'\n')
    metadata.write("boost ratio:\n"+str(ratio)+'\n')
    metadata.close()
    log.close()
    os.chdir("..")

def main():
    for version in Versions:
        log = open("./runnerLog.txt", "w")
        log.write("Running: {0} ... ".format(version))
        run(version)
        log.write("done.\n")
    log.close()

if (__name__ == "__main__"):
    main()