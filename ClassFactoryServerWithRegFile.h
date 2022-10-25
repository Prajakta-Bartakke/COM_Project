#pragma once
#include<unknwn.h>
#include<windows.h>
#include<stdio.h>

//rajesh pradhan
class IFileReader :public IUnknown
{
public:
	virtual HRESULT __stdcall ReadFileData(const char* filepath)=0;
};

// {FAE82944-FE1A-4A53-B49F-D1D57C202AA6}
const CLSID CLSID_CFileReader = { 0xfae82944, 0xfe1a, 0x4a53, 0xb4, 0x9f, 0xd1, 0xd5, 0x7c, 0x20, 0x2a, 0xa6 };

//// {D89F6EB3-6103-455F-A7E8-3992E62389B9}
const IID IID_IFileReader = { 0xd89f6eb3, 0x6103, 0x455f, 0xa7, 0xe8, 0x39, 0x92, 0xe6, 0x23, 0x89, 0xb9 };
