# cries

Instructions for linking the sound engine

1. After pulling and merging the sound engine changes, open up your project
   properties in eclipse.

2. The sound engine includes have been added to the inc folder, so nothing 
   should need to be changed to the Includes settings

3. In the "Libraries" settings under "GCC C++ Linker," add the following to 
   the Libraries (-l) section:
	pthread, 
	IrrKlang

4. In the "Libraries" settings under "GCC C++ Linker," add the following to
   the "Library search path (-L)
	"${workspace_loc:/${ProjName}/lib}"

5. In Eclipse, click on the "Run" tab on the top and select "Debug Configurations."
   Then click on the "Environment" tab. Then select "New." In the "Name" field
   enter LD_LIBRARY_PATH and in the "Value" tab enter ${workspace_loc}/${project_path}/lib 

The project should now be able to compile and link the sound engine.
