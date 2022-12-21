
import numpy
import matplotlib.pyplot as plt

# create a evently spaced range of numbers
x1 = numpy.linspace(0.0, 13.0)

# create some data for the Y directon
y1 = x1 + 10  # this will add 10 to everything in x1 and set it y1


x2 = x1
y2 = numpy.sin(y1) # this will part of a sign wave

fig, (ax1,ax2) = plt.subplots(2,1)
fig.suptitle('some silly plots dude')

ax1.plot(x1,y1, '.-') # bit bubbles with o-
ax1.set_ylabel('my ystuff')

ax2.plot(x2,y2, '.-') # little dots with .-
ax2.set_xlabel('time(s)')
ax2.set_ylabel('bedtime')

plt.show()


