#pragma once
#include "pch.h"
#include <vector>
#include <map>


class Doc
{
public:
	bool load(CString& filename);
	std::vector<CString>& getHeaders();
	std::vector<double> get_collumn(CString headerName);
	std::vector < std::vector<double>>& get_rows();
	bool saveData(CString& filepath, CString& x, CString& y, std::vector<double>& dx, std::vector<double>& dy);
	void clear();
private:
	std::vector<CString>headers;
	std::vector<std::vector<double>>rows;
	std::map<CString, UINT>colmap;
protected:
};