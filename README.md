# IntakeProject

This is my game named "Shiip", created as an intake assignment for BUAS (Breda University of Applied Science)

## How to play the game

When you run the game, you first get into the menu. There are different buttons you can press:

- **Play** button to start the game
- **Score** button to see your previous attempts (including time and date of your games, how many "sheep scores" you got and how long you lasted)\
Moreover, in the **Score** menu, there is an **X** button to delete your game history and **<-** button to return back to the main menu.
- **Exit** button stops the application and saves your in-game progress
- Small **Sound** toggle is located on the top-right corner of the menu screen. Once pressed, it turns off all sounds in the game; click again to enable.

When you are in the game, your main goal is to survive for as long as you can by evading asteroids.
and optionally to turn as many asteroids into "sheep" by using a special weapon that sometimes appears on the map. You can collect it by colliding with it.

The ship can also shoot bullets to damage asteroids and possibly divide them into many smaller asteroids.
If you collected an upgrade, then you can hold the "shoot" button to charge your weapon to shoot a special bullet that will turn all of the asteroids into sheep within a certain range.

Initially, you get four health points, and you lose them if you collide with an asteroid. Then you become invincible to damage for a few seconds. Once your HP becomes 0, your ship gets destroyed, and the game returns to the main menu.

Additionally, if you get 10 "sheep scores", your ship gets improved and its sprite and effect change, making it faster and easier to control.

## Keybinds

| Keyboard Key | Gamepad Key | Action |
| --- | --- | --- |
| <kbd>W</kbd> | <kbd>Right Trigger</kbd> | Move ship forward |
| <kbd>A</kbd> | <kbd>D-pad left</kbd> | Rotate ship left |
| <kbd>D</kbd> | <kbd>D-pad right</kbd> | Rotate ship right |
| <kbd>E</kbd> or <kbd>Space</kbd> | <kbd>X</kbd> | shoot |
| <kbd>Escape</kbd> | <kbd>START</kbd> | close the game |

## How to install the game

- Install .7z file
- Extract all files in an empty folder
- Run the `SHIIP_GAME.exe` file

## License

See [`License`](LICENSE) file.

## Credits

Sprites that were used in this game :

- <https://foozlecc.itch.io/void-main-ship>
- <https://haiyoooo.itch.io/pixel-sheep>
- <https://greedy-toad.itch.io/pixel-art-ui-buttons-pack>
- <https://linkninja.itch.io/simple-explosion-animation>
- <https://slowdevelopment.itch.io/flame-projectile>
- <https://narehop.itch.io/emoji-comic-pack>
- <https://sentient-dream-games.itch.io/pixel-art-vfx-simple-explosion-1>
- <https://skalding.itch.io/volume-and-mute-icon>

SoundEffects were found on this website: 

- <https://pixabay.com>

Some code resources: 
- <https://github.com/3dgep/rasterizer>
- [`A Parallel Algorithm for Polygon Rasterization (Pineda, 1988)`](A_Parallel_Algorithm_for_Polygon_Rasterization.pdf)
- <https://dyn4j.org/2010/01/sat/#sat-axes>