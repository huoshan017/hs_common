#ifndef _HS_CONFIG_READER_H_
#define _HS_CONFIG_READER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;


class HSConfigReader
{
public:
	class DataRow
	{
	public:
		DataRow() {}
		~DataRow() {
			Clear();
		}

		void Clear() {
			m_vecDataString.clear();
		}

		size_t GetFieldSize() {
			return m_vecDataString.size();
		}

		const char* GetFieldByIndex(unsigned int index) {
			if (index > m_vecDataString.size()-1)
				return NULL;

			string& key = m_vecDataString[index];
			return key.c_str();
		}

		void PushValue(const string& value) {
			m_vecDataString.push_back(value);
		}

	private:
		vector<string> m_vecDataString;
	};

public:
	HSConfigReader(int header_rows = 1, int key_row_index = 0, const char* delimiters = ",")
		: m_nHeaderRows(header_rows), m_nKeyRowIndex(key_row_index), m_strDelimiters(delimiters)
	{
	}

	~HSConfigReader()
	{
	}

	int GetHeaderRows() const { return m_nHeaderRows; }
	int GetKeyRowIndex() const { return m_nKeyRowIndex; }
	const string& GetDelimiters() const { return m_strDelimiters; }

	void SetDelimiters(const char* delimiters) {
		m_strDelimiters = delimiters;
	}

	void SetHeaderRows(int rows = 1) {
		m_nHeaderRows = rows;
	}

	void SetKeyRowIndex(int key_row_index = 0) {
		m_nKeyRowIndex = key_row_index;
	}

	bool LoadConfig(const char* file) {
		ifstream in_file(file);
		if (!in_file.is_open())
			return false;

		string line;
		int count = 0;
		//while (getline(in_file, line).rdstate() != ios::eofbit) {
		while (true) {
			getline(in_file, line);
			if (in_file.eof())
				break;
			if (count < m_nHeaderRows) {
				if (count++ == m_nKeyRowIndex) {
					CreateDataRow(line, m_datarowHeader);
				}
				continue;
			}
			DataRow* dr = new DataRow();
			CreateDataRow(line, *dr);
			m_vecDataRows.push_back(dr);
		}

		return true;
	}

	bool ReloadConfig(const char* file) {
		Clear();
		return LoadConfig(file);
	}

	void Clear() {
		m_datarowHeader.Clear();
		vector<DataRow*>::iterator it = m_vecDataRows.begin();
		for (; it!=m_vecDataRows.end(); ++it) {
			if ((*it) != NULL) {
				delete (*it);
			}
		}
		m_vecDataRows.clear();
	}


	// 获得数据条数
	size_t GetDataRowsNum() {
		return m_vecDataRows.size();
	}

	// 获得头
	DataRow& GetHeaderRow() {
		return m_datarowHeader;
	}

	// 获得列数
	size_t GetDataColumnNum() {
		return m_datarowHeader.GetFieldSize();
	}

	// 获得行数据
	DataRow* GetDataRow(size_t index) {
		if (index > GetDataRowsNum()-1)
			return NULL;
		return m_vecDataRows[index];
	}

	// 通过key获得行数据
	DataRow* GetDataRowByKey(const char* key);

private:
	bool CreateDataRow(string& line, DataRow& data_row) {
		string::size_type offset = 0;
		size_t res_offset = 0;
		while (true) {
			res_offset = line.find(m_strDelimiters, offset);
			if (res_offset == string::npos) {
				if (line.length() > offset) {
					res_offset = line.find("\n", offset);
#ifdef _WIN32
					if (res_offset == string::npos) {
						res_offset = line.find("\r", offset);
					}
#endif
					if (res_offset != string::npos)
						data_row.PushValue(line.substr(offset, res_offset-1-offset+1));
				}
				break;
			}
			if (res_offset-1 >= offset) {
				data_row.PushValue(line.substr(offset, res_offset-m_strDelimiters.length()-offset+1));
			} else {
				printf("HSConfigReader::CreateDataRow failed!");
				return false; 
			}
			offset = res_offset + m_strDelimiters.size();
		}
		return true;
	}

private:
	// 头行数，作为key来用
	int m_nHeaderRows;
	// 关键字所在头行的索引
	int m_nKeyRowIndex;
	// 分隔符
	string m_strDelimiters;
	// 头行
	DataRow m_datarowHeader;
	// 数据
	vector<DataRow*> m_vecDataRows;
};

class HSValue
{
public:
	HSValue(const char* value="") : m_bSeted(false), m_strValue(value) {}
	~HSValue() {}


