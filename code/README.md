## Install
- On Arch:
  - pacman -S sdl2 sdl2_image

## Done:

Day 0: Floating grass
  - Move source to src
  - Add third party imports folder
    - Move sdl import to third_party
  - Move resources to res
  - Add textures using texture loading using IMG

# Design

## Task List
4 hours a day

Day 1: 3 hours

POC1:
- Load image of character and render(Done: 2
  hours)
- Load image of background and render it behind
- Test Painters algorithm

POC2:
- Test sizes of image
- Repeat images over area of window
- Clipping textures

Day 2: 3 hours
POC3:
- Move image around screen
- Load background and render

POC4:
- Find sprite sheet
- Image should animate with sprite sheet
- It should move around

Day 3:

- Objects
- Collision based on circles
- Enemies
- Random movement of enemies

Day 3:
- Background map
- Moving camera around image
- Move camera around when character moves

Day 4:
- ECS component system(Import BirchBox component)

Day 5:
- Tilemaps
- Tilemap collisions

Day 6:
- Integrate DearImgUI in a separate project  
- Inventory

Day 7:
- Inventory stacking
- Inventory dropping

Day 8:
- Collision based on tilemaps

https://indiegamedev.net/2020/05/19/an-entity-component-system-with-data-locality-in-cpp/
https://gameprogrammingpatterns.com/component.html

## Commands:

montage font_[1-7].png -tile 8x1 -geometry 16x16+1+1 -background none tile_9x1.png

