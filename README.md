The **tizen-app-assist** library provides APIs to easily build an application layout in accordance with general OOP design.
The APIs consist of some classes to handle the UI and some utility classes but it doesn't provide full features of the Tizen native API.
So, you also needs Tizen native APIs with this library for your application.

This library is implemented by only Tizen native APIs so **Tizen SDK should be installed first** for using it.

* [Tizen SDK download](https://developer.tizen.org/development/tools/download)


### Directory structure

* app-assist-efl
  * base : Common APIs for all profiles
  * wearable : APIs only for the wearable profile
  * projects : Tizen SDK project files of tizen-app-assist library for each profile
* samples : Some of samples
* templates
  * project-template : A template project
* make-docs.sh : A shell-script for making **API reference** doxygen

   ```
   $ make-docs.sh
   ```

* make-project.sh : A shell-script for making an empty application project

  ```
  $ make-project.sh --help
  ```


###[How to build and import tizen-app-assist](docs/how-to-build-and-import-tizen-app-assist.md)

###License
* tizen-app-assist is Open Source software under the Apache License 2.0. Complete license and copyright information can be found in the source code.

### [Governance](docs/governance.md)

### [Developers Certificate of Origin](docs/developers-certificate-of-origin.md)