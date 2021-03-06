SimpleSceneGraph is a C++ library built on top of SDL2 which provides a few features/abstractions for easier 2D graphics.

Features:
	
	1. Layers:
		Windows may have multiple layers, which automatically redraw themselves.
		
	2. Scene Graph:
		2D layers contain a tree-like structure of "Components" such as Sprites, Buttons, Textboxes, etc. which may be moved about around and on top of each other, rotated, scaled, etc.  Components may be combined into Nodes to act as larger components which maintain their positions, rotations, etc. relative to the parent component.
	
	3. Buttons:
		Buttons form one type of simple component.  They have background images, overlay images and overlay text, as well as overridable "callback" functions corresponding to various types of activation events (e.g. onMouseOver(), onMiddleClick(), etc.).
	
	4. Viewports:
		Components in 2D layers (i.e. in a scene graph) are positioned using a float-based world coordinate system independent of the SDL integer-pixel coordinates.  This allows for much easier scaling, positioning, etc.
	
	5. Offscreen Culling:
		Components which are offscreen are automatically not rendered.  Certain components, such as sprites, buttons, etc. which may potentially appear partially onscreen are only partially rendered.  This is potentially useful for sprites which are very large relative to the view area (e.g. a fixed map image, for example), eliminating the need for users to crop their texture source rectangles to the visible area.

A few simple examples/demos are included inside examples/.  To compile and run them, simply use
```
make [Name of Demo]
```
for example,
```
make demo_lorem_ipsum
```

A slightly more substantial example may be [found here](https://github.com/ortegagingrich/rectstack). 

To build the library, use:
```
make lib
```


Coming Soon:
	Doxygen Documentation


