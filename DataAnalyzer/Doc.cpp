#include "Doc.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
bool Doc::load(CString& filename)
{
	std::wifstream file(filename);
	if (!file.is_open())
	{
		return false;
	}
	std::wstring line;
	std::wstring field;
	if (!std::getline(file, line))
	{
		return false;
	}
	std::wstringstream headstream(line);
	while (std::getline(headstream, field, L','))
	{
		headers.push_back(CString(field.c_str()));
		// Store column name -> column index
		UINT columnIndex = headers.size() - 1;
		CString header(field.c_str());
		colmap[header] = columnIndex;
	}

	//data rows reading
	// Read data rows
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::wstringstream rowStream(line);

		std::vector<double> row;

		while (std::getline(rowStream, field, L','))
		{
			try
			{
				row.push_back(std::stod(field));
			}
			catch (...)
			{
				row.push_back(0.0);
			}
		}

		if (row.size() == headers.size())
		{
			rows.push_back(row);
		}
	}
	return true;
}
std::vector<CString>& Doc::getHeaders()
{
	return headers;
}

std::vector<std::vector<double>>& Doc::get_rows()
{
	return rows;
}

std::vector<double> Doc::get_collumn(CString headerName)
{
	std::vector<double> collumns;
	UINT index = colmap[headerName];
	for (int i = 0; i < rows.size(); i++)
	{
		collumns.push_back(rows[i][index]);
	}
	return collumns;
}
bool Doc::saveData(CString& filepath, CString& x, CString& y, std::vector<double>& dx, std::vector<double>& dy)
{
	
	CStdioFile file;
	if (!file.Open(filepath,CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		AfxMessageBox(_T("Could not Save file."));
		return false;
	}
	file.WriteString(x);
	file.WriteString(_T(","));
	file.WriteString(y);
	file.WriteString(_T("\n"));
	for (int i = 0; i < dx.size(); i++)
	{
		CString value;
		value.Format(_T("%.2f"), dx[i]);
		file.WriteString(value);
		file.WriteString(_T(","));
		value.Format(_T("%.2f"), dy[i]);
		file.WriteString(value);
		file.WriteString(_T("\n"));
	}
	file.WriteString(_T("\n"));
	file.Close();
	return true;
}
void Doc::clear()
{
	headers.clear();
	for(auto &row:rows)
	{
		row.clear();
	}
	rows.clear();
	colmap.clear();
		

}