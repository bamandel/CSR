#include "CandidateSearch.h"
#include <shlobj.h>
#include <windows.h>
#include <string>
#include <algorithm>
using namespace std;
int main()
{

    TCHAR path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = ("Locate folder containing patient data.");
	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

	if ( pidl != 0 )
	{
		// get the name of the folder and put it in path
		SHGetPathFromIDList ( pidl, path );

		//Set the current directory to path
		SetCurrentDirectory ( path );
                string name(path);


		// free memory used
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( pidl );
			imalloc->Release ( );
		}
		replace(name.begin(),name.end(),'\\','/');
                CandidateSearch search(name);
	}


 //CandidateSearch search ("Josh");
        cout<<"Please type 'e' when ready to exit"<<endl;
        string input = "";
        getline(cin,input);
        if(input == "e")
            return 0;
    return (0);
}
