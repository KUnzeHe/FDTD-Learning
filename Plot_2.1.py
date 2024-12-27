import subprocess
import matplotlib.pyplot as plt
import numpy as np
import time

# 文件路径
file_path = "D:/HKZ_Coding/FDTD_Learning/Ouput/Ex(2.1).txt"

# 启动 C 程序
c_program = "D:\HKZ_Coding\FDTD_Learning\FDTD_2.1.c New formulation using flus density.exe"  # 替换为你的 C 程序路径
process = subprocess.Popen(c_program, shell=True)

# 初始化绘图
plt.ion()
fig, ax = plt.subplots()
line, = ax.plot([], [], 'b-', label='Ex-k')  # 蓝线
ax.set_ylim(-1.0, 1.0)  # 根据 Ex 值范围设置
ax.set_xlim(0, 200)  # 假设 k 范围是 0 到 200
ax.set_xlabel("k")
ax.set_ylabel("Ex")
ax.set_title("Real-time Ex-k Plot")
ax.legend()

try:
    while True:
        try:
            # 读取文件
            data = np.loadtxt(file_path)
            k = data[:, 0]  # 第一列是 k
            ex = data[:, 1]  # 第二列是 Ex

            # 更新图像
            line.set_xdata(k)
            line.set_ydata(ex)
            ax.relim()
            ax.autoscale_view()

            plt.draw()
            plt.pause(0.1)

        except Exception as e:
            print("Error reading file or plotting:", e)

        time.sleep(0.1)  # 每 0.1 秒检查一次文件内容

except KeyboardInterrupt:
    print("Stopping...")
    plt.ioff()  # 禁用交互模式
    plt.close('all')  # 关闭所有绘图窗口