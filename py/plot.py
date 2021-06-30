import matplotlib.pyplot as plt
import PerlinNoise      # to plot perlin noise
import numpy as np

x = np.linspace(0, 3, 1000)
y = [PerlinNoise.OctavePerlin(i, 0, 0).val() for i in x]
# y = np.random.uniform(0, 1, 1000)

plt.style.use('dark_background')

plt.plot(x, y, "#f3ec8a", label="Noise values")
plt.xlabel("Time --->")
plt.ylabel("Noise values")

plt.title("Perlin Noise Plot",
color="#E6F5FF",
fontsize="xx-large",
style="oblique",
pad=20
)

plt.legend()
plt.ylim(0, 1)
plt.minorticks_on()

plt.show()