# RESTful API for foobar2000

Default port: 20402 = 0x4FB2

Implemented endpoints:

* / [GET, server availability check]
* /playback/events [GET, HTML5 Server-Sent Events, playback state changes]
* /playlists [GET]
* /playlists/events [GET, HTML5 Server-Sent Events, hard-coded dummy data]
 
See test.html for basic functionality tests.

## Source Structure

The component consists of the following modules.

* Core: Contains foobar2000 service implementations, e.g component version information and life cycle listeners.
* Requests: Contains request handler implementations.
* Bridge: Mediates between the foobar2000 and the Poco world.
 
The main reason for this structure is the incompatiblity between Winsock version 1 and 2. Unfortunately, foobar2000 uses Winsock version 1 and Poco::Net uses Winsock version 2. Therefore some care must be taken to not include foobar2000 SDK headers and Poco::Net headers in the same compilation unit. The table below shows which includes are allowed in each module.

<table>
 <thead>
  <tr>
   <td>Module</td>
   <td>Allowed includes for header files</td>
   <td>Allowed includes for implementation files</td>
  </tr>
 </thead>
 <tbody>
  <tr>
   <td>Core</td>
   <td>foobar2000 SDK<br />Poco excluding Poco::Net<br />Bridge</td>
   <td>foobar2000 SDK<br />Poco excluding Poco::Net<br />Bridge</td>
  </tr>
  <tr>
   <td>Requests</td>
   <td>Poco including Poco::Net</td>
   <td>Poco including Poco::Net<br />Bridge</td>
  </tr>
  <tr>
   <td rowspan="2">Bridge</td>
   <td rowspan="2">Poco excluding Poco::Net</td>
   <td>Poco excluding Poco::Net<br />foobar2000 SDK</td>
  </tr>
  <tr>
   <td>Poco including Poco::Net<br />Requests</td>
  </tr>
 </tbody>
</table>

The implementation files in the Bridge module are split into two categories. The proper thing to do would be to split the Bridge module into two modules. However there is only one implementation file at the moment which falls into the second category (for the main server class).

## Links
* [foobar2000 Home Page][1]
* [POCO C++ Libraries][2]
* [Windows Sockets 2][3]

[1]: http://www.foobar2000.org
[2]: http://pocoproject.org/
[3]: http://msdn.microsoft.com/en-us/library/windows/desktop/ms740673(v=vs.85).aspx
