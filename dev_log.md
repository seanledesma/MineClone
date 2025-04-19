*Friday, April 18th 2025*

Trying to figure out what I should do next.

I want to get the camera moving around like a player. here is what I know so far:
- I should keep track of player position and velocity (using player struct)
- I should update velocity by multiplying it with delta time, instead of just ++ing.
- I should keep track of if the player is actually on ground, if not, multiply y velocity (9.8 * deltaTime)
- I should place the camera at about 1.8 blocks in height
- 