from matplotlib import pyplot as plt
import os

NList = [i for i in range(10, 51, 1)]

def run():
    serialtime = 0.0
    paralleltime = 0.0
    
    os.system("make")
    for i in range(5):
        os.system("./matrix_serial > result")
        resf = open("./result", "r")
        serialtime  += (float)(resf.readline())  # type: ignore
        resf.close()
        os.system("./matrix_parallel 16 > result")
        resf = open("./result", "r")
        paralleltime  += (float)(resf.readline())
        resf.close()
    return [serialtime/5.0, paralleltime/5.0]
def main():
    ratios = []
    exetime = []
    log = open("./log.txt","w")
    for i in NList:
            header = open("./matrix.h", "w")
            header.write("#define N %d\n#define M %d\n" % (i, 5))
            header.close()
            results = run()
            log.write("N = {0}\nserial time:{1}\nparallel time:{2}\nboost ratio:{3}\n\n".format(i,results[0], results[1], results[0]/results[1]))
            # print("N = %d" % i)
            print("N = {0}\nserial time:{1}\nparallel time:{2}\nboost ratio:{3}\n\n".format(i,results[0], results[1], results[0]/results[1]))
            exetime.append(results)
            ratios.append(results[0]/results[1])
    log.close()
    data = open("./data.txt", "w")
    data.write(str(NList)+'\n')
    data.write(str(ratios)+'\n')
    data.write(str(exetime)+'\n')
    data.close()

    # fig, ax1 = plt.subplots()
    # color = 'tab:red'
    # ax1.set_xlabel('矩阵大小N')
    # ax1.set_ylabel('time(s)', color = color)
    # ax1.plot(NList, )
    plt.title("Convolution Calculating Analysis Between Ser. & Par.")
    plt.xlabel("Matrix size N")
    plt.ylabel("rate")
    plt.plot(NList, ratios)
    
    plt.savefig("result.jpg")
    # plt.show()



if (__name__ == "__main__"):
    main()
