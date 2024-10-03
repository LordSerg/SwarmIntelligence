# Technology used
C++, GLFW, math, algorithms, VisualStudio

# Overview
This program implements a [swarm intelligence](https://en.wikipedia.org/wiki/Swarm_intelligence) algorithm that demonstrates how a system of individual [agents](https://en.wikipedia.org/wiki/Agent-based_model) can work together to solve problems without a central authority. There is no input required for this program; it provides an output that visualizes how agents within the swarm behave and work towards a common goal.

# About Project
Swarm intelligence refers to the collective behavior of decentralized, self-organized systems, where individual agents follow simple rules and interact locally with one another and their environment. These interactions lead to the emergence of intelligent global behavior, despite each agent having limited capabilities.

Mathematically, swarm intelligence can be modeled using differential equations, probability, and optimization algorithms. Each agent's behavior can be represented using a set of rules based on its position, velocity, and interaction with other agents:

- The position of an agent at time $t$ is denoted by ${x}_i(t)$.
- The velocity of an agent is ${v}_i(t)$, which is influenced by the agent’s own movement and the interactions with neighboring agents.
- The new position of the agent can be calculated as:

  ${x}_i(t+1) = {x}_i(t) + {v}_i(t) \Delta t$
  
- The velocity update is influenced by a weighted combination of self-propulsion, cohesion (tendency to move towards other agents), and alignment (tendency to align velocity with neighbors):

$${v}_i(t+1) = v_i(t) + c_1 {\left( p_i - x_i(t) \right) }+ c_2 \left( \sum_j x_j(t) - x_i(t) \right)$$

  where \( $c_1$ \) and \( $c_2$ \) are constants representing the attraction to a target and the influence of neighboring agents, respectively.

- Swarm intelligence models like these are often used in optimization algorithms (such as [particle swarm optimization](https://en.wikipedia.org/wiki/Particle_swarm_optimization)) and in solving complex real-world problems such as pathfinding, clustering, and more.

## Program example

![gif that shows how program works]()

## Example: A Single Agent's Behavior
A single agent in this system operates by adhering to simple rules:

1. **Perception**: The agent observes its surroundings, identifying the position of other agents and possible targets.
2. **Interaction**: The agent communicates with nearby agents, exchanging information such as target location and environmental obstacles.
3. **Movement**: Based on its perception and interaction, the agent adjusts its velocity and direction. It moves closer to its target while avoiding collisions with other agents or obstacles.

The interaction between agents enables them to work collectively. While a single agent might struggle to find the most efficient path to its target, the collaboration between multiple agents improves the overall efficiency of the swarm, resulting in a robust problem-solving approach.
