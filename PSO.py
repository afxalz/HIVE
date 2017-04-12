import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import copy


class Particle(object):
	"""docstring for Particle"""
	def __init__(self, position,velocity):
		self.position=copy.copy(position)
		self.velocity=copy.copy(velocity)
		self.best_pos=copy.copy(position)

	def update_time_step(self,time_unit):
		self.position+=copy.copy(self.velocity)*time_unit

	def update_velocity(self,velocity):
		self.velocity=copy.copy(velocity)

	def update_best_position(self,position):
		self.best_pos=copy.copy(position)
	
	def get_position(self):
		return copy.copy(self.position)

	def get_best_position(self):
		return copy.copy(self.best_pos)

	def get_velocity(self):
		return copy.copy(self.velocity)

class Swarm(object):
	"""docstring for Swarm"""
	def __init__(self, particle_count,inertia,cognitive,social,random_position_generator,random_velocity_generator,objective_function,alpha):
		self.swarm_best_pos=None
		self.swarm_best_value=None
		self.particles=[]
		self.animation_position=[]
		self.particle_count=particle_count

		self.alpha=alpha
		self.inertia=inertia
		self.cognitive=cognitive
		self.social=social

		self.objective_function=objective_function

		for i in range(particle_count):
			particle=Particle(random_position_generator(),random_velocity_generator())
			self.particles.append(particle)
			particle.best_value=self.objective_function(particle.get_position())
			if ((self.swarm_best_value != None) or (self.swarm_best_value<self.objective_function(particle.get_position()))):
				self.swarm_best_pos=particle.get_position();



	def epoch(self,time_unit):
		swarm_position=np.zeros(shape=(0,2))
		for particle in self.particles:
			particle.update_time_step(time_unit)
			particle.update_velocity((self.inertia*particle.get_velocity())
				+(self.cognitive*np.random.uniform(0,self.alpha)*(self.swarm_best_pos-particle.get_position()))
				+(self.social*np.random.uniform(0,self.alpha)*(particle.get_best_position()-particle.get_position())))
			if(self.objective_function(particle.get_position())>self.objective_function(particle.get_best_position())):
				particle.update_best_position(particle.get_position())
			if(self.objective_function(particle.get_best_position())>self.swarm_best_value):
				self.swarm_best_pos=particle.get_best_position()
				self.swarm_best_value=self.objective_function(particle.get_best_position()) 
			swarm_position=np.vstack([swarm_position, np.array(particle.get_position())])
		self.animation_position.append(swarm_position)


		

	def update_plot(self,i):
		self.scat.set_offsets([np.array(self.animation_position[i])[:,0],np.array(self.animation_position[i])[:,1]])
		return 

	def finish(self):
		self.fig=plt.figure()
		plt.axis([-600, 600, -600, 600])
		self.scat=plt.scatter(np.array(self.animation_position[0])[:,0],np.array(self.animation_position[0])[:,1])

		ani=animation.FuncAnimation(self.fig,self.update_plot,frames=xrange(len(self.animation_position)),interval=100)
		#ani.save('Swarm.mp4', fps=30)
		plt.show()

		print "RMS-> ", np.min([self.objective_function(particle.position) for particle in self.particles])







particle_count=15
inertia=0.5
cognitive=1.49445
social=1.49445
time_unit=1
alpha=0.01
def random_position_generator():
	return np.random.uniform(-600,600,2)

def random_velocity_generator():
	return np.random.uniform(-0.0002,0.0002,2)

def objective_function(position):
	return -np.sqrt(np.mean(position**2))	

MySwarm = Swarm(particle_count,inertia,cognitive,social,random_position_generator,random_velocity_generator,objective_function,alpha)

for i in range(200):
	MySwarm.epoch(time_unit)

MySwarm.finish()

		

