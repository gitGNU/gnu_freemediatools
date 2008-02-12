// Guillaume Vareille 2005-3-27
// this include is a safe solution to avoid duplicating this code 
// it is named .h to suppress attempt to comile it as a cpp file


// Mandatory routine used to return instance of plugin to calling application
OpenObjectLibPlugin * getPluginClass()
{
    return new OpenObjectPlugin();
} ;


OpenObjectPlugin :: OpenObjectPlugin ( ) : OpenObjectLibPlugin ( ) 
{ 
	initializePlugin ( ) ; 
	
	//initalize manager
	objectManager = new QPtrList<pluginObjectManager>;
} ;


// Guillaume Vareille 2005-3-27
// This can't be put in OpenObjectLibPlugin in "openobjectplugintemplate.h"
// because it needs pluginObjectManager to be define.
// So it must be in the class OpenObjectPlugin of every plugin.
// As it is the same for all the plugin and musn't be modified, 
// here is the rigth place
int OpenObjectPlugin :: findObjectLayer ( int aObjectId )
{
	static int oldobject = 0 ;
	static int newobject = 0 ;
	static int thelayer  = 0 ;

	if ( 0 == numberObjects )
		return 0 ;

	newobject = aObjectId ;

	//this is a attempt not to have to re-search if we are drawing the same object
	//ie one active object in the scene...
	if ( oldobject == newobject )
		return thelayer ;

	//ok now we search for objnum in list
	for ( int i = 0 ; i <= numberObjects ; i ++ )
	{
		if ( objectManager -> at ( i ) -> mObjectID == aObjectId )
		{
			thelayer = i ;
			oldobject = newobject ;
			break ;
		}
	}

	return thelayer ;
} ;
