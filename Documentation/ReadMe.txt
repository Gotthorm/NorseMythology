Mythology 

01/27/2019
Began a complete reassessment of the solution:
- Code review of "Midgard"
- Code review of "Framework"

TODO:
- Research the "needs to have dll-interface" warning
- Move data object creation from framework into game module
- Mouse input is fixed but I need to change the input to read buffers instead of single events
	to prevent the queued input when vsync is used.

Alfheimr - Console/Scripting
Asgard - Testing
Svartalfheimr
Helheimr - Input
Jotunheimr
Midgard - Main Application
Muspelheim - Graphics
Niflheim - Messaging/Logging
Nildavellir - Deprecated
Valhalla - Data conversion
Vanaheimr - Utility/Common

Decide whether Surface objects needs to be exposed outside of the Muspelheim module.  
Could just use id's and add modifier methods to the renderer interface.

Setup project dependencies so that updated DLL's get built.
Create virtual interfaces for other projects to hide private and protected and eliminate warnings
Flesh out new renderer and its "surfaces".