// ScriptReloader.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ScriptReloader.h"

//개별 모듈

size_t CScript::GetNumOfObject()
{
	size_t numOfObject = 0;
	for (auto iter = m_Scriptvector.begin(); iter != m_Scriptvector.end(); ++iter)
		numOfObject += (*iter)->size();

	return numOfObject;
}

void CScript::ScriptPrintOut()
{
	string filetail = ".script";
	int iter_pos = 0;
	iter_pos = m_scriptname.find(".csv", iter_pos);

	if (iter_pos != std::string::npos)
		m_scriptname.replace(iter_pos, strlen(".csv"), filetail);
	else
		m_scriptname += filetail;

	ofstream out(m_scriptname);
	out << "Scriptname: " << m_scriptname << endl;
	out << "NumberOfObjects: " << GetNumOfObject() << endl;

	for (auto &iter : m_Scriptvector)
	{
		out << "MeshName: " << iter->Getmeshname() << endl;
		out << "Count: " << iter->size() << endl << endl;
		for (auto &con_iter : iter->GetSCRvectors())
		{
			out << "Pos: " << con_iter.m_x
				<< " " << con_iter.m_y
				<< " " << con_iter.m_z << endl;
			out << "Rotate: " << con_iter.m_yaw
				<< " " << con_iter.m_pitch
				<< " " << con_iter.m_roll << endl;
		}
		out << endl << endl;
	}
	out.close();
}

bool CScript::loadRawScript(string filename)
{
	SetScriptFilename(filename);

	ifstream in(filename);
	if (in.fail())
	{
		in.close();
		return false;
	}
	string ignore;
	string lObject_name;
	float x_pos = 0;
	float y_pos = 0;
	float z_pos = 0;
	float yaw = 0;//pitch, roll 은 아직은 안 받는다.

	while (!in.eof())
	{
		in >> ignore//ObjectName:
			>> lObject_name >> ignore;
		if (ignore != "Pos:")
			in >> ignore;
		in >> x_pos >> y_pos >> z_pos >> ignore >> yaw;
		//동일한 메시 리스트가 있는지 검색
		auto pos = find_if(m_Scriptvector.begin(), m_Scriptvector.end(), [=](const CMeshScript* temp)
		{
			return (temp->Getmeshname() == lObject_name);
		});
		//일치하는 것을 찾으면, 리스트에 추가
		if (pos != m_Scriptvector.end())
		{
			(*pos)->insertObject(x_pos, y_pos, z_pos, yaw);
		}
		//일치하는 메시가 없으면 새로 메쉬리스트를 추가한다
		else if (pos == m_Scriptvector.end())
		{
			m_Scriptvector.push_back(new CMeshScript(lObject_name, x_pos, y_pos, z_pos, yaw));
		}
	}
	in.close();
	return true;
}