	bool				AsBool() {
		if (ToIntegelValue() != 0)
			return true;
		return false;
	}
	char				AsChar() {
		return static_cast<char>(ToIntegelValue());
	}
	unsigned char		AsUchar() {
		return static_cast<unsigned char>(ToIntegelValue());
	}
	short				AsShort() {
		return static_cast<short>(ToIntegelValue());
	}
	unsigned short		AsUshort() {
		return static_cast<unsigned short>(ToIntegelValue());
	}
	int					AsInt() {
		return ToIntegelValue();
	}
	unsigned int		AsUint() {
		return static_cast<unsigned int>(ToIntegelValue());
	}
	float				AsFloat() {
		return static_cast<float>(ToDoubleValue());
	}
	double				AsDouble() {
		return ToDoubleValue();
	}
	long				AsLong() {
		return ToLongValue();
	}
	unsigned long		AsUlong() {
		return static_cast<unsigned long>(ToLongValue());
	}
	long long			AsLonglong() {
		return ToLonglongValue();
	}
	unsigned long long	AsUlonglong() {
		return static_cast<unsigned long long>(ToLonglongValue());
	}
	const char*			AsCString() {
		return m_strValue.c_str();
	}
	string&				AsString() {
		return m_strValue;
	}
private:
	int ToIntegelValue() {
		if (!m_bSeted) {
			m_unionValue.int_value = atoi(m_strValue.c_str());
			m_bSeted = true;
		}
		return m_unionValue.int_value;
	}
	double ToDoubleValue() {
		if (!m_bSeted) {
			m_unionValue.double_value = atof(m_strValue.c_str());
			m_bSeted = true;
		}
		return m_unionValue.double_value;
	}
	long ToLongValue() {
		if (!m_bSeted) {
			m_unionValue.long_value = atol(m_strValue.c_str());
			m_bSeted = true;
		}
		return m_unionValue.long_value;
	}
	long long ToLonglongValue() {
		return m_unionValue.longlong_value;
	}
	 
private:
	struct UnionValue {
		UnionValue() {
			int_value = 0;
			double_value = 0;
			long_value = 0;
			longlong_value = 0;
		}
		int	int_value;
		double double_value;
		long long_value;
		long long longlong_value;
	};

	bool			m_bSeted;
	UnionValue		m_unionValue;
	string			m_strValue;
};

template <typename KeyType>
struct HSKeyTraits; 

template <>
struct HSKeyTraits<int>
{
	typedef int Type;
	static Type To(const char* key_str) {
		return atoi(key_str);
	}
};

template <>
struct HSKeyTraits<unsigned int>
{
	typedef unsigned int Type;
	static Type To(const char* key_str) {
		return strtoul(key_str, NULL, 10);
	}
};

template <>
struct HSKeyTraits<char*>
{
	typedef char* Type;
	static Type To(const char* key_str) {
		return const_cast<char*>(key_str);
	}
};

template <>
struct HSKeyTraits<const char*>
{
	typedef const char* Type;
	static Type To(const char* key_str) {
		return key_str;
	}
};

template <>
struct HSKeyTraits<string>
{
	typedef string Type;
	static Type To(const char* key_str) {
		return string(key_str);
	}
};

template<typename KeyType>
class HSConfigFile
{
public:
	class DataRow {
	public:
		DataRow(HSConfigFile* file, HSConfigReader::DataRow* data) : m_pFile(file), m_pDataRow(data) {}
		~DataRow() {}

		const char* GetValue(const char* field) {
			if (!field)
				return NULL;

			map<string, size_t>::iterator it = m_pFile->m_mapHeader2Index.find(string(field));
			if (it == m_pFile->m_mapHeader2Index.end())
				return NULL;

			return m_pDataRow->GetFieldByIndex(it->second);
		}

		HSValue* GetHSValue(const char* field) {
			if (!field)
				return NULL;

			size_t s = m_pDataRow->GetFieldSize();
			// 创建vector
			if (m_vecHSValue.size()==0 && s>0) {
				size_t i = 0;
				m_vecHSValue.resize(s);
				for (; i<s; ++i) {
					HSValue v(m_pDataRow->GetFieldByIndex(i));
					m_vecHSValue[i] = v;
				}
			}

			map<string, size_t>::iterator it = m_pFile->m_mapHeader2Index.find(string(field));
			if (it == m_pFile->m_mapHeader2Index.end())
				return NULL;

			s = m_vecHSValue.size();
			if (it->second > s-1)
				return NULL;

			return &m_vecHSValue[it->second];
		}

	private:
		HSConfigFile*				m_pFile;
		HSConfigReader::DataRow*	m_pDataRow;
		vector<HSValue>				m_vecHSValue;
	};

public:
	HSConfigFile(const char* key_field = NULL) {
		m_strKeyField = key_field;
	}

