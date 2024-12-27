import subprocess
import matplotlib.pyplot as plt
import numpy as np
import time

# 文件路径配置
ex_file = "D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.2/Ex"  # Ex 数据文件路径
amp0_file = "D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.2/Amp0"  # Amp0 数据文件路径
amp1_file = "D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.2/Amp1"  # Amp1 数据文件路径
amp2_file = "D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.2/Amp2"  # Amp2 数据文件路径

# 启动 C 程序（替换为你的 C 程序路径）
c_program = "D:/HKZ_Coding/FDTD_Learning/FD1D_2.2.c. The Fourier Transform has been added.exe"  # 替换为你的 C 程序可执行文件路径
process = subprocess.Popen(c_program, shell=True)

# 初始化绘图
plt.ion()  # 开启交互模式
fig, axes = plt.subplots(2, 1, figsize=(10, 8))  # 两个子图：Ex 和 Amp 数据

# 上图：Ex-k
ex_line, = axes[0].plot([], [], 'b-', label='Ex-k')  # 蓝线表示 Ex-k
axes[0].set_ylim(-1.0, 1.0)  # 根据 Ex 值范围设置
axes[0].set_xlim(0, 200)  # 假设 k 范围是 0 到 200
axes[0].set_xlabel("k")
axes[0].set_ylabel("Ex")
axes[0].set_title("Real-time Ex-k Plot")
axes[0].legend()

# 下图：Amp0、Amp1、Amp2-k
amp0_line, = axes[1].plot([], [], 'r-', label='Amp0-k')  # 红线表示 Amp0-k
amp1_line, = axes[1].plot([], [], 'g-', label='Amp1-k')  # 绿线表示 Amp1-k
amp2_line, = axes[1].plot([], [], 'y-', label='Amp2-k')  # 黄线表示 Amp2-k
axes[1].set_ylim(0.0, 1.5)  # 根据 Amp 值范围设置
axes[1].set_xlim(0, 200)  # 假设 k 范围是 0 到 200
axes[1].set_xlabel("k")
axes[1].set_ylabel("Amplitude")
axes[1].set_title("Real-time Amp-k Plot")
axes[1].legend()

try:
    while True:
        try:
            # 读取 Ex 数据
            ex_data = np.loadtxt(ex_file)  # 从文件读取 Ex 数据
            k_ex = ex_data[:, 0]  # 第一列是 k
            ex = ex_data[:, 1]  # 第二列是 Ex 值

            # 读取 Amp 数据
            amp0_data = np.loadtxt(amp0_file)  # 从文件读取 Amp0 数据
            amp1_data = np.loadtxt(amp1_file)  # 从文件读取 Amp1 数据
            amp2_data = np.loadtxt(amp2_file)  # 从文件读取 Amp2 数据

            k_amp0 = amp0_data[:, 0]  # 第一列是 k
            amp0 = amp0_data[:, 1]  # 第二列是 Amp0 值

            k_amp1 = amp1_data[:, 0]  # 第一列是 k
            amp1 = amp1_data[:, 1]  # 第二列是 Amp1 值

            k_amp2 = amp2_data[:, 0]  # 第一列是 k
            amp2 = amp2_data[:, 1]  # 第二列是 Amp2 值

            # 更新 Ex-k 图像
            ex_line.set_xdata(k_ex)
            ex_line.set_ydata(ex)
            axes[0].relim()
            axes[0].autoscale_view()

            # 更新 Amp-k 图像
            amp0_line.set_xdata(k_amp0)
            amp0_line.set_ydata(amp0)

            amp1_line.set_xdata(k_amp1)
            amp1_line.set_ydata(amp1)

            amp2_line.set_xdata(k_amp2)
            amp2_line.set_ydata(amp2)

            axes[1].relim()
            axes[1].autoscale_view()

            # 绘图
            plt.draw()
            plt.pause(0.1)

        except FileNotFoundError:
            print("等待 C 程序生成数据文件...")
            time.sleep(0.5)  # 等待数据文件的生成

        except Exception as e:
            print("Error reading file or plotting:", e)

except KeyboardInterrupt:
    print("程序终止...")
    plt.ioff()  # 禁用交互模式
    plt.close('all')  # 关闭所有绘图窗口
    process.terminate()  # 终止 C 程序