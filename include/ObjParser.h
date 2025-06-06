#pragma once
#include <string>
#include <fstream>
#include <vector>

class Tokenizer
{
public:

    static std::vector<std::string> Split(const std::string& stringToSplit, char separator)
    {
        std::vector<std::string> tokens;
        size_t end;
        size_t start = 0;
        while((end = stringToSplit.find(separator, start)) != std::string::npos)
        {
            tokens.push_back(stringToSplit.substr(start, end-start));
            start = end + 1;
        }
        tokens.push_back(stringToSplit.substr(start));
        return tokens;
    }
};

struct Float3
{
    float x;
    float y;
    float z;
};

struct Float2
{
    float x;
    float y;
};

struct Vertex
{
    Float3 point;
    Float2 uv;
    Float3 normal;
};

struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

struct Obj
{
    std::vector<Triangle> triangles;
};


class ObjParser
{
public:
    ObjParser() = delete;

    static bool TryParseObj(const std::string& objPath, Obj& outObj)
    {
        std::ifstream file(objPath);
        if (!file.is_open()) return false;
        std::string line;
        std::vector<Float3> points;
        std::vector<Float2> uvs;
        std::vector<Float3> normals;
        while(std::getline(file, line)) 
        {
            if (line.empty()) continue;
            auto tokens = Tokenizer::Split(line, ' ');
            if (tokens.empty()) continue;
            const std::string& type = tokens[0];

            if(type == "v")
            {
                if(tokens.size() != 4) return false;

                Float3 v;

                try{

                    v.x = std::stof(tokens[1]);
                    v.y = std::stof(tokens[2]);
                    v.z = std::stof(tokens[3]);

                } catch (const std::exception&){

                    return false;
                }
                points.push_back(v);
            }
            else if(type == "vt")
            {
                if(tokens.size() != 3) return false;

                Float2 vt;
                try{

                    vt.x = std::stof(tokens[1]);
                    vt.y = std::stof(tokens[2]);

                } catch (const std::exception&){

                    return false;
                }
                uvs.push_back(vt);
            }
            else if(type == "vn")
            {
                if(tokens.size() != 4) return false;

                Float3 vn;
                try{

                    vn.x = std::stof(tokens[1]);
                    vn.y = std::stof(tokens[2]);
                    vn.z = std::stof(tokens[3]);

                } catch (const std::exception&){

                    return false;
                }
                normals.push_back(vn);
            }
            else if(type == "f")
            {
                if(tokens.size() != 4) return false;

                Triangle triangle;
                Vertex* vertexPointers[] = { &triangle.v1, &triangle.v2, &triangle.v3 };
                for(size_t i = 0; i < 3; ++i)
                {
                    auto faceTokens = Tokenizer::Split(tokens[i + 1], '/');
                    try{

                        int pointIndex = std::stoi(faceTokens[0]) - 1;
                        if (pointIndex < 0 || pointIndex >= points.size()) return false;
                        int uvIndex = std::stoi(faceTokens[1]) - 1;
                        if(uvIndex < 0 || uvIndex >= uvs.size()) return false;
                        int normalIndex = std::stoi(faceTokens[2]) - 1;
                        if(normalIndex < 0 || normalIndex >= normals.size())return false;

                        vertexPointers[i]->point = points[pointIndex];
                        vertexPointers[i]->uv = uvs[uvIndex];
                        vertexPointers[i]->normal = normals[normalIndex];
                        
                    } catch (const std::exception&){

                        return false;
                    }

                }

                outObj.triangles.push_back(triangle);
            }

        }
        return !outObj.triangles.empty();
    }
};