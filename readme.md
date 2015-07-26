# Mega Textures

This is a little experiment to do mega textures. This is meant to be a simple implimentation only.


## What Are Mega Textures

What I mean by mega textures is what existed in Quake Wars, not what was in Rage (Virtual Textures). Its also called dynamic mip mapping.



## Code

All the code surrounding the mega texture itself is in the `code/` directory. `external/` directory are helpers, mainly surrounding SDL use and math etc.


## How It Works (Brifely)

In a nutshell the system keeps a number of 512 x 512 textures. These textures cover a greater and greater area (usually landscape) at a lower and lower resolution. The idea being that the imediate area around the camera can be textured with a highly detailed 512 x 512 texture, while larger areas are covererd by another 512 x 512 texture but stretched over more terrain.


## Pros and Cons

** Pros **
- Very very large areas of terrain can be uniquely textured.
- Very low texture space, 5-6 (Whatever you wish) textures can be used to represent many km(s) of terrain. 

** Cons **
- Primarily 2D, no overhangs or caves.
- Requres constant rebuilding of textures.


## More Information

- http://www.beyond3d.com/content/articles/95/3
- https://en.wikipedia.org/wiki/Enemy_Territory:_Quake_Wars#MegaTexture
- http://wiki.splashdamage.com/index.php/An_Advanced_Terrain_and_Megatexture


## Included Projects

I've only included an xcode project, however rebuilding for Windows should be easy enough. There is nothing OSX specific. You'll just need to get the Windows SDL2 lib, and GLEW or something.


## Licence

Everything inside the `code/` directory is public. The rest is covered by their individual repos/projects.