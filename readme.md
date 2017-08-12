# XML Parser for SL APP
## General description
The purpose of this application is to get current data from the Trafiklab API and present the information on a display. The application will in the end run on a microcontroller in the ESP8266 class which has a TCP/IP stack and WiFi capabilities.

### Trafiklab SL APIs
The projects uses Trafiklabs SL APIs to get real time information from when trains/buses etc. arrive. The projects has a [mirror site over there][Trafiklab project].

APIs used:

  * [Trafiklab SL-platsuppslag]
  * [Trafiklab SL-realtidsinformation 4]

[Trafiklab project]: http://trafiklab.se/projekt/sl-makerspace-sign
[Trafiklab SL-platsuppslag]: http://trafiklab.se/api/sl-platsuppslag
[Trafiklab SL-realtidsinformation 4]: http://trafiklab.se/api/sl-realtidsinformation-4

Note that the Trafiklab APIs return results in Swedish, which requires some special handling of UTF-8 characters outside of the ASCII range (mainly - and perhaps only - the umlaut characters `åäöÅÄÖ`)

## Functional description
The following sections explain the source code. It is separated into categories corresponding to the purpose/source file names.

The code is developed in a Test Driven manner (TDD) inspired by the book [Test-Driven Development for Embedded C](https://pragprog.com/titles/jgade) by James W. Grenning. I have used the [Unity test fixture](https://github.com/ThrowTheSwitch/Unity/tree/master/extras/fixture) developed by the Grenning, which is built on the [Unity test harness](https://github.com/ThrowTheSwitch/Unity). The source code is therefore compiled into several executables:
  
  1. Test runner on development platform
  2. Main executable on development platform
  3. Test runner on target
  4. Main executable on target

The `make` structure might therefore develop into something overwhelming. I will try to keep it as clean as possible and refactor when possible.

### XML parser
The XML parser makes only the simplest parsing that is needed for this project:

  * It can find a node's start and end
  * It can find the inner part of a node
  * It can find the next node within the parent
  * Only leaf nodes will contain text
  * Nodes' attributes will be skipped

#### Tests
  
  * Can find a node within a string
  * Can verify that a node does not exist
  * Can get the inner part of a node

##### SL API Offline files
Two offline example files are used for development tests: **Station lookup** and **Departure lookup**. For both, test that:

  * `StatusCode == 0` (OK)
  * ResponseData node exists

### ESP8266 implementation
The Arduino environment is used for the source code of the target platform (ESP8266).

## License
GNU GPL applies to all files in this project. See the file called `LICENSE` for more information.