	~HSConfigFile() {
		Clear();
	}

	bool Load(const char* file) {
		if (!m_reader.LoadConfig(file))
			return false;

		size_t i = 0;
		HSConfigReader::DataRow& d = m_reader.GetHeaderRow();
		const char* key_field = NULL;
		size_t key_index = 0;
		// 遍历头索引
		for (; i<d.GetFieldSize(); ++i) {
			const char* field_str = d.GetFieldByIndex(i);
			if (!field_str)
				continue;
			// 关键字段作为key
			if (strcmp(field_str, m_strKeyField.c_str()) == 0) {
				key_field = field_str;
				key_index = i;
			}
			// 字段头对应索引
			m_mapHeader2Index.insert(make_pair(string(field_str), i));
		}

		// 遍历所有记录
		for (i=0; i<m_reader.GetDataRowsNum(); ++i) {
			HSConfigReader::DataRow* pd = m_reader.GetDataRow(i);
			if (!pd)
				continue;
			const char* key = pd->GetFieldByIndex(key_index);
			if (!key)
				continue;

			HSKeyTraits<KeyType>::Type key_value = HSKeyTraits<KeyType>::To(key);
			m_mapDataKey2Index.insert(make_pair(key_value, i));
		}

		return true;
	}

	bool Reload(const char* file) {
		Clear();
		return Load(file);
	}

	void Clear() {
		m_reader.Clear();
		m_mapHeader2Index.clear();
		m_mapDataKey2Index.clear();
		m_mapDataKey2Data.clear();
		m_mapDataKey2ValueVector.clear();
	}

	void SetKeyField(const char* key_field = NULL) {
		if (key_field) {
			m_strKeyField = key_field;
		}
	}

	void SetDelimiters(const char* delimiters = ",") {
		m_reader.SetDelimiters(delimiters);
	}

	void SetHeaderRows(int rows = 1) {
		m_reader.SetHeaderRows(rows);
	}

	void SetKeyRowIndex(int key_row_index = 0) {
		m_reader.SetKeyRowIndex(key_row_index);
	}

	DataRow* GetDataRow(const KeyType& key_value) {
		//string key_string(key);
		map<KeyType, DataRow>::iterator data_it = m_mapDataKey2Data.find(key_value);
		if (data_it == m_mapDataKey2Data.end()) {
			map<KeyType, size_t>::iterator it = m_mapDataKey2Index.find(key_value);
			if (it == m_mapDataKey2Index.end())
				return NULL;

			HSConfigReader::DataRow* data = m_reader.GetDataRow(it->second);
			if (!data)
				return NULL;

			m_mapDataKey2Data.insert(make_pair(key_value, DataRow(this, data)));
		}

		data_it = m_mapDataKey2Data.find(key_value);
		return &(data_it->second);
	}

	const char* GetData(const KeyType& key, const char* field) {
		DataRow* row = GetDataRow(key);
		if (!row)
			return NULL;

		return row->GetValue(field);
	}

	HSValue* GetHSValue(const KeyType& key, const char* field) {
		DataRow* row = GetDataRow(key);
		if (!row)
			return NULL;

		return row->GetHSValue(field);
	} 

private:
	HSConfigReader m_reader;									// 保存配置数据
	string m_strKeyField;										// 关键字段字符串
	map<string, size_t> m_mapHeader2Index;						// 头对应的索引，可以找到索引对应的数据
	map<KeyType, size_t> m_mapDataKey2Index;					// 记录关键字对应索引，查找记录
	map<KeyType, DataRow> m_mapDataKey2Data;					// 记录关键字对应数据
	map<KeyType, vector<HSValue> > m_mapDataKey2ValueVector;	// 保存成HSValue的格式
};

#include "singleton.hpp"
/*
template<typename ConfigType>
class HSConfigFileManager : public Singleton<HSConfigFileManager<ConfigType> >
{
public:
	bool LoadConfig(const char* file, const char* key_field, const ConfigType& config_type) {
		if (!file)
			return false;

		HSConfigFile* file_ptr = new HSConfigFile(key_field);
		if (!file_ptr->Load(file)) {
			delete file_ptr;
			return false;
		}

		m_mapFiles.insert(make_pair(config_type, file_ptr));

		return true;
	}

	void Clear() {
		map<ConfigType, HSConfigFile*>::iterator it = m_mapFiles.begin();
		for (; it!=m_mapFiles.end(); ++it) {
			if (it->second) {
				delete it->second;
			}
		}
		m_mapFiles.clear();
	}



private:
	map<ConfigType, HSConfigFile*> m_mapFiles;
};*/

#endif