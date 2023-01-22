import numpy as np
import matplotlib.pyplot as plt

# Fixing random state for reproducibility
np.random.seed(19680801)

dt = 0.01
t = np.arange(0, 10, dt)
print("len is {}".format(len(t)))
nse1 = np.random.randn(len(t))                 # white noise 1
nse2 = np.random.randn(len(t))                 # white noise 2

# Two signals with a coherent part at 10 Hz and a random part
s1 = np.sin(2 * np.pi * 10 * t) + 5
s2 = np.sin(2 * np.pi * 10 * t) + nse2
s3 = np.sin(2 * np.pi * 10 * t) + nse2
s4 = np.sin(2 * np.pi * 10 * t) + nse2
s5 = np.sin(2 * np.pi * 10 * t) + nse2

# returns a array of axes
fig, axs = plt.subplots(3, 1)
axs[0].plot(t, s1, t, s2, t, s3, t, s4, t, s5)
axs[0].set_xlim(0, 2)
axs[0].set_xlabel('Time')
axs[0].set_ylabel('s1 and s2')
axs[0].grid(True)

cxy = axs[1].cohere(s1, s2, 256, 1. / dt)
axs[1].set_ylabel('Coherence')

axs[2].grid(True)

fig.tight_layout()
plt.show()
