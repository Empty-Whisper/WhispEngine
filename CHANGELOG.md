# Changelog

## [Assignment3.1] - 28/12/2019
### Added

- Script independance
- Don't mouse picking in Game Panel
- Street FBX
- More Brofiler
- ZeroBrane autocomplete

### Fixed

- Save and Load scripts duplicates and don't load some inspector vars


## [Assignment3.0] - 26/12/2019
### Added

- Integrate Lua and LuaBridge
- Scripting System
- Scripting Functions
	- Log
	- Input
		- Keyboard
		- Mouse
	- Gameobject
		- active
		- name
		- transform
		- Instantiate
		- Destroy
	- transform
		- position
		- rotation
		- scale
		- forward, up, right
		- set position, rotation and scale
		- Find, GetChild...
		- SetParent
	- Vector3
	- Quaternion
	- Time
	- Random
- Component Script with editable variables
- Lua Macros for inspector
- Play, Pause and One Frame system with scripts
- Drag&Drop scripts in GameObjects inspector
- More Brofiler Categories
- Prefabs (Save and Load)
- Beginning of web page
- Script Editor in engine
- Shortcut Panel Editor
- Saves Camera rotation when save scene
- Tank base scripts

### Changed

- Panel Resources Optimized :D (5 ms won)

### Removed

- glmath.h

## [Assignment2] - 24/11/2019
### Added

- Hierarchy GameObjects
- Components properties on inspector
- Octree
- Scene Serialization (Load and Save)
- Part of Bounding Boxes
- Own Format
- Resource manager
- Library and Assets logic
- .meta file
- Reference Counting
- Start/Pause/Stop and recover init state
- Transform Camera editor to Fustum
- Camera class logic 
- Frustum culling
- Component Camer
- Camera Preview
- Game panel
- Viewport class logic
- Mouse Picking
- Substract all Modules from App.h
- Add Guizmos
- Guizmos logic to Game Objects

## Fixed
- Now meshes have always same ID
- Camera Movement

## [Assignment1.3] - 27/10/2019
### Added
- Bounding Box

### Changed
- Camera controls work with bounding boxes

### Fixed
- When import an fbx with polygons with more tha 3 vertex, crashes because cannot calculate normals faces

## [Assignment1.2] - 26/10/2019
### Added
- Can deselect GameObjects clicking in hierarchy window
- All panels can be access in Windows menu bar
- Brofiler

### Fixed
- Dt code
- Can Focus on an object without need to move mouse to scene panel
- #11
- Drag and drop of pictures (png and jpg)

## [Assignment1.1] - 26/10/2019
### Added
- Drag and Drop GameObjects in hierarchy
- Move Up/Down, delete GameObjects in hierarchy
- Buttons to create Empty GameObjects
- Inspector improved info
- Delete components
- AABB
- Clear Button console

### Changed
- Improved recalculation matrix
- Mesh creates with material and only can delete mesh
- Shortcuts

### Fixed
- #4
- #6
- Outline improved
- Draw normals
- Cannot change GameObject name
- #8
- #9
- Hide texture to show of Engine logo
- Remaximize window displays scene tab in black
- Demo shapes crash

## [0.3.0] - 21/10/2019
### Added
- GameObject System with Components
- Components
  - Transform
  - Mesh
  - Material
- Hierarchy GameObjects
- Import FBX generating good GameObject hierarchy
- Panel Hierarchy
- Assets folder organization
- Panel Inspector displaying all components
- Active/Deactive & Delete GameObjects
- Outline display in selected object
- Docking Improved. Added Scene in a panel

### Fixed
- Normals display

### Changed
- Camera Controls Improved

## [0.2] - 11/10/2019
### Added
- Docking panels
- DevIL
  - Import textures (Drag&Drop)
  - Selectable Textures in UI
- Objects lighting
- Icon Program

### Changed
- Camera Controls
- Grid in an array buffer

## [0.1] - 7/10/2019
### Added
- Core loop
- ImGui library
  - Configuration Panel with basic app info
  - Console Panel
  - Create Panel to create primitives
  - Inspector Panel with data of all scene objects
  - Main menu bar
- Assimp library
- Drag & Drop functionality (FBX)
- JSON Parser (Modern C++)
- Math Geo Lib
- mmgr
- PCG random number generator
- GPU Detect library (DeviceID)

- Camera basic controls (WASD R F mouse rotation)
