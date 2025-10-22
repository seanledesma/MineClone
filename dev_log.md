*Wednesday, October 22nd 2025*
So I'm thinking of how I'm going to structure the data of my game. I will use a 3D array for blocks in a chunk like I 
originally did, but I will take a lot more care in how I organize those chunks. My plan is to have each chunk be of type
struct Chunk, the struct will have variables for 3D chunk coords and the key used to hash and find in the table.
ex:
struct ChunkEntry {
    int cx, cy, cz;
    int key;
} ChunkEntry;
I will start out by making the first chunk, at floor(player.pos / CHUNK_SIZE).
I'll need two

*Saturday, October 18th 2025*
So I've decided to start over, lol.

*Friday, April 18th 2025*

Trying to figure out what I should do next.

I want to get the camera moving around like a player. here is what I know so far:
- I should keep track of player position and velocity (using player struct)
- I should update velocity by multiplying it with delta time, instead of just ++ing.
- I should keep track of if the player is actually on ground, if not, multiply y velocity (9.8 * deltaTime)
- I should place the camera at about 1.8 blocks in height
- 


*Tuesday, April 29th 20205*
I am thinking of how I want to generate more than one chunk. I think it would be useful to
have an array of some sort, maybe a 2D array. I could have it just be an array of the x and z axis, however I'd need to know 
the size of the array at run time. So perhaps something that can grow as the player discovers the world. Could just use one array, 
and resize it to add more on to it as the player goes on discovering. 

I actually think it's not a bad idea to use a 2d array, the only caveat being I can't use world position as indices, because they can go negative.