# How to import tizen-app-assist library to your application

To use the tizen-app-assist library, you need to include it in your application project directly by either including the source code directly or importing prebuilt shared object library in your application package.

Refer to following guides.

## Build

1. Clone the tizen-app-assist git or download the zip file and extract it into your workspace folder.

2. There are two sdk project files prepared under app-assist-efl/projects folder. 
  * mobile/mobile2.4
  * wearable/wearable2.3.1

3. Choose a proper project and import it.
  ```
  From the Tizen IDE, File >  Import > Select Tizen/Tizen Project, and browse and select the project folder to be imported.
  ```

4. Build it and verify that the libapp-assist-efl.so shared object file has been created.

## Import

5. Add 'inc' folder of the imported project to your application project include path. 
  ```
  Open your application project Properties > C/C++ Build > Settings > "Tool Settings" Tab > C++ Compiler/Inclues, click '+' button and 'Workspace...' button and select the 'inc' folder of the imported project.
  ```

6. Add the library to your application project for linking
  ```
  Open your application project Properties > C/C++ Build > Settings > "Tool Settings" Tab > C++ Linker > Libraries > add "app-assist-efl"
  ```

7. If your application is based on Tizen 2.3.x SDK, add 'WUSING_SDK_2_3' to your application project.
  ```
  Open your application project Properties > C/C++ Build > Settings > Tool Settings / C++ Compiler / Preprocessor)
  ```

8. Make your application to refer to app-assist library, then when you build and package your application, app-assist library will be built and copied into your application's lib folder and packaged together automatically.
  ```
  Your application project Properties > Project References > Check the imported project, to make app-assist-efl library be built before.
  Your application project Properties > Tizen SDK > Package > Multi > Check the imported project under the 'Shared Library Projects' section, to make it be packaged together in your applications.
  ```

You can build the tizen-app-assist library as a static library as well. Then its size might be smaller than a shared library's one.
