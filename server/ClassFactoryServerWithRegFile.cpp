#include"ClassFactoryServerWithRegFile.h"

class CFileReader :public IFileReader
{
private:
	long m_cRef;

public:
	//constructor and destructor
	CFileReader();
	~CFileReader();

	//IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall	AddRef();
	ULONG __stdcall Release();

	//IFileReader specific method
	HRESULT __stdcall ReadFileData(const char*);

};

class CFileReaderClassFactory :public IClassFactory
{
private:
	long m_cRef;

public:
	//constructor and destructor
	CFileReaderClassFactory();
	~CFileReaderClassFactory();


	//IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	//IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown* pUnkouter, REFIID riid, void** ppv);
	HRESULT __stdcall LockServer(BOOL);

};

long glNumberOfActiveComponents;
long glNumberOfServerLocks;

BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

CFileReader::CFileReader()
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CFileReader::~CFileReader()
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CFileReader::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IFileReader*>(this);
	}
	else if (riid == IID_IFileReader)
	{
		*ppv = static_cast<IFileReader*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CFileReader::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CFileReader::Release()
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT CFileReader::ReadFileData(const char* filepath)
{
	//read kelela data file madhe write 
	MessageBox(NULL, TEXT("ReadFileData function"), TEXT("success"), MB_OK);
	char data[1024];
	FILE* fp_read = fopen(filepath, "r");
	FILE* fp_write = fopen("C:\\Projects\\01_Dialouge_box\\Dialouge_version5\\FileReader\\op.txt", "a");
	while (fgets(data, 10, fp_read) != NULL)
	{
		fprintf(fp_write, "%s", data);
	}
	fclose(fp_write);
	fclose(fp_read);

	return S_OK;
}

CFileReaderClassFactory::CFileReaderClassFactory()
{
	m_cRef = 1;

}

CFileReaderClassFactory::~CFileReaderClassFactory()
{
	//code
}

HRESULT CFileReaderClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CFileReaderClassFactory::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CFileReaderClassFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		return m_cRef;
	}

	return m_cRef;
}

HRESULT CFileReaderClassFactory::CreateInstance(IUnknown* pUnkouter, REFIID riid, void** ppv)
{
	CFileReader* pCFileReader = NULL;
	HRESULT hr;

	if (pUnkouter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}
	pCFileReader = new CFileReader();

	if (pCFileReader == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCFileReader->QueryInterface(riid, ppv);

	pCFileReader->Release();

	return hr;
}

HRESULT CFileReaderClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLocks);
	}

	return S_OK;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	if (rclsid != CLSID_CFileReader)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	CFileReaderClassFactory* pCFileReaderClassFactory = NULL;
	HRESULT hr;

	pCFileReaderClassFactory = new CFileReaderClassFactory();

	if (pCFileReaderClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCFileReaderClassFactory->QueryInterface(riid, ppv);
	pCFileReaderClassFactory->Release();
	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}