## Web Site Manager (WSM)



## Introduction

Web Site Manager (WSM) is an application that is designed specifically to manage the files that are stored in a web site project directory.
A web site directory structure will have webpage related files such as image, video, sound, html page and .css files in a structured form of specific directories.
Web site manager not only manages the simple tasks of any file manager software such as move, delete and copying of files, but it also performs the tasks of 
tracking and updating the html links and source dependencies that exist with the html web page files of the project.

The Web Site Manager application is not a conventional file manager with two or more lists of directory contents in a panel, but is a file manager that displays
the file structure of a web project as a tree. (Fig 01) 

<div align="center">
    <img src="./Documentation/wsm_screenshot00.png" width="600px"</img> 
</div>
 
 <p style="text-align: center;">Fig 01</p>

Inspiration for doing this came from Dreamwaever which has these capabilities, but since Adobe asks too high a price for monthly subscriptions for a part time user
of such software, I decided to try and emulate as best I could this feature that could not be found in any other application. It only took me a few hours to figure
out the basics of how to do this from scratch, and I was agast why no other software that edits and manages html web pages and files has not this feature.

This is still in development, but most of the features required are present. Moving, renaming, deleting and displaying of files with some simple
analytics such as viewing file links and selecting file types are present and functional. 

The application uses ImGui and is coded in ISO C++20 Standard as a visual studio community project.

A user manual and code documentation are not yet provided, but will be inculded in a future update.

## Installation:

This project is a Visual Studio 2022 SDK 10 ISO C++ 20 Standard project set up for windows. 

1 : Download the files within the ws_manager repository to a desired directory location.

2 : Open visual studio and open the visual studio project ws_manager.sln file.

3 : Compile and run the code. 

## How to use

1  : One will need a website directory structure with all the files to display html web pages in a browser present

2  : Select the drop down menu "Project" in the main application window and select the add project option. Navigate to the directory where the website project exists and select the website project directory to store the project directory
	 
3  : In the main window panel select with the left mousebutton the drop down combo button next to the text Select Web Site Project and select the directory that was defined in step 2.

4  : A tree structure of the directories and files that exist in the selected web site project should now be displayed.

5  : Selection of files and directories is made with the left mouse button. Multiple selections by pressing the control key.

6  : Hovering over a file or directory and pressing the right mouse button will display menu options to take actions on the hovered item or selection.

## Dependencies
    These are a list of the current third party dependacies for this project

    glfw
    glew
    ImGui
    imgui-docking
    spdlog


Dependency source header files to be defined where $(ProjectDir) is the project directory where the header files are located

    $(ProjectDir)thirdparty\assimp
    $(ProjectDir)thirdparty\glew\include
    $(ProjectDir)thirdparty\glfw\include
    $(ProjectDir)thirdparty\glm-1.0.1
    $(ProjectDir)thirdparty\ImGUI\imgui_docking
    $(ProjectDir)thirdparty\ImGUI\imgui_docking\backends
    $(ProjectDir)thirdparty\stb_image\include
    $(ProjectDir)thirdparty\tiny_obj_loader\include
    $(ProjectDir)thirdparty\tinyply\include
    $(ProjectDir)thirdparty\tinyxml\include
    $(ProjectDir)thirdparty\ImGuiFileDialog
    $(ProjectDir)thirdparty\utf8\include
    $(ProjectDir)thirdparty\glad\include
    $(ProjectDir)thirdparty\spdlog\include

Dependency libs to be defined where $(ProjectDir) is the project directory where the lib files are located

    $(ProjectDir)libs\Release\glfw3.lib
    $(ProjectDir)libs\Release\glew32.lib
    $(ProjectDir)libs\Release\spdlog.lib
    $(ProjectDir)libs\Release\assimp-vc142-mtd.lib
    $(ProjectDir)libs\Release\imgui_gradient.lib 
    opengl32.lib

Note : The existing lib files have been compiled for visual studio SDK 10.0 version using  ISO C++ 20 language standard. Thus may not work for other versions of visual  studio and C++ language standard and need to be recompiled.

## Source Code

Because this is a working project, within the source code is a lot of debugging code that has largely been commented out.

Much of the code has been written for as easy reading as much as possible to understand what the code does and is for. However some of the code that has been adopted or copied from 3rd parties may follow a different naming convention.



