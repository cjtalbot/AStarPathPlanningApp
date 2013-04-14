Christine Talbot
ITCS 6150 / 8150
Project Report
Due: 12/2/2010

PROBLEM DESCRIPTION & OBJECTIVE
Finding a path through an environment while avoiding obstacles can be a challenging task for a robot. Completing this optimally is even more difficult. This application attempts to provide a method for achieving such a task. The user is asked to setup a random environment, place the robot in the environment, and tell it where its goal is within the environment. The robot is allowed to achieve offline planning prior to moving about in the environment and therefore has full knowledge of the environment.


The objective is to utilize planning with an A* algorithm to allow the robot to plan out its moves in order to find an optimal path through this environment without hitting or going through obstacles. Ideally, I had hoped to also expand this to encompass online searching through the environment with LRTA* algorithm usage as well in order to compare those two methods. However, due to time constraints, only the offline planning methods were utilized.