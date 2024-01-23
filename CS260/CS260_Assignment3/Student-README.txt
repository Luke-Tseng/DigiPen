CS 260 Assignment 3 Automation
Authored by: Matthew Picioccio
DigiPen Institute of Technology
Last Updated: Fall 2020

FILES: 
assign3test-log.bat:  Executes the test for a specified directory prefix and logs the output for later review
assign3test-nolog.bat:  Executes the test for a specified directory prefix and allows the output to go to console, for easy visual review
CS260_Assignment3_Client.exe: Test client used by the automation, with usage "CS260_Assignment3_Client.exe <url> <proxy_port>".  It creates a HTTP request, with Connection: Close, for that URL, but sends it to the given proxy port on localhost.

REQUIREMENTS:
The batch files expect that that the Visual Studio solutions follow the submission naming and format requirements.  
Example of correct directory:
mattpic-CS260-3
-> CS260_Assignment3.sln 
-> CS260_Assignment3.vcxproj 
-> CS260_Assignment3.vcxproj.filters
-> CS260_Assignment3.vcxproj.user
-> *.h
-> *.cpp/.c
NOTE: all naming is important, as is the flat directory structure for at least the Visual Studio files.  A common mistake is to include a root directory inside the .zip, with all files in that directory - that will trigger a penalty, per the rubric.

INSTRUCTIONS:
1) Unzip this automation package to a specific directory (ex: c:\assign3).
2) Copy/move your assignment directory inside that automation directory (ex: c:\assign3\mattpic-CS260-3).
3) Open "Developer Command Prompt for Visual Studio 2019" and navigate to the root directory (ex: c:\assign3)
	NOTE: If you do not use the developer command prompt, the automation will likely fail to find msbuild.exe.
4) Test your code by running assign3test-nolog.bat directory (i.e., assign3test-nolog.bat mattpic-CS260-3) (easier by typing mattTAB).  It will run multiple clients in parallel.
5) Observe the output timing behavior - do all clients receive data at the same time, or do they wait in serial?  Any long delays? etc.
6) Close the proxy window left behind by the automation with Ctrl-C *before starting the next test*.   If you do not do this, you will get strange errors from subsequent runs.
7) You can instead run assign3test-log.bat and compare the results to the provided example.  Logs will be created in the results directory (ex: c:\assign3\results\mattpic-CS260-3), and the examples are in the Example directory (ex: c:\assign3\Example).

NOTES:
* You must close the proxy window before starting the next test, or subsequent tests will fail.  It cannot easily be captured for closure.
* The automation is run on the Release executable, but it builds both Debug and Release for easy validation.
* Note that if either build fails, the automation will stop, but errors from the tests are logged in the -error.txt files and will not stop the automation.
* Note that the -error.txt log files should all be empty during correct operation.  Any cerr output from the *test client* will be routed there.
* Generally, you will find the -nolog.bat file more useful during development.  The -log.bat version and example logs are present to aid analysis, especially if you're running into any issues.
* You may receive a one-time Windows Firewall request for your proxy and/or the test client.  You should accept that request.  It should not disrupt the test results, though. 
* Contact your instructor and/or TAs if you have any questions or run into any issues with the automation.