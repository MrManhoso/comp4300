# Game Engine can
- Create game objects as entity instances
- Add component data to entities
- Implement game play via Systems
    - Also handle user input
- Pause/exit game
- Init/load config from file

# Game Engine cannot/does not
- Handle more than one 'scene'
- Load textures nor sound assets
- Have menu

# What to do
## Player
- Is represented by a shape defined in the config file
- Must spawn at center at start/after death
- Dies when colliding with enemy
- Moves at speed S read from config file. Up: W, Left: A, Right: D, Down: S
- Note! If player is moving diagonally at speed S it is NOT moving S in both axes. Is is moving S on the diagonal. To do this normalize the vector and multiply by S
- Can only move inside the window
- Will shoot a bullet toward the mouse pointer when left mouse button clicked. Speed and lifespan of bullet is read from config file.

### Special ability
- Free to come up with ability (weapon)
- Fired when right button clicked
#### Must
- Spawn multiple entities, eg multiple bullets
- Entities must have unique graphic associated with it
- Unique game mechanic introduced via new component. Remember to add to component tuple!
- Have a cool-down timer

Note! Properties are not in the config file

## Enemies
- Will spawn in a random location on the screen every X frames. X is defined in the config file.
- Must not overlap the sides of the screen at time of spawn
- Have random shapes of vertecies between a max and min, specified in config file
- Random color on spawn
- Random speed on spawn, between min and max as specified in config file.
- Should bounce of edges
- Given a score when spawned of N*100 where N = nr vertices.
- When (large) enemies collide with bullet or player they are destroyed and N smaller enemies spawn in place, where N is number of vertecies of destroyed enemy.
    - Each small enemy has same nr vertecies and color as original
    - They travel outward at angels at a fixed interval equal to 360/vertices
    - Eg 6 vertices => 6 smaller enemies traveling at 60 degrees
    - Should be given reasonable speed
    - The are active for a given time and should fade away
    - Each small enemy gets the score double to the original


## Score
- When killed by player bullet the score of the enemy is added to game score
- The score should be displayed in top left corner with the font specified in config file

## Drawing
- All entities should be given a slow rotation
- Any special effects which does not alter gameplay can be added
- Any entity with a lifespan that is still alive should have its color alpha channel set to a ratio depending on how long it has left to live. Eg: If 80 lifespan and 25 frames remaining, alpha value should be set to (float)25/80 * 255. Alpha should go from 255 -> 0 (last frame alive)

## GUI
Use ImGui with these properties:
- Option to turn off each system independently with exception of rendering and gui systems
- For each entity in the game the gui must list the id, tag and position. You must display a list of all entities as well as lists of entities by their tag. It must also be possible to detroy an entity by interacting with the UI element associated with the entity.
- Possible to change enemy spawn interval 
- Possible to manually spawn enemies

## Misc
- P key pause the game
- Esc closes the game

## Config file
One line each specifying: Window size, Font format, player, bulle specs, enemy specs (in that order). Format:
- Window W H FL FS. FL = framelimit, FS = Full screen mode (1=YES, 0=NO)
- Font F S R G B
- Player SR CR S FR FG FB OR OG OB OT V.  
    - SR: Shape radius int
    - CR: Collision radius int
    - S: Speed float
    - FR, FG, FB: Fill color
    - OR...: Outline color
    - OT: Outline thickness
    - V: Shape vertices
- Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
    - SMIN/SMAX: min/max speed floats
    - VMIN/VMAX: min/max vertices ints
    - L: Small lifespan int
    - SP: Spawn interval int
- Bullet SR CR S FR FG FB OR OG OB OT V L
    - L: lifespan int