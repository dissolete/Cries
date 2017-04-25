# cries

Instructions for linking the sound engine

1. After pulling and merging the sound engine changes, open up your project
   properties in eclipse.

2. The sound engine includes have been added to the inc folder, so nothing 
   should need to be changed to the Includes settings

3. In the "Libraries" settings under "GCC C++ Linker," add the following to 
   the Libraries (-l) section:
	pthread
	IrrKlang

4. In the "Libraries" settings under "GCC C++ Linker," add the following to
   the "Library search path (-L)
	"${workspace_loc:/${ProjName}/lib}"

The project should now be able to compile and link the sound engine.