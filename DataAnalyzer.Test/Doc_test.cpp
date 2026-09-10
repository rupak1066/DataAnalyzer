#pragma once
#include"pch.h"
#include "Doc.h"
#include<vector>
#include<map>
//need to use local variable mdoc for it to be true unit tests
TEST(Doc, loading)
	{
		Doc mdoc;
		CString mString("D:/Projects/DataAnalyzer/test.csv");
		ASSERT_TRUE(mdoc.load(mString)) << "Failed To load a Valid file";
		auto headers = mdoc.getHeaders();
		ASSERT_EQ(headers.size(), 2);
		ASSERT_EQ(headers[0], _T("d_x"));
		ASSERT_EQ(headers[1], _T("e_x"));

		mString = _T("Invalid.csv");
		ASSERT_FALSE(mdoc.load(mString)) << "Gives False information of loading invalid file";

		mString = _T("D:/Projects/DataAnalyzer/test_text.txt");
		ASSERT_TRUE(mdoc.load(mString)) << "fails to load .txt files";

		mString = _T("D:/Projects/DataAnalyzer/empty.csv");
		ASSERT_FALSE(mdoc.load(mString)) << "Gives False information of loading valid but empty file";

	}
TEST(Doc, Column_mapping)
{
	Doc mdoc;
	CString mString("D:/Projects/DataAnalyzer/test.csv");
	ASSERT_TRUE(mdoc.load(mString)) << "Failed To load  file";

	ASSERT_EQ(mdoc.get_collumn(_T("d_x")).size(), mdoc.get_collumn(_T("e_x")).size()) << "not equal size";
	ASSERT_EQ(mdoc.get_collumn(_T("d_x")), (std::vector<double>{4.00, 48.00, 12.00, 16.00, 20.00,3.00,7.00,11.00}))<<"mapping is faulty";
	ASSERT_EQ(mdoc.get_collumn(_T("e_x")), (std::vector<double>{5.00, 10.00, 15.00, 20.00, 25.00, 6.00, 7.00, 1.00})) << "mapping is faulty";
}

TEST(Doc, data_integrity)
{
	Doc mdoc;
	CString mString("D:/Projects/DataAnalyzer/test.csv");
	ASSERT_TRUE(mdoc.load(mString)) << "Failed To load  file";
	auto rows = mdoc.get_rows();
	ASSERT_EQ(rows.size(), 8);
	ASSERT_EQ(rows, (std::vector<std::vector<double>>{ { 4, 5 }, { 48, 10 }, { 12, 15 }, { 16, 20 }, { 20, 25 }, { 3, 6 }, { 7, 7 }, { 11, 1 } }));
	

}

TEST(Doc, data_clear)
{
	Doc mdoc;
	CString mString("D:/Projects/DataAnalyzer/test.csv");
	ASSERT_TRUE(mdoc.load(mString)) << "Failed To load  file";
	mdoc.clear();
	ASSERT_EQ(mdoc.get_rows().size(), 0);
	ASSERT_EQ(mdoc.getHeaders().size(), 0) << "headers not getting cleared even after calling clear";
}

