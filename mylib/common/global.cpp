/************************************************************************************************
 *
 *		MTC Project - Client Main Project - ClientGlobalVariables
 *
 *
 *		@desc The global variables for the client.
 *		@author igarciad
 *
 ************************************************************************************************/

#include "global.h"

namespace mylib {

QHash<QString,QVariant> G::g;

G& G::global(){
	static G instance;
	return instance;
}

} // namespace mylib
	
