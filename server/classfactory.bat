cl.exe /c /EHsc ClassFactoryServerWithRegFile.cpp

link.exe ClassFactoryServerWithRegFile.obj /DLL /def:ClassFactoryServerWithRegFile.def user32.lib gdi32.lib