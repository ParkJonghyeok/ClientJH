#pragma once

class SCRvector
{
public:
	SCRvector(float x = 0, float y = 0, float z = 0, float yaw = 0, float pitch = 0, float roll = 0)
		: m_x(x), m_y(y), m_z(z), m_yaw(yaw), m_pitch(pitch), m_roll(roll)
	{};
	float m_x;
	float m_y;
	float m_z;
	float m_yaw;
	float m_pitch;
	float m_roll;
};

class CMeshScript
{
	string m_meshname;
	vector<SCRvector> m_SCRvectors;
private:

public:
	CMeshScript(string filename, float x, float y, float z, float yaw = 0, float pitch = 0, float roll = 0) : m_meshname(filename)
	{
		m_SCRvectors.push_back(SCRvector(x, y, z, yaw, pitch, roll));
	};
	void Setmeshname(string meshname) { m_meshname = meshname; }

	string Getmeshname() const { return m_meshname; };

	vector<SCRvector>& GetSCRvectors()
	{
		return m_SCRvectors;
	}
	void insertObject(float x, float y, float z, float yaw = 0, float pitch = 0, float roll = 0)
	{
		m_SCRvectors.push_back(SCRvector(x, y, z, yaw, pitch, roll));
	};
	void insertObject(SCRvector vector)
	{
		m_SCRvectors.push_back(vector);
	};
	size_t size() const { return m_SCRvectors.size(); }
};

class CScript
{
	string m_scriptname;
	//size_t m_nObject;
	vector<CMeshScript*> m_Scriptvector;
	//static CScript* m_pScript;
private:
	CScript() {};
	~CScript()
	{
		for (auto i : m_Scriptvector)
			delete i;
	}
public:
	static CScript* Create()
	{
		static CScript instance;
		return &instance;
	}
	//스크립트 파일 이름을 저장한다, 수정하지 않을 경우 처음 로드한 제목을 유지, 변경한다
	void SetScriptFilename(string filename)
	{
		m_scriptname = filename;
	}
	//오브젝트 데이터를 삽입한다
	size_t GetNumOfObject();
	
	bool loadRawScript(string filename);
	
	void ScriptPrintOut();
	
	void Initialize()
	{
		m_scriptname.clear();
		for (auto iter : m_Scriptvector)
			delete iter;
		m_Scriptvector.clear();
	}
	bool DecideValidatedMesh(string& meshname);
};



