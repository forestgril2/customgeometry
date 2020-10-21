
#include "examplegeometry.h"
#include <QRandomGenerator>

#include <cstdio>
#include <iostream>
#include <string>

// ASSIMP LIBRARY INCLUDE
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>

//#include "assimphelpers.h"

static const aiScene* scene;
static bool isAssimpReadDone = false;

void DoTheErrorLogging(const std::string&& pError)
{
	std::cout << pError << std::endl;
}

void DoTheSceneProcessing(const aiScene* scene)
{
	std::cout << "DoTheSceneProcessing(), numMeshes:                       " << scene->mNumMeshes << std::endl;
	std::cout << "DoTheSceneProcessing(), scene->mMeshes[0]->mNumVertices: " << scene->mMeshes[0]->mNumVertices << std::endl;
}

bool DoTheImportThing(const std::string& pFile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
							  aiProcess_CalcTangentSpace |
							  aiProcess_Triangulate |
							  aiProcess_JoinIdenticalVertices |
							  aiProcess_SortByPType);
	// If the import failed, report it
	if(!scene)
	{
		DoTheErrorLogging(std::string(importer.GetErrorString()));
		return false;
	}

	DoTheSceneProcessing(scene);

	isAssimpReadDone = true;
	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}

ExampleTriangleGeometry::ExampleTriangleGeometry()
{
    updateData();
}

void ExampleTriangleGeometry::setNormals(bool enable)
{
    if (m_hasNormals == enable)
        return;

    m_hasNormals = enable;
    emit normalsChanged();
    updateData();
    update();
}

void ExampleTriangleGeometry::setNormalXY(float xy)
{
    if (m_normalXY == xy)
        return;

    m_normalXY = xy;
    emit normalXYChanged();
    updateData();
    update();
}

void ExampleTriangleGeometry::setUV(bool enable)
{
    if (m_hasUV == enable)
        return;

    m_hasUV = enable;
    emit uvChanged();
    updateData();
    update();
}

void ExampleTriangleGeometry::setUVAdjust(float f)
{
    if (m_uvAdjust == f)
        return;

    m_uvAdjust = f;
    emit uvAdjustChanged();
    updateData();
    update();
}

void ExampleTriangleGeometry::updateData()
{
    if (isAssimpReadDone)
		return;

    if(DoTheImportThing("C:/ProjectsData/stl_files/mandoblasterlow.stl"))
    {
		// Now we can access the file's contents.
//		DoTheSceneProcessing(scene);
    }

	clear();

    int stride = 3 * sizeof(float);
    if (m_hasNormals)
        stride += 3 * sizeof(float);
    if (m_hasUV)
        stride += 2 * sizeof(float);

    QByteArray v;
    v.resize(3 * stride);
    float *p = reinterpret_cast<float *>(v.data());

    // a triangle, front face = counter-clockwise
//    *p++ = -1.0f; *p++ = -1.0f; *p++ = 0.0f;
//    if (m_hasNormals) {
//        *p++ = m_normalXY; *p++ = m_normalXY; *p++ = 1.0f;
//    }
//    if (m_hasUV) {
//        *p++ = 0.0f + m_uvAdjust; *p++ = 0.0f + m_uvAdjust;
//    }
//    *p++ = 1.0f; *p++ = -1.0f; *p++ = 0.0f;
//    if (m_hasNormals) {
//        *p++ = m_normalXY; *p++ = m_normalXY; *p++ = 1.0f;
//    }
//    if (m_hasUV) {
//        *p++ = 1.0f - m_uvAdjust; *p++ = 0.0f + m_uvAdjust;
//    }
//    *p++ = 0.0f; *p++ = 1.0f; *p++ = 0.0f;
//    if (m_hasNormals) {
//        *p++ = m_normalXY; *p++ = m_normalXY; *p++ = 1.0f;
//    }
//    if (m_hasUV) {
//        *p++ = 1.0f - m_uvAdjust; *p++ = 1.0f - m_uvAdjust;
//    }

// a triangle, front face = counter-clockwise
    *p++ = -1.0f; *p++ = -1.0f; *p++ = 0.0f;
    *p++ = 1.0f; *p++ = -1.0f; *p++ = 0.0f;
    *p++ = 0.0f; *p++ = 1.0f; *p++ = 0.0f;

    setVertexData(v);
    setStride(stride);

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);

    if (m_hasNormals) {
        addAttribute(QQuick3DGeometry::Attribute::NormalSemantic,
                     3 * sizeof(float),
                     QQuick3DGeometry::Attribute::F32Type);
    }

    if (m_hasUV) {
        addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic,
                     m_hasNormals ? 6 * sizeof(float) : 3 * sizeof(float),
                     QQuick3DGeometry::Attribute::F32Type);
    }

    std::cout << " ########## DATA UPDATE IN " << __FUNCTION__ << std::endl;
}

ExamplePointGeometry::ExamplePointGeometry()
{
    updateData();
}

void ExamplePointGeometry::updateData()
{
    clear();

    const int N = 2000;

    const int stride = 3 * sizeof(float);

    QByteArray v;
    v.resize(N * stride);
    float *p = reinterpret_cast<float *>(v.data());

    for (int i = 0; i < N; ++i) {
        const float x = float(QRandomGenerator::global()->bounded(200.0f) - 100.0f) / 20.0f;
        const float y = float(QRandomGenerator::global()->bounded(200.0f) - 100.0f) / 20.0f;
        *p++ = x;
        *p++ = y;
        *p++ = 0.0f;
    }

    setVertexData(v);
    setStride(stride);

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Points);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}