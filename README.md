# Components

Components is a repository of specific Xcode project dealing with MacMap's main windows, graphics, geographic indexation... These projects generate plugins that are required for starting MacMap.

**Compilation :**
You can open each project with Xcode then compile, or use xcodebuild in a terminal window. As plugins are linked to MacMap frameworks, you have to build MacMapSuite.framework, MacMapStats.framework, and internal MacMap frameworks (mainly mox_intf.framework) before building components.

Note: MacMap projects are currently built with Xcode 11 against Mac OS X 10.12 SDK.

**Installation location :**
Plugins are installed in the standard MacMap plugins directory.
