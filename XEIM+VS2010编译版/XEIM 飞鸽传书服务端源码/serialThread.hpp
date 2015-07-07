/********************************************************************
	created:	2002/09/18
	created:	18:9:2002   21:05
	filename: 	C:\Development c++\Serial communication\SerialApp\serialThread.hpp
	file path:	C:\Development c++\Serial communication\SerialApp
	file base:	serialThread
	file ext:	hpp
	author:		Chaiyasit T.
	
	purpose:	
*********************************************************************/

#ifndef SERIALTHREAD_HPP
#define SERIALTHREAD_HPP

// class SerialThread
//-----------------------------------------------------------------------------
// Description: This class is inherited from CWinThread, use to handle in part 
//              of thread process (muliti tasking).
// 
#include <list>
#include <string>
using namespace std;

class CXsvrDlg;              // Forward Declaration.

class SerialThread : public CWinThread
{
public:
  // To enable objects of CObject-derived classes to be created dynamically at 
  // run timeto enable objects of CObject-derived classes to be created 
  // dynamically at run time
	enum { SMS_AT = 10,
			SMS_CMGF = 11,
			SMS_CLIP = 12,
			SMS_CLOSE = 13,
			SMS_UNKNOW = -1
		};

	// m_dwToDo;
	enum {
		SMS_WRITE = 1,
		SMS_READ = 2,
		SMS_WAIT = 0
	};

	DECLARE_DYNCREATE(SerialThread)  
   
  SerialThread ();               // Constructor.
  virtual ~SerialThread();       // Destructor.
public:
	BOOL ProcessWrite(LPCTSTR lpszMess, DWORD dwLen);
	void ProcessClose();
	BOOL ProcessWrite();
	void ReadTimeOut(LPCTSTR mess);
	BOOL ProcessRead(LPCTSTR szMess);
	BOOL SendSMS(LPCTSTR lpszSMS);
  virtual BOOL InitInstance();   // virtual function that derive from base-class.
  virtual int Run();             // virtual function that derive from base-class.  

  BOOL OpenCOM1Port();
  void setOwner(CXsvrDlg* ptrDialog)
  {ptrDlg = ptrDialog;}          // Assign the pointer to pointerDlg is as agent 
                                 // of Class CSerialAppDlg.
private:
	BOOL m_bSMSOpenPortActivate;
	DWORD m_dwToDo;
	list<string> m_listMess;
	CXsvrDlg* ptrDlg;         // The pointer that pointer to object CSerialAppDlg. 
};


// class SCC
//-----------------------------------------------------------------------------
// Description: This class is as the interface that is used to access the object
//              of Class SerialCtl.
//
class SCC
{
public:
  SCC() {};   // Constructor.
  ~SCC() {};  // Destructor.
public:
  static SerialCtl& serialCtl() // Implement as the singleton funciton.
  {
    static SerialCtl serial;
    return serial;
  }
};


#endif //SERIALTHREAD_HPP

/****************************End of file**************************************/