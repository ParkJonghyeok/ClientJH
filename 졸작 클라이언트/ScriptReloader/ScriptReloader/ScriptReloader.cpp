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


bool CScript::DecideValidatedMesh(string& meshname)
{
	int strIter_pos = 0;

	if ((meshname == "Main") || (meshname == "Directional")
		|| (meshname == "Plane"))//제외해야할 이름(메쉬가 아님)
		return false;
	
	strIter_pos = meshname.find("wheel", strIter_pos);

	if (strIter_pos != std::string::npos)//계층 구조 메쉬 잔여물(차량 바퀴), 관련 정보는 나중에 따로 수집할 예정임
		return false;
	else
		return true;
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
		
		if(!DecideValidatedMesh(lObject_name))	
			continue;
		
		//현재 메쉬리스트에 등록된 것인지 확인한다.
		auto scriptpos = find_if(m_Scriptvector.begin(), m_Scriptvector.end(), [=](const CMeshScript* temp)	{
			return (temp->Getmeshname() == lObject_name);
		});

		if(scriptpos != m_Scriptvector.end())
		{
			auto Iter_begin = (*scriptpos)->GetSCRvectors().begin();
			auto Iter_end = (*scriptpos)->GetSCRvectors().end();
			auto SCRvpos = find_if(Iter_begin, Iter_end, [=](const SCRvector& tempvec) 
			{
				return (tempvec.m_x == x_pos) && (tempvec.m_y == y_pos) && (tempvec.m_z == z_pos) && (tempvec.m_yaw == yaw);
			});
			if(SCRvpos == Iter_end) (*scriptpos)->insertObject(x_pos, y_pos, z_pos, yaw);
		}
		//일치하는 메시가 없으면 새로 메쉬리스트를 추가한다
		else if (scriptpos == m_Scriptvector.end())
		{
			m_Scriptvector.push_back(new CMeshScript(lObject_name, x_pos, y_pos, z_pos, yaw));
		}
	}
	in.close();
	return true;
}