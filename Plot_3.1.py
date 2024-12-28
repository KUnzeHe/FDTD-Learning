import subprocess
import matplotlib.pyplot as plt
import numpy as np
import time
from mpl_toolkits.mplot3d import Axes3D

# C 程序路径和输出文件路径
c_program = "D:/HKZ_Coding/FDTD_Learning/FDTD_3.1.c. 2D TM.exe"  # 替换为你的 C 程序路径
output_file = "D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_3.1/Ez.txt"  # 替换为你的 Ez.txt 文件路径

# 启动 C 程序
process = subprocess.Popen(c_program, shell=True)

# 初始化绘图
plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# 固定坐标轴范围
x_min, x_max = 0, 60  # 根据问题中的 IE 值
y_min, y_max = 0, 60  # 根据问题中的 JE 值
z_min, z_max = 0, 0.12  # 根据 Ez 的可能取值范围（可以根据实际情况调整）

ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)
ax.set_zlim(z_min, z_max)

ax.set_title("Real-time Ez Field Visualization")
ax.set_xlabel('X (grid)')
ax.set_ylabel('Y (grid)')
ax.set_zlabel('Ez')

try:
    while True:
        try:
            # 读取数据文件
            data = np.loadtxt(output_file)

            # 将数据重整为二维数组
            y = data[:, 0].astype(int)  # 第一列是 j (y 方向的索引)
            x = data[:, 1].astype(int)  # 第二列是 i (x 方向的索引)
            ez = data[:, 2]  # 第三列是 Ez 值

            # 确定网格大小
            JE = x.max() + 1
            IE = y.max() + 1
            ez_grid = np.zeros((JE, IE))

            for k in range(len(x)):
                ez_grid[x[k], y[k]] = ez[k]

            # 创建网格
            X, Y = np.meshgrid(range(IE), range(JE))

            # 清除旧图并绘制新图
            ax.clear()
            ax.plot_surface(X, Y, ez_grid, cmap='viridis')

            # 重新设置固定的坐标轴范围
            ax.set_xlim(x_min, x_max)
            ax.set_ylim(y_min, y_max)
            ax.set_zlim(z_min, z_max)

            ax.set_title(f"Real-time Ez Field Visualization (T = {int(data.shape[0] / (JE * IE))})")
            ax.set_xlabel('X (grid)')
            ax.set_ylabel('Y (grid)')
            ax.set_zlabel('Ez')

            # 更新绘图
            plt.draw()
            plt.pause(0.1)

        except Exception as e:
            print("Error reading file or plotting:", e)

        time.sleep(0.1)  # 每 0.1 秒检查一次文件内容

except KeyboardInterrupt:
    print("Stopping...")
    plt.ioff()  # 禁用交互模式
    plt.close('all')