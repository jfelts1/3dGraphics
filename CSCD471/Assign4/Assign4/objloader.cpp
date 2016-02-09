#include "objloader.h"
using std::string;
using std::vector;
using glm::vec3;
using std::stringstream;

objLoader::objLoader(std::istream& objectInput)
{
    objectInput.seekg(0,objectInput.end);
    size_t len = objectInput.tellg();
    objectInput.seekg(0,objectInput.beg);
    m_data.resize(len,' ');
    char* begin = &*m_data.begin();
    objectInput.read(begin,len);
}

vector<vec3> objLoader::getVertices()const
{
    vector<vec3> ret;
    auto pos = m_data.find("vn ");
    auto pos2 = m_data.find("\n",pos);
    stringstream tmp(m_data.substr(pos+sizeof("vn "),pos2-sizeof("vn ")));
    float x,y,z;
    while(pos != string::npos && pos2 != string::npos)
    {
        //printf("pos:%llu, pos2:%llu, pos2-pos:%llu\n",pos+sizeof("vn "),pos2,pos2-pos-sizeof("vn "));
        //std::cout << tmp.str() <<std::endl;
        tmp>>x;
        tmp>>y;
        tmp>>z;
        printf("x:%f, y:%f, z:%f\n",x,y,z);
        ret.emplace_back(vec3(x,y,z));
        pos = m_data.find("vn ",pos+1);
        pos2 = m_data.find("\n",pos);
        if(pos!=string::npos && pos2!= string::npos)
        {
            tmp = stringstream(m_data.substr(pos+sizeof("vn "),pos2-pos-sizeof("vn ")));
        }
    }

    return ret;
}

vector<vec3> objLoader::getNormals()const
{
    vector<vec3> ret;
    return ret;
}

vector<float> objLoader::getTextures()const
{
    vector<float> ret;
    return ret;
}

vector<GLuint> objLoader::getIndices()const
{
    vector<GLuint> ret;

    return ret;
}
