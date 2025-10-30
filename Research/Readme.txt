SOLUTION MANUAL [LIBZMQ DLL MISSING]:

I had issues with LIBZMQ only solution I found was manually copying the .dll file from a certain location and then pasting it into the folder that has my .exe file.


0.	Issue with missing "libzmq-v143-mt-gd-4_3_5.dll"
	-> Make sure you build it all first..
	--> Also try to run it all afterwards.. 
	---> Once you gained the pop-up, it creates the bin folder & .dll file that you need!

1.	Go to: ... \SFML-ImGui_StartPoint\out\build\x64-Debug\_deps\libzmq-build\bin\Debug
	-> Find & Copy: "libzmq-v143-mt-gd-4_3_5.dll" File.

2.	Go to: ... \SFML-ImGui_StartPoint\out\build\x64-Debug\src\Debug
	-> Paste the "libzmq-v143-mt-gd-4_3_5.dll" File in this folder.

3.	All should correctly now!