# Final Project Proposal

## Group Members:

Ricky Lin, Armistead Williams, Bogdan Sotnikov
       
# Intentions:

We are going to create a battleship board game using networking and semaphores.
    
# Intended usage:

Our project would be a battle ship board game. There would first be a setup menu for new clients to choose who they want to fight. That menu would have leaderboards of all current clients that are online and their wins and loss statistics. Once a client chooses another one to fight, they would be placed in a subserver where they could have turn based gaming with each other. The server would receive whoever wins and loses and change the leaderboard. Also, as an additional feature, any another client that joins could spectate the game. 
# Technical Details:

## We will be using select/sockets to send data, networking, subservers to run individual matches, and signals to end the program and game.

  Ricky - The game mechanics
  
  Bogdan - The game menu, leaderboard, basically everything that is outside of the game.
  
  Armistead - The networking and sharing data part of the server. 
    
# Intended pacing:

- Basic functionality of the game by Friday in class.
- Make everything run together by Wednesday and fix any bugs and stuff by the deadline. 
