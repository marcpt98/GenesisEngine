# Fork
- This is a fork from https://github.com/marcpages2020/GenesisEngine made by https://github.com/marcpages2020.
- The first and second assignment are made by him.
- Our original engine (first and second assignment): https://github.com/Sauko22/Unfalse-Engine.
- The third assignment will be made by us.

# Unfalse-Engine
Unfalse Engine is a game engine made with C & C++ by two students from the 3rd grade of Design and Development of Videogames, from the CITM UPC (Terrassa).
Marc Palma & Pol Juan were the responsibles to make this project come true.

## Repository
https://github.com/marcpt98/Unfalse-Engine-2.0

## Team
- Pol Juan Santos: https://github.com/Sauko22
- Marc Palma Turon: https://github.com/marcpt98

## Skeletal Animation
### General Info
- The model that is loaded into the scene starts in the original pose.
- In the Hierarchy window can be seen the bone hierarchy, the gameobject with mesh and texture data and the main gameobject that manages the animation.

### Animation Inspector
With the main gameobject selected, all the component animation options can be seen in the inspector:
  - Enabled: Enable/Disable update of the component.
  - Path: Shows the assets and library path of the animation.
  - Animation info: Shows the channels, duration of the animation that is currently on progress, ticks and the progress of the current animation.
  - Show Bones: Enable/Disable the inside skeleton (activate wireframe in order to see it).
  - UID: Resource UID.

### Play/Pause/Stop/Resume
- Play: In order to see the character animated and the differents animations this button must be pressed.
- Stop: At any time of the animation, this button can be pressed in order to return to the original character position.
- Pause/Resume: While play is enabled, this button can be pressed in order to stop the animation in the current frame. If this button is pressed again, the animation continues.

### Play Animations
This model has a total of 3 animations:
- Idle: This is the default animation. It'll be played when Play button is pressed or when another animation has finished.
- Walk: This animation can be triggered by holding key "2". It'll be played until the key is released.
- Attack: This animation can be triggered by pressing key "1". It'll perform an attack and, after the animation, if the key "2" is still pressed will return to the walk animation and if not, it'll return to idle. This animation can be triggered either in the idle or walking animation.

## Controls: 
  To freely move the camera in the scene, the scene window must be selected or focused.

### Camera Controls
 - W / Up Arrow: Move Forward
 - A / Left Arrow: Move Left
 - S / Down Arrow: Move Backwards
 - D / Right Arrow: Move Right

 - Mouse Wheel: Zoom In/Out
 - Middle Mouse: Drag to move horizontally and vertically. 
 - Right Click: Orbit around the selected object. (If an object is not selected it will orbit around the center of the scene).
 - F: Focus the camera around the selected object.(If an object is not selected it will focus around the center of the scene).
 - O: Move Up.
 - L: Move Down.
 - Hold Shift: Duplicate movement speed.
 
### Game Objects Inspector actions
 - Transform: Translate, Rotate and Scale the object. 
 - Mesh: Toggle the view of the vertex and face normals. 
 - Material: 
     - Toggle the checkers image. 
     - Remove the current texture. 

### Actions in tabs:
 - Windows: Enable the engine windows. 
 - Edit: 
   - Configuration: Change the engine settings and view graphs about the performance. 

 - Game Object: There are six primitives available to be created at any time. These are: Cube, Plane, Cylinder, Sphere,
   Cone and Pyramid. These are duplicated, the firsts are generated automatically and have no texture coordinates and the 
   ones marked with FBX will be loaded from FBX files and will have texture coordinates. 

 - About: Get the pages where the project is held in GitHub and get more information about the engine.

### Scene Saving
 - Scenes can be saved and loaded from the editor main bar. Even though they can be saved anywhere it is recommended to do so in the scenes folder to keep everything organised. Select the desired folder and write the name of the scene with no extension, it will be added automatically. If an existing scene is selected it will be overwritten. 

### Importing Files
 For proper file import it is recommended that the files are already inside the Assets folder before starting the engine. If they are not, files can be dragged and dropped onto the engine in order to be imported. One last way to import files is copying them in the Assets folder and push the Reload button under the Assets hierarchy in the Assets window. Even though fbx files can be imported directly dropping them onto the engine it is recommended that for textures they are first copied in a folder inside assets and then dragged onto the engine to acces the importing options. 

 ### Importing Options
There are plenty of importing options specially for textures. When the file is dropped onto the engine an importing window will pop up to select the desired options. The only model format supported is fbx so if a warning or error message is displayed saying that a file from a different format can't be imported, this is the reason. 

### Camera Culling
 - The camera in the center of the screen is the main camera. Move it to see how camera culling is applied to objects. In the menu bar editor camera culling can be applied to see all objects in scene. 

### Things to take into account
 - At the end of the inspector window there is a ab called Resources. In this tab it is shown all the resources which are currently loaded into memory. If numbers seem to not match for textures take into account that assets window icon textures and preview textures are included in the list.

 ## Additional functionality

 ### First Assignment
 - The engine settings are loaded directly from a JSON file named config and located in GenesisEngine/Assets/Config. 
   this settings include the modules values and which windows are opened on start. 
 - When and FBX is loaded the hierarchy of the objects is mantained and the name of the objects in the FBX will be assigned
   to the new Game Object.  
 - Objects can be deleted selecting them and pressing SUPR.  

## Second Assignment
 - Scenes can be saved with custom name and overwritten by just clicking on them.
 - Textures can be previewed in the assets window. 
 - Interactive path in assets window which lets the user move easilly bacwards in the file or folder path. 
 - Meshes can be selected individually from inside a model pushing the arrow next to the file in the assets window. 
 - Inside the configuration window there is an option under the resources tab which lets you delete all meta files and library files at shutdown so projects can be cleaned before releases or sending to mates. 
