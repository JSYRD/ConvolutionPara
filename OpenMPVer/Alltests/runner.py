from matplotlib import pyplot as plt
import os

threadsList = [i for i in range(1, 33)]

def runSerial():
    serialTime = 0.0
    for i in range(5):
        os.system("./matrix_serial > result")
        resf = open("./result", "r")
        serialTime += (float)(resf.readline())  # type: ignore
        resf.close()
    serialTime /= 5.0
    return serialTime


def runParallel(threads):
    paralleltime = 0.0
    for i in range(5):
        os.system("./matrix_parallel %d > result" % (threads))
        resf = open("./result", "r")
        paralleltime += (float)(resf.readline())
        resf.close()
    paralleltime/=5.0
    return paralleltime

def main():
    ratios = []
    log = open("./log.txt","w")

    serialTime = runSerial()
    parallelTime = []
    log.write("serial time:%d\n"%(serialTime))
    for j in threadsList:
        print("j = %d Testing"%(j))
        temp = runParallel(j)
        parallelTime.append(temp)
        print("Thread_num = {0}\nparallel time:{1}\nboost ratio:{2}\n\n".format(j, temp, serialTime/temp))
        log.write("Thread_num = {0}\nparallel time:{1}\nboost ratio:{2}\n\n".format(j, temp, serialTime/temp))
        ratios.append(serialTime/temp)
    log.close()

    data = open("./data.txt", "w")
    data.write(str(threadsList)+'\n')
    data.write(str(serialTime)+'\n')
    data.write(str(ratios)+'\n')
    data.write(str(parallelTime)+'\n')
    data.close()

    # fig, ax1 = plt.subplots()
    # color = 'tab:red'
    # ax1.set_xlabel('矩阵大小N')
    # ax1.set_ylabel('time(s)', color = color)
    # ax1.plot(NList, )
    plt.title("Different thread_num effection analysis")
    plt.xlabel("thread number")
    plt.ylabel("rate")
    plt.plot(threadsList, ratios)
    
    plt.savefig("result.jpg")
    # plt.show()



if (__name__ == "__main__"):
    main()
