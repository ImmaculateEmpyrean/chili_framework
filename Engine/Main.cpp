/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Main.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "graphics/MainWindow.h"
#include "Game.h"
#include "utilities/ChiliException.h"

static HINSTANCE hInst;
static LPWSTR pArgs;

int execMain()
{
	try
	{
		MainWindow wnd(hInst, pArgs);
		try
		{
			Game theGame(wnd);
			while (wnd.ProcessMessage())
			{
				theGame.Go();
			}
		}
		catch (const ChiliException& e)
		{
			const std::wstring eMsg = e.GetFullMessage() +
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox(e.GetExceptionType(), eMsg);
		}
		catch (const std::exception& e)
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr(e.what());
			const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox(L"Unhandled STL Exception", eMsg);
		}
		catch (...)
		{
			wnd.ShowMessageBox(L"Unhandled Non-STL Exception",
				L"\n\nException caught at Windows message loop.");
		}
	}
	catch (const ChiliException& e)
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		MessageBox(nullptr, eMsg.c_str(), e.GetExceptionType().c_str(), 0x00000000L);
	}
	catch (const std::exception& e)
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr(e.what());
		const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
			L"\n\nException caught at main window creation.";
		MessageBox(nullptr, eMsg.c_str(), L"Unhandled STL Exception", 0x00000000L);
	}
	catch (...)
	{
		MessageBox(nullptr, L"\n\nException caught at main window creation.",
			L"Unhandled Non-STL Exception", 0x00000000L);
	}

	return 0;
}


#if _WIN64
int WINAPI wmain(int argc, wchar_t* argv[])
{
	hInst = GetModuleHandle(NULL);
	pArgs = *argv;

	execMain();
}
#else
std::unique_ptr<wchar_t> GetWC(char* c)
{
	std::unique_ptr<wchar_t> returnContainer;

	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];

	mbstowcs(wc, c, cSize);
	returnContainer.reset(wc);

	return returnContainer;
}

//for some reason __stdcall is not working for 32 bit.. bit more research has to be done regarding this..
int __cdecl main(int argc, char** argv)
{
	hInst = GetModuleHandle(NULL);
	pArgs = GetWC(*argv).release();

	execMain();
}
#endif