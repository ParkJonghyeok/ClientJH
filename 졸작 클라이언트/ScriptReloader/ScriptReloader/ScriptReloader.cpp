// ScriptReloader.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ScriptReloader.h"

//���� ���

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
		|| (meshname == "Plane"))//�����ؾ��� �̸�(�޽��� �ƴ�)
		return false;
	
	strIter_pos = meshname.find("wheel", strIter_pos);

	if (strIter_pos != std::string::npos)//���� ���� �޽� �ܿ���(���� ����), ���� ������ ���߿� ���� ������ ������
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
	float yaw = 0;//pitch, roll �� ������ �� �޴´�.

	while (!in.eof())
	{
		in >> ignore//ObjectName:
			>> lObject_name >> ignore;
		if (ignore != "Pos:")
			in >> ignore;
		in >> x_pos >> y_pos >> z_pos >> ignore >> yaw;
		//������ �޽� ����Ʈ�� �ִ��� �˻�
		
		if(!DecideValidatedMesh(lObject_name))	
			continue;
		
		//���� �޽�����Ʈ�� ��ϵ� ������ Ȯ���Ѵ�.
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
		//��ġ�ϴ� �޽ð� ������ ���� �޽�����Ʈ�� �߰��Ѵ�
		else if (scriptpos == m_Scriptvector.end())
		{
			m_Scriptvector.push_back(new CMeshScript(lObject_name, x_pos, y_pos, z_pos, yaw));
		}
	}
	in.close();
	return true;
}