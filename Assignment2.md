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
